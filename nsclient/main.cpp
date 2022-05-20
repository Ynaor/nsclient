/*
Authors:				Shachar Cohen (313416521) & Yuval Naor (312497084)
Project:				Programming Assignment 1: Noisy Channel
Project description:	Sender-Receiver communication through a noisy channel
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


/*
hostent dnsQuery(char *aIpAddress){
	// Interacts with the server
}
*/

void WinsockInit(WSADATA *wsaData)
{
	const auto api_ver = MAKEWORD(2, 2);
	if (WSAStartup(api_ver, wsaData) != NO_ERROR)
	{
		std::cerr << "Winsock initialization failed\n" << WSAGetLastError();
		exit(1);
	}
}
SOCKET newSocket(sockaddr_in *aClientAddr, int* aAutoPort, BOOL aIsListen)
{
	SOCKET s;

	// set socket parameters
	aClientAddr->sin_family = AF_INET;
	aClientAddr->sin_port = RANDOM_PORT;
	
	// create the new socket. DGRAM for UDP
	if ((s = socket(AF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET)
	{
		std::cerr << "Could not create TCP socket\n" << WSAGetLastError();
		exit(1);
	}

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

	// set the auto selected port from operating system
	int addrSize = sizeof(*aClientAddr);
	getsockname(s, (SOCKADDR*)aClientAddr, &addrSize);
	*aAutoPort = ntohs(aClientAddr->sin_port);

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

int main(int argc, char* argv[]){

	const int MAXSIZE = 256;  // String max size
	char ip_as_string[MAXSIZE];
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
		std::cout << "Enter the Domain Name you with to query: ";
		std::cin >> domain_name;

		std::transform(domain_name.begin(), domain_name.end(), domain_name.begin(), ::tolower);  // converting all strings to lower case

		if (!domain_name.compare("quit"))
		{
			break;
		}

		// can continue with the ns lookup
		isValidDomainName(domain_name);



	




		dnsHeader header;
		dnsQuestion question;
		dnsRR rr;

		// header initialize
		memset (&header, 0, sizeof(header));
		header.id = htons(0);
		header.qdcount = htons(1);
		header.rcode = 0;
		header.aa = 0;
		header.tc = 0;
		header.rd = 1;
		header.opcode = 0;
		header.qr = 0;
		header.z = 0;
		header.ra = 0;
		header.ancount = 0;
		header.nscount = 0;
		header.arcount = 0;
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	/*


	// TODO: handle with all there  main order
	int DNSServerPort = 0;
	sockaddr_in DNSServerAddr;
		

	// setting the ip given by the user as the server ip address. need to validate the ip address is okay
	DNSServerAddr.sin_addr.s_addr = inet_addr(ip_as_string);

	// Init Winsock2
	WSADATA wsadata;
	WinsockInit(&wsadata);
	SOCKET SenderListenSock = newSocket(&DNSServerAddr, &DNSServerPort, TRUE);
	
	// setting timeout for 2 seconds max
	timeout.tv_sec = 2;
	timeout.tv_usec = 0;
	*/
	}
	return 0;
}
