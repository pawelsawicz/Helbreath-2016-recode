#ifndef _ABOUTDLGPROC_H
#define _ABOUTDLGPROC_H

#define _WINSOCKAPI_    // stops windows.h including winsock.h
#include <windows.h>


int		G_iLevel;
char	G_cWorldServerName[32];
char	G_cPlayerName[12];
char	G_cMapName[11];
char	G_cMapMessage[30];

#endif