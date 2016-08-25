#inlcude "Sniffer.h"

int main( int argc, char *argv[] )
{
    Interface interface("lo");

    while( i < 4 ) {
        if( interface.RecvPacket() ) {
            std::cout << interface.GetLastPacket()->BufferInHex() << "\n";
        }
    }

    return 0;
}
