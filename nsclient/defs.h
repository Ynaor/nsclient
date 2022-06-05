/*
Authors:				Shachar Cohen (313416521) & Yuval Naor (312497084)
Project:				Programming Assignment 1: Noisy Channel
Project description:	Sender-Receiver communication through a noisy channel
*/

#pragma once
#pragma comment(lib, "Ws2_32.lib")

#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define RANDOM_PORT         0
#define	BUFFERMAXSIZE		256		// String max size
#define PORT				53		// Default port
#define MAX_WAIT_TIME		2000	// Max wait time for receiving response from the DNS server in miliseconds
#define IP_LEN				15		// IPV4 len in bytes - 12 digits + 3 dots 
#define MAX_DNS_ANSWERS		50		// The maximum number of the DNS server answers
#define MAX_ANSWERS         1       // For now, we support only 1 ip answer
#define TIMEOUT_MILI        2000    // max time to wait for UDP socket connection