#include "Common.h"

void VA_Log(const char* tag, const char* msg, va_list args)
{
	printf("[%s] ", tag);

	vprintf_s(msg, args);

	printf("\n");
}

void Log(const char* tag, const char* msg, ...)
{
	va_list args;

	va_start(args, msg);
	VA_Log(tag, msg, args);
	va_end(args);

	return;
}

void CloseOnError(BOOL condition, BOOL cleanup, const char* msg, ...)
{
	if (!condition)
		return;

	va_list args;

	va_start(args, msg);
	VA_Log(LOG_ERROR, msg, args);
	va_end(args);

	if (cleanup)
		WSACleanup();

	exit(EXIT_FAILURE);

	return;
}

void CloseOnSocketError(BOOL condition, SOCKET sock, const char* msg, ...)
{
	if (!condition)
		return;

	va_list args;

	va_start(args, msg);
	VA_Log(LOG_ERROR, msg, args);
	va_end(args);

	closesocket(sock);
	WSACleanup();

	exit(EXIT_FAILURE);

	return;
}

void InitWSA(void)
{
	WSADATA wsaData;

	CloseOnError(WSAStartup(WINSOCK_VERSION, &wsaData) != 0, FALSE, "InitWSA(): WSAStartup() failed!");

	return;
}

SOCKET SocketCreate(void)
{
	SOCKET sock = INVALID_SOCKET;
	sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	CloseOnSocketError(sock == INVALID_SOCKET, sock, "SocketCreate(): socket() failed!");

	return sock;
}

SOCKADDR_IN ServiceCreate(const char* addr, u_short port)
{
	SOCKADDR_IN service;
	service.sin_family = AF_INET;
	service.sin_port = htons(port);
	service.sin_addr.s_addr = inet_addr(addr);

	return service;
}

void Wait(clock_t milliseconds)
{
	clock_t start = clock();
	while (clock() - start < milliseconds);
	return;
}

size_t StrInput(char* buffer, size_t capacity)
{
	int c;
	size_t count = 0;

	while ((c = getchar()) != EOF && c != '\n' && count < capacity - 1)
		*(buffer + count++) = (char)c;

	*(buffer + count) = '\0';

	return count;
}