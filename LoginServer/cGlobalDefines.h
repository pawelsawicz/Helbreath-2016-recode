#ifndef _cGlobalDefines_H
#define _cGlobalDefines_H

#define MAXCLIENTSOCKETS	1000
#define MAXMAPSOCKETS		10
#define MAXMAPLIST			40

#define WM_USER_TIMERSIGNAL		WM_USER + 10
#define WM_MAP_SOCKET			WM_USER + 100 
#define WM_CLIENT_SOCKET		WM_USER + 300 


#define MAXALLOWEDQUERYERROR		10

typedef char _ADDRESS[20];
typedef char CONFIG[50000];

typedef signed char     sBYTE;
typedef signed short    sWORD;
typedef signed long     sDWORD;



#endif