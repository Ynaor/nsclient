/*
Authors:				Shachar Cohen (313416521) & Yuval Naor (312497084)
Project:				Programming Assignment 2: Name Server client
Project description:	Implementation of a simple DNS client
*/

#pragma comment(lib, "Ws2_32.lib")
#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <winsock2.h>
#include <ws2def.h>
#include <cstdint>
#include <iostream>
#include <stdlib.h>
#include "defs.h"
#include "dns_message.h"
#include <string>
#include <WS2tcpip.h>
#include <regex>
#include "DnsClient.h"


DnsClient::DnsClient()
{
	InitWinsock();
	mDnsSocket = createNewSocket(&mDnsServerAddr, DnsIpAddressAsString, FALSE);


	mDnsAnswer = (struct hostent*)malloc(sizeof(struct hostent)); // free this allocated memory
	mDnsAnswer->h_name		= NULL;
	mDnsAnswer->h_aliases	= NULL;
	mDnsAnswer->h_length = 0;
	memset(OutputBuffer, 0, sizeof(OutputBuffer));
}

void DnsClient::InitWinsock()
{
	WSADATA wsadata;
	WinsockInit(&wsadata);

}

SOCKET createNewSocket(SOCKADDR_IN *aClientAddr, char* address, BOOL aIsListen)
{
	SOCKET s;

	// set socket parameters
	aClientAddr->sin_family = AF_INET;
	aClientAddr->sin_port = htons(PORT);
	aClientAddr->sin_addr.s_addr = inet_addr(address);
	
	// create the new socket. DGRAM for UDP
	if ((s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == INVALID_SOCKET)
	{
		std::cerr << "Could not create UDP socket\n" << WSAGetLastError();
		exit(1);
	}

	// TODO: need to decide if an error with setting time out should cause ditching the program + This part is pretty much copied from u'r friend
	// Set the waiting time limit for incoming communication with the DNS server
	/*int maxWaitTime = MAX_WAIT_TIME;

	timeval sock_tv = {0};
	sock_tv.tv_sec = 2;
	sock_tv.tv_usec = 0;

	if (setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, (const char*)&sock_tv, sizeof(sock_tv)) == SOCKET_ERROR) {
		std::cerr << "Could not set wait time limit for incoming DNS communication\n" << WSAGetLastError();
	}*/

	// if socket is for listening: will handle it correctly
	if (aIsListen)
	{
		// bind
		int bindRes = bind(s, (SOCKADDR*)aClientAddr, sizeof(*aClientAddr));
		if (bindRes)
		{
			std::cerr << "Error while binding new socket\n" << WSAGetLastError();
			exit(1);
		}

		//listen
		int listenRes = listen(s, 1);
		if (listenRes)
		{
			std::cerr << "Could not listen to socket\n" << WSAGetLastError();
			exit(1);
		}
	}
	return s;
}

void DnsClient::setDnsHeader(dnsHeader *aHeader)
{
	memset(aHeader, 0, sizeof(aHeader));

	aHeader->id = htons(mQueryID);
	aHeader->qdcount = htons(1);
	aHeader->rcode = 0;
	aHeader->aa = 0;
	aHeader->tc = 0;
	aHeader->rd = 1;
	aHeader->opcode = 0;
	aHeader->qr = 0;
	aHeader->z = 0;
	aHeader->ra = 0;
	aHeader->ancount = 0;
	aHeader->nscount = 0;
	aHeader->arcount = 0;

	mQueryID++;
}

void DnsClient::setDnsQname(std::string aDomainName, char *aDnsQname)
{
	memset(aDnsQname, 0, BUFFERMAXSIZE);
	strcpy(aDnsQname + 1, aDomainName.c_str());
	int sequenceLen = 0;
	aDnsQname[0] = '.';
	unsigned int len = strlen(aDnsQname);
	for (int i = len - 1; i > -1; --i)
		if (aDnsQname[i] == '.') {
			sprintf(aDnsQname + i, "%c%s", sequenceLen, aDnsQname + i + 1);
			sequenceLen = 0;
		}
		else {
			sequenceLen++;
		}
	aDnsQname[strlen(aDnsQname)] = '\0';
}


hostent* DnsClient::dnsQuery(std::string adomainName, char* aDnsIpAddress)
{
	setDnsHeader(&mHeader);													// Initialize the header
	setDnsQname(DnsIpAddressAsString, Qname);								// Convert the input domain name to a Qname format

	memcpy(OutputBuffer, &mHeader, sizeof(dnsHeader));						// Adding the header to output message
	mMessageLength = sizeof(dnsHeader);

	memcpy(OutputBuffer + mMessageLength, Qname, strlen(Qname) + 1);		// Adding QNAME to message
	mMessageLength += strlen(Qname);
	mMessageLength++;													

	// Setting question params
	mQuestion.qclass = htons(1);          
	mQuestion.qtype	= htons(1);	 
	memcpy(OutputBuffer + mMessageLength, &mQuestion, sizeof(dnsQuestion));	// Adding the actual Question to the message
	mMessageLength += sizeof(dnsQuestion);


	// Send the new query to the DNS server
	if (sendto(mDnsSocket, (char*)OutputBuffer, mMessageLength+1, 0, (struct sockaddr*)&mDnsServerAddr, sizeof(mDnsServerAddr)) == SOCKET_ERROR)
	{
		std::cerr << "Could not send message to DNS server\n" << WSAGetLastError();
		closesocket(mDnsSocket);
		exit(1);
	}


	// Receive the DNS server answer
	int sockAddrSize = sizeof(mDnsServerAddr);
	if (recvfrom(mDnsSocket, (char*)InputBuffer, sizeof(InputBuffer), 0, (struct sockaddr*)&mDnsServerAddr, &sockAddrSize) == SOCKET_ERROR)
	{
		int error = WSAGetLastError();
		// in case the error is do to timeout print an appropriate message and return to main
		if (error == WSAETIMEDOUT) {
			std::cerr << "DNS serever did not respond within the time limit\n" << error;
			closesocket(mDnsSocket);
			return mDnsAnswer;
		}
		else {
			std::cerr << "failed to receive message from the DNS serever\n" << error;
			closesocket(mDnsSocket);
			return mDnsAnswer; // TODO: perhaps we should end the program
		}
	}


	 // TODO: continue here
	memcpy(&mHeader, InputBuffer, sizeof(dnsHeader));

	// TODO: check if there is an answer

	int arcount = ntohs(mHeader.arcount); 
	int nscount = ntohs(mHeader.nscount);
	int ancount = ntohs(mHeader.ancount);

	// TODO: need to make sure this is the only case that the domain is non-existent 
	if (ancount == 0)
	{
		std::cerr << "DNS Server Could not find the specified domain name\n" << std::endl;
		closesocket(mDnsSocket);
		return dnsAnswer;
	}

	char buffer[BUFFERMAXSIZE];
	int offset = sizeof(dnsHeader);
	int qnameLen = 0;
	memset(buffer, 0, sizeof(buffer));
	decompress(InputBuffer, buffer, offset, &qnameLen);
	offset += qnameLen + sizeof(dnsQuestion);
	char* ipList[MAX_DNS_ANSWERS] = { 0 };					// Curentlly implemented on a single ip 
	char *ipPtr = (char*)malloc(sizeof(char)*16);


	//int ipList_idx = 0;

	// Parse DNS server answers
	while (ancount > 0) {
			--ancount;
			if (answerParser(InputBuffer, &offset, ipPtr))
				continue;
			else
				break;
	}


	dnsAnswer->h_addrtype	= AF_INET;
	dnsAnswer->h_length		= 4;
	dnsAnswer->h_addr_list = &ipPtr;
	closesocket(mDnsSocket);
	return dnsAnswer;

}

