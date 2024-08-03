#ifndef CHAT_APP_CLIENT_H__
#define CHAT_APP_CLIENT_H__

#include "Common.h"

extern void ClientInit(const char* addr, u_short port);
extern void ClientRun(void);
extern void ClientShutdown(void);

#endif // !CHAT_APP_CLIENT_H__
