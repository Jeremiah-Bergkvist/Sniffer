#ifndef __INTERFACE_H__
#define __INTERFACE_H__

#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <vector>
#include <sstream>
#include <errno.h>              // Error nums and messages
#include <netinet/if_ether.h>   // ETH_P_ALL
#include <net/if.h>             // Interface stuff
#include <sys/ioctl.h>          // Change interface properties
#include <linux/if_packet.h>    // sockaddr_ll

#include "Packet.h"

class Interface{
public:
    Interface( std::string device );
    ~Interface( void );

    void ShowCapture( void );
    bool RecvPacket( void );
    Packet *GetLastPacket( void );
    std::vector<Packet *> GetAllPackets( void );

private:
    std::string device;
    unsigned char data[65536];
    int sockFd;
    std::vector<Packet*> packets;
};

#endif // __INTERFACE_H__
