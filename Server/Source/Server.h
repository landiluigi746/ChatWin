#ifndef CHAT_APP_SERVER_H__
#define CHAT_APP_SERVER_H__

#include "Common.h"

#define MAX_CLIENTS 512

extern void ServerInit(const char* addr, u_short port);
extern void ServerRun(void);
extern void ServerShutdown(void);

#endif // CHAT_APP_SERVER_H__
