#include "utils.h"
#include <string>
#include <regex>
#include <iostream>
#include <WS2tcpip.h>


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
    std::cerr << "Domain name isn't valid\n" << std::endl;
	 exit(1);
  }
}

void isValidIpAddress(const char *ipAddress)
{
    struct sockaddr_in sa;
    int result = inet_pton(AF_INET, ipAddress, &(sa.sin_addr));

	if (!result)
	{
		std::cerr << "Ip address is invalid!\n" << std::endl;
		exit(1);
	}
	else
		std::cout << "Ip address is valid" << std::endl;
		return;

}