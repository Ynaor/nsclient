/*
Authors:				Shachar Cohen (313416521) & Yuval Naor (312497084)
Project:				Programming Assignment 1: Noisy Channel
Project description:	Sender-Receiver communication through a noisy channel
*/

#pragma once

#define RANDOM_PORT         0
#define	MAXSIZE				256		// String max size
#define PORT				53		// Default port
#define MAX_WAIT_TIME		2000	// Max wait time for receiving response from the DNS server in miliseconds
#define IP_LEN				15		// IPV4 len in bytes - 12 digits + 3 dots 
#define MAX_DNS_ANSWERS		50		// The maximum number of the DNS server answers
#define MAX_ANSWERS         1       // TODO: fill this

struct timeval timeout;

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

hostent* dnsQuery(std::string domainName, char* dns_ip_address);

void WinsockInit(WSADATA* wsaData);

SOCKET newSocket(SOCKADDR_IN* aClientAddr, char* address, BOOL aIsListen);

void isValidIpAddress(const char* ipAddress);

void isValidDomainName(std::string str);
void setDnsHeader(dnsHeader* header);

/*
* This function gets a domain name and returns the dns format domain name
* For example:
* For domainName: "zoot.tau.ac.il" the function will set dnsDomainName to: "4zoot3tau2ac2il"
*/
void setDnsQname(std::string domainName, char* dnsQname);

/*
* This function decompresses a message received from the DNS server
*/
// TODO: could'nt figure out how this works - taken from reference
void decompress(char* msg, char* buffer, int msgOffset, int* bufferLength);

/*
* This function gets the received message from the DNS server and saves the parsing of it in ...........
*/
int answerParser(char* msg_received, int* answer_length, char* ipAddress);

int main(int argc, char* argv[]);
