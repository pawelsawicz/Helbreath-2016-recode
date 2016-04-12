// LoginServer.cpp : Defines the entry point for the application.
//
#include "cMain.h"
#include "Application.h"

#define MAX_LOADSTRING 100

// Global Variables:

HWND hWnd;
HINSTANCE hInst;								// current instance
HINSTANCE       G_hInstance = NULL;
MMRESULT G_mmTimer;
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
void Initialize(char * pCmdLine, HINSTANCE hInst);
void CALLBACK _TimerFunc(UINT wID, UINT wUser, DWORD dwUSer, DWORD dw1, DWORD dw2);
MMRESULT _StartTimer(DWORD dwTime);
void _StopTimer(MMRESULT timerid);

char			G_cMsgList[120 * 50];
BOOL            G_cMsgUpdated = FALSE;

extern int G_iAddTable31[64][510], G_iAddTable63[64][510];
extern int G_iAddTransTable31[510][64], G_iAddTransTable63[510][64];


class cClient Client = NULL;
class cLoginServer LoginServer;
//class cMain * Main = NULL;
class cMain Main;
class cSocket Socket;

extern "C" __declspec(dllimport) int __FindHackingDll__(char *);


int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: Place code here.

	HACCEL hAccelTable;

	HINSTANCE hDll;
	char cSearchDll[] = "rd`qbg-ckk";
	char cRealName[12];

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_CLIENT, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}
	Initialize((char *)lpCmdLine, hInstance);

	srand((unsigned)time(NULL));
	char *pJammer = new char[(rand() % 100) + 1];
	//Main = new class cMain;
	ZeroMemory(cRealName, sizeof(cRealName));
	strcpy(cRealName, cSearchDll);
	for (WORD i = 0; i < strlen(cRealName); i++)
	if (cRealName[i] != NULL) cRealName[i]++;

	hDll = LoadLibrary(cRealName);
	if (hDll == NULL)
	{
		MessageBox(NULL, "don't find search.dll", "ERROR!", MB_OK);
		return 0;
	}

	typedef int (MYPROC)(char *);
	MYPROC *pFindHook;
	pFindHook = (MYPROC *)GetProcAddress(hDll, "__FindHackingDll__");

	if (pFindHook == NULL)
	{
		MessageBox(NULL, "can't find search.dll", "ERROR!", MB_OK);
		return 0;
	}
	else if ((*pFindHook)("CRCCHECK") != 1)
	{
		return 0;
	}
	FreeLibrary(hDll);

	//Main.StartUp();

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_CLIENT));

	// Main message loop:
	/*
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			Main.UpdateScreen();
		}
		//else if (Main.m_bIsProgramActive) Main.UpdateScreen();
	 if(Main.m_cGameMode == GAMEMODE_ONLOADING) Main.UpdateScreen_OnLoading(FALSE);
		else WaitMessage();
		
	}

	return (int) msg.wParam;
	*/
	EventLoop();
	return 0;
}
void EventLoop()
{
	MSG msg;
	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
		{
			if (!GetMessage(&msg, NULL, 0, 0)) return;// msg.wParam;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else if (Main.m_bIsProgramActive) Main.UpdateScreen();
		else if (Main.m_cGameMode == GAMEMODE_ONLOADING) Main.UpdateScreen_OnLoading(FALSE);
		else WaitMessage();
	}
}
void Initialize(char * pCmdLine, HINSTANCE hInst)
{
	int iX, iY, iSum;
	
	if (Main.bInit(hWnd, hInst, pCmdLine) == FALSE)
	{
		PostQuitMessage(0);
		return;
	}
	G_mmTimer = _StartTimer(1000);
	for (iX = 0; iX < 64; iX++)
	for (iY = 0; iY < 510; iY++)
	{
		iSum = iX + (iY - 255);
		if (iSum <= 0)  iSum = 1;
		if (iSum >= 31) iSum = 31;
		G_iAddTable31[iX][iY] = iSum;
		iSum = iX + (iY - 255);
		if (iSum <= 0)  iSum = 1;
		if (iSum >= 63) iSum = 63;
		G_iAddTable63[iX][iY] = iSum;
		if ((iY - 255) < iX) G_iAddTransTable31[iY][iX] = iX;
		else if ((iY - 255) > 31) G_iAddTransTable31[iY][iX] = 31;
		else G_iAddTransTable31[iY][iX] = iY - 255;
		if ((iY - 255) < iX) G_iAddTransTable63[iY][iX] = iX;
		else if ((iY - 255) > 63) G_iAddTransTable63[iY][iX] = 63;
		else G_iAddTransTable63[iY][iX] = iY - 255;
	}
}

void UpdateScreen()
{
	if (G_cMsgUpdated == TRUE) {
		InvalidateRect(hWnd, NULL, TRUE);
		G_cMsgUpdated = FALSE;
	}
}

//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDC_CLIENT));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName = MAKEINTRESOURCE(IDC_CLIENT);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	int cx = GetSystemMetrics(SM_CXFULLSCREEN) / 2;
	int cy = GetSystemMetrics(SM_CYFULLSCREEN) / 2;
	if (cy>280) cy -= 40;
   hInst = hInstance; // Store instance handle in our global variable

   //hWnd = CreateWindow(szWindowClass, szTitle, WS_POPUP, cx - 320, cy - 240,
	//   640, 480, NULL, NULL, hInstance, NULL);

   hWnd = CreateWindowEx(NULL, szWindowClass, szTitle, WS_POPUP, cx - 320, cy - 240,
		   640, 480, NULL, NULL, hInstance, NULL);
   if (!hWnd)
   {
      return FALSE;
   }
   G_hInstance = hInstance;
   ShowWindow(hWnd, nCmdShow);//SW_SHOWDEFAULT
   UpdateWindow(hWnd);
   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;

	switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_SETCURSOR:
		SetCursor(NULL);
		return TRUE;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_KEYUP:
		Main.OnKeyUp(wParam, lParam);
		break;

	case WM_USER_TIMERSIGNAL:
		Main.OnTimer();
		break;

	case WM_LOGIN_SOCKET:
	case WM_CLIENT_SOCKET:
		Main.SocketsMessages(message, wParam, lParam);
		break;
	case WM_ACTIVATEAPP:
		if (wParam == 0)
		{
			Main.m_bIsProgramActive = FALSE;
			Main.m_DInput.SetAcquire(FALSE);
		}
		else
		{
			Main.m_bIsProgramActive = TRUE;
			Main.m_DInput.SetAcquire(TRUE);
			Main.m_bCtrlPressed = FALSE;

			if (Main.bCheckImportantFile() == FALSE)
			{
				MessageBox(Main.m_hWnd, "File checksum error! Get Update again please!", "ERROR1", MB_ICONEXCLAMATION | MB_OK);
				PostQuitMessage(0);
				return 0;
			}
			if (__FindHackingDll__("CRCCHECK") != 1)
			{
				Main.ChangeGameMode(GAMEMODE_ONQUIT);
				return NULL;
			}
		}
		return DefWindowProc(hWnd, message, wParam, lParam);
	
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
void DisplayInLogList(char * cMsg)
{
	char cTemp[120 * 50];

	G_cMsgUpdated = TRUE;
	ZeroMemory(cTemp, sizeof(cTemp));
	memcpy((cTemp + 120), G_cMsgList, 120 * 49);
	memcpy(cTemp, cMsg, strlen(cMsg));
	memcpy(G_cMsgList, cTemp, 120 * 50);
	LogFile(cMsg);
}
void LogFile(char * cStr)
{
	FILE * pFile;
	char cBuffer[512];
	SYSTEMTIME SysTime;

	pFile = fopen("Logs\\Login.log", "at");
	if (pFile == NULL) return;
	ZeroMemory(cBuffer, sizeof(cBuffer));
	GetLocalTime(&SysTime);
	wsprintf(cBuffer, "(%4d:%2d:%2d:%2d:%2d) - ", SysTime.wYear, SysTime.wMonth, SysTime.wDay, SysTime.wHour, SysTime.wMinute);
	strcat(cBuffer, cStr);
	strcat(cBuffer, "\n");
	fwrite(cBuffer, 1, strlen(cBuffer), pFile);
	fclose(pFile);
}
void CALLBACK _TimerFunc(UINT wID, UINT wUser, DWORD dwUSer, DWORD dw1, DWORD dw2)
{
	PostMessage(hWnd, WM_USER_TIMERSIGNAL, wID, NULL);
}


MMRESULT _StartTimer(DWORD dwTime)
{
	TIMECAPS caps;
	timeGetDevCaps(&caps, sizeof(caps));
	timeBeginPeriod(caps.wPeriodMin);
	return timeSetEvent(dwTime, 0, _TimerFunc, 0, (UINT)TIME_PERIODIC);
}


void _StopTimer(MMRESULT timerid)
{
	TIMECAPS caps;
	if (timerid != 0)
	{
		timeKillEvent(timerid);
		timerid = 0;
		timeGetDevCaps(&caps, sizeof(caps));
		timeEndPeriod(caps.wPeriodMin);
	}
}
