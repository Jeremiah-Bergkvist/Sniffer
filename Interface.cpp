#include "Interface.h"

Interface::Interface( std::string device )
{
    // Needed for promiscuous mode
    struct ifreq ifopts;

    // Socket file descriptor
    this->sockFd = 0;

    // Create raw socket
    this->sockFd = socket( AF_PACKET, SOCK_RAW, htons( ETH_P_ALL) );
    if( sockFd < 0 ) {
        std::cerr << "Error creating raw socket. Probably not root.\n";
        exit( 1 );
    }

    // Read the current interface flags
    strncpy(ifopts.ifr_name, device.c_str(), IFNAMSIZ);
    if( ioctl( this->sockFd, SIOCGIFFLAGS, &ifopts ) == -1 ) {
        std::cerr << "Error reading interface (" + device + ") flags.\n";
        exit( 2 );
    }

    // Set interface to promiscuous mode
    ifopts.ifr_flags |= IFF_PROMISC;
    if( ioctl( this->sockFd, SIOCSIFFLAGS, &ifopts ) == -1 ) {
        std::cerr << "Error setting promiscuous flag on interface (" + device + ").\n";
        exit( 3 );
    }

    // Set socket to be reusable
    int sockopt = 0;
    if( setsockopt( this->sockFd, SOL_SOCKET, SO_REUSEADDR, &sockopt, sizeof(sockopt) ) == -1 ) {
        std::cerr << "Cannot set socket to be reusable.\n";
        exit( 4 );
    }
}

Interface::~Interface( void )
{
    for( unsigned int i = 0; i < this->packets.size(); i++ ) {
        delete this->packets.at( i );
    }
}

bool Interface::RecvPacket( void )
{
    // Stores address info about packet received
    struct sockaddr_ll src_addr = { 0 };
    socklen_t addr_len = sizeof( struct sockaddr_ll );

    // Receive a packet
    int data_size = recvfrom( this->sockFd, this->data, sizeof( this->data ), 0, ( struct sockaddr* ) &src_addr, &addr_len );
    if( data_size < 0 ) {
        std::cerr << strerror( errno );
        return false;
    }

    // Add new packet to the vector of existing packets
    this->packets.push_back( new Packet( this->data, data_size ) );

    return true;
}

Packet *Interface::GetLastPacket( void )
{
    return this->packets.back();
}

std::vector<Packet*> Interface::GetAllPackets( void )
{
    return this->packets;
}
