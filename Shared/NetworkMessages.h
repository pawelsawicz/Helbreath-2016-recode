#ifndef	NETMESSAGES_H
#define NETMESSAGES_H
#pragma	once

#include "TypeDefs.h"
#include "version.h"

enum NetMessages_CommonType
{
	MSGID_TEST_MSG = 0x0A01,
	MSGID_RESPONSE_LOG,
	MSGID_REQUEST_CREATENEWACCOUNT,
	LOGRESMSGTYPE_NEWACCOUNTCREATED,
	LOGRESMSGTYPE_ALREADYEXISTINGACCOUNT,
	MSGTYPE_CONFIRM,
	MSGTYPE_REJECT,
	MAX_MSG
};

#endif