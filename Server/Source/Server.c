#include "Server.h"

typedef struct
{
    SOCKET socket;
    UINT64 id;
    SOCKADDR_IN addr;
    char username[BUF_SIZE];
    char buffer[BUF_SIZE];
    HANDLE thread;
    DWORD threadId;
} Client;

typedef struct
{
    SOCKET socket;
    SOCKADDR_IN service;
    Client clients[MAX_CLIENTS];
    size_t clientsNum;
} Server;

static Server server = { 0 };

static DWORD WINAPI HandleClient(LPVOID unused);
static void SendMessageToAll(UINT64 senderId, const char* senderName, char* msg);

void ServerInit(const char* addr, u_short port)
{
    InitWSA();

    server.socket = SocketCreate();
    server.service = ServiceCreate(addr, port);

    CloseOnSocketError(bind(server.socket, (SOCKADDR*)&server.service, sizeof(server.service)) == SOCKET_ERROR, server.socket, "ServerInit(): bind() failed!");
    Log(LOG_INFO, "ServerInit(): bind() to %s:%hu done successfully!", inet_ntoa(server.service.sin_addr), ntohs(server.service.sin_port));

    CloseOnSocketError(listen(server.socket, SOMAXCONN) == SOCKET_ERROR, server.socket, "ServerInit(): listen() failed!");
    Log(LOG_INFO, "ServerInit(): listen() done successfully!");

    return;
}

void ServerRun(void)
{
    Client newClient = { 0 };
    int addrSize = sizeof(newClient.addr);

    while (TRUE)
    {
        newClient.socket = accept(server.socket, (SOCKADDR*)&newClient.addr, &addrSize);

        if (newClient.socket == INVALID_SOCKET)
            continue;

        if (server.clientsNum >= MAX_CLIENTS)
        {
            Log(LOG_WARNING, "Can't accept new connection because the server is full!");
            continue;
        }

        server.clients[server.clientsNum++] = newClient;

        newClient.id = server.clientsNum;
        newClient.thread = CreateThread(NULL, 0, &HandleClient, NULL, 0, &newClient.threadId);

        if (newClient.thread == NULL)
            Log(LOG_WARNING, "Unable to create new thread for client");
        else
        {
            Log(LOG_INFO, "New client connected!");
            CloseHandle(newClient.thread);
        }
    }

    return;
}

void ServerShutdown(void)
{
    closesocket(server.socket);
    WSACleanup();

    return;
}

DWORD WINAPI HandleClient(LPVOID unused)
{
    Client client = server.clients[server.clientsNum - 1];
    int recvCount;

    const char welcomeMsg[] = "You connected successfully to the server! Enter your username: ";
    send(client.socket, welcomeMsg, sizeof(welcomeMsg) - 1, 0);

    recvCount = recv(client.socket, client.username, BUF_SIZE, 0);

    if (recvCount > 0)
    {
        Log(LOG_INFO, "%s connected!", client.username);
    }
    else if (recvCount == 0)
    {
        Log(LOG_INFO, "Unknown client disconnected!");
        return TRUE;
    }

    while (TRUE)
    {
        ZeroMemory(client.buffer, BUF_SIZE);
        recvCount = recv(client.socket, client.buffer, BUF_SIZE, 0);

        if (recvCount > 0)
        {
            if (strcmp(client.buffer, "/exit") == 0)
                break;

            Log(LOG_INFO, "%s says: %s", client.username, client.buffer);
            SendMessageToAll(client.id, client.username, client.buffer);
        }
        else if (recvCount == 0)
            break;
    }

    closesocket(client.socket);
    Log(LOG_INFO, "%s disconnected!", client.username);

    return TRUE;
}

void SendMessageToAll(UINT64 senderId, const char* senderName, const char* msg)
{
    size_t i;
    char buffer[BUF_SIZE * 2 + 5];
    int len = sprintf_s(buffer, BUF_SIZE * 2 + 5, "[%s]: %s", senderName, msg);

    for (i = 0; i < server.clientsNum; i++)
        if(server.clients[i].id != senderId)
            send(server.clients[i].socket, buffer, len, 0);

    return;
}
