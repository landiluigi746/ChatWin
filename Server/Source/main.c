#include "Server.h"

#pragma comment(lib, "Ws2_32.lib")

int main(const int argc, const char* argv[])
{
    if (argc < 3)
    {
        printf("Usage: .\\Server.exe address port\n");
        printf("Example: .\\Server.exe 127.0.0.1 27015\n");
        exit(0);
    }

    char* endStr;
    u_short port = (u_short) strtoul(argv[2], &endStr, 0);

    if (port == 0 || endStr == argv[2])
    {
        printf("Couldn't convert port %s to an unsigned short\n", argv[2]);
        exit(0);
    }

    ServerInit(argv[1], port);
    ServerRun();
    ServerShutdown();

    return 0;
}