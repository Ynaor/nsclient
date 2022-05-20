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

    unsigned short id;             // message id number

    unsigned char qr: 1;           // Specifies whether this message is a query (0), or a response (1)
    unsigned char opcode :4;       // Specifies the kind of query in this message. (0) for standard query, (1) for inverse query
    unsigned char aa :1;           // Authoritive answer
    unsigned char tc :1;           // TrunCation message
    unsigned char rd :1;           // Recursion desired
    unsigned char ra :1;           // Recursion Available
    unsigned char z :3;            // Reserved - must be All Zeros
    unsigned char rcode :4;        /* 0 No error condition
                                      1 Format error   
                                      2 Server failure
                                      3 Name Error 
                                      4 Not Implemented 
                                      5 Refused 
                                      6-15 Reserved for future use. */

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
    unsigned short type;          // specifies the meaning of the data in the RDATA field
	unsigned short _class;        // specieies the class of the data in the RDATA field
	unsigned int ttl;             // time before record cache may be discarded
	unsigned short rdlength;      // length of RDATA field, according to the type and class
};
