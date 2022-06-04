/*
Authors:				Shachar Cohen (313416521) & Yuval Naor (312497084)
Project:				Programming Assignment 2: Name Server client
Project description:	Implementation of a simple DNS client
*/


class DnsClient
{
public:

    /********************************************************************
    * Implementing DnsClient as a singelton class
    ********************************************************************/
    static DnsClient& Instance()
    {
        static DnsClient instance;
        return instance;
    }

    /********************************************************************
    * Destructor - default implementation
    ********************************************************************/
    virtual ~DnsClient() = default;


    /********************************************************************
    * The function which is in fact quries the DNS server
    ********************************************************************/
    hostent *dnsQuery(std::string adomainName, char* aDnsIpAddress);




private:
    DnsClient();

    /********************************************************************
    * Set default values in the DNS header
    ********************************************************************/    
    void setDnsHeader(dnsHeader *aHeader);

    /********************************************************************
    * Convert the given domain name from the user to a Qname format
    ********************************************************************/   
    void setDnsQname(std::string aDomainName, char *aDnsQname);

    /********************************************************************
    * Initialize Winsock Library
    ********************************************************************/    
    void InitWinsock();



    // members
    unsigned short mQueryID = 0;
    const int pointerMask = 0xc000;
    struct hostent* mDnsAnswer;
    dnsHeader mHeader;
	dnsQuestion mQuestion;
    unsigned char OutputBuffer[2 * BUFFERMAXSIZE]; 
	char InputBuffer[4 * BUFFERMAXSIZE];	
	char Qname[BUFFERMAXSIZE];			// the domain name in dns format
    unsigned int mMessageLength;

    // networking
    SOCKET mDnsSocket;
	SOCKADDR_IN mDnsServerAddr;

    char *DnsIpAddressAsString;

};


// Out of class scope functions

SOCKET createNewSocket(SOCKADDR_IN *aClientAddr, char* aAddress, BOOL aIsListen);