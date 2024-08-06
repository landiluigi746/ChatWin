#include "Client.h"
#include <curses.h>

typedef struct
{
	SOCKET socket;
	BOOL connected;
	SOCKADDR_IN service;
	HANDLE threads[2];
	DWORD threadsId[2];
	char username[BUF_SIZE];
} Client;

static Client client = { 0 };

static WINDOW* titleWindow = NULL;
static WINDOW* chatWindow = NULL;
static WINDOW* inputWindow = NULL;

static void UIInit(void);

static DWORD WINAPI Send(LPVOID unused);
static DWORD WINAPI Receive(LPVOID unused);

static BOOL WINAPI Close(DWORD ctrlType);

void ClientInit(const char* addr, u_short port)
{
	int recvCount;
	char buffer[BUF_SIZE] = { 0 };

	ResizeConsoleWindow(800, 600);
	SetConsoleTitleA("ChatWin - Client");
	SetConsoleCtrlHandler(&Close, TRUE);

	UIInit();
	InitWSA();

	client.socket = SocketCreate();
	client.service = ServiceCreate(addr, port);

	CloseOnSocketError(connect(client.socket, (SOCKADDR*)&client.service, sizeof(client.service)) == SOCKET_ERROR, client.socket, "ClientInit(): connect() failed");

	Log(LOG_INFO, "ClientInit(): connect() to %s:%hu done successfully!", inet_ntoa(client.service.sin_addr), ntohs(client.service.sin_port));

	recvCount = recv(client.socket, buffer, BUF_SIZE, 0);
	
	if (recvCount > 0)
	{
		waddstr(chatWindow, buffer);
		wrefresh(chatWindow);

		wgetnstr(inputWindow, client.username, BUF_SIZE - 1);
		wclear(inputWindow);
		
		waddstr(chatWindow, client.username);
		wrefresh(chatWindow);

		send(client.socket, client.username, (int)strlen(client.username), 0);

		wprintw(titleWindow, " | %s:%hu", inet_ntoa(client.service.sin_addr), ntohs(client.service.sin_port));
		wrefresh(titleWindow);

		wclear(chatWindow);
		waddstr(chatWindow, "Welcome to ChatWin!");
		wrefresh(chatWindow);
	}
	else
		CloseOnSocketError(TRUE, client.socket, "ClientInit(): Welcome message not received from server!");

	client.connected = TRUE;

	return;
}

void ClientRun(void)
{
	client.threads[0] = CreateThread(NULL, 0, &Send, NULL, 0, &client.threadsId[0]);
	client.threads[1] = CreateThread(NULL, 0, &Receive, NULL, 0, &client.threadsId[1]);

	CloseOnSocketError(client.threads[0] == NULL || client.threads[1] == NULL, client.socket, "ClientRun(): Failed to create threads!");

	WaitForMultipleObjects(2, client.threads, TRUE, INFINITE);

	return;
}

void ClientShutdown(void)
{
	delwin(titleWindow);
	delwin(chatWindow);
	delwin(inputWindow);
	endwin();

	CloseHandle(client.threads[0]);
	CloseHandle(client.threads[1]);

	closesocket(client.socket);
	WSACleanup();

	return;
}

void UIInit(void)
{
	initscr();

	titleWindow = newwin(2, COLS, 0, 0);
	chatWindow = newwin(LINES - 6, COLS, 3, 0);
	inputWindow = newwin(1, COLS, LINES - 2, 0);

	scrollok(chatWindow, TRUE);

	refresh();

	waddstr(titleWindow, "ChatWin - Client");
	wrefresh(titleWindow);

	return;
}

DWORD WINAPI Send(LPVOID unused)
{
	char buffer[BUF_SIZE] = { 0 };

	while (client.connected)
	{
		ZeroMemory(buffer, BUF_SIZE);
		wgetnstr(inputWindow, buffer, BUF_SIZE - 1);
		wclear(inputWindow);

		if (buffer[0] == '\0')
			continue;

		Wait(10);

		wprintw(chatWindow, "\n[You]: %s", buffer);
		wrefresh(chatWindow);
		send(client.socket, buffer, (int)strlen(buffer), 0);

		if (strcmp(buffer, CHAT_CMD_EXIT) == 0)
			client.connected = FALSE;
	}

	return TRUE;
}

DWORD WINAPI Receive(LPVOID unused)
{
	char buffer[BUF_SIZE * 2 + 5] = { 0 };
	int recvCount;

	while (client.connected)
	{
		ZeroMemory(buffer, BUF_SIZE * 2 + 5);
		recvCount = recv(client.socket, buffer, BUF_SIZE * 2 + 5, 0);

		if (recvCount > 0)
		{
			wprintw(chatWindow, "\n%s", buffer);
			wrefresh(chatWindow);
		}

		Wait(10);
	}

	return TRUE;
}

BOOL WINAPI Close(DWORD ctrlType)
{
	switch (ctrlType)
	{
		case CTRL_CLOSE_EVENT:
		case CTRL_LOGOFF_EVENT:
		case CTRL_SHUTDOWN_EVENT:
			send(client.socket, CHAT_CMD_EXIT, strlen(CHAT_CMD_EXIT), 0);
			client.connected = FALSE;
			ClientShutdown();

			return TRUE;
		default:
			break;
	}

	return FALSE;
}