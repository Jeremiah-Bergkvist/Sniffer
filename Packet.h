#ifndef __PACKET_H__
#define __PACKET_H__

#include <iostream>
#include <ipmanip>
#include <cstring>
#include <cstdlib>
#include <sstream>
#include <arpa/inet.h> // u_intX data types

/** Add UDP */

// Set packet byte alignment
#pragma pack(push)
#pragma pack(1)
struct eth_header{
    u_int8_t dhost[6];  // Dest MAC
    u_int8_t shost[6];  // Source MAC
    u_int16_t type;     // IP, ARP, RARP, etc...
};

struct ipv4_header{
    u_int8_t ihl:4;     // IP header length
    u_int8_t ver:4;     // IP version
    u_int8_t tos;       // Type of service
    u_int16_t len;      // Packet length
    u_int16_t id;       // Identification
    u_int16_t off;      // Fragment offset
    #define IP_RF 0x8000    // Flags reserved
    #define IP_DF 0x4000    // Don't fragment
    #define IP_MF 0x2000    // Must fragment
    #define IP_OFFMASK 0x1FFF // Fragments offset
    #define IP_RF 0x8000    // Flags reserved
    u_int8_t ttl;       // Time to live
    u_int8_t proto;     // Protocol
    u_int16_t sum;      // Checksum
    struct in_addr src; // Source IP
    struct in_addr dst; // Destination IP
};

struct ipv6_header{
    u_int32_t vtf;      // Version, traffic class, flow label
    #define IP_VER 0xF0000000   // 4 Bits
    #define IP_TC  0x0FF00000   // 8 Bits
    #define IP_FL  0x000FFFFF   // 20 Bits
    u_int16_t len;      // Payload length
    u_int8_t  nxt;      // Next header type
    u_int8_t hop;       // Hop limit
    u_int16_t src[8];   // Source Address
    u_int16_t dst[8];   // Destination Address
};

struct arp_header{
    u_int16_t hw_type;  // Type of hardware
    u_int16_t p_type;   // Protocol type
    u_int8_t  hw_len;   // Hardware address length
    u_int8_t  p_len;    // Protocol address length
    u_int16_t opcode;   // Operation code
    u_int8_t shost[6];  // Source HW address
    struct in_addr src; // Source IP address
    u_int8_t dhost[6];  // Destination HW address
    struct in_addr dst; // Destination IP address
};

struct icmp_header{
    u_int8_t  type;     // The type of the message
    u_int8_t  code;     // ICMP type code
    u_int16_t sum;      // Checksum
    u_int16_t id;       // Identifier to aide in matching request/reply
    u_int16_t seq;      // Sequence number to aide in matching request/reply
};

struct tcp_header{
    u_int16_t sport;    // Source port
    u_int16_t dport;    // Destination port
    u_int32_t seq;      // Sequence number
    u_int32_t ack;      // Acknowledgement number
    u_int8_t ns:1;      // nonce sum flag
    u_int8_t res1:3;    // Reserved
    u_int8_t off:4;     // Data Offset
    u_int8_t            // TCP Flags
        f_fin:1,        // Finished flag
        f_syn:1,        // Synchronize sequence numbers to start connection
        f_rst:1,        // Reset flag to terminate an existing connection
        f_psh:1,        // Push flag sends data to the application
        f_ack:1,        // Acknowledgement flag
        f_urg:1,        // Urgent flag
        f_ecn:1,        // ECN-Echo flag
        f_cwr:1;        // Conjestion window reduced flag
    u_int16_t win;      // Window number
    u_int16_t sum;      // Checksum
    u_int16_t urg;      // Urgent pointer
};
#pragma pack(pop)

class Packet{
public:
    Packet( u_int8_t *data, int size );
    ~Packet( void );

    // Getters
    bool IsIpv4( void ) { return this->flag_ipv4; }
    bool IsIpv6( void ) { return this->flag_ipv6; }
    bool IsIcmp( void ) { return this->flag_icmp; }
    bool IsTcp ( void ) { return this->flag_tcp;  }
    bool IsArp ( void ) { return this->flag_arp;  }
    bool IsBad ( void ) { return this->flag_bad;  }

    u_int8_t *GetPayload()       { return this->payload; }
    u_int32_t GetPayloadLength() { return this->payloadLength; }

    std::string BufferInHex( void );

private:
    struct eth_header *ethHdr;
    struct ipv4_header *ipv4Hdr;
    struct ipv6_header *ipv6Hdr;
    struct arp_header *arpHdr;
    struct icmp_header *icmpHdr;
    struct tcp_header *tcpHdr;

    bool flag_eth;
    bool flag_ipv4;
    bool flag_ipv6;
    bool flag_icmp;
    bool flag_tcp;
    bool flag_arp;
    bool flag_bad;

    u_int8_t *payload;
    u_int32_t payloadLength;
};

#endif // __PACKET_H__


