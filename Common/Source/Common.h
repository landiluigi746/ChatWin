#ifndef CHAT_APP_COMMON_H__
#define CHAT_APP_COMMON_H__

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define WIN32_LEAN_AND_MEAN

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <Windows.h>
#include <WinSock2.h>
#include <WS2tcpip.h>

#define LOG_INFO "INFO"
#define LOG_WARNING "WARNING"
#define LOG_ERROR "ERROR"

#define CHAT_CMD_EXIT "/exit"

#define BUF_SIZE 128

extern void VA_Log(const char* tag, const char* msg, va_list args);
extern void Log(const char* tag, const char* msg, ...);
extern void CloseOnError(BOOL condition, BOOL cleanup, const char* msg, ...);
extern void CloseOnSocketError(BOOL condition, SOCKET sock, const char* msg, ...);

extern void InitWSA(void);
extern SOCKET SocketCreate(void);
extern SOCKADDR_IN ServiceCreate(const char* addr, u_short port);

extern void Wait(clock_t milliseconds);
extern size_t StrInput(char* buffer, size_t capacity);

extern void ResizeConsoleWindow(int width, int height);

#endif // CHAT_APP_COMMON_H__