/*
Authors:				Shachar Cohen (313416521) & Yuval Naor (312497084)
Project:				Programming Assignment 2: Name Server client
Project description:	Implementation of a simple DNS client
*/

#pragma comment(lib, "Ws2_32.lib")
#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS

// ************************************************
// ***********       Includes       ***************
// ************************************************
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

unsigned short	queryID = 0;
const int		pointer_mask = 49152; // 1100 0000 0000 0000


hostent *dnsQuery(std::string domainName, char* dns_ip_address)
{
	struct hostent* dnsAnswer = (struct hostent*)malloc(sizeof(struct hostent)); // TODO: Not sure why the malloc is needed - this is how ur friend did it + Not sure it is null in case of failure
	// TODO: Consider a initializing func

	// Initialize dnsAnswer
	
	dnsAnswer->h_name		= NULL;
	dnsAnswer->h_aliases	= NULL;
	dnsAnswer->h_length = 0;
	
	SOCKET dnsSocket;
	SOCKADDR_IN dnsServerAddr;

	// Init Winsock2
	// TODO: add failure handling here
	WSADATA wsadata;
	WinsockInit(&wsadata);

	dnsSocket = createNewSocket(&dnsServerAddr, dns_ip_address, FALSE);

	dnsHeader header;
	dnsQuestion question;

	unsigned char send_message[2 * BUFFERMAXSIZE]; // TODO: not sure how this size was chosen
	char recv_message[4 * BUFFERMAXSIZE];	// TODO: not sure how this size was chosen
	//char aux[12 * MAXSIZE];
	char qname[BUFFERMAXSIZE];			// the domain name in dns format
	//char host[MAXSIZE];

	//int length;
	//struct in_addr ptr;
	//unsigned int ptr_convers;

	memset(send_message, 0, sizeof(send_message));

	setDnsHeader(&header); //Initialize the header
	setDnsQname(domainName, qname); // set the QNAME in dns format

	
	memcpy(send_message, &header, sizeof(dnsHeader));			// add header to message
	unsigned int msg_len = sizeof(dnsHeader);

	memcpy(send_message + msg_len, qname, strlen(qname) + 1);	// add QNAME to message
	msg_len += strlen(qname);
	msg_len++;													// TODO: NOT SURE WHY THIS IS NEEDED

	// set question params
	question.qclass		= htons(1);          
	question.qtype		= htons(1);	 
	memcpy(send_message + msg_len, &question, sizeof(dnsQuestion));	// add question to the message
	msg_len += sizeof(dnsQuestion);


	// Send new query to DNS server
	if (sendto(dnsSocket, (char*)send_message, msg_len+1, 0, (struct sockaddr*)&dnsServerAddr, sizeof(dnsServerAddr)) == SOCKET_ERROR)
	{
		std::cerr << "Could not send message to DNS server\n" << WSAGetLastError();
		closesocket(dnsSocket);
		exit(1);
	}


	// Receive the DNS server answer
	int sockAddrSize = sizeof(dnsServerAddr);
	if (recvfrom(dnsSocket, (char*)recv_message, sizeof(recv_message), 0, (struct sockaddr*)&dnsServerAddr, &sockAddrSize) == SOCKET_ERROR)
	{
		int error = WSAGetLastError();
		// in case the error is do to timeout print an appropriate message and return to main
		if (error == WSAETIMEDOUT) {
			std::cerr << "DNS serever did not respond within the time limit\n" << error;
			closesocket(dnsSocket);
			return dnsAnswer;
		}
		else {
			std::cerr << "failed to receive message from the DNS serever\n" << error;
			closesocket(dnsSocket);
			return dnsAnswer; // TODO: perhaps we should end the program
		}
	}





	// TODO: this is how the reference checked the header of recv. not sure its ok format wise (big vs small endian)
	memcpy(&header, recv_message, sizeof(dnsHeader));

	// TODO: check if there is an answer

	int arcount = ntohs(header.arcount); 
	int nscount = ntohs(header.nscount);
	int ancount = ntohs(header.ancount);

	// TODO: need to make sure this is the only case that the domain is non-existent 
	if (ancount == 0)
	{
		std::cerr << "DNS Server Could not find the specified domain name\n" << std::endl;
		closesocket(dnsSocket);
		return dnsAnswer;
	}

	char buffer[BUFFERMAXSIZE];
	int offset = sizeof(dnsHeader);
	int qnameLen = 0;
	memset(buffer, 0, sizeof(buffer));
	decompress(recv_message, buffer, offset, &qnameLen);
	offset += qnameLen + sizeof(dnsQuestion);
	char* ipList[MAX_DNS_ANSWERS] = { 0 };					// Curentlly implemented on a single ip 
	char *ipPtr = (char*)malloc(sizeof(char)*16);


	//int ipList_idx = 0;

	// Parse DNS server answers
	while (ancount > 0) {
			--ancount;
			if (answerParser(recv_message, &offset, ipPtr))
				continue;
			else
				break;
	}


	dnsAnswer->h_addrtype	= AF_INET;
	dnsAnswer->h_length		= 4;
	dnsAnswer->h_addr_list = &ipPtr;
	closesocket(dnsSocket);
	return dnsAnswer;
	

}


void WinsockInit(WSADATA *wsaData)
{
	const auto api_ver = MAKEWORD(2, 2);
	if (WSAStartup(api_ver, wsaData) != NO_ERROR)
	{
		std::cerr << "Winsock initialization failed\n" << WSAGetLastError();
		exit(1);
	}
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

	/*
	* TODO:
	* Not sure why this is necessary, therefore for now i deleted the input of: int *aAutoPort
	* 
	// set the auto selected port from operating system
	int addrSize = sizeof(*aClientAddr);
	getsockname(s, (SOCKADDR*)aClientAddr, &addrSize);
	*aAutoPort = ntohs(aClientAddr->sin_port);
	*/
	return s;
}

void isValidIpAddress(const char *ipAddress)
{
    struct sockaddr_in sa;
    int result = inet_pton(AF_INET, ipAddress, &(sa.sin_addr));

	if (!result)
	{
		std::cerr << "Ip address is invalid!" << std::endl;
		exit(1);
	}
	else
		std::cout << "Ip address is valid" << std::endl;
		return;

}

void isValidDomainName(std::string str)
{
 
  // Regex pattern to make sure the domain name is valid. 
  // This Regex pattern has been generated with the assist of an online regex generator
  const std::regex pattern("^(?!-)[A-Za-z0-9-]+([\\-\\.]{1}[a-z0-9]+)*\\.[A-Za-z]{2,6}$");
 
  if (str.empty())
  {
     std::cerr << "Domain name can't be empty" << std::endl;
	 exit(1);
  }

  if(regex_match(str, pattern)) // domain name is valid
  {
    return;
  }

  else
  {
    std::cerr << "Domain name isn't valid" << std::endl;
	 exit(1);
  }
}

void setDnsHeader(dnsHeader* header)
{
	memset(header, 0, sizeof(header));
	header->id = htons(queryID);
	header->qdcount = htons(1);
	header->rcode = 0;
	header->aa = 0;
	header->tc = 0;
	header->rd = 1;
	header->opcode = 0;
	header->qr = 0;
	header->z = 0;
	header->ra = 0;
	header->ancount = 0;
	header->nscount = 0;
	header->arcount = 0;

	queryID++;
}

/*
* This function gets a domain name and returns the dns format domain name
* For example:
* For domainName: "zoot.tau.ac.il" the function will set dnsDomainName to: "4zoot3tau2ac2il"
*/
void setDnsQname(std::string domainName, char* dnsQname)
{
	memset(dnsQname, 0, BUFFERMAXSIZE);
	strcpy(dnsQname + 1, domainName.c_str());
	int sequenceLen = 0;
	dnsQname[0] = '.';
	unsigned int len = strlen(dnsQname);
	for (int i = len - 1; i > -1; --i)
		if (dnsQname[i] == '.') {
			sprintf(dnsQname + i, "%c%s", sequenceLen, dnsQname + i + 1);
			sequenceLen = 0;
		}
		else {
			sequenceLen++;
		}
	dnsQname[strlen(dnsQname)] = '\0';
}


/*
* This function decompresses a message received from the DNS server
*/
// TODO: could'nt figure out how this works - taken from reference
void decompress(char* msg, char* buffer, int msgOffset, int* bufferLength) {
	int i = msgOffset;
	int newLine = *bufferLength;

	while (msg[i] != '\0') {
		unsigned int ptr;
		memcpy(&ptr, msg + i, 2);				// Copy 2 bytes from message to ptr
		ptr = ntohs(ptr);						// convert format
		if (pointer_mask <= ptr) {				// if ptr is in fact a pointer recursively decompress message
			int ptrOffset = ptr - pointer_mask;
			*bufferLength = newLine;
			decompress(msg, buffer, ptrOffset, &newLine);
			*bufferLength += 2;
			return;
		}
		else {
			int mark = msg[i++];		// TODO: not sure why this is the limit of the following for loop
			for (int j = 0; j < mark; ++j) {
				buffer[newLine++] = msg[i++];
			}
			buffer[newLine++] = '.';	// TODO: not sure if necessary 
		}
	}

	*bufferLength = newLine + 1;
	buffer[newLine] = 0;				// TODO: not sure if necessary
}

/*
* This function gets the received message from the DNS server and saves the parsing of it in ........... 
*/
int answerParser(char* msg_received, int* answer_length, char* ipAddress) {
	dnsRR rr;
	char name[BUFFERMAXSIZE], type[BUFFERMAXSIZE];
	//char answer[MAXSIZE];
	int length = 0;

	decompress(msg_received, name, *answer_length, &length);
	*answer_length += length;
	memcpy(&rr, msg_received + (*answer_length), sizeof(dnsRR));
	memset(type, 0, sizeof(type));
	(*answer_length) += sizeof(dnsRR);
	rr.rdlength = ntohs(rr.rdlength);
	rr.type = ntohs(rr.type);

	int l = *answer_length;
	*answer_length += rr.rdlength;

	if (1 != rr.type) {
		return 1;
	}
	/*
	ipAddress[0] = (char)((int)msg_received[l]);
	ipAddress[1] = '.';
	ipAddress[2] = (char)((int)msg_received[l+1]);
	ipAddress[3] = '.';
	ipAddress[4] = (char)((int)msg_received[l+2]);
	ipAddress[5] = '.';
	ipAddress[6] = (char)((int)msg_received[l+3]);
	ipAddress[7] = '\0';


	*/

	sprintf(ipAddress, "%d.%d.%d.%d", (unsigned char)msg_received[l], (unsigned char)msg_received[l + 1], (unsigned char)msg_received[l + 2], (unsigned char)msg_received[l + 3]);
	return 0;

}

int main(int argc, char* argv[]){

	char ip_as_string[BUFFERMAXSIZE];
	std::string domain_name;


	if (argc < 2)
	{
		std::cerr << "Number of argument passed to nsclient.exe isn't correct";
		exit(1);
	}

	// Get the server IP address as an argument and validate it
	
	strcpy(ip_as_string, argv[1]); 
	isValidIpAddress(ip_as_string);

	while(true) // will continue stay in this loop, until the user types 'quit'
	{
	
		// Get the desired domain name and validate it
		std::cout << "Enter the Domain Name you wish to query: ";
		std::cin >> domain_name;

		std::transform(domain_name.begin(), domain_name.end(), domain_name.begin(), ::tolower);  // converting all strings to lower case

		if (!domain_name.compare("quit"))
		{
			break;
		}

		// can continue with the ns lookup
		isValidDomainName(domain_name);

		struct hostent* dnsAnswer = dnsQuery(domain_name, ip_as_string);

		// Parse response if successfull
		if (dnsAnswer->h_length != 0) {
			printf("%s \n", dnsAnswer->h_addr_list[0]);
		}
	}
	return 0;
}


// TODO: free all malloc vars