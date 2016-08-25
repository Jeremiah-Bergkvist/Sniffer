#include "Packet.h"

Packet::Packet( u_int8_t *data, int size )
{
    // Set initial packet size to zero
    this->payloadLength = 0;
    this->payload = NULL;

    // Flags for packet
    this->flag_eth = true;
    this->flag_ipv4 = false;
    this->flag_ipv6 = false;
    this->flag_icmp = false;
    this->flag_tcp = false;
    this->flag_arp = false;
    this->flag_bad = false;

    // Structure pointers to inside the payload
    this->ethHdr = NULL;
    this->ipv4Hdr = NULL;
    this->ipv6Hdr = NULL;
    this->arpHdr = NULL;
    this->icmpHdr = NULL;
    this->tcpHdr = NULL;

    // Ensure the packet does not exceed size limit of 65536 bytes
    if( size < 0 || size > 65536 ) {
        std::cerr << "Packet exceeds size limit!\n";
        exit(5);
    }

    // Allocate enough space to hold the data passed
    this->payloadLength = size;
    this->payload = new u_int8_t[ size ];
    if( payload == NULL ) {
        std::cerr << "Cannot allocate enough memory for packet!\n";
        exit(6);
    }

    // Zero out the new packet buffer
    memset( this->payload, 0, size );

    // Copy the read packet into the buffer
    memcpy( this->payload, data, size );

    // Set the offset for the Ethernet header
    this->ethHdr = ( struct eth_header* ) ( data );

    // IPv4
    if( ntohs( this->ethHdr->type) == 0x0800 ) {
        this->ipv4Hdr = ( struct ipv4_header* ) ( this->payload + sizeof( struct eth_header ) );
        this->flags_ipv4 = true;

        //Length of packet is invalid
        if( this->ipv4Hdr->ihl < 5 || this->ipv4Hdr->ihl > 15 ) {
            this->flag_bad = true;
            return;
        }
        // ICMP
        else if( this->ipv4Hdr->ver == 0x1 ) {
            this->icmpHdr = ( struct icmp_header* ) ( this->payload + sizeof( struct eth_header  ) + this->ipv4Hdr->ihl * 4 );
            this->flag_icmp = true;
        }
        // TCP
        else if( this->ipv4Hdr->ver == 0x6 ) {
            this->tcpHdr = ( struct tcp_header* ) ( this->payload + sizeof( struct eth_header  ) + this->ipv4Hdr->ihl * 4 );
            this->flag_tcp = true;
        }
    }// end ipv4
    // IPv6
    else if( ntohs( this->ethHdr->type) == 0x86DD ) {
        this->ipv6Hdr = ( struct ipv6_header* ) ( this->payload + sizeof( struct eth_header ) );
        this->flags_ipv6 = true;

        // ICMP
        else if( this->ipv6Hdr->nxt== 58 ) {
            this->icmpHdr = ( struct icmp_header* ) ( this->payload + sizeof( struct eth_header  ) + sizeof( struct ipv6_header ) );
            this->flag_icmp = true;
        }
        // TCP
        else if( this->ipv6Hdr->nxt== 6 ) {
            this->tcpHdr = ( struct tcp_header* ) ( this->payload + sizeof( struct eth_header  ) + sizeof( struct ipv6_header ) );
            this->flag_tcp = true;
        }
    }// end ipv6
    // ARP
    else if( ntohs( ethHdr->type ) == 0x8006 ) {
        this->arpHdr = ( struct arp_header* ) ( this->payload + sizeof( struct eth_header ) );
    }// end arp
}// end Packet()

Packet::~Packet( void )
{
    // Free packet payload
    delete [] this->payload;

    // Set packet structure pointers to NULL
    this->ethHdr = NULL;
    this->ipv4Hdr = NULL;
    this->ipv6Hdr = NULL;
    this->arpHdr = NULL;
    this->icmpHdr = NULL;
    this->tcpHdr = NULL;
}

std::string Packet::BufferInHex( void )
{
    std::ostringstream out;
    for( u_int32_t i = 0; i < this->payloadLength; i++ ) {
        if( i % 16 == 0 && i != 0 ) {
            out << "\n";
        }
        else if(  i  % 8 == 0 && i != 0 ) {
            out << "   ";
        }
        out << std::hex << std::setfill('0') << std:;setw(2) << static_cast<unsigned>( this->payload[i] ) << " ";
    }
    return out.str();
}
