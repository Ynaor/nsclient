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


/*
hostent dnsQuery(char *aIpAddress){
	// Interacts with the server
}
*/

void validateIpAddress(const char *ipAddress)
{
    struct sockaddr_in sa;
    int result = inet_pton(AF_INET, ipAddress, &(sa.sin_addr));
	std::cout << "Result is: " << result << std::endl;

}




int main(int argc, char* argv[]){

	const int MAXSIZE = 256;  // String max size
	char ip_as_string[MAXSIZE];

	if (argc < 2)
	{
		std::cerr << "Number of argument passed to nsclient.exe isn't correct";
		exit(1);
	}

	// set the dns server as given by the user
	
	strcpy(ip_as_string, argv[1]); 

	validateIpAddress(ip_as_string);

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
