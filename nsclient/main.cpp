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
#include <algorithm>
#include "utils.h"
#include "DnsClient.h"



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

		// converting all strings to lower case
		std::transform(domain_name.begin(), domain_name.end(), domain_name.begin(), ::tolower);  

		if (!domain_name.compare("quit"))
		{
			break;
		}

		// can continue with the ns lookup
		isValidDomainName(domain_name);


		struct hostent* dnsAnswer = DnsClient::Instance().dnsQuery(domain_name, ip_as_string);

		if (dnsAnswer != NULL)
		{
			if (dnsAnswer->h_length != 0)
			{
			printf("%s \n\n", dnsAnswer->h_addr_list[0]);
			}
		}
	}	
	return 0;
}
