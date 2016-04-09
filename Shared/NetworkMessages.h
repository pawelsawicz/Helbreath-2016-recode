#ifndef	NETMESSAGES_H
#define NETMESSAGES_H
#pragma	once

#include "TypeDefs.h"
#include "version.h"

enum NetMessages_CommonType
{
	MSGID_TEST_MSG = 0x0A01,
	MSGTYPE_CONFIRM,
	MSGTYPE_REJECT,
	MAX_MSG
};

#endif