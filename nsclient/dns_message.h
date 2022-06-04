#pragma once

/****************************************************************************************
************************		DNS message structs		*********************************
****************************************************************************************/

/* -DNS Header- */
/**                                 1  1  1  1  1  1
      0  1  2  3  4  5  6  7  8  9  0  1  2  3  4  5
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |                      ID                       |
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |QR|   Opcode  |AA|TC|RD|RA|   Z    |   RCODE   |
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |                    QDCOUNT                    |
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |                    ANCOUNT                    |
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |                    NSCOUNT                    |
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |                    ARCOUNT                    |
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
*/

#pragma pack(1)                    // removing redundant padding inside the struct
struct dnsHeader{

    unsigned short id;			// ID number: to be incremented for consequitive mesasges.

    unsigned char rd : 1;		// recursion desired field
    unsigned char tc : 1;		// is message truncated
    unsigned char aa : 1;		// is the answer authoritive?
    unsigned char opcode : 4;	// opcode for type of message
    unsigned char qr : 1;		// opcode for query or reponse

    unsigned char rcode : 4;	// reponse code for the packet
    unsigned char cd : 1;		// check?
    unsigned char ad : 1;		// is the data authenticated
    unsigned char z : 1;		// does nothing, very relateable 
    unsigned char ra : 1;		// recursion available

    unsigned short qdcount;        // number of entries in the question section
	unsigned short ancount;        // number of resource records in the answer section.
	unsigned short nscount;        // number of name server resource records in the authority records section
	unsigned short arcount;        // number of resource records in the additional records section.
};


/* - Question Section - */
/**                                 1  1  1  1  1  1
      0  1  2  3  4  5  6  7  8  9  0  1  2  3  4  5
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |                                               |
    /                     QNAME                     /
    /                                               /
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |                     QTYPE                     |
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |                     QCLASS                    |
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
*/
struct dnsQuestion{

    unsigned short qtype;          // two octet code which specifies the type of the query
    unsigned short qclass;         // two octet code that specifies the class of the query
};


/* Resource record format */
/**                                 1  1  1  1  1  1
      0  1  2  3  4  5  6  7  8  9  0  1  2  3  4  5
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |                                               |
    /                                               /
    /                      NAME                     /
    |                                               |
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |                      TYPE                     |
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |                     CLASS                     |
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |                      TTL                      |
    |                                               |
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |                   RDLENGTH                    |
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--|
    /                     RDATA                     /
    /                                               /
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
*/

struct dnsRR{
    unsigned short  type;       // specifies the meaning of the data in the RDATA field
	unsigned short  _class;     // specieies the class of the data in the RDATA field
	unsigned int    ttl;        // time before record cache may be discarded
	unsigned short  rdlength;   // length of RDATA field, according to the type and class
};

