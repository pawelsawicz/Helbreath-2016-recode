// LoginServer.cpp : Defines the entry point for the application.
//
#include "cMain.h"
#include "Application.h"

#define MAX_LOADSTRING 100

// Global Variables:
HWND hWnd;
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

char			G_cMsgList[120 * 50];
BOOL            G_cMsgUpdated = FALSE;

class cClient Client = NULL;
class cLoginServer LoginServer;
//class cMain * Main = NULL;
class cMain Main;

class cMapServer MapServer = NULL;
class cSocket Socket;

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_LOGINSERVER, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	Main.StartUp();

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LOGINSERVER));

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			UpdateScreen();
		}
	}

	return (int) msg.wParam;
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
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LOGINSERVER));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_LOGINSERVER);
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
	//HWND hWnd;

   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
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
	int wmId, wmEvent, i;
	PAINTSTRUCT ps;
	HDC hdc;
	char * cMsg;

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
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		SetBkMode(hdc, TRANSPARENT);

		for (i = 0; i < 20; i++) {
			cMsg = (char *)(G_cMsgList + i * 120);
			TextOut(hdc, 5, 5 + 350 - i * 16, cMsg, strlen(cMsg));
		}
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_KEYUP:
		Main.OnKeyUp(wParam, lParam);
		break;

	case WM_MAP_SOCKET:
	case WM_CLIENT_SOCKET:
		Main.SocketsMessages(message, wParam, lParam);
		break;

	default:

		if ((message >= WM_MAP_SOCKET) && (message < WM_MAP_SOCKET + MAXMAPSOCKETS))
			Main.SocketsMessages(message, wParam, lParam);

		else if ((message >= WM_CLIENT_SOCKET) && (message < WM_CLIENT_SOCKET + MAXCLIENTSOCKETS))
			Main.SocketsMessages(message, wParam, lParam);

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