#include "Client.h"

typedef struct
{
	SOCKET socket;
	SOCKADDR_IN service;
	HANDLE threads[2];
	DWORD threadsId[2];
	char username[BUF_SIZE];
} Client;

static Client client = { 0 };

static DWORD WINAPI Send(LPVOID unused);
static DWORD WINAPI Receive(LPVOID unused);

void ClientInit(const char* addr, u_short port)
{
	int recvCount;
	size_t len;
	char buffer[BUF_SIZE] = { 0 };

	InitWSA();

	client.socket = SocketCreate();
	client.service = ServiceCreate(addr, port);

	CloseOnSocketError(connect(client.socket, (SOCKADDR*)&client.service, sizeof(client.service)) == SOCKET_ERROR, client.socket, "ClientInit(): connect() failed");

	Log(LOG_INFO, "ClientInit(): connect() to %s:%hu done successfully!", inet_ntoa(client.service.sin_addr), ntohs(client.service.sin_port));

	recvCount = recv(client.socket, buffer, BUF_SIZE, 0);
	
	if (recvCount > 0)
	{
		printf("%s", buffer);
		len = StrInput(client.username, BUF_SIZE);
		send(client.socket, client.username, (int)strlen(client.username), 0);
	}
	else
		CloseOnSocketError(TRUE, client.socket, "The server encountered an error while receiving username!");

	system("cls");

	return;
}

void ClientRun(void)
{
	client.threads[0] = CreateThread(NULL, 0, &Send, NULL, 0, &client.threadsId[0]);
	client.threads[1] = CreateThread(NULL, 0, &Receive, NULL, 0, &client.threadsId[1]);

	CloseOnSocketError(client.threads[0] == NULL || client.threads[1] == NULL, client.socket, "ClientRun(): Failed to create threads!");

	WaitForMultipleObjects(2, client.threads, TRUE, INFINITE);

	CloseHandle(client.threads[0]);
	CloseHandle(client.threads[1]);

	return;
}

void ClientShutdown(void)
{
	closesocket(client.socket);
	WSACleanup();

	return;
}

DWORD WINAPI Send(LPVOID unused)
{
	char buffer[BUF_SIZE] = { 0 };
	size_t len;

	while (TRUE)
	{
		len = StrInput(buffer, BUF_SIZE);

		if (buffer[0] == '\0')
			continue;

		Wait(10);

		send(client.socket, buffer, (int)len, 0);
	}

	return TRUE;
}

DWORD WINAPI Receive(LPVOID unused)
{
	char buffer[BUF_SIZE * 2 + 5] = { 0 };
	int recvCount;

	while (TRUE)
	{
		ZeroMemory(buffer, BUF_SIZE * 2 + 5);
		recvCount = recv(client.socket, buffer, BUF_SIZE * 2 + 5, 0);

		if (recvCount > 0)
		{
			printf("%s\n", buffer);
		}

		Wait(10);
	}

	return TRUE;
}