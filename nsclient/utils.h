#pragma once
/*
Authors:				Shachar Cohen (313416521) & Yuval Naor (312497084)
Project:				Programming Assignment 1: Noisy Channel
Project description:	Sender-Receiver communication through a noisy channel
*/

#include <string>


/****************************************************************************
* Check that the input domain name is indeed in a domain-name structure
****************************************************************************/
void isValidDomainName(std::string str);

/****************************************************************************
* Check that the given DNS server IP address is indeed in a IP address format
*****************************************************************************/
void isValidIpAddress(const char *ipAddress);