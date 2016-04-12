#include "cMain.h"

char   G_cSpriteAlphaDegree;
int G_iAddTable31[64][510], G_iAddTable63[64][510];
int G_iAddTransTable31[510][64], G_iAddTransTable63[510][64];

long G_lTransG100[64][64], G_lTransRB100[64][64];
long G_lTransG70[64][64], G_lTransRB70[64][64];
long G_lTransG50[64][64], G_lTransRB50[64][64];
long G_lTransG25[64][64], G_lTransRB25[64][64];
long G_lTransG2[64][64], G_lTransRB2[64][64];

char G_cCmdLine[256], G_cCmdLineTokenA[120], G_cCmdLineTokenA_Lowercase[120], G_cCmdLineTokenB[120], G_cCmdLineTokenC[120], G_cCmdLineTokenD[120], G_cCmdLineTokenE[120];

char _cDrawingOrder[] = { 0, 1, 0, 0, 0, 0, 0, 1, 1 };
char _cMantleDrawingOrder[] = { 0, 1, 1, 1, 0, 0, 0, 2, 2 };
char _cMantleDrawingOrderOnRun[] = { 0, 1, 1, 1, 1, 1, 1, 1, 1 };

short _tmp_sOwnerType, _tmp_sAppr1, _tmp_sAppr2, _tmp_sAppr3, _tmp_sAppr4;
int _tmp_iStatus;
char  _tmp_cAction, _tmp_cDir, _tmp_cFrame, _tmp_cName[12];
int   _tmp_iChatIndex, _tmp_dx, _tmp_dy, _tmp_iApprColor, _tmp_iEffectType, _tmp_iEffectFrame, _tmp_dX, _tmp_dY; 
WORD  _tmp_wObjectID;
char cDynamicObjectData1, cDynamicObjectData2, cDynamicObjectData3, cDynamicObjectData4;
WORD  wFocusObjectID;
short sFocus_dX, sFocus_dY;
char  cFocusAction, cFocusFrame, cFocusDir, cFocusName[12];
short sFocusX, sFocusY, sFocusOwnerType, sFocusAppr1, sFocusAppr2, sFocusAppr3, sFocusAppr4;
int iFocuiStatus;
int   iFocusApprColor;

char __cSpace2[] = { 6, 4, 6, 6, 6, 6, 6, 6, 6, 6, 6 };
char __cSpace[] = { 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 6, 8, 7, 8, 8, 9, 10, 9, 7, 8, 8, 8, 8, 8, 8, 8,
15, 16, 12, 17, 14, 15, 14, 16, 10, 13, 19, 10, 17, 17, 15, 14, 15, 16, 13, 17, 16, 16, 20, 17, 16, 14,
8, 8, 8, 8, 8, 8, 8, 6, 7, 8, 7, 7, 7, 7, 4, 7, 7, 4, 11, 7, 8, 8, 7, 8, 6, 5, 8, 9, 14, 8, 9, 8, 8, 8, 8, 8,
8, 8, 8, 8, 8, 8, 8 };
int _iAttackerHeight[] = { 0, 35, 35, 35, 35, 35, 35, 0, 0, 0,
5,  // Slime
35, // Skeleton
40, // Stone-Golem
45, // Cyclops
35,// OrcMage
35,// ShopKeeper
5, // GiantAnt
8, // Scorpion
35,// Zombie
35,// Gandalf
35,// Howard
35,// Guard
10,// Amphis
38,// Clay-Golem
35,// Tom
35,// William
35,// Kennedy
35,// Hellhound
50,// Troll
45,// Orge
55,// Liche
65,// Demon
46,// Unicorn
49,// WereWolf
55,// Dummy
35,// Energysphere
75,// Arrow Guard Tower
75,// Cannon Guard Tower
50,// Mana Collector
50,// Detector
50,// Energy Shield Generator
50,// Grand Magic Generator
50,// ManaStone 42
40,// Light War Beetle
35,// GHK
40,// GHKABS
35,// TK
60,// BG
40,// Stalker
70,// HellClaw
85,// Tigerworm
50,// Catapult
85,// Gargoyle
70,// Beholder
40,// Dark-Elf
20,// Bunny
20,// Cat
40,// Giant-Frog
80,// Mountain-Giant
85,// Ettin
50,// Cannibal-Plant
50, // Rudolph 61 //Snoopy....
80, // Direboar 62
90, // Frost 63
40, // Crops 64
80, // IceGolem 65
190, // Wyvern 66
35, // npc 67
35, // npc 68
35, // npc 69
100, // Dragon 70
90, // Centaur 71
75, // ClawTurtle 72
200, // FireWyvern 73
80, // GiantCrayfish 74
120, // Gi Lizard 75
100, // Gi Tree 76
100, // Master Orc 77
80, // Minaus 78
100, // Nizie 79
25,  // Tentocle 80
200, // Abaddon	 81
60, // Sorceress 82
60, // ATK 83
70, // MasterElf 84
60, // DSK 85
50, // HBT 86
60, // CT 87
60, // Barbarian 88
60, // AGC 89
35, // ncp 90 Gail
35  // Gate 91
};
cMain::cMain()
{
	// Creates long-lived sockets by assigning them dynamic memory.
	G_pListenSock = new cSocket;
	G_pListenSock->Initialize(); // Setup winsock
	G_pLoginSock = new struct cLoginSocket();
	G_pClientSock = new struct cClientSocket();
	for (int i = 0; i < MAXCLIENTSOCKETS; i++) {
		G_pClientSock->acceptedSockets[i] = NULL;
	}

	//
	m_cGameMode = GAMEMODE_ONLOADING;
}


cMain::~cMain()
{
	delete G_pListenSock;
	delete G_pLoginSock;
	delete G_pClientSock;
}
bool cMain::bInit(HWND hWnd, HINSTANCE hInst, char * pCmdLine)
{
	int iIndex;
	int i;
	class CStrTok * pStrTok;
	char seps[] = "&= ,\t\n";
	char * token;
	// CLEROTH - BUG
	for (i = 0; i < MAXSPRITES; i++)
		m_pSprite[i] = NULL;
	if (pCmdLine != NULL)
	{
		ZeroMemory(G_cCmdLine, sizeof(G_cCmdLine));
		ZeroMemory(G_cCmdLineTokenA, sizeof(G_cCmdLineTokenA));
		ZeroMemory(G_cCmdLineTokenB, sizeof(G_cCmdLineTokenB));
		ZeroMemory(G_cCmdLineTokenC, sizeof(G_cCmdLineTokenC));
		ZeroMemory(G_cCmdLineTokenD, sizeof(G_cCmdLineTokenD));
		ZeroMemory(G_cCmdLineTokenE, sizeof(G_cCmdLineTokenE));

		strcpy(G_cCmdLine, pCmdLine);

		iIndex = 0;
		pStrTok = new class CStrTok(pCmdLine, seps);
		token = pStrTok->pGet();
		while (token != NULL)
		{
			switch (iIndex) {
			case 0:	strcpy(G_cCmdLineTokenA, token); break;
			case 1: strcpy(G_cCmdLineTokenB, token); break;
			case 2: strcpy(G_cCmdLineTokenC, token); break;
			case 3: strcpy(G_cCmdLineTokenD, token); break;
			case 4: strcpy(G_cCmdLineTokenE, token); break;
			}
			token = pStrTok->pGet();
			iIndex++;
		}
		delete pStrTok;
	}

	ZeroMemory(G_cCmdLineTokenA_Lowercase, sizeof(G_cCmdLineTokenA_Lowercase));
	strcpy(G_cCmdLineTokenA_Lowercase, G_cCmdLineTokenA);
	_strlwr(G_cCmdLineTokenA_Lowercase);

	if (memcmp(G_cCmdLineTokenA_Lowercase, "/egparam", 8) == 0)
	{
		ZeroMemory(G_cCmdLineTokenA, sizeof(G_cCmdLineTokenA));
		memcpy(G_cCmdLineTokenA, "dataq", 5);
	}
	m_hWnd = hWnd;
	m_bCommandAvailable = TRUE;
	m_pCGameMonitor = NULL;
	m_dwTime = G_dwGlobalTime;
	m_bSoundFlag = m_DSound.Create(m_hWnd);
	m_bMusicStat = m_bSoundStat = m_bSoundFlag;
	m_bIsHideLocalCursor = FALSE;
	m_cEnterCheck = m_cTabCheck = m_cLeftArrowCheck = NULL;

	if (bCheckImportantFile() == FALSE)
	{
		MessageBox(m_hWnd, "File checksum error! Get Update again please!", "ERROR1", MB_ICONEXCLAMATION | MB_OK);
		return FALSE;
	}

	if (_bDecodeBuildItemContents() == FALSE)
	{
		MessageBox(m_hWnd, "File checksum error! Get Update again please!", "ERROR2", MB_ICONEXCLAMATION | MB_OK);
		return FALSE;
	}

	if (bReadLoginConfigFile("login.cfg") == FALSE)
	{
		MessageBox(m_hWnd, "login.cfg file contains wrong infomation.", "ERROR", MB_ICONEXCLAMATION | MB_OK);
		return FALSE;
	}

	if (bReadItemNameConfigFile() == FALSE)
	{
		MessageBox(m_hWnd, "ItemName.cfg file contains wrong infomation.", "ERROR", MB_ICONEXCLAMATION | MB_OK);
		return FALSE;
	}

	if (bInitMagicCfgList() == FALSE) {
		MessageBox(m_hWnd, "MAGICCFG.TXT file contains wrong infomation.", "ERROR", MB_ICONEXCLAMATION | MB_OK);
		return FALSE;
	}
	// Skill
	if (bInitSkillCfgList() == FALSE)
	{
		MessageBox(m_hWnd, "SKILLCFG.TXT file contains wrong infomation.", "ERROR", MB_ICONEXCLAMATION | MB_OK);
		return FALSE;
	}

	if (m_DDraw.bInit(m_hWnd) == FALSE)
	{
		MessageBox(m_hWnd, "This program requires DirectX7.0a!", "ERROR", MB_ICONEXCLAMATION | MB_OK);
		return FALSE;
	}

	if (m_DInput.bInit(hWnd, hInst) == FALSE) {
		MessageBox(m_hWnd, "This program requires DirectX7.0a!", "ERROR", MB_ICONEXCLAMATION | MB_OK);
		return FALSE;
	}


	m_hPakFile = CreateFile("sprites\\New-Dialog.pak", GENERIC_READ, NULL, NULL, OPEN_EXISTING, NULL, NULL);
	m_pSprite[SPRID_INTERFACE_ND_LOADING] = new class CSprite(m_hPakFile, &m_DDraw, "New-Dialog", 0, FALSE);
	CloseHandle(m_hPakFile);

	m_hPakFile = CreateFile("sprites\\interface2.pak", GENERIC_READ, NULL, NULL, OPEN_EXISTING, NULL, NULL);
	m_pSprite[SPRID_INTERFACE_ADDINTERFACE] = new class CSprite(m_hPakFile, &m_DDraw, "interface2", 0, FALSE);
	m_pSprite[SPRID_INTERFACE_CRAFTING] = new class CSprite(m_hPakFile, &m_DDraw, "interface2", 3, FALSE);
	CloseHandle(m_hPakFile);

	// CLEROTH - LOAD FONTS BEFORE MAIN LOADING
	m_hPakFile = CreateFile("sprites\\interface2.pak", GENERIC_READ, NULL, NULL, OPEN_EXISTING, NULL, NULL);
	if (m_hPakFile != INVALID_HANDLE_VALUE)
	{
		m_pSprite[SPRID_INTERFACE_SPRFONTS2] = new class CSprite(m_hPakFile, &m_DDraw, "interface2", 1, FALSE);
		m_pSprite[SPRID_INTERFACE_F1HELPWINDOWS] = new class CSprite(m_hPakFile, &m_DDraw, "interface2", 2, FALSE);
		CloseHandle(m_hPakFile);
	}

	m_hPakFile = CreateFile("sprites\\sprfonts.pak", GENERIC_READ, NULL, NULL, OPEN_EXISTING, NULL, NULL);
	if (m_hPakFile != INVALID_HANDLE_VALUE)
	{
		m_pSprite[SPRID_INTERFACE_FONT1] = new class CSprite(m_hPakFile, &m_DDraw, "sprfonts", 0, FALSE);
		m_pSprite[SPRID_INTERFACE_FONT2] = new class CSprite(m_hPakFile, &m_DDraw, "sprfonts", 1, FALSE);
		CloseHandle(m_hPakFile);
	}

	m_stMCursor.sX = 0;
	m_stMCursor.sY = 0;
	m_pMapData = new class CMapData(this);
	ZeroMemory(m_cPlayerName, sizeof(m_cPlayerName));
	ZeroMemory(m_cAccountName, sizeof(m_cAccountName));
	ZeroMemory(m_cAccountPassword, sizeof(m_cAccountPassword));

	m_sPlayerType = 2;
	m_cPlayerTurn = 0;
	// Snoopy: fixed here
	m_cDialogBoxOrder[60] = 29;
	m_cDialogBoxOrder[59] = 30; // 29π¯ GaugePannel

	m_cMenuDir = 4;
	m_cMenuDirCnt = 0;
	m_cMenuFrame = 0;

	m_cSoundVolume = 100;
	m_cMusicVolume = 100;

	m_Misc.ColorTransfer(m_DDraw.m_cPixelFormat, RGB(70, 70, 80), &m_wWR[1], &m_wWG[1], &m_wWB[1]); // Light-blue
	m_Misc.ColorTransfer(m_DDraw.m_cPixelFormat, RGB(70, 70, 80), &m_wWR[2], &m_wWG[2], &m_wWB[2]); // light-blue
	m_Misc.ColorTransfer(m_DDraw.m_cPixelFormat, RGB(70, 70, 80), &m_wWR[3], &m_wWG[3], &m_wWB[3]); // light-blue
	m_Misc.ColorTransfer(m_DDraw.m_cPixelFormat, RGB(70, 100, 70), &m_wWR[4], &m_wWG[4], &m_wWB[4]); // Green
	m_Misc.ColorTransfer(m_DDraw.m_cPixelFormat, RGB(130, 90, 10), &m_wWR[5], &m_wWG[5], &m_wWB[5]); // Critical
	m_Misc.ColorTransfer(m_DDraw.m_cPixelFormat, RGB(42, 53, 111), &m_wWR[6], &m_wWG[6], &m_wWB[6]); // Heavy-blue
	m_Misc.ColorTransfer(m_DDraw.m_cPixelFormat, RGB(145, 145, 145), &m_wWR[7], &m_wWG[7], &m_wWB[7]); // White
	m_Misc.ColorTransfer(m_DDraw.m_cPixelFormat, RGB(120, 100, 120), &m_wWR[8], &m_wWG[8], &m_wWB[8]); // Violet
	m_Misc.ColorTransfer(m_DDraw.m_cPixelFormat, RGB(75, 10, 10), &m_wWR[9], &m_wWG[9], &m_wWB[9]); // Heavy-Red
	m_Misc.ColorTransfer(m_DDraw.m_cPixelFormat, RGB(135, 104, 30), &m_wR[10], &m_wG[10], &m_wB[10]);	// Gold, buggy

	m_Misc.ColorTransfer(m_DDraw.m_cPixelFormat, RGB(200 / 2, 200 / 2, 200 / 2), &m_wR[0], &m_wG[0], &m_wB[0]);
	m_Misc.ColorTransfer(m_DDraw.m_cPixelFormat, RGB(0x50 / 2, 0x50 / 2, 0xC0 / 2), &m_wR[1], &m_wG[1], &m_wB[1]); // Indigo Blue
	m_Misc.ColorTransfer(m_DDraw.m_cPixelFormat, RGB(79, 79, 62), &m_wR[2], &m_wG[2], &m_wB[2]); // Custom-Weapon Color
	m_Misc.ColorTransfer(m_DDraw.m_cPixelFormat, RGB(135, 104, 30), &m_wR[3], &m_wG[3], &m_wB[3]); // Gold
	m_Misc.ColorTransfer(m_DDraw.m_cPixelFormat, RGB(255 / 2, 36 / 2, 0), &m_wR[4], &m_wG[4], &m_wB[4]); // Crimson
	m_Misc.ColorTransfer(m_DDraw.m_cPixelFormat, RGB(10, 60, 10), &m_wR[5], &m_wG[5], &m_wB[5]); // Green
	m_Misc.ColorTransfer(m_DDraw.m_cPixelFormat, RGB(0x50 / 2, 0x50 / 2, 0x50 / 2), &m_wR[6], &m_wG[6], &m_wB[6]); // Gray
	m_Misc.ColorTransfer(m_DDraw.m_cPixelFormat, RGB(0x5F / 2, 0x9E / 2, 0xA0 / 2), &m_wR[7], &m_wG[7], &m_wB[7]); // Aqua
	m_Misc.ColorTransfer(m_DDraw.m_cPixelFormat, RGB(0xFF / 2, 0x69 / 2, 0xB4 / 2), &m_wR[8], &m_wG[8], &m_wB[8]); // Pink
	m_Misc.ColorTransfer(m_DDraw.m_cPixelFormat, RGB(90, 60, 90), &m_wR[9], &m_wG[9], &m_wB[9]); // Violet

	m_Misc.ColorTransfer(m_DDraw.m_cPixelFormat, RGB(0, 35, 60), &m_wR[10], &m_wG[10], &m_wB[10]); // Blue
	m_Misc.ColorTransfer(m_DDraw.m_cPixelFormat, RGB(0xD2 / 2, 0xB4 / 2, 0x8C / 2), &m_wR[11], &m_wG[11], &m_wB[11]); // Tan
	m_Misc.ColorTransfer(m_DDraw.m_cPixelFormat, RGB(0xBD / 2, 0xB7 / 2, 0x6B / 2), &m_wR[12], &m_wG[12], &m_wB[12]); // Khaki
	m_Misc.ColorTransfer(m_DDraw.m_cPixelFormat, RGB(85, 85, 8), &m_wR[13], &m_wG[13], &m_wB[13]); // Yellow
	m_Misc.ColorTransfer(m_DDraw.m_cPixelFormat, RGB(75, 10, 10), &m_wR[14], &m_wG[14], &m_wB[14]); // Red
	m_Misc.ColorTransfer(m_DDraw.m_cPixelFormat, RGB(0x30, 0x30, 0x30), &m_wR[15], &m_wG[15], &m_wB[15]); // Black



#ifndef _DEBUG
	m_pCGameMonitor = new class CGameMonitor;
	//===============================================
	// badword.txt
	if (!m_Misc._iConvertFileXor("contents\\badword.txt", "contents\\badword.tmp", 35))
	{
		MessageBox(m_hWnd, "BADWORD.TXT file contains wrong infomation.", "ERROR", MB_ICONEXCLAMATION | MB_OK);
		return FALSE;
	}
	m_pCGameMonitor->iReadBadWordFileList("contents\\badword.tmp");
	DeleteFile("contents\\badword.tmp");

	if (!m_Misc._iConvertFileXor("contents\\Curse.txt", "contents\\Curse.tmp", 35))
	{
		MessageBox(m_hWnd, "CURSE.TXT file contains wrong infomation.", "ERROR", MB_ICONEXCLAMATION | MB_OK);
		return FALSE;
	}
	m_curse.LoadCurse("contents\\Curse.tmp");
	DeleteFile("contents\\Curse.tmp");
	//==============================================
#endif

	_LoadGameMsgTextContents();
	ZeroMemory(m_cWorldServerName, sizeof(m_cWorldServerName));

	return TRUE;
}
void cMain::OnTimer()
{
	if (m_cGameMode < 0) return;
	DWORD dwTime = timeGetTime();

	if (m_cGameMode != GAMEMODE_ONLOADING) {
		if ((dwTime - m_dwCheckSprTime) > 8000)
		{
			m_dwCheckSprTime = dwTime;
			if (m_bIsProgramActive) ReleaseUnusedSprites();
			//			if ((m_pGSock != NULL) && (m_pGSock->m_bIsAvailable == TRUE))
			////bSendCommand(MSGID_COMMAND_CHECKCONNECTION, MSGTYPE_CONFIRM, NULL, NULL, NULL, NULL, NULL);
		}
	}

	if (m_cGameMode == GAMEMODE_ONMAINGAME)
	{
		if ((dwTime - m_dwCheckConnTime) > 5000)
		{
			m_dwCheckConnTime = dwTime;
			//if ((m_bIsCrusadeMode) && (m_iCrusadeDuty == NULL)) EnableDialogBox(33, 1, NULL, NULL);
		}

		if ((dwTime - m_dwCheckChatTime) > 2000)
		{
			m_dwCheckChatTime = m_dwTime;
			ReleaseTimeoverChatMsg();
			if (m_cCommandCount >= 6)
			{
				m_iNetLagCount++;
				if (m_iNetLagCount >= 7)
				{
					ChangeGameMode(GAMEMODE_ONCONNECTIONLOST);
					//delete m_pGSock;
					//m_pGSock = NULL;
					return;
				}
			}
			else m_iNetLagCount = NULL;
		}

		/*
		if ((G_bIsCalcSocketConnected == FALSE) && ((dwTime - G_dwCalcSocketTime) > 5000))
		{	//delete m_pGSock;
		//m_pGSock = NULL;
		ChangeGameMode(GAMEMODE_ONQUIT);
		m_bEscPressed = FALSE;
		PlaySound('E', 14, 5);
		if (m_bSoundFlag) m_pESound[38]->bStop();
		if ((m_bSoundFlag) && (m_bMusicStat == TRUE))
		{
		if (m_pBGM != NULL) m_pBGM->bStop();
		}
		return;
		}

		if ((G_pCalcSocket != NULL) && (G_bIsCalcSocketConnected == TRUE)) {
		if ((dwTime - G_dwCalcSocketSendTime) > 1000*5) {
		if (memcmp(G_cCmdLineTokenA_Lowercase, "wisetop", 7) == 0)
		{}else
		{	char cPacket[120];
		int  iSended;
		WORD * wp;
		ZeroMemory(cPacket, sizeof(cPacket));
		cPacket[0] = 0;					// Key
		wp  = (WORD *)(cPacket +1);
		*wp = 5;
		iSended = G_pCalcSocket->iSendMsgBlockingMode(cPacket, 5);
		}
		G_dwCalcSocketSendTime = dwTime;
		}	}	}
		*/
	}
}

void cMain::StartUp()
{
	char Log[120];

	DisplayInLogList("(*) Starting Server!");
	bReadConfigFile("MapServer.cfg");

	// Listen for client connections.
	G_pClientSock->listenSocket = G_pListenSock->ListenSocket(IpAddress, G_pClientSock->port, G_pClientSock->BlockingMode, G_pClientSock->MaxConnections, G_pClientSock->wmEvent);
	if (G_pClientSock->listenSocket == NULL){
		wsprintf(Log, "(ERROR) Failed Listening For Client: %ld Port:%d\n", G_pClientSock->listenSocket, G_pClientSock->port);
		DisplayInLogList(Log);
	}
	else {
		wsprintf(Log, "(*) Listening For Client: %ld Port:%d\n", G_pClientSock->listenSocket, G_pClientSock->port);
		DisplayInLogList(Log);
	}

	DisplayInLogList("");
	DisplayInLogList("(*) Server Started! Press HOME to register this map server on the login server.");
}
void cMain::OnKeyUp(WPARAM wParam, LPARAM lParam)
{
	char Log[120];
	DWORD * dwp;
	char  SendData[15];
	WORD  * wp;
	short * sp;
	char  * cp;

	switch (wParam) {
		//Home key pressed
	case VK_HOME:
		// Register this map server on the login server.
		if (G_pLoginSock->connectSocket == NULL) {
			G_pLoginSock->connectSocket = G_pListenSock->ConnectSocket(IpAddress, G_pLoginSock->port, G_pLoginSock->wmEvent);
			if (G_pLoginSock->connectSocket == NULL){
				wsprintf(Log, "(ERROR) Failed to connect Login Server On Socket: %ld Port:%d\n", G_pLoginSock->connectSocket, G_pLoginSock->port);
				DisplayInLogList(Log);
			}
			else {
				wsprintf(Log, "(*) Connected To Login Server On Socket: %ld Port:%d\n", G_pLoginSock->connectSocket, G_pLoginSock->port);
				DisplayInLogList(Log);
			}

			break;

	case 0x53://s
		ZeroMemory(SendData, sizeof(SendData));

		dwp = (DWORD*)SendData;
		*dwp = MSGID_TEST_MSG;

		wp = (WORD *)(SendData + 4);
		*wp = MSGTYPE_CONFIRM;

		cp = (char *)(SendData + 8);

		wp = (WORD *)cp;
		*wp = 32;
		cp += 2;

		sp = (short *)cp;
		*sp = 23;
		cp += 2;

		sp = (short *)cp;
		*sp = 35;
		cp += 2;

		G_pListenSock->SendSocket(G_pLoginSock->connectSocket, SendData, 14);
		break;

		}
	}
}

void cMain::SocketsMessages(UINT message, WPARAM wParam, LPARAM lParam)
{
	int WSAEvent;
	char Log[120];

	/*	FD_READ		notifies when data arrives at our socket
		FD_WRITE	receives notifications of readiness for writing
		FD_ACCEPT	notifies of events for incoming connections
		FD_CONNECT	notifies of events for socket connections
		FD_CLOSE	alerts when a socket has been closed
		*/
	WSAEvent = WSAGETSELECTEVENT(lParam);
	switch (message)
	{
	case WM_LOGIN_SOCKET:
		switch (WSAEvent) {
		case FD_READ:
			msgDecode();
			break;
		case FD_WRITE:
			break;
		case FD_ACCEPT:
			break;
		case FD_CONNECT:
			break;
		case FD_CLOSE:
			break;
		}
		break;
	case WM_CLIENT_SOCKET:
		switch (WSAEvent) {
		case FD_READ:
			msgDecode();
			break;
		case FD_WRITE:
			break;
		case FD_ACCEPT:
			// A client requests to connect to the map server. Accept it.
			for (UINT i = 0; i < MAXCLIENTSOCKETS; i++){
				if (G_pClientSock->acceptedSockets[i] == NULL){
					G_pClientSock->acceptedSockets[i] = G_pListenSock->AcceptSocket(G_pClientSock->listenSocket, message);
					wsprintf(Log, "Accepted client (%d/%d). Socket: %ld\n", i + 1, MAXCLIENTSOCKETS, G_pClientSock->acceptedSockets[i]);
					DisplayInLogList(Log);
					return;
				}
				wsprintf(Log, "(ERROR) Cannot accept any more client. Limit reached (%d/%d). \n", MAXCLIENTSOCKETS, MAXCLIENTSOCKETS);
				DisplayInLogList(Log);
			}
			break;
		case FD_CONNECT:
			break;
		case FD_CLOSE:
			break;
		}
		break;
	}
}

bool cMain::bReadConfigFile(char * filename)
{
	FILE * pFile;
	HANDLE hFile;
	DWORD  dwFileSize;
	char * cp, *token, cTxt[120];
	ConfigFileReadMode cReadMode = ConfigFileReadMode::Undefined;
	char seps[] = "= ,\t\n";
	int	iIPindex = 0;

	hFile = CreateFile(filename, GENERIC_READ, NULL, NULL, OPEN_EXISTING, NULL, NULL);
	dwFileSize = GetFileSize(hFile, NULL);
	if (hFile != INVALID_HANDLE_VALUE) CloseHandle(hFile);


	pFile = fopen(filename, "rt");
	if (pFile == NULL) {

		DisplayInLogList("(ERROR) Cannot open configuration file.");
		return FALSE;
	}
	else {
		DisplayInLogList("(*) Reading configuration file...");
		cp = new char[dwFileSize + 1];
		ZeroMemory(cp, dwFileSize + 1);
		fread(cp, dwFileSize, 1, pFile);

		token = strtok(cp, seps);
		while (token != NULL)   {

			if (cReadMode != ConfigFileReadMode::Undefined) {
				switch (cReadMode) {
				case ConfigFileReadMode::ServerName:
					ZeroMemory(ServerName, sizeof(ServerName));
					strcpy(ServerName, token);
					wsprintf(cTxt, "(*) Server Name : %s", ServerName);
					DisplayInLogList(cTxt);
					cReadMode = ConfigFileReadMode::Undefined;
					break;
				case ConfigFileReadMode::Address:
					ZeroMemory(IpAddress, sizeof(IpAddress));
					strcpy(IpAddress, token);
					wsprintf(cTxt, "(*) Server Ip Address : %s", IpAddress);
					DisplayInLogList(cTxt);
					cReadMode = ConfigFileReadMode::Undefined;
					break;
				case ConfigFileReadMode::ServerPort:
					G_pClientSock->port = atoi(token);
					wsprintf(cTxt, "(*) Server Port : %d", G_pClientSock->port);
					DisplayInLogList(cTxt);
					cReadMode = ConfigFileReadMode::Undefined;
					break;
				case ConfigFileReadMode::LoginPort:
					G_pLoginSock->port = atoi(token);
					wsprintf(cTxt, "(*) Login Server Port : %d", G_pLoginSock->port);
					DisplayInLogList(cTxt);
					cReadMode = ConfigFileReadMode::Undefined;
					break;
				case ConfigFileReadMode::Version:
					ZeroMemory(Version, sizeof(Version));
					strcpy(Version, token);
					wsprintf(cTxt, "(*) Server Version : %s", Version);
					DisplayInLogList(cTxt);
					cReadMode = ConfigFileReadMode::Undefined;
					break;
				}

			}
			else {
				if (memcmp(token, "server-name", 11) == 0) cReadMode = ConfigFileReadMode::ServerName;
				if (memcmp(token, "address", 7) == 0)     cReadMode = ConfigFileReadMode::Address;
				if (memcmp(token, "server-port", 11) == 0) cReadMode = ConfigFileReadMode::ServerPort;
				if (memcmp(token, "login-port", 10) == 0) cReadMode = ConfigFileReadMode::LoginPort;
				if (memcmp(token, "version", 7) == 0) cReadMode = ConfigFileReadMode::Version;
			}
			token = strtok(NULL, seps);
		}

		delete cp;
	}
	if (pFile != NULL) fclose(pFile);
	DisplayInLogList("(*) Reading configuration file successful");
	DisplayInLogList("");
	return TRUE;

}
void cMain::msgDecode()
{
	const int recvbuflen = 512;
	char recvbuf[512] = "";
	int byteReceived;
	DWORD  *dwpMsgID;;
	WORD * wpResult;
	short * sp;
	char  * cp;
	short Temp1, Temp2, Temp3;
	char Log[120];

	memset(recvbuf, 0, sizeof recvbuf);
	byteReceived = G_pListenSock->RecvSocket(G_pLoginSock->connectSocket, recvbuf, recvbuflen);
	wsprintf(Log, "Byte received:%d data:%s\n", byteReceived, recvbuf);
	DisplayInLogList(Log);

	dwpMsgID = (DWORD *)(recvbuf);
	switch (*dwpMsgID) {

	case MSGID_TEST_MSG:
		DisplayInLogList("MSGID_TEST_MSG");
		wpResult = (WORD *)(recvbuf + 4);
		switch (*wpResult) {
		case MSGTYPE_CONFIRM:
			DisplayInLogList("MSGTYPE_CONFIRM");
			break;
		case MSGTYPE_REJECT:
			DisplayInLogList("MSGTYPE_REJECT");
			break;
		}
		cp = (char *)(recvbuf + 8);

		sp = (short *)cp;
		Temp1 = *sp;
		cp += 2;

		sp = (short *)cp;
		Temp2 = *sp;
		cp += 2;

		sp = (short *)cp;
		Temp3 = *sp;
		cp += 2;
		wsprintf(Log, "Temp1: %d Temp2: %d Temp3: %d \n", Temp1, Temp2, Temp3);
		DisplayInLogList(Log);
		break;
	}
}


void cMain::UpdateScreen()
{
	
	switch (m_cGameMode) {

	case GAMEMODE_ONAGREEMENT:
		UpdateScreen_OnAgreement(); //unused by HBx server..
		break;

	case GAMEMODE_ONCREATENEWACCOUNT:
		UpdateScreen_OnCreateNewAccount();
		break;


	case GAMEMODE_ONVERSIONNOTMATCH:
		UpdateScreen_OnVersionNotMatch();
		break;

	case GAMEMODE_ONCONNECTING:
		UpdateScreen_OnConnecting();
		break;

	case GAMEMODE_ONMAINMENU:
		UpdateScreen_OnMainMenu();
		break;

	case GAMEMODE_ONLOADING:
		UpdateScreen_OnLoading(TRUE);
		break;

	case GAMEMODE_ONMAINGAME:
		//UpdateScreen_OnGame();
		break;

	case GAMEMODE_ONWAITINGINITDATA:
		UpdateScreen_OnWaitInitData();
		break;

	case GAMEMODE_ONCONNECTIONLOST:
		UpdateScreen_OnConnectionLost();
		break;

	case GAMEMODE_ONMSG:
		UpdateScreen_OnMsg();
		break;

	case GAMEMODE_ONLOGIN:
		UpdateScreen_OnLogin();
		break;

	case GAMEMODE_ONSELECTSERVER:
		UpdateScreen_OnSelectServer();
		break;

	case GAMEMODE_ONQUIT:
		UpdateScreen_OnQuit();
		break;

	case GAMEMODE_ONQUERYFORCELOGIN:
		UpdateScreen_OnQueryForceLogin();
		break;

	case GAMEMODE_ONSELECTCHARACTER:
		UpdateScreen_OnSelectCharacter();
		break;

	case GAMEMODE_ONCREATENEWCHARACTER:
		UpdateScreen_OnCreateNewCharacter();
		break;

	case GAMEMODE_ONWAITINGRESPONSE:
		UpdateScreen_OnWaitingResponse();
		break;

	case GAMEMODE_ONQUERYDELETECHARACTER:
		UpdateScreen_OnQueryDeleteCharacter();
		break;

	case GAMEMODE_ONLOGRESMSG:
		//UpdateScreen_OnLogResMsg();
		break;

	case GAMEMODE_ONCHANGEPASSWORD:
		UpdateScreen_OnChangePassword();
		break;
	}
}
void cMain::UpdateScreen_OnLoading(bool bActive)
{
	int i;
	if (bActive) UpdateScreen_OnLoading_Progress();

	switch (m_cLoading) {
	case 0:
	{	m_hPakFile = CreateFile("sprites\\interface.pak", GENERIC_READ, NULL, NULL, OPEN_EXISTING, NULL, NULL);
	if (m_hPakFile != INVALID_HANDLE_VALUE) {
		m_pSprite[SPRID_MOUSECURSOR] = new class CSprite(m_hPakFile, &m_DDraw, "interface", 0, FALSE);
		m_pSprite[SPRID_INTERFACE_SPRFONTS] = new class CSprite(m_hPakFile, &m_DDraw, "interface", 1, FALSE);
		CloseHandle(m_hPakFile);
	}

	m_hPakFile = CreateFile("sprites\\Newmaps.pak", GENERIC_READ, NULL, NULL, OPEN_EXISTING, NULL, NULL);
	if (m_hPakFile != INVALID_HANDLE_VALUE) {
		m_pSprite[SPRID_INTERFACE_NEWMAPS1] = new class CSprite(m_hPakFile, &m_DDraw, "Newmaps", 0, FALSE);
		m_pSprite[SPRID_INTERFACE_NEWMAPS2] = new class CSprite(m_hPakFile, &m_DDraw, "Newmaps", 1, FALSE);
		m_pSprite[SPRID_INTERFACE_NEWMAPS3] = new class CSprite(m_hPakFile, &m_DDraw, "Newmaps", 2, FALSE);
		m_pSprite[SPRID_INTERFACE_NEWMAPS4] = new class CSprite(m_hPakFile, &m_DDraw, "Newmaps", 3, FALSE);
		m_pSprite[SPRID_INTERFACE_NEWMAPS5] = new class CSprite(m_hPakFile, &m_DDraw, "Newmaps", 4, FALSE);
		CloseHandle(m_hPakFile);
	}

	m_hPakFile = CreateFile("sprites\\LoginDialog.pak", GENERIC_READ, NULL, NULL, OPEN_EXISTING, NULL, NULL);
	if (m_hPakFile != INVALID_HANDLE_VALUE) {
		m_pSprite[SPRID_INTERFACE_ND_LOGIN] = new class CSprite(m_hPakFile, &m_DDraw, "LoginDialog", 0, FALSE);
#ifdef MAKE_ACCOUNT
		m_pSprite[SPRID_INTERFACE_ND_NEWACCOUNT] = new class CSprite(m_hPakFile, &m_DDraw, "LoginDialog", 1, FALSE);
		m_pSprite[SPRID_INTERFACE_ND_AGREEMENT] = new class CSprite(m_hPakFile, &m_DDraw, "LoginDialog", 2, FALSE);
#endif
		CloseHandle(m_hPakFile);
	}
#ifdef MAKE_ACCOUNT			// CLEROTH - ACC - Snoopy: fixed to use without special pak
	m_hPakFile = CreateFile("sprites\\CreateNewAcc.pak", GENERIC_READ, NULL, NULL, OPEN_EXISTING, NULL, NULL);
	if (m_hPakFile != INVALID_HANDLE_VALUE) {
		m_pSprite[SPRID_INTERFACE_ND_NEWACCOUNT] = new class CSprite(m_hPakFile, &m_DDraw, "CreateNewAcc", 0, FALSE);
		CloseHandle(m_hPakFile);
	}
	else
	{
		m_hPakFile = CreateFile("sprites\\New-Dialog.pak", GENERIC_READ, NULL, NULL, OPEN_EXISTING, NULL, NULL);
		m_pSprite[SPRID_INTERFACE_ND_NEWACCOUNT] = new class CSprite(m_hPakFile, &m_DDraw, "New-Dialog", 2, FALSE);
		CloseHandle(m_hPakFile);
	}
#endif
	m_hPakFile = CreateFile("sprites\\New-Dialog.pak", GENERIC_READ, NULL, NULL, OPEN_EXISTING, NULL, NULL);
	if (m_hPakFile != INVALID_HANDLE_VALUE) {
		m_pSprite[SPRID_INTERFACE_ND_MAINMENU] = new class CSprite(m_hPakFile, &m_DDraw, "New-Dialog", 1, FALSE);
		m_pSprite[SPRID_INTERFACE_ND_QUIT] = new class CSprite(m_hPakFile, &m_DDraw, "New-Dialog", 2, FALSE);
		CloseHandle(m_hPakFile);
	}

	m_hPakFile = CreateFile("sprites\\GameDialog.pak", GENERIC_READ, NULL, NULL, OPEN_EXISTING, NULL, NULL);
	if (m_hPakFile != INVALID_HANDLE_VALUE) {
		m_pSprite[SPRID_INTERFACE_ND_GAME1] = new class CSprite(m_hPakFile, &m_DDraw, "GameDialog", 0, FALSE);
		m_pSprite[SPRID_INTERFACE_ND_GAME2] = new class CSprite(m_hPakFile, &m_DDraw, "GameDialog", 1, FALSE);
		m_pSprite[SPRID_INTERFACE_ND_GAME3] = new class CSprite(m_hPakFile, &m_DDraw, "GameDialog", 2, FALSE);
		m_pSprite[SPRID_INTERFACE_ND_GAME4] = new class CSprite(m_hPakFile, &m_DDraw, "GameDialog", 3, FALSE);
		m_pSprite[SPRID_INTERFACE_ND_CRUSADE] = new class CSprite(m_hPakFile, &m_DDraw, "GameDialog", 4, FALSE);
		//m_pSprite[SPRID_INTERFACE_GUIDEMAP] =      new class CSprite(m_hPakFile, &m_DDraw, "GameDialog", 5, FALSE);
		m_pSprite[SPRID_INTERFACE_ND_ICONPANNEL] = new class CSprite(m_hPakFile, &m_DDraw, "GameDialog", 6, FALSE);
		m_pSprite[SPRID_INTERFACE_ND_INVENTORY] = new class CSprite(m_hPakFile, &m_DDraw, "GameDialog", 7, FALSE);
		m_pSprite[SPRID_INTERFACE_ND_SELECTCHAR] = new class CSprite(m_hPakFile, &m_DDraw, "GameDialog", 8, FALSE);
		m_pSprite[SPRID_INTERFACE_ND_NEWCHAR] = new class CSprite(m_hPakFile, &m_DDraw, "GameDialog", 9, FALSE);
		m_pSprite[SPRID_INTERFACE_ND_NEWEXCHANGE] = new class CSprite(m_hPakFile, &m_DDraw, "GameDialog", 10, FALSE);
		CloseHandle(m_hPakFile);
	}

	m_hPakFile = CreateFile("sprites\\DialogText.pak", GENERIC_READ, NULL, NULL, OPEN_EXISTING, NULL, NULL);
	if (m_hPakFile != INVALID_HANDLE_VALUE) {
		m_pSprite[SPRID_INTERFACE_ND_TEXT] = new class CSprite(m_hPakFile, &m_DDraw, "DialogText", 0, FALSE);
		m_pSprite[SPRID_INTERFACE_ND_BUTTON] = new class CSprite(m_hPakFile, &m_DDraw, "DialogText", 1, FALSE);
		CloseHandle(m_hPakFile);
	}
	MakeSprite("Telescope", SPRID_INTERFACE_GUIDEMAP, 32, FALSE);	  // Snoopy: 20->32
	MakeSprite("Telescope2", SPRID_INTERFACE_GUIDEMAP + 35, 4, FALSE); // Snoopy: Ajout.351 (heldenian maps)
	MakeSprite("monster", SPRID_INTERFACE_MONSTER, 1, FALSE);
	m_cLoading = 4;
	}
		break;
	case 4:
	{	MakeTileSpr("maptiles1", 0, 32, TRUE);
	m_hPakFile = CreateFile("sprites\\structures1.pak", GENERIC_READ, NULL, NULL, OPEN_EXISTING, NULL, NULL); //æ»æ≤¥¬ ≈∏¿œ ∑Œµ˘ æ»«—¥Ÿ.2002.09.06ªÛ«œ
	if (m_hPakFile != INVALID_HANDLE_VALUE) {
		m_pTileSpr[1 + 50] = new class CSprite(m_hPakFile, &m_DDraw, "structures1", 1, TRUE);
		m_pTileSpr[5 + 50] = new class CSprite(m_hPakFile, &m_DDraw, "structures1", 5, TRUE);
		CloseHandle(m_hPakFile);
	}
	MakeTileSpr("Sinside1", 70, 27, FALSE);
	MakeTileSpr("Trees1", 100, 46, TRUE);
	MakeTileSpr("TreeShadows", 150, 46, TRUE);
	MakeTileSpr("objects1", 200, 10, TRUE); // snoopy: 8->10
	MakeTileSpr("objects2", 211, 5, TRUE);
	MakeTileSpr("objects3", 216, 4, TRUE);
	MakeTileSpr("objects4", 220, 2, TRUE); //snoopy: 1->2
	m_cLoading = 8;
	}
		break;
	case 8:
	{	MakeTileSpr("Tile223-225", 223, 3, TRUE);
	MakeTileSpr("Tile226-229", 226, 4, TRUE);
	MakeTileSpr("objects5", 230, 9, TRUE);	// Snoopy
	MakeTileSpr("objects6", 238, 4, TRUE);	// Snoopy
	MakeTileSpr("objects7", 242, 7, TRUE);	// Snoopy
	MakeTileSpr("maptiles2", 300, 15, TRUE);//- Index 300
	MakeTileSpr("maptiles4", 320, 10, TRUE);
	MakeTileSpr("maptiles5", 330, 19, TRUE);
	MakeTileSpr("maptiles6", 349, 4, TRUE);
	MakeTileSpr("maptiles353-361", 353, 9, TRUE);
	MakeTileSpr("Tile363-366", 363, 4, TRUE);
	MakeTileSpr("Tile367-367", 367, 1, TRUE); // Add by Snoopy (fountains)
	MakeTileSpr("Tile370-381", 370, 12, TRUE);// Tile370~381
	MakeTileSpr("Tile382-387", 382, 6, TRUE);
	MakeTileSpr("Tile388-402", 388, 15, TRUE);
	m_cLoading = 12;
	}
		break;
	case 12:
	{
			   MakeTileSpr("Tile403-405", 403, 3, TRUE);
			   MakeTileSpr("Tile406-421", 406, 16, TRUE);
			   MakeTileSpr("Tile422-429", 422, 8, TRUE);
			   MakeTileSpr("Tile430-443", 430, 14, TRUE);
			   MakeTileSpr("Tile444-444", 444, 1, TRUE);
			   MakeTileSpr("Tile445-461", 445, 17, TRUE);
			   MakeTileSpr("Tile462-473", 462, 12, TRUE);	// Diuuude
			   MakeTileSpr("Tile474-478", 474, 5, TRUE);	// Diuuude
			   MakeTileSpr("Tile479-488", 479, 10, TRUE);	// Diuuude
			   MakeTileSpr("Tile489-522", 489, 34, TRUE);	// Diuuude Drunken City
			   MakeTileSpr("Tile523-530", 523, 8, TRUE);	// Diuuude Rampart
			   MakeTileSpr("Tile531-540", 531, 10, TRUE);	// Diuuude GodH + Pont
			   MakeTileSpr("Tile541-545", 541, 5, TRUE);	// Diuuude GodH

			   // SPRID_ITEMPACK_PIVOTPOINT+0
			   MakeSprite("item-pack", SPRID_ITEMPACK_PIVOTPOINT + 1, 27, FALSE);
			   m_hPakFile = CreateFile("sprites\\item-pack.pak", GENERIC_READ, NULL, NULL, OPEN_EXISTING, NULL, NULL);
			   if (m_hPakFile != INVALID_HANDLE_VALUE) {
				   m_pSprite[SPRID_ITEMPACK_PIVOTPOINT + 20] = new class CSprite(m_hPakFile, &m_DDraw, "item-pack", 17, FALSE); //
				   m_pSprite[SPRID_ITEMPACK_PIVOTPOINT + 21] = new class CSprite(m_hPakFile, &m_DDraw, "item-pack", 18, FALSE); //
				   m_pSprite[SPRID_ITEMPACK_PIVOTPOINT + 22] = new class CSprite(m_hPakFile, &m_DDraw, "item-pack", 19, FALSE); // Angels
				   CloseHandle(m_hPakFile);
			   }

			   // SPRID_ITEMGROUND_PIVOTPOINT+1
			   MakeSprite("item-ground", SPRID_ITEMGROUND_PIVOTPOINT + 1, 19, FALSE);
			   m_hPakFile = CreateFile("sprites\\item-ground.pak", GENERIC_READ, NULL, NULL, OPEN_EXISTING, NULL, NULL);
			   if (m_hPakFile != INVALID_HANDLE_VALUE)
			   {
				   m_pSprite[SPRID_ITEMGROUND_PIVOTPOINT + 20] = new class CSprite(m_hPakFile, &m_DDraw, "item-ground", 17, FALSE);
				   m_pSprite[SPRID_ITEMGROUND_PIVOTPOINT + 21] = new class CSprite(m_hPakFile, &m_DDraw, "item-ground", 18, FALSE);
				   m_pSprite[SPRID_ITEMGROUND_PIVOTPOINT + 22] = new class CSprite(m_hPakFile, &m_DDraw, "item-ground", 19, FALSE);//Angels
				   CloseHandle(m_hPakFile);
			   }
			   MakeSprite("item-dynamic", SPRID_ITEMDYNAMIC_PIVOTPOINT, 3, FALSE);// Snoopy 2-> 3 (flags)
			   m_cLoading = 16;
	}
		break;
	case 16:
	{
			   m_hPakFile = CreateFile("sprites\\item-equipM.pak", GENERIC_READ, NULL, NULL, OPEN_EXISTING, NULL, NULL);
			   if (m_hPakFile != INVALID_HANDLE_VALUE) {
				   m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + 0] = new class CSprite(m_hPakFile, &m_DDraw, "item-equipM", 0, FALSE);	// body
				   m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + 1] = new class CSprite(m_hPakFile, &m_DDraw, "item-equipM", 1, FALSE);	// 1-swords
				   m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + 2] = new class CSprite(m_hPakFile, &m_DDraw, "item-equipM", 2, FALSE);	// 2-bows
				   m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + 3] = new class CSprite(m_hPakFile, &m_DDraw, "item-equipM", 3, FALSE);	// 3-shields
				   m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + 4] = new class CSprite(m_hPakFile, &m_DDraw, "item-equipM", 4, FALSE);	// 4-tunics
				   m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + 5] = new class CSprite(m_hPakFile, &m_DDraw, "item-equipM", 5, FALSE);	// 5-shoes
				   m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + 7] = new class CSprite(m_hPakFile, &m_DDraw, "item-equipM", 6, FALSE);	// 6-berk
				   m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + 8] = new class CSprite(m_hPakFile, &m_DDraw, "item-equipM", 7, FALSE);	// 7-hoses
				   m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + 9] = new class CSprite(m_hPakFile, &m_DDraw, "item-equipM", 8, FALSE);	// 8-bodyarmor
				   m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + 15] = new class CSprite(m_hPakFile, &m_DDraw, "item-equipM", 11, FALSE); // Axe hammer
				   m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + 17] = new class CSprite(m_hPakFile, &m_DDraw, "item-equipM", 12, FALSE); // Wands
				   m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + 18] = new class CSprite(m_hPakFile, &m_DDraw, "item-equipM", 9, FALSE);  // hair
				   m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + 19] = new class CSprite(m_hPakFile, &m_DDraw, "item-equipM", 10, FALSE); // undies
				   m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + 20] = new class CSprite(m_hPakFile, &m_DDraw, "item-equipM", 13, FALSE); // capes
				   m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + 21] = new class CSprite(m_hPakFile, &m_DDraw, "item-equipM", 14, FALSE); // helm
				   CloseHandle(m_hPakFile);
			   }

			   m_hPakFile = CreateFile("sprites\\item-pack.pak", GENERIC_READ, NULL, NULL, OPEN_EXISTING, NULL, NULL);
			   if (m_hPakFile != INVALID_HANDLE_VALUE)
			   {
				   m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + 16] = new class CSprite(m_hPakFile, &m_DDraw, "item-pack", 15); // Necks
				   //Snoopy: Angels pandents
				   m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + 22] = new class CSprite(m_hPakFile, &m_DDraw, "item-pack", 19); // Angels
				   CloseHandle(m_hPakFile);
			   }

			   m_hPakFile = CreateFile("sprites\\item-equipW.pak", GENERIC_READ, NULL, NULL, OPEN_EXISTING, NULL, NULL);
			   if (m_hPakFile != INVALID_HANDLE_VALUE) {
				   m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + 40] = new class CSprite(m_hPakFile, &m_DDraw, "item-equipW", 0, FALSE); // body
				   m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + 41] = new class CSprite(m_hPakFile, &m_DDraw, "item-equipW", 1, FALSE); // 1-swords
				   m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + 42] = new class CSprite(m_hPakFile, &m_DDraw, "item-equipW", 2, FALSE); // 2-bows
				   m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + 43] = new class CSprite(m_hPakFile, &m_DDraw, "item-equipW", 3, FALSE); // 3-shields
				   m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + 45] = new class CSprite(m_hPakFile, &m_DDraw, "item-equipW", 4, FALSE); // 4-shoes
				   m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + 50] = new class CSprite(m_hPakFile, &m_DDraw, "item-equipW", 5, FALSE); // 5-Soustif
				   m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + 51] = new class CSprite(m_hPakFile, &m_DDraw, "item-equipW", 6, FALSE); // 6 berk
				   m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + 52] = new class CSprite(m_hPakFile, &m_DDraw, "item-equipW", 7, FALSE); // 7 hose
				   m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + 53] = new class CSprite(m_hPakFile, &m_DDraw, "item-equipW", 8, FALSE); // 8-hoses
				   m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + 55] = new class CSprite(m_hPakFile, &m_DDraw, "item-equipW", 11, FALSE); // Axe hammer
				   m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + 57] = new class CSprite(m_hPakFile, &m_DDraw, "item-equipW", 12, FALSE); // Wands
				   m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + 58] = new class CSprite(m_hPakFile, &m_DDraw, "item-equipW", 9, FALSE); // hair
				   m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + 59] = new class CSprite(m_hPakFile, &m_DDraw, "item-equipW", 10, FALSE);// undies
				   m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + 60] = new class CSprite(m_hPakFile, &m_DDraw, "item-equipW", 13, FALSE);// capes
				   m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + 61] = new class CSprite(m_hPakFile, &m_DDraw, "item-equipW", 14, FALSE);// helm
				   CloseHandle(m_hPakFile);
			   }

			   m_hPakFile = CreateFile("sprites\\item-pack.pak", GENERIC_READ, NULL, NULL, OPEN_EXISTING, NULL, NULL);
			   if (m_hPakFile != INVALID_HANDLE_VALUE) {
				   m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + 56] = new class CSprite(m_hPakFile, &m_DDraw, "item-pack", 15);// necks
				   //Snoopy: Angels pandents
				   m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + 62] = new class CSprite(m_hPakFile, &m_DDraw, "item-pack", 19); // Angels
				   CloseHandle(m_hPakFile);
			   }
			   MakeSprite("Bm", 500 + 15 * 8 * 0, 96, TRUE);// Black Man (Type: 1)
			   MakeSprite("Wm", 500 + 15 * 8 * 1, 96, TRUE);// White Man (Type: 2)
			   MakeSprite("Ym", 500 + 15 * 8 * 2, 96, TRUE);// Yellow Man (Type: 3)
			   m_cLoading = 20;
	}
		break;
	case 20:
	{	MakeSprite("TutelarAngel1", SPRID_TUTELARYANGELS_PIVOTPOINT + 50 * 0, 48, FALSE);//(STR)
	MakeSprite("TutelarAngel2", SPRID_TUTELARYANGELS_PIVOTPOINT + 50 * 1, 48, FALSE);//(DEX)
	MakeSprite("TutelarAngel3", SPRID_TUTELARYANGELS_PIVOTPOINT + 50 * 2, 48, FALSE);//(INT)
	MakeSprite("TutelarAngel4", SPRID_TUTELARYANGELS_PIVOTPOINT + 50 * 3, 48, FALSE);//(MAG)
	MakeSprite("Bw", 500 + 15 * 8 * 3, 96, TRUE);// Black Woman (Type: 4)
	MakeSprite("Ww", 500 + 15 * 8 * 4, 96, TRUE);// White Woman (Type: 5)
	MakeSprite("Yw", 500 + 15 * 8 * 5, 96, TRUE);// Yellow Woman (Type: 6)
	m_cLoading = 24;
	}
		break;
	case 24:
	{	MakeSprite("slm", SPRID_MOB + 7 * 8 * 0, 40, TRUE);// Slime (Type: 10)
	MakeSprite("ske", SPRID_MOB + 7 * 8 * 1, 40, TRUE);// Skeleton (Type: 11)
	MakeSprite("Gol", SPRID_MOB + 7 * 8 * 2, 40, TRUE);// Stone-Golem (Type: 12)
	MakeSprite("Cyc", SPRID_MOB + 7 * 8 * 3, 40, TRUE);// Cyclops (Type: 13)
	MakeSprite("Orc", SPRID_MOB + 7 * 8 * 4, 40, TRUE);// Orc (Type: 14)
	MakeSprite("Shopkpr", SPRID_MOB + 7 * 8 * 5, 8);		// ShopKeeper-Woman (Type: 15)
	MakeSprite("Ant", SPRID_MOB + 7 * 8 * 6, 40, TRUE);//  Giant-Ant (Type: 16)
	MakeSprite("Scp", SPRID_MOB + 7 * 8 * 7, 40, TRUE);//  Scorpion (Type: 17)
	MakeSprite("Zom", SPRID_MOB + 7 * 8 * 8, 40, TRUE);//  Zombie (Type: 18)
	MakeSprite("Gandlf", SPRID_MOB + 7 * 8 * 9, 8, TRUE);// Gandalf ∏ (Type: 19)
	MakeSprite("Howard", SPRID_MOB + 7 * 8 * 10, 8, TRUE);// Howard ∫∏∞¸º“ ¡÷¿Œ (Type: 20)
	MakeSprite("Guard", SPRID_MOB + 7 * 8 * 11, 40, TRUE);// Guard (Type: 21)
	MakeSprite("Amp", SPRID_MOB + 7 * 8 * 12, 40, TRUE);// Amphis (Type: 22)
	MakeSprite("Cla", SPRID_MOB + 7 * 8 * 13, 40, TRUE);// Clay-Golem (Type: 23)
	MakeSprite("tom", SPRID_MOB + 7 * 8 * 14, 8, TRUE);// Tom (Type: 24)
	MakeSprite("William", SPRID_MOB + 7 * 8 * 15, 8, TRUE);// William (Type: 25)
	m_cLoading = 28;
	}
		break;
	case 28:
	{	MakeSprite("Kennedy", SPRID_MOB + 7 * 8 * 16, 8, TRUE);// Kennedy (Type: 26)
	MakeSprite("Helb", SPRID_MOB + 7 * 8 * 17, 40, TRUE);// Hellbound (Type: 27)
	MakeSprite("Troll", SPRID_MOB + 7 * 8 * 18, 40, TRUE);// Troll (Type: 28)
	MakeSprite("Orge", SPRID_MOB + 7 * 8 * 19, 40, TRUE);// Orge (Type: 29)
	MakeSprite("Liche", SPRID_MOB + 7 * 8 * 20, 40, TRUE);// Liche (Type: 30)
	MakeSprite("Demon", SPRID_MOB + 7 * 8 * 21, 40, TRUE);// Demon (Type: 31)
	MakeSprite("Unicorn", SPRID_MOB + 7 * 8 * 22, 40, TRUE);// Unicorn (Type: 32)
	MakeSprite("WereWolf", SPRID_MOB + 7 * 8 * 23, 40, TRUE);// WereWolf (Type: 33)
	MakeSprite("Dummy", SPRID_MOB + 7 * 8 * 24, 40, TRUE);// Dummy (Type: 34)
	m_hPakFile = CreateFile("sprites\\Effect5.pak", GENERIC_READ, NULL, NULL, OPEN_EXISTING, NULL, NULL); // Energy-Ball (Type: 35)
	if (m_hPakFile != INVALID_HANDLE_VALUE)
	{
		for (i = 0; i < 40; i++)
			m_pSprite[SPRID_MOB + i + 7 * 8 * 25] = new class CSprite(m_hPakFile, &m_DDraw, "Effect5", 0, TRUE);

		CloseHandle(m_hPakFile);
	}
	m_cLoading = 32;
	}
		break;
	case 32:
	{MakeSprite("GT-Arrow", SPRID_MOB + 7 * 8 * 26, 40, TRUE);// Arrow-GuardTower (Type: 36)
	MakeSprite("GT-Cannon", SPRID_MOB + 7 * 8 * 27, 40, TRUE);// Cannon-GuardTower (Type: 37)
	MakeSprite("ManaCollector", SPRID_MOB + 7 * 8 * 28, 40, TRUE);// Mana Collector (Type: 38)
	MakeSprite("Detector", SPRID_MOB + 7 * 8 * 29, 40, TRUE);// Detector (Type: 39)
	MakeSprite("ESG", SPRID_MOB + 7 * 8 * 30, 40, TRUE);// ESG (Type: 40)
	MakeSprite("GMG", SPRID_MOB + 7 * 8 * 31, 40, TRUE);// GMG (Type: 41)
	MakeSprite("ManaStone", SPRID_MOB + 7 * 8 * 32, 40, TRUE);// ManaStone (Type: 42)
	MakeSprite("LWB", SPRID_MOB + 7 * 8 * 33, 40, TRUE);// Light War Beetle (Type: 43)
	MakeSprite("GHK", SPRID_MOB + 7 * 8 * 34, 40, TRUE);// God's Hand Knight (Type: 44)
	MakeSprite("GHKABS", SPRID_MOB + 7 * 8 * 35, 40, TRUE);// God's Hand Knight with Armored Battle Steed (Type: 45)
	MakeSprite("TK", SPRID_MOB + 7 * 8 * 36, 40, TRUE);// Temple Knight (Type: 46)
	MakeSprite("BG", SPRID_MOB + 7 * 8 * 37, 40, TRUE);// Battle Golem (Type: 47)
	m_cLoading = 36;
	}
		break;
	case 36:
	{
			   MakeSprite("Stalker", SPRID_MOB + 7 * 8 * 38, 40, TRUE);// Stalker (Type: 48)
			   MakeSprite("Hellclaw", SPRID_MOB + 7 * 8 * 39, 40, TRUE);// Hellclaw (Type: 49)
			   MakeSprite("Tigerworm", SPRID_MOB + 7 * 8 * 40, 40, TRUE);// Tigerworm (Type: 50)
			   MakeSprite("Catapult", SPRID_MOB + 7 * 8 * 41, 40, TRUE);// Catapult (Type: 51)
			   MakeSprite("Gagoyle", SPRID_MOB + 7 * 8 * 42, 40, TRUE);// Gargoyle (Type: 52)
			   MakeSprite("Beholder", SPRID_MOB + 7 * 8 * 43, 40, TRUE);// Beholder (Type: 53)
			   MakeSprite("DarkElf", SPRID_MOB + 7 * 8 * 44, 40, TRUE);// Dark-Elf (Type: 54)
			   MakeSprite("Bunny", SPRID_MOB + 7 * 8 * 45, 40, TRUE);// Bunny (Type: 55)
			   MakeSprite("Cat", SPRID_MOB + 7 * 8 * 46, 40, TRUE);// Cat (Type: 56)
			   MakeSprite("GiantFrog", SPRID_MOB + 7 * 8 * 47, 40, TRUE);// GiantFrog (Type: 57)
			   MakeSprite("MTGiant", SPRID_MOB + 7 * 8 * 48, 40, TRUE);// Mountain Giant (Type: 58)
			   m_cLoading = 40;
	}
		break;
	case 40:
	{	MakeSprite("Ettin", SPRID_MOB + 7 * 8 * 49, 40, TRUE);// Ettin (Type: 59)
	MakeSprite("CanPlant", SPRID_MOB + 7 * 8 * 50, 40, TRUE);// Cannibal Plant (Type: 60)
	MakeSprite("Rudolph", SPRID_MOB + 7 * 8 * 51, 40, TRUE);// Rudolph (Type: 61)
	MakeSprite("DireBoar", SPRID_MOB + 7 * 8 * 52, 40, TRUE);// Boar (Type: 62)
	MakeSprite("frost", SPRID_MOB + 7 * 8 * 53, 40, TRUE);// Frost (Type: 63)
	MakeSprite("Crop", SPRID_MOB + 7 * 8 * 54, 40, TRUE);// Crop(Type: 64)
	MakeSprite("IceGolem", SPRID_MOB + 7 * 8 * 55, 40, TRUE);// IceGolem (Type: 65)
	MakeSprite("Wyvern", SPRID_MOB + 7 * 8 * 56, 24, TRUE);// Wyvern (Type: 66)
	MakeSprite("McGaffin", SPRID_MOB + 7 * 8 * 57, 16, TRUE);// McGaffin (Type: 67)
	MakeSprite("Perry", SPRID_MOB + 7 * 8 * 58, 16, TRUE);// Perry (Type: 68)
	MakeSprite("Devlin", SPRID_MOB + 7 * 8 * 59, 16, TRUE);// Devlin (Type: 69)
	MakeSprite("Barlog", SPRID_MOB + 7 * 8 * 60, 40, TRUE);// Barlog (Type: 70)
	MakeSprite("Centaurus", SPRID_MOB + 7 * 8 * 61, 40, TRUE);// Centaurus (Type: 71)
	MakeSprite("ClawTurtle", SPRID_MOB + 7 * 8 * 62, 40, TRUE);// Claw-Turtle (Type: 72)
	MakeSprite("FireWyvern", SPRID_MOB + 7 * 8 * 63, 24, TRUE);// Fire-Wyvern (Type: 73)
	MakeSprite("GiantCrayfish", SPRID_MOB + 7 * 8 * 64, 40, TRUE);// Giant-Crayfish (Type: 74)
	MakeSprite("GiantLizard", SPRID_MOB + 7 * 8 * 65, 40, TRUE);// Giant-Lizard (Type: 75)
	m_cLoading = 44;
	}
		break;
	case 44:
	{	// New NPCs - Diuuude - fixed by Snoopy
			   MakeSprite("GiantPlant", SPRID_MOB + 7 * 8 * 66, 40, TRUE);// Giant-Plant (Type: 76)
			   MakeSprite("MasterMageOrc", SPRID_MOB + 7 * 8 * 67, 40, TRUE);// MasterMage-Orc (Type: 77)
			   MakeSprite("Minotaurs", SPRID_MOB + 7 * 8 * 68, 40, TRUE);// Minotaurs (Type: 78)
			   MakeSprite("Nizie", SPRID_MOB + 7 * 8 * 69, 40, TRUE);// Nizie (Type: 79)
			   MakeSprite("Tentocle", SPRID_MOB + 7 * 8 * 70, 40, TRUE);// Tentocle (Type: 80)
			   MakeSprite("yspro", SPRID_MOB + 7 * 8 * 71, 32, TRUE);// Abaddon (Type: 81)
			   MakeSprite("Sorceress", SPRID_MOB + 7 * 8 * 72, 40, TRUE);// Sorceress (Type: 82)
			   MakeSprite("TPKnight", SPRID_MOB + 7 * 8 * 73, 40, TRUE);// TPKnight (Type: 83)
			   MakeSprite("ElfMaster", SPRID_MOB + 7 * 8 * 74, 40, TRUE);// ElfMaster (Type: 84)
			   MakeSprite("DarkKnight", SPRID_MOB + 7 * 8 * 75, 40, TRUE);// DarkKnight (Type: 85)
			   MakeSprite("HBTank", SPRID_MOB + 7 * 8 * 76, 32, TRUE);// HeavyBattleTank (Type: 86)
			   MakeSprite("CBTurret", SPRID_MOB + 7 * 8 * 77, 32, TRUE);// CBTurret (Type: 87)
			   MakeSprite("Babarian", SPRID_MOB + 7 * 8 * 78, 40, TRUE);// Babarian (Type: 88)
			   MakeSprite("ACannon", SPRID_MOB + 7 * 8 * 79, 32, TRUE);// ACannon (Type: 89)
			   m_cLoading = 48;
	}
		break;
	case 48:
	{	MakeSprite("Gail", SPRID_MOB + 7 * 8 * 80, 8, TRUE); // Gail (Type: 90)
	MakeSprite("Gate", SPRID_MOB + 7 * 8 * 81, 24, TRUE);// Heldenian Gate (Type: 91)/**/
	m_hPakFile = CreateFile("sprites\\Mpt.pak", GENERIC_READ, NULL, NULL, OPEN_EXISTING, NULL, NULL);
	if (m_hPakFile != INVALID_HANDLE_VALUE)
	{
		for (i = 0; i < 12; i++) m_pSprite[SPRID_UNDIES_M + i + 15 * 0] = new class CSprite(m_hPakFile, &m_DDraw, "Mpt", i + 12 * 0, TRUE);
		for (i = 0; i < 12; i++) m_pSprite[SPRID_UNDIES_M + i + 15 * 1] = new class CSprite(m_hPakFile, &m_DDraw, "Mpt", i + 12 * 1, TRUE);
		for (i = 0; i < 12; i++) m_pSprite[SPRID_UNDIES_M + i + 15 * 2] = new class CSprite(m_hPakFile, &m_DDraw, "Mpt", i + 12 * 2, TRUE);
		for (i = 0; i < 12; i++) m_pSprite[SPRID_UNDIES_M + i + 15 * 3] = new class CSprite(m_hPakFile, &m_DDraw, "Mpt", i + 12 * 3, TRUE);
		for (i = 0; i < 12; i++) m_pSprite[SPRID_UNDIES_M + i + 15 * 4] = new class CSprite(m_hPakFile, &m_DDraw, "Mpt", i + 12 * 4, TRUE);
		for (i = 0; i < 12; i++) m_pSprite[SPRID_UNDIES_M + i + 15 * 5] = new class CSprite(m_hPakFile, &m_DDraw, "Mpt", i + 12 * 5, TRUE);
		for (i = 0; i < 12; i++) m_pSprite[SPRID_UNDIES_M + i + 15 * 6] = new class CSprite(m_hPakFile, &m_DDraw, "Mpt", i + 12 * 6, TRUE);
		for (i = 0; i < 12; i++) m_pSprite[SPRID_UNDIES_M + i + 15 * 7] = new class CSprite(m_hPakFile, &m_DDraw, "Mpt", i + 12 * 7, TRUE);
		CloseHandle(m_hPakFile);
	}
	m_cLoading = 52;
	}
		break;

	case 52:
	{	m_hPakFile = CreateFile("sprites\\Mhr.pak", GENERIC_READ, NULL, NULL, OPEN_EXISTING, NULL, NULL);
	if (m_hPakFile != INVALID_HANDLE_VALUE)
	{
		for (i = 0; i < 12; i++) m_pSprite[SPRID_HAIR_M + i + 15 * 0] = new class CSprite(m_hPakFile, &m_DDraw, "Mhr", i + 12 * 0, TRUE);
		for (i = 0; i < 12; i++) m_pSprite[SPRID_HAIR_M + i + 15 * 1] = new class CSprite(m_hPakFile, &m_DDraw, "Mhr", i + 12 * 1, TRUE);
		for (i = 0; i < 12; i++) m_pSprite[SPRID_HAIR_M + i + 15 * 2] = new class CSprite(m_hPakFile, &m_DDraw, "Mhr", i + 12 * 2, TRUE);
		for (i = 0; i < 12; i++) m_pSprite[SPRID_HAIR_M + i + 15 * 3] = new class CSprite(m_hPakFile, &m_DDraw, "Mhr", i + 12 * 3, TRUE);
		for (i = 0; i < 12; i++) m_pSprite[SPRID_HAIR_M + i + 15 * 4] = new class CSprite(m_hPakFile, &m_DDraw, "Mhr", i + 12 * 4, TRUE);
		for (i = 0; i < 12; i++) m_pSprite[SPRID_HAIR_M + i + 15 * 5] = new class CSprite(m_hPakFile, &m_DDraw, "Mhr", i + 12 * 5, TRUE);
		for (i = 0; i < 12; i++) m_pSprite[SPRID_HAIR_M + i + 15 * 6] = new class CSprite(m_hPakFile, &m_DDraw, "Mhr", i + 12 * 6, TRUE);
		for (i = 0; i < 12; i++) m_pSprite[SPRID_HAIR_M + i + 15 * 7] = new class CSprite(m_hPakFile, &m_DDraw, "Mhr", i + 12 * 7, TRUE);
		CloseHandle(m_hPakFile);
	}
	MakeSprite("MLArmor", SPRID_BODYARMOR_M + 15 * 1, 12, TRUE);
	MakeSprite("MCMail", SPRID_BODYARMOR_M + 15 * 2, 12, TRUE);
	MakeSprite("MSMail", SPRID_BODYARMOR_M + 15 * 3, 12, TRUE);
	MakeSprite("MPMail", SPRID_BODYARMOR_M + 15 * 4, 12, TRUE);
	MakeSprite("Mtunic", SPRID_BODYARMOR_M + 15 * 5, 12, TRUE);
	MakeSprite("MRobe1", SPRID_BODYARMOR_M + 15 * 6, 12, TRUE);
	MakeSprite("MSanta", SPRID_BODYARMOR_M + 15 * 7, 12, TRUE);
	MakeSprite("MHRobe1", SPRID_BODYARMOR_M + 15 * 10, 12, TRUE); //hero
	MakeSprite("MHRobe2", SPRID_BODYARMOR_M + 15 * 11, 12, TRUE); //hero
	MakeSprite("MHPMail1", SPRID_BODYARMOR_M + 15 * 8, 12, TRUE); //hero
	MakeSprite("MHPMail2", SPRID_BODYARMOR_M + 15 * 9, 12, TRUE); //hero
	MakeSprite("MShirt", SPRID_BERK_M + 15 * 1, 12, TRUE);
	MakeSprite("MHauberk", SPRID_BERK_M + 15 * 2, 12, TRUE);
	MakeSprite("MHHauberk1", SPRID_BERK_M + 15 * 3, 12, TRUE);
	MakeSprite("MHHauberk2", SPRID_BERK_M + 15 * 4, 12, TRUE);
	m_cLoading = 56;
	}
		break;
	case 56:
	{	MakeSprite("MTrouser", SPRID_LEGG_M + 15 * 1, 12, TRUE);
	MakeSprite("MHTrouser", SPRID_LEGG_M + 15 * 2, 12, TRUE);
	MakeSprite("MCHoses", SPRID_LEGG_M + 15 * 3, 12, TRUE);
	MakeSprite("MLeggings", SPRID_LEGG_M + 15 * 4, 12, TRUE);
	MakeSprite("MHLeggings1", SPRID_LEGG_M + 15 * 5, 12, TRUE); // hero
	MakeSprite("MHLeggings2", SPRID_LEGG_M + 15 * 6, 12, TRUE); // hero
	MakeSprite("MShoes", SPRID_BOOT_M + 15 * 1, 12, TRUE);
	MakeSprite("MLBoots", SPRID_BOOT_M + 15 * 2, 12, TRUE);
	m_hPakFile = CreateFile("sprites\\Msw.pak", GENERIC_READ, NULL, NULL, OPEN_EXISTING, NULL, NULL);
	if (m_hPakFile != INVALID_HANDLE_VALUE) {
		for (i = 0; i < 56; i++) m_pSprite[SPRID_WEAPON_M + i + 64 * 1] = new class CSprite(m_hPakFile, &m_DDraw, "Msw", i + 56 * 0, TRUE);
		for (i = 0; i < 56; i++) m_pSprite[SPRID_WEAPON_M + i + 64 * 2] = new class CSprite(m_hPakFile, &m_DDraw, "Msw", i + 56 * 1, TRUE);
		for (i = 0; i < 56; i++) m_pSprite[SPRID_WEAPON_M + i + 64 * 3] = new class CSprite(m_hPakFile, &m_DDraw, "Msw", i + 56 * 2, TRUE);
		for (i = 0; i < 56; i++) m_pSprite[SPRID_WEAPON_M + i + 64 * 4] = new class CSprite(m_hPakFile, &m_DDraw, "Msw", i + 56 * 3, TRUE);
		for (i = 0; i < 56; i++) m_pSprite[SPRID_WEAPON_M + i + 64 * 6] = new class CSprite(m_hPakFile, &m_DDraw, "Msw", i + 56 * 5, TRUE);
		for (i = 0; i < 56; i++) m_pSprite[SPRID_WEAPON_M + i + 64 * 7] = new class CSprite(m_hPakFile, &m_DDraw, "Msw", i + 56 * 6, TRUE);
		for (i = 0; i < 56; i++) m_pSprite[SPRID_WEAPON_M + i + 64 * 8] = new class CSprite(m_hPakFile, &m_DDraw, "Msw", i + 56 * 7, TRUE);
		for (i = 0; i < 56; i++) m_pSprite[SPRID_WEAPON_M + i + 64 * 9] = new class CSprite(m_hPakFile, &m_DDraw, "Msw", i + 56 * 8, TRUE);
		for (i = 0; i < 56; i++) m_pSprite[SPRID_WEAPON_M + i + 64 * 10] = new class CSprite(m_hPakFile, &m_DDraw, "Msw", i + 56 * 9, TRUE);
		for (i = 0; i < 56; i++) m_pSprite[SPRID_WEAPON_M + i + 64 * 11] = new class CSprite(m_hPakFile, &m_DDraw, "Msw", i + 56 * 10, TRUE);
		for (i = 0; i < 56; i++) m_pSprite[SPRID_WEAPON_M + i + 64 * 12] = new class CSprite(m_hPakFile, &m_DDraw, "Msw", i + 56 * 11, TRUE);
		CloseHandle(m_hPakFile);
	}
	m_cLoading = 60;
	}
		break;
	case 60:
	{	MakeSprite("Mswx", SPRID_WEAPON_M + 64 * 5, 56, TRUE);
	MakeSprite("Msw2", SPRID_WEAPON_M + 64 * 13, 56, TRUE);
	MakeSprite("Msw3", SPRID_WEAPON_M + 64 * 14, 56, TRUE);
	MakeSprite("MStormBringer", SPRID_WEAPON_M + 64 * 15, 56, TRUE);
	MakeSprite("MDarkExec", SPRID_WEAPON_M + 64 * 16, 56, TRUE);
	MakeSprite("MKlonessBlade", SPRID_WEAPON_M + 64 * 17, 56, TRUE);
	MakeSprite("MKlonessAstock", SPRID_WEAPON_M + 64 * 18, 56, TRUE);
	MakeSprite("MDebastator", SPRID_WEAPON_M + 64 * 19, 56, TRUE);
	MakeSprite("MAxe1", SPRID_WEAPON_M + 64 * 20, 56, TRUE);// Axe
	MakeSprite("MAxe2", SPRID_WEAPON_M + 64 * 21, 56, TRUE);
	MakeSprite("MAxe3", SPRID_WEAPON_M + 64 * 22, 56, TRUE);
	MakeSprite("MAxe4", SPRID_WEAPON_M + 64 * 23, 56, TRUE);
	MakeSprite("MAxe5", SPRID_WEAPON_M + 64 * 24, 56, TRUE);
	MakeSprite("MPickAxe1", SPRID_WEAPON_M + 64 * 25, 56, TRUE);
	MakeSprite("MAxe6", SPRID_WEAPON_M + 64 * 26, 56, TRUE);
	MakeSprite("Mhoe", SPRID_WEAPON_M + 64 * 27, 56, TRUE);
	MakeSprite("MKlonessAxe", SPRID_WEAPON_M + 64 * 28, 56, TRUE);
	MakeSprite("MLightBlade", SPRID_WEAPON_M + 64 * 29, 56, TRUE);
	m_cLoading = 64;
	}
		break;
	case 64:
	{	MakeSprite("MHammer", SPRID_WEAPON_M + 64 * 30, 56, TRUE);
	MakeSprite("MBHammer", SPRID_WEAPON_M + 64 * 31, 56, TRUE);
	MakeSprite("MBabHammer", SPRID_WEAPON_M + 64 * 32, 56, TRUE);
	MakeSprite("MBShadowSword", SPRID_WEAPON_M + 64 * 33, 56, TRUE);
	MakeSprite("MBerserkWand", SPRID_WEAPON_M + 64 * 34, 56, TRUE);
	MakeSprite("Mstaff1", SPRID_WEAPON_M + 64 * 35, 56, TRUE);// Staff
	MakeSprite("Mstaff2", SPRID_WEAPON_M + 64 * 36, 56, TRUE);
	MakeSprite("MStaff3", SPRID_WEAPON_M + 64 * 37, 56, TRUE);
	MakeSprite("MReMagicWand", SPRID_WEAPON_M + 64 * 38, 56, TRUE);
	MakeSprite("MKlonessWand", SPRID_WEAPON_M + 64 * 39, 56, TRUE);
	// Bows 40 41 below
	MakeSprite("MDirectBow", SPRID_WEAPON_M + 64 * 42, 56, TRUE);
	MakeSprite("MFireBow", SPRID_WEAPON_M + 64 * 43, 56, TRUE);
	m_cLoading = 68;
	}
		break;
	case 68:
	{	MakeSprite("Mbo", SPRID_WEAPON_M + 64 * 40, 56, TRUE);
	m_hPakFile = CreateFile("sprites\\Mbo.pak", GENERIC_READ, NULL, NULL, OPEN_EXISTING, NULL, NULL);
	if (m_hPakFile != INVALID_HANDLE_VALUE)
	{
		for (i = 0; i < 56; i++) m_pSprite[SPRID_WEAPON_M + i + 64 * 41] = new class CSprite(m_hPakFile, &m_DDraw, "Mbo", i + 56 * 1, TRUE);
		CloseHandle(m_hPakFile);
	}
	m_hPakFile = CreateFile("sprites\\Msh.pak", GENERIC_READ, NULL, NULL, OPEN_EXISTING, NULL, NULL);
	if (m_hPakFile != INVALID_HANDLE_VALUE)
	{
		for (i = 0; i < 7; i++) m_pSprite[SPRID_SHIELD_M + i + 8 * 1] = new class CSprite(m_hPakFile, &m_DDraw, "Msh", i + 7 * 0, TRUE);
		for (i = 0; i < 7; i++) m_pSprite[SPRID_SHIELD_M + i + 8 * 2] = new class CSprite(m_hPakFile, &m_DDraw, "Msh", i + 7 * 1, TRUE);
		for (i = 0; i < 7; i++) m_pSprite[SPRID_SHIELD_M + i + 8 * 3] = new class CSprite(m_hPakFile, &m_DDraw, "Msh", i + 7 * 2, TRUE);
		for (i = 0; i < 7; i++) m_pSprite[SPRID_SHIELD_M + i + 8 * 4] = new class CSprite(m_hPakFile, &m_DDraw, "Msh", i + 7 * 3, TRUE);
		for (i = 0; i < 7; i++)	m_pSprite[SPRID_SHIELD_M + i + 8 * 5] = new class CSprite(m_hPakFile, &m_DDraw, "Msh", i + 7 * 4, TRUE);
		for (i = 0; i < 7; i++) m_pSprite[SPRID_SHIELD_M + i + 8 * 6] = new class CSprite(m_hPakFile, &m_DDraw, "Msh", i + 7 * 5, TRUE);
		for (i = 0; i < 7; i++) m_pSprite[SPRID_SHIELD_M + i + 8 * 7] = new class CSprite(m_hPakFile, &m_DDraw, "Msh", i + 7 * 6, TRUE);
		for (i = 0; i < 7; i++) m_pSprite[SPRID_SHIELD_M + i + 8 * 8] = new class CSprite(m_hPakFile, &m_DDraw, "Msh", i + 7 * 7, TRUE);
		for (i = 0; i < 7; i++) m_pSprite[SPRID_SHIELD_M + i + 8 * 9] = new class CSprite(m_hPakFile, &m_DDraw, "Msh", i + 7 * 8, TRUE);
		CloseHandle(m_hPakFile);
	}
	m_cLoading = 72;
	}
		break;
	case 72:
	{	MakeSprite("Mmantle01", SPRID_MANTLE_M + 15 * 1, 12, TRUE);
	MakeSprite("Mmantle02", SPRID_MANTLE_M + 15 * 2, 12, TRUE);
	MakeSprite("Mmantle03", SPRID_MANTLE_M + 15 * 3, 12, TRUE);
	MakeSprite("Mmantle04", SPRID_MANTLE_M + 15 * 4, 12, TRUE);
	MakeSprite("Mmantle05", SPRID_MANTLE_M + 15 * 5, 12, TRUE);
	MakeSprite("Mmantle06", SPRID_MANTLE_M + 15 * 6, 12, TRUE);
	MakeSprite("MHelm1", SPRID_HEAD_M + 15 * 1, 12, TRUE);
	MakeSprite("MHelm2", SPRID_HEAD_M + 15 * 2, 12, TRUE);
	MakeSprite("MHelm3", SPRID_HEAD_M + 15 * 3, 12, TRUE);
	MakeSprite("MHelm4", SPRID_HEAD_M + 15 * 4, 12, TRUE);
	MakeSprite("MHCap1", SPRID_HEAD_M + 15 * 11, 12, TRUE);
	MakeSprite("MHCap2", SPRID_HEAD_M + 15 * 12, 12, TRUE);
	MakeSprite("MHHelm1", SPRID_HEAD_M + 15 * 9, 12, TRUE);
	MakeSprite("MHHelm2", SPRID_HEAD_M + 15 * 10, 12, TRUE);
	MakeSprite("NMHelm1", SPRID_HEAD_M + 15 * 5, 12, TRUE);
	MakeSprite("NMHelm2", SPRID_HEAD_M + 15 * 6, 12, TRUE);
	MakeSprite("NMHelm3", SPRID_HEAD_M + 15 * 7, 12, TRUE);
	MakeSprite("NMHelm4", SPRID_HEAD_M + 15 * 8, 12, TRUE);
	m_cLoading = 76;
	}
		break;
	case 76:
	{	m_hPakFile = CreateFile("sprites\\Wpt.pak", GENERIC_READ, NULL, NULL, OPEN_EXISTING, NULL, NULL);
	if (m_hPakFile != INVALID_HANDLE_VALUE) {
		for (i = 0; i < 12; i++) m_pSprite[SPRID_UNDIES_W + i + 15 * 0] = new class CSprite(m_hPakFile, &m_DDraw, "Wpt", i, TRUE);
		for (i = 0; i < 12; i++) m_pSprite[SPRID_UNDIES_W + i + 15 * 1] = new class CSprite(m_hPakFile, &m_DDraw, "Wpt", i + 12, TRUE);
		for (i = 0; i < 12; i++) m_pSprite[SPRID_UNDIES_W + i + 15 * 2] = new class CSprite(m_hPakFile, &m_DDraw, "Wpt", i + 12 * 2, TRUE);
		for (i = 0; i < 12; i++) m_pSprite[SPRID_UNDIES_W + i + 15 * 3] = new class CSprite(m_hPakFile, &m_DDraw, "Wpt", i + 12 * 3, TRUE);
		for (i = 0; i < 12; i++) m_pSprite[SPRID_UNDIES_W + i + 15 * 4] = new class CSprite(m_hPakFile, &m_DDraw, "Wpt", i + 12 * 4, TRUE);
		for (i = 0; i < 12; i++) m_pSprite[SPRID_UNDIES_W + i + 15 * 5] = new class CSprite(m_hPakFile, &m_DDraw, "Wpt", i + 12 * 5, TRUE);
		for (i = 0; i < 12; i++) m_pSprite[SPRID_UNDIES_W + i + 15 * 6] = new class CSprite(m_hPakFile, &m_DDraw, "Wpt", i + 12 * 6, TRUE);
		for (i = 0; i < 12; i++) m_pSprite[SPRID_UNDIES_W + i + 15 * 7] = new class CSprite(m_hPakFile, &m_DDraw, "Wpt", i + 12 * 7, TRUE);
		CloseHandle(m_hPakFile);
	}

	m_hPakFile = CreateFile("sprites\\Whr.pak", GENERIC_READ, NULL, NULL, OPEN_EXISTING, NULL, NULL);
	if (m_hPakFile != INVALID_HANDLE_VALUE) {
		for (i = 0; i < 12; i++) m_pSprite[SPRID_HAIR_W + i + 15 * 0] = new class CSprite(m_hPakFile, &m_DDraw, "Whr", i + 0, TRUE);
		for (i = 0; i < 12; i++) m_pSprite[SPRID_HAIR_W + i + 15 * 1] = new class CSprite(m_hPakFile, &m_DDraw, "Whr", i + 12, TRUE);
		for (i = 0; i < 12; i++) m_pSprite[SPRID_HAIR_W + i + 15 * 2] = new class CSprite(m_hPakFile, &m_DDraw, "Whr", i + 12 * 2, TRUE);
		for (i = 0; i < 12; i++) m_pSprite[SPRID_HAIR_W + i + 15 * 3] = new class CSprite(m_hPakFile, &m_DDraw, "Whr", i + 12 * 3, TRUE);
		for (i = 0; i < 12; i++) m_pSprite[SPRID_HAIR_W + i + 15 * 4] = new class CSprite(m_hPakFile, &m_DDraw, "Whr", i + 12 * 4, TRUE);
		for (i = 0; i < 12; i++) m_pSprite[SPRID_HAIR_W + i + 15 * 5] = new class CSprite(m_hPakFile, &m_DDraw, "Whr", i + 12 * 5, TRUE);
		for (i = 0; i < 12; i++) m_pSprite[SPRID_HAIR_W + i + 15 * 6] = new class CSprite(m_hPakFile, &m_DDraw, "Whr", i + 12 * 6, TRUE);
		for (i = 0; i < 12; i++) m_pSprite[SPRID_HAIR_W + i + 15 * 7] = new class CSprite(m_hPakFile, &m_DDraw, "Whr", i + 12 * 7, TRUE);
		CloseHandle(m_hPakFile);
	}
	m_cLoading = 80;
	}
		break;
	case 80:
	{
			   MakeSprite("WBodice1", SPRID_BODYARMOR_W + 15 * 1, 12, TRUE);
			   MakeSprite("WBodice2", SPRID_BODYARMOR_W + 15 * 2, 12, TRUE);
			   MakeSprite("WLArmor", SPRID_BODYARMOR_W + 15 * 3, 12, TRUE);
			   MakeSprite("WCMail", SPRID_BODYARMOR_W + 15 * 4, 12, TRUE);
			   MakeSprite("WSMail", SPRID_BODYARMOR_W + 15 * 5, 12, TRUE);
			   MakeSprite("WPMail", SPRID_BODYARMOR_W + 15 * 6, 12, TRUE);
			   MakeSprite("WRobe1", SPRID_BODYARMOR_W + 15 * 7, 12, TRUE);
			   MakeSprite("WSanta", SPRID_BODYARMOR_W + 15 * 8, 12, TRUE);
			   MakeSprite("WHRobe1", SPRID_BODYARMOR_W + 15 * 11, 12, TRUE); // hero
			   MakeSprite("WHRobe2", SPRID_BODYARMOR_W + 15 * 12, 12, TRUE); // hero
			   MakeSprite("WHPMail1", SPRID_BODYARMOR_W + 15 * 9, 12, TRUE); //hero
			   MakeSprite("WHPMail2", SPRID_BODYARMOR_W + 15 * 10, 12, TRUE); //hero
			   MakeSprite("WChemiss", SPRID_BERK_W + 15 * 1, 12, TRUE);
			   MakeSprite("WShirt", SPRID_BERK_W + 15 * 2, 12, TRUE);
			   MakeSprite("WHauberk", SPRID_BERK_W + 15 * 3, 12, TRUE);
			   MakeSprite("WHHauberk1", SPRID_BERK_W + 15 * 4, 12, TRUE);
			   MakeSprite("WHHauberk2", SPRID_BERK_W + 15 * 5, 12, TRUE);
			   MakeSprite("WSkirt", SPRID_LEGG_W + 15 * 1, 12, TRUE);
			   MakeSprite("WTrouser", SPRID_LEGG_W + 15 * 2, 12, TRUE);
			   MakeSprite("WHTrouser", SPRID_LEGG_W + 15 * 3, 12, TRUE);
			   MakeSprite("WHLeggings1", SPRID_LEGG_W + 15 * 6, 12, TRUE);
			   MakeSprite("WHLeggings2", SPRID_LEGG_W + 15 * 7, 12, TRUE);
			   MakeSprite("WCHoses", SPRID_LEGG_W + 15 * 4, 12, TRUE);
			   MakeSprite("WLeggings", SPRID_LEGG_W + 15 * 5, 12, TRUE);
			   MakeSprite("WShoes", SPRID_BOOT_W + 15 * 1, 12, TRUE);
			   MakeSprite("WLBoots", SPRID_BOOT_W + 15 * 2, 12, TRUE);
			   m_cLoading = 84;
	}
		break;
	case 84:
	{	m_hPakFile = CreateFile("sprites\\Wsw.pak", GENERIC_READ, NULL, NULL, OPEN_EXISTING, NULL, NULL);
	if (m_hPakFile != INVALID_HANDLE_VALUE) {
		for (i = 0; i < 56; i++) m_pSprite[SPRID_WEAPON_W + i + 64 * 1] = new class CSprite(m_hPakFile, &m_DDraw, "Wsw", i + 56 * 0, TRUE);
		for (i = 0; i < 56; i++) m_pSprite[SPRID_WEAPON_W + i + 64 * 2] = new class CSprite(m_hPakFile, &m_DDraw, "Wsw", i + 56 * 1, TRUE);
		for (i = 0; i < 56; i++) m_pSprite[SPRID_WEAPON_W + i + 64 * 3] = new class CSprite(m_hPakFile, &m_DDraw, "Wsw", i + 56 * 2, TRUE);
		for (i = 0; i < 56; i++) m_pSprite[SPRID_WEAPON_W + i + 64 * 4] = new class CSprite(m_hPakFile, &m_DDraw, "Wsw", i + 56 * 3, TRUE);
		for (i = 0; i < 56; i++) m_pSprite[SPRID_WEAPON_W + i + 64 * 6] = new class CSprite(m_hPakFile, &m_DDraw, "Wsw", i + 56 * 5, TRUE);
		for (i = 0; i < 56; i++) m_pSprite[SPRID_WEAPON_W + i + 64 * 7] = new class CSprite(m_hPakFile, &m_DDraw, "Wsw", i + 56 * 6, TRUE);
		for (i = 0; i < 56; i++) m_pSprite[SPRID_WEAPON_W + i + 64 * 8] = new class CSprite(m_hPakFile, &m_DDraw, "Wsw", i + 56 * 7, TRUE);
		for (i = 0; i < 56; i++) m_pSprite[SPRID_WEAPON_W + i + 64 * 9] = new class CSprite(m_hPakFile, &m_DDraw, "Wsw", i + 56 * 8, TRUE);
		for (i = 0; i < 56; i++) m_pSprite[SPRID_WEAPON_W + i + 64 * 10] = new class CSprite(m_hPakFile, &m_DDraw, "Wsw", i + 56 * 9, TRUE);
		for (i = 0; i < 56; i++) m_pSprite[SPRID_WEAPON_W + i + 64 * 11] = new class CSprite(m_hPakFile, &m_DDraw, "Wsw", i + 56 * 10, TRUE);
		for (i = 0; i < 56; i++) m_pSprite[SPRID_WEAPON_W + i + 64 * 12] = new class CSprite(m_hPakFile, &m_DDraw, "Wsw", i + 56 * 11, TRUE);
		CloseHandle(m_hPakFile);
	}
	MakeSprite("Wswx", SPRID_WEAPON_W + 64 * 5, 56, TRUE);
	MakeSprite("Wsw2", SPRID_WEAPON_W + 64 * 13, 56, TRUE);
	MakeSprite("Wsw3", SPRID_WEAPON_W + 64 * 14, 56, TRUE); // TheVampire
	MakeSprite("WStormBringer", SPRID_WEAPON_W + 64 * 15, 56, TRUE);
	MakeSprite("WDarkExec", SPRID_WEAPON_W + 64 * 16, 56, TRUE);
	MakeSprite("WKlonessBlade", SPRID_WEAPON_W + 64 * 17, 56, TRUE);
	MakeSprite("WKlonessAstock", SPRID_WEAPON_W + 64 * 18, 56, TRUE);
	MakeSprite("WDebastator", SPRID_WEAPON_W + 64 * 19, 56, TRUE);
	m_cLoading = 88;
	}
		break;
	case 88:
	{	MakeSprite("WAxe1", SPRID_WEAPON_W + 64 * 20, 56, TRUE);// Axe
	MakeSprite("WAxe2", SPRID_WEAPON_W + 64 * 21, 56, TRUE);
	MakeSprite("WAxe3", SPRID_WEAPON_W + 64 * 22, 56, TRUE);
	MakeSprite("WAxe4", SPRID_WEAPON_W + 64 * 23, 56, TRUE);
	MakeSprite("WAxe5", SPRID_WEAPON_W + 64 * 24, 56, TRUE);
	MakeSprite("WpickAxe1", SPRID_WEAPON_W + 64 * 25, 56, TRUE);
	MakeSprite("WAxe6", SPRID_WEAPON_W + 64 * 26, 56, TRUE);
	MakeSprite("Whoe", SPRID_WEAPON_W + 64 * 27, 56, TRUE);
	MakeSprite("WKlonessAxe", SPRID_WEAPON_W + 64 * 28, 56, TRUE);
	MakeSprite("WLightBlade", SPRID_WEAPON_W + 64 * 29, 56, TRUE);
	MakeSprite("WHammer", SPRID_WEAPON_W + 64 * 30, 56, TRUE);
	MakeSprite("WBHammer", SPRID_WEAPON_W + 64 * 31, 56, TRUE);
	MakeSprite("WBabHammer", SPRID_WEAPON_W + 64 * 32, 56, TRUE);
	MakeSprite("WBShadowSword", SPRID_WEAPON_W + 64 * 33, 56, TRUE);
	MakeSprite("WBerserkWand", SPRID_WEAPON_W + 64 * 34, 56, TRUE);
	MakeSprite("Wstaff1", SPRID_WEAPON_W + 64 * 35, 56, TRUE);// Staff
	MakeSprite("Wstaff2", SPRID_WEAPON_W + 64 * 36, 56, TRUE);
	MakeSprite("WStaff3", SPRID_WEAPON_W + 64 * 37, 56, TRUE);
	MakeSprite("WKlonessWand", SPRID_WEAPON_W + 64 * 39, 56, TRUE);
	MakeSprite("WReMagicWand", SPRID_WEAPON_W + 64 * 38, 56, TRUE);
	// bows 40 41 below
	MakeSprite("WDirectBow", SPRID_WEAPON_W + 64 * 42, 56, TRUE);
	MakeSprite("WFireBow", SPRID_WEAPON_W + 64 * 43, 56, TRUE);
	m_cLoading = 92;
	}
		break;
	case 92:
	{	MakeSprite("Wmantle01", SPRID_MANTLE_W + 15 * 1, 12, TRUE);
	MakeSprite("Wmantle02", SPRID_MANTLE_W + 15 * 2, 12, TRUE);
	MakeSprite("Wmantle03", SPRID_MANTLE_W + 15 * 3, 12, TRUE);
	MakeSprite("Wmantle04", SPRID_MANTLE_W + 15 * 4, 12, TRUE);
	MakeSprite("Wmantle05", SPRID_MANTLE_W + 15 * 5, 12, TRUE);
	MakeSprite("Wmantle06", SPRID_MANTLE_W + 15 * 6, 12, TRUE);
	MakeSprite("WHelm1", SPRID_HEAD_W + 15 * 1, 12, TRUE);
	MakeSprite("WHelm4", SPRID_HEAD_W + 15 * 4, 12, TRUE);
	MakeSprite("WHHelm1", SPRID_HEAD_W + 15 * 9, 12, TRUE);
	MakeSprite("WHHelm2", SPRID_HEAD_W + 15 * 10, 12, TRUE);
	MakeSprite("WHCap1", SPRID_HEAD_W + 15 * 11, 12, TRUE);
	MakeSprite("WHCap2", SPRID_HEAD_W + 15 * 12, 12, TRUE);
	MakeSprite("NWHelm1", SPRID_HEAD_W + 15 * 5, 12, TRUE);
	MakeSprite("NWHelm2", SPRID_HEAD_W + 15 * 6, 12, TRUE);
	MakeSprite("NWHelm3", SPRID_HEAD_W + 15 * 7, 12, TRUE);
	MakeSprite("NWHelm4", SPRID_HEAD_W + 15 * 8, 12, TRUE);
	m_cLoading = 96;
	}
		break;
	case 96:
	{	MakeSprite("Wbo", SPRID_WEAPON_W + 64 * 40, 56, TRUE);// Bow
	m_hPakFile = CreateFile("sprites\\Wbo.pak", GENERIC_READ, NULL, NULL, OPEN_EXISTING, NULL, NULL);
	if (m_hPakFile != INVALID_HANDLE_VALUE)
	{
		for (i = 0; i < 56; i++) m_pSprite[SPRID_WEAPON_W + i + 64 * 41] = new class CSprite(m_hPakFile, &m_DDraw, "Wbo", i + 56 * 1, TRUE);
		CloseHandle(m_hPakFile);
	}
	m_hPakFile = CreateFile("sprites\\Wsh.pak", GENERIC_READ, NULL, NULL, OPEN_EXISTING, NULL, NULL);
	if (m_hPakFile != INVALID_HANDLE_VALUE) {
		for (i = 0; i < 7; i++) m_pSprite[SPRID_SHIELD_W + i + 8 * 1] = new class CSprite(m_hPakFile, &m_DDraw, "Wsh", i + 7 * 0, TRUE);
		for (i = 0; i < 7; i++) m_pSprite[SPRID_SHIELD_W + i + 8 * 2] = new class CSprite(m_hPakFile, &m_DDraw, "Wsh", i + 7 * 1, TRUE);
		for (i = 0; i < 7; i++) m_pSprite[SPRID_SHIELD_W + i + 8 * 3] = new class CSprite(m_hPakFile, &m_DDraw, "Wsh", i + 7 * 2, TRUE);
		for (i = 0; i < 7; i++) m_pSprite[SPRID_SHIELD_W + i + 8 * 4] = new class CSprite(m_hPakFile, &m_DDraw, "Wsh", i + 7 * 3, TRUE);
		for (i = 0; i < 7; i++)	m_pSprite[SPRID_SHIELD_W + i + 8 * 5] = new class CSprite(m_hPakFile, &m_DDraw, "Wsh", i + 7 * 4, TRUE);
		for (i = 0; i < 7; i++) m_pSprite[SPRID_SHIELD_W + i + 8 * 6] = new class CSprite(m_hPakFile, &m_DDraw, "Wsh", i + 7 * 5, TRUE);
		for (i = 0; i < 7; i++) m_pSprite[SPRID_SHIELD_W + i + 8 * 7] = new class CSprite(m_hPakFile, &m_DDraw, "Wsh", i + 7 * 6, TRUE);
		for (i = 0; i < 7; i++) m_pSprite[SPRID_SHIELD_W + i + 8 * 8] = new class CSprite(m_hPakFile, &m_DDraw, "Wsh", i + 7 * 7, TRUE);
		for (i = 0; i < 7; i++) m_pSprite[SPRID_SHIELD_W + i + 8 * 9] = new class CSprite(m_hPakFile, &m_DDraw, "Wsh", i + 7 * 8, TRUE);
		CloseHandle(m_hPakFile);
	}
	m_cLoading = 100;
	}
		break;
	case 100:
	{	MakeEffectSpr("effect", 0, 10, FALSE);
	MakeEffectSpr("effect2", 10, 3, FALSE);
	MakeEffectSpr("effect3", 13, 6, FALSE);
	MakeEffectSpr("effect4", 19, 5, FALSE);
	m_hPakFile = CreateFile("sprites\\effect5.pak", GENERIC_READ, NULL, NULL, OPEN_EXISTING, NULL, NULL);
	if (m_hPakFile != INVALID_HANDLE_VALUE)
	{
		for (i = 0; i <= 6; i++) // Because effectn∞0 is EnergySphere
			m_pEffectSpr[i + 24] = new class CSprite(m_hPakFile, &m_DDraw, "effect5", i + 1, FALSE);
		CloseHandle(m_hPakFile);
	}
	MakeEffectSpr("CruEffect1", 31, 9, FALSE);
	MakeEffectSpr("effect6", 40, 5, FALSE);
	MakeEffectSpr("effect7", 45, 12, FALSE);
	MakeEffectSpr("effect8", 57, 9, FALSE);
	MakeEffectSpr("effect9", 66, 21, FALSE);

	MakeEffectSpr("effect10", 87, 2, FALSE); // Effets Hero items
	MakeEffectSpr("effect11", 89, 14, FALSE); // Cancel, stormBlade, resu, GateHeldenian....etc
	//NB: Charge 15 du client 3.51, mais il n'y a que 14 ds le PAK
	MakeEffectSpr("effect11s", 104, 1, FALSE); // effet sort mais je ne sais pas lequel
	// Manque des effets ici .....
	// MakeEffectSpr( "effect13", 108, 2, FALSE); // not loaded by client 351 (Heldenian gates death)
	//MakeEffectSpr( "yseffect2", 141, 8, FALSE); // Wrong in 351 client...
	MakeEffectSpr("yseffect2", 140, 8, FALSE); // Abaddon's death
	MakeEffectSpr("effect12", 148, 4, FALSE); // Slates auras
	MakeEffectSpr("yseffect3", 152, 16, FALSE); // Fumerolles ou ame qui s'envole
	//MakeEffectSpr( "yseffect4", 167, 7, FALSE); // Wrong in 351 client
	MakeEffectSpr("yseffect4", 133, 7, FALSE); // Abaddon's map thunder.


	if (m_bSoundFlag) // Attention il y a un autre systeme de chargement ds la v351
	{
		for (i = 1; i <= 24; i++)
		{
			wsprintf(G_cTxt, "sounds\\C%d.wav", i);
			m_pCSound[i] = new class CSoundBuffer(m_DSound.m_lpDS, m_DSound.m_DSCaps, G_cTxt);
		}

		for (i = 1; i <= 156; i++)
		{
			wsprintf(G_cTxt, "sounds\\M%d.wav", i);
			m_pMSound[i] = new class CSoundBuffer(m_DSound.m_lpDS, m_DSound.m_DSCaps, G_cTxt);
		}
		for (i = 1; i <= 53; i++)
		{
			wsprintf(G_cTxt, "sounds\\E%d.wav", i);
			m_pESound[i] = new class CSoundBuffer(m_DSound.m_lpDS, m_DSound.m_DSCaps, G_cTxt);
		}
	}
	ChangeGameMode(GAMEMODE_ONMAINMENU);
	}
		break;
	}
}
void cMain::ChangeGameMode(char cMode)
{
	m_cGameMode = cMode;
	m_cGameModeCount = 0;
	m_dwTime = G_dwGlobalTime;

#ifndef SELECTSERVER
	if (cMode == GAMEMODE_ONSELECTSERVER)
	{
		ZeroMemory(m_cWorldServerName, sizeof(m_cWorldServerName));
		strcpy(m_cWorldServerName, NAME_WORLDNAME1);
		m_cGameMode = GAMEMODE_ONLOGIN;
	}
#endif
}

void cMain::UpdateScreen_OnLoading_Progress()
{
	m_DDraw.ClearBackB4();
	DrawNewDialogBox(SPRID_INTERFACE_ND_LOADING, 0, 0, 0, TRUE);
	DrawVersion(TRUE);
	int iBarWidth;
	iBarWidth = (int)m_cLoading;
	m_pSprite[SPRID_INTERFACE_ND_LOADING]->PutSpriteFastWidth(472, 442, 1, iBarWidth, G_dwGlobalTime);
	m_DDraw.iFlip();
}

void cMain::MakeSprite(char* FileName, short sStart, short sCount, bool bAlphaEffect)
{
	int iTotalimage;
	DWORD nCount;
	char PathName[28];
	wsprintf(PathName, "sprites\\%s.pak", FileName);
	HANDLE m_hPakFile = CreateFile(PathName, GENERIC_READ, NULL, NULL, OPEN_EXISTING, NULL, NULL);
	if (m_hPakFile == INVALID_HANDLE_VALUE) return;
	SetFilePointer(m_hPakFile, 20, NULL, FILE_BEGIN);
	ReadFile(m_hPakFile, (char *)&iTotalimage, 4, &nCount, NULL);
	for (short i = 0; i < sCount; i++)
	{
		if (i < iTotalimage) m_pSprite[i + sStart] = new class CSprite(m_hPakFile, &m_DDraw, FileName, i, bAlphaEffect);
	}
	CloseHandle(m_hPakFile);
}

void cMain::MakeTileSpr(char* FileName, short sStart, short sCount, bool bAlphaEffect)
{
	int iTotalimage;
	DWORD nCount;
	char PathName[28];
	wsprintf(PathName, "sprites\\%s.pak", FileName);
	HANDLE m_hPakFile = CreateFile(PathName, GENERIC_READ, NULL, NULL, OPEN_EXISTING, NULL, NULL);
	if (m_hPakFile == INVALID_HANDLE_VALUE) return;
	SetFilePointer(m_hPakFile, 20, NULL, FILE_BEGIN);
	ReadFile(m_hPakFile, (char *)&iTotalimage, 4, &nCount, NULL);
	for (short i = 0; i < sCount; i++)
	{
		if (i < iTotalimage) m_pTileSpr[i + sStart] = new class CSprite(m_hPakFile, &m_DDraw, FileName, i, bAlphaEffect);
	}
	CloseHandle(m_hPakFile);
}

void cMain::MakeEffectSpr(char* FileName, short sStart, short sCount, bool bAlphaEffect)
{
	int iTotalimage;
	DWORD nCount;
	char PathName[28];
	wsprintf(PathName, "sprites\\%s.pak", FileName);
	HANDLE m_hPakFile = CreateFile(PathName, GENERIC_READ, NULL, NULL, OPEN_EXISTING, NULL, NULL);
	if (m_hPakFile == INVALID_HANDLE_VALUE) return;
	SetFilePointer(m_hPakFile, 20, NULL, FILE_BEGIN);
	ReadFile(m_hPakFile, (char *)&iTotalimage, 4, &nCount, NULL);
	for (short i = 0; i < sCount; i++)
	{
		if (i < iTotalimage) m_pEffectSpr[i + sStart] = new class CSprite(m_hPakFile, &m_DDraw, FileName, i, bAlphaEffect);
	}
	CloseHandle(m_hPakFile); 
}

void cMain::bAddNewEffect(short sType, int sX, int sY, int dX, int dY, char cStartFrame, int iV1)
{
	int i;
	short sAbsX, sAbsY, sDist;
	long lPan;
	int  iV2 = 0;
	if (m_cDetailLevel == 0) // Detail Level Low
	{
		switch (sType) {
		case 8:
		case 9:
		case 11:
		case 12:
		case 14:
		case 15:
			return;
		}
	}
	//if (m_bIsProgramActive == FALSE) return;
	sAbsX = abs(((m_sViewPointX / 32) + 10) - dX);
	sAbsY = abs(((m_sViewPointY / 32) + 7) - dY);
	if (sAbsX > sAbsY) sDist = sAbsX;
	else sDist = sAbsY;

	for (i = 0; i < MAXEFFECTS; i++)
	if (m_pEffectList[i] == NULL)
	{
		m_pEffectList[i] = new class CEffect;
		m_pEffectList[i]->m_sType = sType;
		m_pEffectList[i]->m_sX = sX;
		m_pEffectList[i]->m_sY = sY;
		m_pEffectList[i]->m_dX = dX;
		m_pEffectList[i]->m_dY = dY;
		m_pEffectList[i]->m_iV1 = iV1;
		m_pEffectList[i]->m_cFrame = cStartFrame;
		m_pEffectList[i]->m_dwTime = m_dwCurTime;

		switch (sType) {
		case 1: // coup normal
			m_pEffectList[i]->m_mX = sX * 32;
			m_pEffectList[i]->m_mY = sY * 32 - _iAttackerHeight[iV1];
			m_pEffectList[i]->m_iErr = 0;
			m_pEffectList[i]->m_cMaxFrame = 2;
			m_pEffectList[i]->m_dwFrameTime = 10;
			break;

		case 2:	// FlÍche qui vole
			m_pEffectList[i]->m_mX = sX * 32;
			m_pEffectList[i]->m_mY = sY * 32 - _iAttackerHeight[iV1];
			m_pEffectList[i]->m_iErr = 0;
			m_pEffectList[i]->m_cMaxFrame = 0;
			m_pEffectList[i]->m_dwFrameTime = 10;
			m_pEffectList[i]->m_cDir = m_Misc.cCalcDirection(sX, sY, dX, dY);
			PlaySound('C', 4, sDist);
			break;

		case 4: // Gold
			m_pEffectList[i]->m_mX = sX * 32;
			m_pEffectList[i]->m_mY = sY * 32;
			m_pEffectList[i]->m_cMaxFrame = 12;
			m_pEffectList[i]->m_dwFrameTime = 100;
			sAbsX = abs(((m_sViewPointX / 32) + 10) - sX);
			sAbsY = abs(((m_sViewPointY / 32) + 7) - sY);
			if (sAbsX > sAbsY) sDist = sAbsX;
			else sDist = sAbsY;
			lPan = -(((m_sViewPointX / 32) + 10) - sX) * 1000;
			PlaySound('E', 12, sDist, lPan);
			break;

		case 5: // FireBall Fire Explosion
			m_pEffectList[i]->m_mX = sX;
			m_pEffectList[i]->m_mY = sY;
			m_pEffectList[i]->m_cMaxFrame = 11;
			m_pEffectList[i]->m_dwFrameTime = 10;
			sAbsX = abs(320 - (sX - m_sViewPointX));
			sAbsY = abs(240 - (sY - m_sViewPointY));
			if (sAbsX > sAbsY) sDist = sAbsX;
			else sDist = sAbsY;
			sDist = sDist / 32;
			lPan = -(((m_sViewPointX / 32) + 10) - sX) * 1000;
			PlaySound('E', 4, sDist, lPan);
			SetCameraShakingEffect(sDist);
			break;

		case 6:	 // Energy Bolt
		case 10: // Lightning Arrow
			m_pEffectList[i]->m_mX = sX;
			m_pEffectList[i]->m_mY = sY;
			m_pEffectList[i]->m_cMaxFrame = 14;
			m_pEffectList[i]->m_dwFrameTime = 10;
			sAbsX = abs(320 - (sX - m_sViewPointX));
			sAbsY = abs(240 - (sY - m_sViewPointY));
			if (sAbsX > sAbsY) sDist = sAbsX;
			else sDist = sAbsY;
			sDist = sDist / 32;
			lPan = -(320 - (sX - m_sViewPointX)) * 1000;
			PlaySound('E', 2, sDist, lPan);
			SetCameraShakingEffect(sDist);
			break;

		case 7: // Magic Missile Explosion
			m_pEffectList[i]->m_mX = sX;
			m_pEffectList[i]->m_mY = sY;
			m_pEffectList[i]->m_cMaxFrame = 5;
			m_pEffectList[i]->m_dwFrameTime = 50;
			sAbsX = abs(320 - (sX - m_sViewPointX));
			sAbsY = abs(240 - (sY - m_sViewPointY));
			if (sAbsX > sAbsY) sDist = sAbsX;
			else sDist = sAbsY;
			sDist = sDist / 32;
			lPan = -(320 - (sX - m_sViewPointX)) * 1000;
			PlaySound('E', 3, sDist, lPan);
			break;

		case 8: // Burst
			m_pEffectList[i]->m_mX = sX;
			m_pEffectList[i]->m_mY = sY;
			m_pEffectList[i]->m_cMaxFrame = 4;
			m_pEffectList[i]->m_dwFrameTime = 30;
			break;

		case 9: // Burst
			m_pEffectList[i]->m_mX = sX;
			m_pEffectList[i]->m_mY = sY;
			m_pEffectList[i]->m_rX = 6 - (rand() % 12);
			m_pEffectList[i]->m_rY = -8 - (rand() % 6);
			m_pEffectList[i]->m_cMaxFrame = 14;
			m_pEffectList[i]->m_dwFrameTime = 30;
			break;

		case 11:
			m_pEffectList[i]->m_mX = sX;
			m_pEffectList[i]->m_mY = sY;
			m_pEffectList[i]->m_rX = 6 - (rand() % 12);
			if (iV2 == 0)
				m_pEffectList[i]->m_rY = -2 - (rand() % 4);
			else m_pEffectList[i]->m_rY = -2 - (rand() % 10);
			m_pEffectList[i]->m_cMaxFrame = 8;
			m_pEffectList[i]->m_dwFrameTime = 30;
			break;

		case 12: // Burst
			m_pEffectList[i]->m_mX = sX;
			m_pEffectList[i]->m_mY = sY;
			m_pEffectList[i]->m_rX = 8 - (rand() % 16);
			m_pEffectList[i]->m_rY = 4 - (rand() % 12);
			m_pEffectList[i]->m_cMaxFrame = 10;
			m_pEffectList[i]->m_dwFrameTime = 30;
			break;

		case 13: // Bulles druncncity
			m_pEffectList[i]->m_mX = sX;
			m_pEffectList[i]->m_mY = sY;
			m_pEffectList[i]->m_cMaxFrame = 18;
			m_pEffectList[i]->m_dwFrameTime = 20;
			break;

		case 14: // Traces de pas ou Tremor (pas en low detail)
			m_pEffectList[i]->m_mX = sX;
			if (m_pEffectList[i]->m_iV1 > 0) // Case if hit by an arrow
			{
				m_pEffectList[i]->m_mY = sY - (_iAttackerHeight[m_pEffectList[i]->m_iV1] / 4 + rand() % (_iAttackerHeight[m_pEffectList[i]->m_iV1] / 2));
				m_pEffectList[i]->m_mX = sX + (rand() % 5) - 2;
			}
			else m_pEffectList[i]->m_mY = sY;
			m_pEffectList[i]->m_cMaxFrame = 4;
			m_pEffectList[i]->m_dwFrameTime = 100;
			m_pEffectList[i]->m_iV1 = iV1;
			break;

		case 15: //
			m_pEffectList[i]->m_mX = sX;
			m_pEffectList[i]->m_mY = sY;
			m_pEffectList[i]->m_cMaxFrame = 16;
			m_pEffectList[i]->m_dwFrameTime = 80;
			break;

		case 16: //
			m_pEffectList[i]->m_mX = sX * 32;
			m_pEffectList[i]->m_mY = sY * 32 - 40;
			m_pEffectList[i]->m_iErr = 0;
			m_pEffectList[i]->m_cMaxFrame = NULL;
			m_pEffectList[i]->m_dwFrameTime = 20;
			break;

		case 17:
			m_pEffectList[i]->m_mX = sX + (rand() % 20) - 40;
			m_pEffectList[i]->m_mY = sY + (rand() % 20) - 40;
			m_pEffectList[i]->m_rX = 8 - (rand() % 16);
			m_pEffectList[i]->m_rY = 4 - (rand() % 12);
			m_pEffectList[i]->m_mX3 = sX;
			m_pEffectList[i]->m_mY3 = sY;
			m_pEffectList[i]->m_iV1 = 0;
			m_pEffectList[i]->m_dwFrameTime = 20;
			break;

		case 18:
			m_pEffectList[i]->m_mX = sX;
			m_pEffectList[i]->m_mY = sY;
			m_pEffectList[i]->m_cMaxFrame = 10;
			m_pEffectList[i]->m_dwFrameTime = 50;
			sAbsX = abs(320 - (sX - m_sViewPointX));
			sAbsY = abs(240 - (sY - m_sViewPointY));
			if (sAbsX > sAbsY) sDist = sAbsX;
			else sDist = sAbsY;
			sDist = sDist / 32;
			SetCameraShakingEffect(sDist);
			break;

		case 20:
		case 21:
		case 22:
		case 23:
		case 24:
		case 25:
		case 26:
		case 27: // Critical strike with a weapon
			m_pEffectList[i]->m_mX = sX * 32;
			m_pEffectList[i]->m_mY = sY * 32 - 40;
			m_pEffectList[i]->m_iErr = 0;
			m_pEffectList[i]->m_cMaxFrame = 0;
			m_pEffectList[i]->m_dwFrameTime = 10;
			m_pEffectList[i]->m_cDir = m_Misc.cCalcDirection(sX, sY, dX, dY);
			break;

		case 30: // Mass-Fire-Strike (called 1 time)
			m_pEffectList[i]->m_mX = sX;
			m_pEffectList[i]->m_mY = sY;
			m_pEffectList[i]->m_cMaxFrame = 9;
			m_pEffectList[i]->m_dwFrameTime = 40;
			sAbsX = abs(320 - (sX - m_sViewPointX));
			sAbsY = abs(240 - (sY - m_sViewPointY));
			if (sAbsX > sAbsY) sDist = sAbsX;
			else sDist = sAbsY;
			sDist = sDist / 32;
			lPan = -(320 - (sX - m_sViewPointX)) * 1000;
			PlaySound('E', 4, sDist, lPan);
			SetCameraShakingEffect(sDist * 2);
			break;

		case 31: // Mass-Fire-Strike (called 3 times)
		case 252: //
			m_pEffectList[i]->m_mX = sX;
			m_pEffectList[i]->m_mY = sY;
			m_pEffectList[i]->m_cMaxFrame = 8;
			m_pEffectList[i]->m_dwFrameTime = 40;
			sAbsX = abs(320 - (sX - m_sViewPointX));
			sAbsY = abs(240 - (sY - m_sViewPointY));
			if (sAbsX > sAbsY) sDist = sAbsX;
			else sDist = sAbsY;
			sDist = sDist / 32;
			lPan = -(320 - (sX - m_sViewPointX)) * 1000;
			PlaySound('E', 4, sDist, lPan);
			SetCameraShakingEffect(sDist);
			break;

		case 32: //
			m_pEffectList[i]->m_mX = sX;
			m_pEffectList[i]->m_mY = sY;
			m_pEffectList[i]->m_iErr = 0;
			m_pEffectList[i]->m_cMaxFrame = 4;
			m_pEffectList[i]->m_dwFrameTime = 100;
			break;

		case 33: //
			m_pEffectList[i]->m_mX = sX;
			m_pEffectList[i]->m_mY = sY;
			m_pEffectList[i]->m_cMaxFrame = 16;
			m_pEffectList[i]->m_dwFrameTime = 10;
			break;

		case 34: //
			m_pEffectList[i]->m_mX = sX * 32;
			m_pEffectList[i]->m_mY = sY * 32 - 40;
			m_pEffectList[i]->m_iErr = 0;
			m_pEffectList[i]->m_cMaxFrame = NULL;
			m_pEffectList[i]->m_dwFrameTime = 20;
			sAbsX = abs(320 - (sX - m_sViewPointX));
			sAbsY = abs(240 - (sY - m_sViewPointY));
			if (sAbsX > sAbsY) sDist = sAbsX;
			else sDist = sAbsY;
			sDist = sDist / 32;
			SetCameraShakingEffect(sDist);
			break;

		case 35: // Snoopy: rajout (pour Mass Magic-Missile)
			m_pEffectList[i]->m_mX = sX;
			m_pEffectList[i]->m_mY = sY;
			m_pEffectList[i]->m_cMaxFrame = 18;
			m_pEffectList[i]->m_dwFrameTime = 40;
			sAbsX = abs(320 - (sX - m_sViewPointX));
			sAbsY = abs(240 - (sY - m_sViewPointY));
			if (sAbsX > sAbsY) sDist = sAbsX;
			else sDist = sAbsY;
			sDist = sDist / 32;
			lPan = -(320 - (sX - m_sViewPointX)) * 1000;
			PlaySound('E', 4, sDist, lPan);
			SetCameraShakingEffect(sDist * 2);
			break;

		case 36: // Snoopy: Rajout (pour Mass Magic-Missile)
			m_pEffectList[i]->m_mX = sX;
			m_pEffectList[i]->m_mY = sY;
			m_pEffectList[i]->m_cMaxFrame = 15;
			m_pEffectList[i]->m_dwFrameTime = 40;
			sAbsX = abs(320 - (sX - m_sViewPointX));
			sAbsY = abs(240 - (sY - m_sViewPointY));
			if (sAbsX > sAbsY) sDist = sAbsX;
			else sDist = sAbsY;
			sDist = sDist / 32;
			lPan = -(320 - (sX - m_sViewPointX)) * 1000;
			PlaySound('E', 4, sDist, lPan);
			SetCameraShakingEffect(sDist);
			break;

		case 40: //
			m_pEffectList[i]->m_mX = sX;
			m_pEffectList[i]->m_mY = sY;
			m_pEffectList[i]->m_cMaxFrame = 15;
			m_pEffectList[i]->m_dwFrameTime = 30;
			sAbsX = abs(320 - (sX - m_sViewPointX));
			sAbsY = abs(240 - (sY - m_sViewPointY));
			if (sAbsX > sAbsY) sDist = sAbsX;
			else sDist = sAbsY;
			sDist = sDist / 32;
			lPan = ((sX - m_sViewPointX) - 320) * 30;
			PlaySound('E', 45, sDist, lPan);
			break;

		case 41: // Large Type 1, 2, 3, 4
		case 42:
		case 43:
		case 44:
		case 45: // Small Type 1, 2
		case 46:
			m_pEffectList[i]->m_mX = sX;
			m_pEffectList[i]->m_mY = sY - 220;
			m_pEffectList[i]->m_cMaxFrame = 14;
			m_pEffectList[i]->m_dwFrameTime = 20;
			m_pEffectList[i]->m_iV1 = 20;
			sAbsX = abs(320 - (sX - m_sViewPointX));
			sAbsY = abs(240 - (sY - m_sViewPointY));
			if (sAbsX > sAbsY) sDist = sAbsX;
			else sDist = sAbsY;
			sDist = sDist / 32;
			lPan = ((sX - m_sViewPointX) - 320) * 30;
			PlaySound('E', 46, sDist, lPan);
			break;

		case 47: // Blizzard
		case 48: // Blizzard
		case 49: // Blizzard
			m_pEffectList[i]->m_mX = sX;
			m_pEffectList[i]->m_mY = sY - 220;
			m_pEffectList[i]->m_cMaxFrame = 12;
			m_pEffectList[i]->m_dwFrameTime = 20;
			m_pEffectList[i]->m_iV1 = 20;
			sAbsX = abs(320 - (sX - m_sViewPointX));
			sAbsY = abs(240 - (sY - m_sViewPointY));
			if (sAbsX > sAbsY) sDist = sAbsX;
			else sDist = sAbsY;
			sDist = sDist / 32;
			lPan = ((sX - m_sViewPointX) - 320) * 30;
			PlaySound('E', 46, sDist, lPan);
			break;

		case 50: //
			m_pEffectList[i]->m_mX = sX;
			m_pEffectList[i]->m_mY = sY;
			m_pEffectList[i]->m_cMaxFrame = 12;
			m_pEffectList[i]->m_dwFrameTime = 50;
			sAbsX = abs(320 - (sX - m_sViewPointX));
			sAbsY = abs(240 - (sY - m_sViewPointY));
			if (sAbsX > sAbsY) sDist = sAbsX;
			else sDist = sAbsY;
			sDist = sDist / 32;
			lPan = ((sX - m_sViewPointX) - 320) * 30;
			if ((rand() % 4) == 1) SetCameraShakingEffect(sDist);
			PlaySound('E', 47, sDist, lPan);
			break;

		case 51:
			m_pEffectList[i]->m_mX = sX;
			m_pEffectList[i]->m_mY = sY;
			m_pEffectList[i]->m_cMaxFrame = 9; //15;
			m_pEffectList[i]->m_dwFrameTime = 80;
			break;

		case 52: // Protect ring
			m_pEffectList[i]->m_mX = sX;
			m_pEffectList[i]->m_mY = sY;
			m_pEffectList[i]->m_cMaxFrame = 15;
			m_pEffectList[i]->m_dwFrameTime = 80;
			sAbsX = abs(320 - (sX - m_sViewPointX));
			sAbsY = abs(240 - (sY - m_sViewPointY));
			if (sAbsX > sAbsY) sDist = sAbsX;
			else sDist = sAbsY;
			sDist = sDist / 32;
			lPan = ((sX - m_sViewPointX) - 320) * 30;
			PlaySound('E', 5, sDist, lPan);
			break;

		case 53: // Hold twist
			m_pEffectList[i]->m_mX = sX;
			m_pEffectList[i]->m_mY = sY;
			m_pEffectList[i]->m_cMaxFrame = 15;
			m_pEffectList[i]->m_dwFrameTime = 80;
			sAbsX = abs(320 - (sX - m_sViewPointX));
			sAbsY = abs(240 - (sY - m_sViewPointY));
			if (sAbsX > sAbsY) sDist = sAbsX;
			else sDist = sAbsY;
			sDist = sDist / 32;
			lPan = ((sX - m_sViewPointX) - 320) * 30;
			PlaySound('E', 5, sDist, lPan);
			break;

		case 54: // star twingkling (effect armes brillantes)
		case 55: // Unused
			m_pEffectList[i]->m_mX = sX;
			m_pEffectList[i]->m_mY = sY;
			m_pEffectList[i]->m_cMaxFrame = 10;
			m_pEffectList[i]->m_dwFrameTime = 15;
			break;

		case 56: //  Mass-Chill-Wind
			m_pEffectList[i]->m_mX = sX;
			m_pEffectList[i]->m_mY = sY;
			m_pEffectList[i]->m_cMaxFrame = 14;
			m_pEffectList[i]->m_dwFrameTime = 30;
			sAbsX = abs(320 - (sX - m_sViewPointX));
			sAbsY = abs(240 - (sY - m_sViewPointY));
			if (sAbsX > sAbsY) sDist = sAbsX;
			else sDist = sAbsY;
			sDist = sDist / 32;
			lPan = ((sX - m_sViewPointX) - 320) * 30;
			PlaySound('E', 45, sDist, lPan);
			break;

		case 57: //
			m_pEffectList[i]->m_mX = sX;
			m_pEffectList[i]->m_mY = sY;
			m_pEffectList[i]->m_cMaxFrame = 16;
			m_pEffectList[i]->m_dwFrameTime = 80;
			break;

		case 60: //
			m_pEffectList[i]->m_mX = sX + 300;
			m_pEffectList[i]->m_mY = sY - 460;
			m_pEffectList[i]->m_cMaxFrame = 10;
			m_pEffectList[i]->m_dwFrameTime = 50;
			break;

		case 61: //
			m_pEffectList[i]->m_mX = sX;
			m_pEffectList[i]->m_mY = sY;
			m_pEffectList[i]->m_cMaxFrame = 16;
			m_pEffectList[i]->m_dwFrameTime = 10;
			sAbsX = abs(320 - (sX - m_sViewPointX));
			sAbsY = abs(240 - (sY - m_sViewPointY));
			if (sAbsX > sAbsY) sDist = sAbsX;
			else sDist = sAbsY;
			sDist = sDist / 32;
			lPan = -(((m_sViewPointX / 32) + 10) - sX) * 1000;
			PlaySound('E', 4, sDist, lPan);
			SetCameraShakingEffect(sDist, 2);
			break;

		case 62: //
			m_pEffectList[i]->m_mX = sX;
			m_pEffectList[i]->m_mY = sY;
			m_pEffectList[i]->m_cMaxFrame = 6;
			m_pEffectList[i]->m_dwFrameTime = 100;
			break;

		case 63: //
			m_pEffectList[i]->m_mX = sX;
			m_pEffectList[i]->m_mY = sY;
			m_pEffectList[i]->m_cMaxFrame = 16;
			m_pEffectList[i]->m_dwFrameTime = 20;
			break;

		case 64: //
			m_pEffectList[i]->m_mX = sX;
			m_pEffectList[i]->m_mY = sY;
			m_pEffectList[i]->m_cMaxFrame = 15;
			m_pEffectList[i]->m_dwFrameTime = 20;
			break;

		case 65: // Crusade's MS
			m_pEffectList[i]->m_mX = sX;
			m_pEffectList[i]->m_mY = sY;
			m_pEffectList[i]->m_cMaxFrame = 30;
			m_pEffectList[i]->m_dwFrameTime = 80;
			break;

		case 66: // Crusade MS explosion
			m_pEffectList[i]->m_mX = sX;
			m_pEffectList[i]->m_mY = sY;
			m_pEffectList[i]->m_cMaxFrame = 14;
			m_pEffectList[i]->m_dwFrameTime = 30;
			sAbsX = abs(320 - (sX - m_sViewPointX));
			sAbsY = abs(240 - (sY - m_sViewPointY));
			if (sAbsX > sAbsY) sDist = sAbsX;
			else sDist = sAbsY;
			sDist = sDist / 32;
			lPan = -(((m_sViewPointX / 32) + 10) - sX) * 1000;
			PlaySound('E', 4, sDist, lPan);
			SetCameraShakingEffect(sDist, 2);
			break;

		case 67: // Crusade's MS fire + smoke ?
			m_pEffectList[i]->m_mX = sX;
			m_pEffectList[i]->m_mY = sY;
			m_pEffectList[i]->m_cMaxFrame = 27;
			m_pEffectList[i]->m_dwFrameTime = 10;
			break;

		case 68: // worm-bite
			m_pEffectList[i]->m_mX = sX;
			m_pEffectList[i]->m_mY = sY;
			m_pEffectList[i]->m_cMaxFrame = 17;
			m_pEffectList[i]->m_dwFrameTime = 30;
			sAbsX = abs(320 - (sX - m_sViewPointX));
			sAbsY = abs(240 - (sY - m_sViewPointY));
			if (sAbsX > sAbsY) sDist = sAbsX;
			else sDist = sAbsY;
			sDist = sDist / 32;
			lPan = -(((m_sViewPointX / 32) + 10) - sX) * 1000;
			PlaySound('E', 4, sDist, lPan);
			m_pEffectList[i]->m_iV1 = sDist;
			//SetCameraShakingEffect(sDist, 2);
			break;

		case 69: // identique au cas 70
			m_pEffectList[i]->m_mX = sX;
			m_pEffectList[i]->m_mY = sY;
			m_pEffectList[i]->m_cMaxFrame = 11;
			m_pEffectList[i]->m_dwFrameTime = 30;
			sAbsX = abs(320 - (sX - m_sViewPointX));
			sAbsY = abs(240 - (sY - m_sViewPointY));
			if (sAbsX > sAbsY) sDist = sAbsX;
			else sDist = sAbsY;
			sDist = sDist / 32;
			lPan = -(((m_sViewPointX / 32) + 10) - (sX / 32)) * 1000;
			PlaySound('E', 42, sDist, lPan);
			break;

		case 70: // identtique au cas 69
			m_pEffectList[i]->m_mX = sX;
			m_pEffectList[i]->m_mY = sY;
			m_pEffectList[i]->m_cMaxFrame = 11;
			m_pEffectList[i]->m_dwFrameTime = 30;
			sAbsX = abs(320 - (sX - m_sViewPointX));
			sAbsY = abs(240 - (sY - m_sViewPointY));
			if (sAbsX > sAbsY) sDist = sAbsX;
			else sDist = sAbsY;
			sDist = sDist / 32;
			lPan = -(((m_sViewPointX / 32) + 10) - (sX / 32)) * 1000;
			PlaySound('E', 42, sDist, lPan);
			break;

		case 71: //
			m_pEffectList[i]->m_mX = sX * 32;
			m_pEffectList[i]->m_mY = sY * 32;
			m_pEffectList[i]->m_iErr = 0;
			m_pEffectList[i]->m_cMaxFrame = NULL;
			m_pEffectList[i]->m_dwFrameTime = 20;
			sAbsX = abs(320 - (sX - m_sViewPointX));
			sAbsY = abs(240 - (sY - m_sViewPointY));
			if (sAbsX > sAbsY) sDist = sAbsX;
			else sDist = sAbsY;
			sDist = sDist / 32;
			SetCameraShakingEffect(sDist);
			break;

		case 72: // Blizzard
			m_pEffectList[i]->m_mX = sX;
			m_pEffectList[i]->m_mY = sY;
			m_pEffectList[i]->m_cMaxFrame = 15;
			m_pEffectList[i]->m_dwFrameTime = 20;
			sAbsX = abs(320 - (sX - m_sViewPointX));
			sAbsY = abs(240 - (sY - m_sViewPointY));
			if (sAbsX > sAbsY) sDist = sAbsX;
			else sDist = sAbsY;
			sDist = sDist / 32;
			lPan = ((sX - m_sViewPointX) - 320) * 30;
			if ((rand() % 4) == 1) SetCameraShakingEffect(sDist);
			PlaySound('E', 47, sDist, lPan);
			break;

		case 73:
			m_pEffectList[i]->m_mX = sX;
			m_pEffectList[i]->m_mY = sY;
			m_pEffectList[i]->m_cMaxFrame = 15;
			m_pEffectList[i]->m_dwFrameTime = 60;
			break;

		case 74:
			m_pEffectList[i]->m_mX = sX;
			m_pEffectList[i]->m_mY = sY;
			m_pEffectList[i]->m_cMaxFrame = 19;
			m_pEffectList[i]->m_dwFrameTime = 40;
			break;

		case 75: //ice golem
			m_pEffectList[i]->m_mX = sX;
			m_pEffectList[i]->m_mY = sY;
			m_pEffectList[i]->m_dX = dX;
			m_pEffectList[i]->m_dY = dY;
			m_pEffectList[i]->m_cMaxFrame = 16;
			m_pEffectList[i]->m_dwFrameTime = 40;
			break;

		case 76: //ice golem
			m_pEffectList[i]->m_mX = sX;
			m_pEffectList[i]->m_mY = sY;
			m_pEffectList[i]->m_dX = dX;
			m_pEffectList[i]->m_dY = dY;
			m_pEffectList[i]->m_cMaxFrame = 16;
			m_pEffectList[i]->m_dwFrameTime = 40;
			break;

		case 77: //ice golem
			m_pEffectList[i]->m_mX = sX;
			m_pEffectList[i]->m_mY = sY;
			m_pEffectList[i]->m_dX = dX;
			m_pEffectList[i]->m_dY = dY;
			m_pEffectList[i]->m_cMaxFrame = 16;
			m_pEffectList[i]->m_dwFrameTime = 40;
			break;

		case 80: // Snoopy: rajouÈ, implÈmentÈ en dernier ds la v351
			m_pEffectList[i]->m_mX = sX;
			m_pEffectList[i]->m_mY = sY;
			m_pEffectList[i]->m_iV1 = 20;
			m_pEffectList[i]->m_cMaxFrame = 30;
			m_pEffectList[i]->m_dwFrameTime = 25;
			sAbsX = abs(320 - (sX - m_sViewPointX));
			sAbsY = abs(240 - (sY - m_sViewPointY));
			if (sAbsX > sAbsY) sDist = sAbsX;
			else sDist = sAbsY;
			sDist = sDist / 32;
			SetCameraShakingEffect(sDist);
			break;

		case 81: //  Snoopy: Rajout (StormBlade)
			m_pEffectList[i]->m_mX = sX * 32;
			m_pEffectList[i]->m_mY = sY * 32;
			m_pEffectList[i]->m_iErr = 0;
			m_pEffectList[i]->m_cMaxFrame = 27;
			m_pEffectList[i]->m_dwFrameTime = 40;
			break;

		case 82: //  Snoopy: Rajout (Gate Apocalypse)
			m_pEffectList[i]->m_cMaxFrame = 30;
			m_pEffectList[i]->m_dwFrameTime = 40;
			break;

		case 100: // MagicMissile is Flying
			m_pEffectList[i]->m_mX = sX * 32;
			m_pEffectList[i]->m_mY = sY * 32 - 40;
			m_pEffectList[i]->m_iErr = 0;
			m_pEffectList[i]->m_cMaxFrame = NULL;
			m_pEffectList[i]->m_dwFrameTime = 20;
			lPan = -(((m_sViewPointX / 32) + 10) - dX) * 1000;
			PlaySound('E', 1, sDist, lPan);
			break;

		case 101: // Heal
		case 111: // Staminar-Drain
		case 121: // Great Heal
		case 123: // Staminar-Recovery
		case 128: // Great-Staminar-Recovery
			m_pEffectList[i]->m_cMaxFrame = 14;
			m_pEffectList[i]->m_dwFrameTime = 80;
			lPan = -(((m_sViewPointX / 32) + 10) - dX) * 1000;
			PlaySound('E', 5, sDist, lPan);
			break;

		case 102: // CreateFood
		case 122: // Recall
		case 126: // Possession
		case 127: // Poison
		case 134: // DetectInvi
		case 136: // Cure
		case 142: // Confuse language
		case 152: // Polymorph
		case 153: // Mass-Poison
		case 162: // Confusion
		case 171: // Mass-Confusion
			m_pEffectList[i]->m_cMaxFrame = 13;
			m_pEffectList[i]->m_dwFrameTime = 120;
			lPan = -(((m_sViewPointX / 32) + 10) - dX) * 1000;
			PlaySound('E', 5, sDist, lPan);
			break;

		case 110: // Energy-Bolt
			m_pEffectList[i]->m_mX = sX * 32;
			m_pEffectList[i]->m_mY = sY * 32 - 40;
			m_pEffectList[i]->m_iErr = 0;
			m_pEffectList[i]->m_cMaxFrame = NULL;
			m_pEffectList[i]->m_dwFrameTime = 20;
			lPan = -(((m_sViewPointX / 32) + 10) - dX) * 1000;
			PlaySound('E', 1, sDist, lPan);
			break;

		case 112: // Recall
		case 131: // Summon
		case 132: // Invi
			m_pEffectList[i]->m_cMaxFrame = 12;
			m_pEffectList[i]->m_dwFrameTime = 80;
			lPan = -(((m_sViewPointX / 32) + 10) - dX) * 1000;
			PlaySound('E', 5, sDist, lPan);
			break;

		case 113: // Defense-Shield
		case 144: // Great-Defense-Shield
			m_pEffectList[i]->m_cMaxFrame = 12;
			m_pEffectList[i]->m_dwFrameTime = 120;
			lPan = -(((m_sViewPointX / 32) + 10) - dX) * 1000;
			PlaySound('E', 5, sDist, lPan);
			break;

		case 114: // Celebrating Light
			bAddNewEffect(69 + (rand() % 2), dX * 32 + 20 - (rand() % 40), dY * 32 + 20 - (rand() % 40), NULL, NULL, -12);
			bAddNewEffect(69 + (rand() % 2), dX * 32 + 20 - (rand() % 40), dY * 32 + 20 - (rand() % 40), NULL, NULL, -9);
			bAddNewEffect(69 + (rand() % 2), dX * 32 + 20 - (rand() % 40), dY * 32 + 20 - (rand() % 40), NULL, NULL, -6);
			bAddNewEffect(69 + (rand() % 2), dX * 32 + 20 - (rand() % 40), dY * 32 + 20 - (rand() % 40), NULL, NULL, -3);
			bAddNewEffect(69 + (rand() % 2), dX * 32 + 20 - (rand() % 40), dY * 32 + 20 - (rand() % 40), NULL, NULL, 0);
			delete m_pEffectList[i];
			m_pEffectList[i] = NULL;
			break;

		case 120: // Fire Ball
			m_pEffectList[i]->m_mX = sX * 32;
			m_pEffectList[i]->m_mY = sY * 32 - 40;
			m_pEffectList[i]->m_iErr = 0;
			m_pEffectList[i]->m_cMaxFrame = NULL;
			m_pEffectList[i]->m_dwFrameTime = 20;
			m_pEffectList[i]->m_cDir = m_Misc.cCalcDirection(sX, sY, dX, dY);
			lPan = -(((m_sViewPointX / 32) + 10) - dX) * 1000;
			PlaySound('E', 1, sDist, lPan);
			break;

		case 124: // Protect form N.M
		case 133: // Protection from Magic
			bAddNewEffect(52, dX * 32, dY * 32, NULL, NULL, 0, 0);
			delete m_pEffectList[i];
			m_pEffectList[i] = NULL;
			break;

		case 125: // Hold Person
		case 135: // Paralyze
			bAddNewEffect(53, dX * 32, dY * 32, NULL, NULL, 0, 0);
			delete m_pEffectList[i];
			m_pEffectList[i] = NULL;
			break;

		case 130: // Fire Strike
		case 137: // Lightning Arrow
			m_pEffectList[i]->m_mX = sX * 32;
			m_pEffectList[i]->m_mY = sY * 32 - 40;
			m_pEffectList[i]->m_iErr = 0;
			m_pEffectList[i]->m_cMaxFrame = NULL;
			m_pEffectList[i]->m_dwFrameTime = 20;
			m_pEffectList[i]->m_cDir = m_Misc.cCalcDirection(sX, sY, dX, dY);
			lPan = -(((m_sViewPointX / 32) + 10) - dX) * 1000;
			PlaySound('E', 1, sDist, lPan);
			break;

		case 138: // Tremor.
			lPan = -(((m_sViewPointX / 32) + 10) - dX) * 1000;
			PlaySound('E', 4, sDist, lPan);
			SetCameraShakingEffect(sDist, 2);
			bAddNewEffect(14, dX * 32 + (rand() % 120) - 60, dY * 32 + (rand() % 80) - 40, NULL, NULL, 0, 0);
			bAddNewEffect(14, dX * 32 + (rand() % 120) - 60, dY * 32 + (rand() % 80) - 40, NULL, NULL, 0, 0);
			bAddNewEffect(14, dX * 32 + (rand() % 120) - 60, dY * 32 + (rand() % 80) - 40, NULL, NULL, 0, 0);
			bAddNewEffect(14, dX * 32 + (rand() % 120) - 60, dY * 32 + (rand() % 80) - 40, NULL, NULL, 0, 0);
			bAddNewEffect(14, dX * 32 + (rand() % 120) - 60, dY * 32 + (rand() % 80) - 40, NULL, NULL, 0, 0);
			bAddNewEffect(14, dX * 32 + (rand() % 120) - 60, dY * 32 + (rand() % 80) - 40, NULL, NULL, 0, 0);
			bAddNewEffect(14, dX * 32 + (rand() % 120) - 60, dY * 32 + (rand() % 80) - 40, NULL, NULL, 0, 0);

			bAddNewEffect(14, dX * 32 + (rand() % 120) - 60, dY * 32 + (rand() % 80) - 40, NULL, NULL, 0, 0);
			bAddNewEffect(14, dX * 32 + (rand() % 120) - 60, dY * 32 + (rand() % 80) - 40, NULL, NULL, 0, 0);
			bAddNewEffect(14, dX * 32 + (rand() % 120) - 60, dY * 32 + (rand() % 80) - 40, NULL, NULL, 0, 0);
			bAddNewEffect(14, dX * 32 + (rand() % 120) - 60, dY * 32 + (rand() % 80) - 40, NULL, NULL, 0, 0);
			bAddNewEffect(14, dX * 32 + (rand() % 120) - 60, dY * 32 + (rand() % 80) - 40, NULL, NULL, 0, 0);
			bAddNewEffect(14, dX * 32 + (rand() % 120) - 60, dY * 32 + (rand() % 80) - 40, NULL, NULL, 0, 0);
			bAddNewEffect(14, dX * 32 + (rand() % 120) - 60, dY * 32 + (rand() % 80) - 40, NULL, NULL, 0, 0);
			m_pEffectList[i]->m_cMaxFrame = 2;
			m_pEffectList[i]->m_dwFrameTime = 10;
			break;

		case 143: // Lightning
			m_pEffectList[i]->m_mX = sX * 32;
			m_pEffectList[i]->m_mY = sY * 32 - 50;
			m_pEffectList[i]->m_iErr = 0;
			m_pEffectList[i]->m_rX = 5 - (rand() % 10);
			m_pEffectList[i]->m_rY = 5 - (rand() % 10);
			m_pEffectList[i]->m_cMaxFrame = 7;
			m_pEffectList[i]->m_dwFrameTime = 10;
			lPan = -(((m_sViewPointX / 32) + 10) - dX) * 1000;
			PlaySound('E', 40, sDist, lPan);
			break;

		case 145: // ChillWind
			m_pEffectList[i]->m_cMaxFrame = 2;
			m_pEffectList[i]->m_dwFrameTime = 10;
			break;

		case 147: // Triple-Energy-Bolt
			m_pEffectList[i]->m_cMaxFrame = NULL;
			m_pEffectList[i]->m_dwFrameTime = 20;
			break;

		case 150: // Berserk : Cirlcle 6 magic
		case 177: // Illusion-Movement
		case 180: // Illusion
		case 183: // Inhibition-Casting
		case 190: // Mass-Illusion
		case 195: // Mass-Illusion-Movement
			m_pEffectList[i]->m_cMaxFrame = 11;
			m_pEffectList[i]->m_dwFrameTime = 100;
			lPan = -(((m_sViewPointX / 32) + 10) - dX) * 1000;
			PlaySound('E', 5, sDist, lPan);
			break;

		case 151: // LightningBolt
			m_pEffectList[i]->m_mX = sX * 32;
			m_pEffectList[i]->m_mY = sY * 32 - 50;
			m_pEffectList[i]->m_iErr = 0;
			m_pEffectList[i]->m_rX = 5 - (rand() % 10);
			m_pEffectList[i]->m_rY = 5 - (rand() % 10);
			m_pEffectList[i]->m_cMaxFrame = 10;
			m_pEffectList[i]->m_dwFrameTime = 10;
			lPan = -(((m_sViewPointX / 32) + 10) - dX) * 1000;
			PlaySound('E', 40, sDist, lPan);
			break;

		case 156: // Mass-Ligtning-Arrow
			m_pEffectList[i]->m_cMaxFrame = 3;
			m_pEffectList[i]->m_dwFrameTime = 130;
			break;

		case 157: // Ice-Strike
			m_pEffectList[i]->m_cMaxFrame = 2;
			m_pEffectList[i]->m_dwFrameTime = 10;
			break;

		case 160: // Energy-Strike
			m_pEffectList[i]->m_cMaxFrame = 7;
			m_pEffectList[i]->m_dwFrameTime = 80;
			break;

		case 161: // Mass-Fire-Strike
		case 251: //
			m_pEffectList[i]->m_mX = sX * 32;
			m_pEffectList[i]->m_mY = sY * 32 - 40;
			m_pEffectList[i]->m_iErr = 0;
			m_pEffectList[i]->m_cMaxFrame = NULL;
			m_pEffectList[i]->m_dwFrameTime = 20;
			m_pEffectList[i]->m_cDir = m_Misc.cCalcDirection(sX, sY, dX, dY);
			lPan = -(((m_sViewPointX / 32) + 10) - dX) * 1000;
			PlaySound('E', 1, sDist, lPan);
			break;

		case 163: // Mass-Chill-Wind
			m_pEffectList[i]->m_cMaxFrame = 2;
			m_pEffectList[i]->m_dwFrameTime = 10;
			break;

		case 164: // worm-bite
			lPan = -(((m_sViewPointX / 32) + 10) - dX) * 1000;
			PlaySound('E', 4, sDist, lPan);
			bAddNewEffect(14, dX * 32 + (rand() % 120) - 60, dY * 32 + (rand() % 80) - 40, NULL, NULL, 0, 0);
			bAddNewEffect(14, dX * 32 + (rand() % 120) - 60, dY * 32 + (rand() % 80) - 40, NULL, NULL, 0, 0);
			bAddNewEffect(14, dX * 32 + (rand() % 120) - 60, dY * 32 + (rand() % 80) - 40, NULL, NULL, 0, 0);
			bAddNewEffect(14, dX * 32 + (rand() % 120) - 60, dY * 32 + (rand() % 80) - 40, NULL, NULL, 0, 0);
			bAddNewEffect(14, dX * 32 + (rand() % 120) - 60, dY * 32 + (rand() % 80) - 40, NULL, NULL, 0, 0);
			bAddNewEffect(14, dX * 32 + (rand() % 120) - 60, dY * 32 + (rand() % 80) - 40, NULL, NULL, 0, 0);
			bAddNewEffect(14, dX * 32 + (rand() % 120) - 60, dY * 32 + (rand() % 80) - 40, NULL, NULL, 0, 0);

			bAddNewEffect(14, dX * 32 + (rand() % 120) - 60, dY * 32 + (rand() % 80) - 40, NULL, NULL, 0, 0);
			bAddNewEffect(14, dX * 32 + (rand() % 120) - 60, dY * 32 + (rand() % 80) - 40, NULL, NULL, 0, 0);
			bAddNewEffect(14, dX * 32 + (rand() % 120) - 60, dY * 32 + (rand() % 80) - 40, NULL, NULL, 0, 0);
			bAddNewEffect(14, dX * 32 + (rand() % 120) - 60, dY * 32 + (rand() % 80) - 40, NULL, NULL, 0, 0);
			bAddNewEffect(14, dX * 32 + (rand() % 120) - 60, dY * 32 + (rand() % 80) - 40, NULL, NULL, 0, 0);
			bAddNewEffect(14, dX * 32 + (rand() % 120) - 60, dY * 32 + (rand() % 80) - 40, NULL, NULL, 0, 0);
			bAddNewEffect(14, dX * 32 + (rand() % 120) - 60, dY * 32 + (rand() % 80) - 40, NULL, NULL, 0, 0);
			m_pEffectList[i]->m_cMaxFrame = 1;
			m_pEffectList[i]->m_dwFrameTime = 10;
			break;

		case 165: // Absolute-Magic-Protection
			m_pEffectList[i]->m_cMaxFrame = 21;
			m_pEffectList[i]->m_dwFrameTime = 70;
			lPan = -(((m_sViewPointX / 32) + 10) - dX) * 1000;
			PlaySound('E', 5, sDist, lPan);
			break;

		case 166: // Armor Break
			m_pEffectList[i]->m_cMaxFrame = 13;
			m_pEffectList[i]->m_dwFrameTime = 80;
			lPan = -(((m_sViewPointX / 32) + 10) - dX) * 1000;
			PlaySound('E', 5, sDist, lPan);
			break;

		case 170: // Bloody-Shock-Wave
			m_pEffectList[i]->m_cMaxFrame = 7;
			m_pEffectList[i]->m_dwFrameTime = 80;
			break;

		case 172: // Mass-Ice-Strike
			m_pEffectList[i]->m_cMaxFrame = 2;
			m_pEffectList[i]->m_dwFrameTime = 10;
			break;

		case 174: // Lightning-Strike
			m_pEffectList[i]->m_cMaxFrame = 5;
			m_pEffectList[i]->m_dwFrameTime = 120;
			break;

		case 176: // Snoopy: Ajout Cancellation
			m_pEffectList[i]->m_cMaxFrame = 23;
			m_pEffectList[i]->m_dwFrameTime = 60;
			sDist = sDist / 32;
			lPan = -(((m_sViewPointX / 32) + 10) - sX) * 1000;
			PlaySound('E', 5, sDist, lPan);
			break;

		case 181: // MS
			m_pEffectList[i]->m_mX = dX * 32 + 300;
			m_pEffectList[i]->m_mY = dY * 32 - 460;
			m_pEffectList[i]->m_cMaxFrame = 10;
			m_pEffectList[i]->m_dwFrameTime = 25;
			break;

		case 182: // Snoopy: Ajout Mass-Magic-Missile
			m_pEffectList[i]->m_mX = sX * 32;
			m_pEffectList[i]->m_mY = sY * 32 - 40;
			m_pEffectList[i]->m_iErr = 0;
			m_pEffectList[i]->m_cMaxFrame = NULL;
			m_pEffectList[i]->m_dwFrameTime = 20;
			lPan = -(((m_sViewPointX / 32) + 10) - dX) * 1000;
			PlaySound('E', 1, sDist, lPan);
			break;

		case 244: // Snoopy: dÈplacÈ pour nvx sorts: Aura du casteur de Mass MagicMissile
			//case 184: // effet sur le caster pour MassMM
			m_pEffectList[i]->m_cMaxFrame = 29;
			m_pEffectList[i]->m_dwFrameTime = 80;
			m_pEffectList[i]->m_mX = sX;
			m_pEffectList[i]->m_mY = sY;
			break;

		case 191: // Blizzard
			m_pEffectList[i]->m_cMaxFrame = 7;
			m_pEffectList[i]->m_dwFrameTime = 80;
			break;

			//case 192: // Hero set Effect
		case 242: // Hero set Effect
			m_pEffectList[i]->m_cMaxFrame = 30;
			m_pEffectList[i]->m_dwFrameTime = 40;
			break;

			//case 193: // Hero set Effect
		case 243: // Hero set Effect
			m_pEffectList[i]->m_cMaxFrame = 19;
			m_pEffectList[i]->m_dwFrameTime = 18;
			break;

		case 194: // Resurrection
			m_pEffectList[i]->m_cMaxFrame = 30;
			m_pEffectList[i]->m_dwFrameTime = 40;
			break;

		case 196: // Snoopy: Ajout de Earth-Shock-Wave
			m_pEffectList[i]->m_mX = sX * 32;
			m_pEffectList[i]->m_mY = sY * 32;
			m_pEffectList[i]->m_iErr = 0;
			m_pEffectList[i]->m_cMaxFrame = 30;
			m_pEffectList[i]->m_dwFrameTime = 25;
			sAbsX = abs(320 - (sX - m_sViewPointX));
			sAbsY = abs(240 - (sY - m_sViewPointY));
			if (sAbsX > sAbsY) sDist = sAbsX;
			else sDist = sAbsY;
			sDist = sDist / 32;
			SetCameraShakingEffect(sDist);
			break;
		case 200: //
		case 201: //
		case 202: //
			m_pEffectList[i]->m_mX = sX;
			m_pEffectList[i]->m_mY = sY;
			m_pEffectList[i]->m_cMaxFrame = 15;
			m_pEffectList[i]->m_dwFrameTime = 25;
			break;

		case 203: //
			m_pEffectList[i]->m_mX = sX;
			m_pEffectList[i]->m_mY = sY;
			m_pEffectList[i]->m_cMaxFrame = 18;
			m_pEffectList[i]->m_dwFrameTime = 70;
			break;

		case 204: //
		case 205: //
			m_pEffectList[i]->m_mX = sX;
			m_pEffectList[i]->m_mY = sY;
			m_pEffectList[i]->m_cMaxFrame = 12;
			m_pEffectList[i]->m_dwFrameTime = 70;
			break;

		case 206: //
			m_pEffectList[i]->m_mX = sX;
			m_pEffectList[i]->m_mY = sY;
			m_pEffectList[i]->m_cMaxFrame = 3;
			m_pEffectList[i]->m_dwFrameTime = 70;
			break;

		case 250: //
			m_pEffectList[i]->m_mX = sX * 32;
			m_pEffectList[i]->m_mY = sY * 32 - 40;
			m_pEffectList[i]->m_iErr = 0;
			m_pEffectList[i]->m_cMaxFrame = 0;
			m_pEffectList[i]->m_dwFrameTime = 10;
			break;

		default:
			delete m_pEffectList[i];
			m_pEffectList[i] = NULL;
			break;
		}
		if (m_pEffectList[i] != NULL)
		{
			m_pEffectList[i]->m_mX2 = m_pEffectList[i]->m_mX;
			m_pEffectList[i]->m_mY2 = m_pEffectList[i]->m_mY;
		}
		return;
	}
}

void cMain::DrawEffects()
{
	int i, dX, dY, iDvalue, tX, tY, rX, rY, rX2, rY2, rX3, rY3, rX4, rY4, rX5, rY5, iErr;
	char  cTempFrame;
	DWORD dwTime = m_dwCurTime;



	for (i = 0; i < MAXEFFECTS; i++)
	if ((m_pEffectList[i] != NULL) && (m_pEffectList[i]->m_cFrame >= 0))
	{
		switch (m_pEffectList[i]->m_sType) {
		case 1: // Normal hit
			if (m_pEffectList[i]->m_cFrame < 0) break;
			dX = (m_pEffectList[i]->m_mX) - m_sViewPointX;
			dY = (m_pEffectList[i]->m_mY) - m_sViewPointY;
			m_pEffectSpr[8]->PutTransSprite_NoColorKey(dX, dY, m_pEffectList[i]->m_cFrame, dwTime);
			break;

		case 2: // Arrow flying
			dX = (m_pEffectList[i]->m_mX) - m_sViewPointX;
			dY = (m_pEffectList[i]->m_mY) - m_sViewPointY;
			cTempFrame = (m_pEffectList[i]->m_cDir - 1) * 2;
			if (cTempFrame < 0) break;
			m_pEffectSpr[7]->PutSpriteFast(dX, dY, cTempFrame, dwTime);
			break;

		case 4: // gold
			/* 1.5
			if (m_pEffectList[i]->m_cFrame < 9) break;
			cTempFrame = m_pEffectList[i]->m_cFrame - 9;
			dX  = (m_pEffectList[i]->m_mX)  - m_sViewPointX;
			dY  = (m_pEffectList[i]->m_mY)  - m_sViewPointY;
			m_pEffectSpr[1]->PutSpriteFast(dX, dY-40, cTempFrame, dwTime);
			*/
			break;

		case 5: // FireBall Fire Explosion
			cTempFrame = m_pEffectList[i]->m_cFrame;
			if (cTempFrame < 0) break;
			dX = (m_pEffectList[i]->m_mX) - m_sViewPointX;
			dY = (m_pEffectList[i]->m_mY) - m_sViewPointY;
			iDvalue = (cTempFrame - 8)*(-5);
			if (cTempFrame < 7)
				m_pEffectSpr[3]->PutTransSprite_NoColorKey(dX, dY, cTempFrame, dwTime);
			else m_pEffectSpr[3]->PutTransSpriteRGB(dX, dY, cTempFrame, iDvalue, iDvalue, iDvalue, dwTime);
			break;

		case 6:	 // Energy Bolt
		case 10: // Lightning Arrow
			cTempFrame = m_pEffectList[i]->m_cFrame;
			if (cTempFrame < 0) break;
			dX = (m_pEffectList[i]->m_mX) - m_sViewPointX;
			dY = (m_pEffectList[i]->m_mY) - m_sViewPointY;
			iDvalue = (cTempFrame - 7)*(-6);
			if (cTempFrame < 6)
				m_pEffectSpr[6]->PutTransSprite_NoColorKey(dX, dY, cTempFrame, dwTime);
			else m_pEffectSpr[6]->PutTransSpriteRGB(dX, dY, cTempFrame, iDvalue, iDvalue, iDvalue, dwTime); // RGB2
			break;

		case 7: // Magic Missile Explosion
			cTempFrame = m_pEffectList[i]->m_cFrame;
			dX = (m_pEffectList[i]->m_mX) - m_sViewPointX;
			dY = (m_pEffectList[i]->m_mY) - m_sViewPointY;
			iDvalue = (cTempFrame - 4)*(-3);
			if (cTempFrame < 4)
				m_pEffectSpr[6]->PutTransSprite_NoColorKey(dX, dY, cTempFrame, dwTime);
			else m_pEffectSpr[6]->PutTransSpriteRGB(dX, dY, cTempFrame, iDvalue, iDvalue, iDvalue, dwTime); // RGB2
			break;

		case 8: // Burst
			cTempFrame = m_pEffectList[i]->m_cFrame;
			cTempFrame = 4 - cTempFrame;
			if (cTempFrame < 0) break;
			dX = (m_pEffectList[i]->m_mX) - m_sViewPointX;
			dY = (m_pEffectList[i]->m_mY) - m_sViewPointY;
			m_pEffectSpr[11]->PutTransSprite_NoColorKey(dX, dY, cTempFrame, dwTime);
			break;

		case 9: // Burst
			cTempFrame = (rand() % 5);
			if (cTempFrame < 0) break;
			dX = (m_pEffectList[i]->m_mX) - m_sViewPointX;
			dY = (m_pEffectList[i]->m_mY) - m_sViewPointY;
			m_pEffectSpr[11]->PutTransSprite_NoColorKey(dX, dY, cTempFrame, dwTime);
			break;

		case 11: // pt grenat
			cTempFrame = (rand() % 5) + 5;
			if (cTempFrame < 0) break;
			dX = (m_pEffectList[i]->m_mX) - m_sViewPointX;
			dY = (m_pEffectList[i]->m_mY) - m_sViewPointY;
			m_pEffectSpr[11]->PutTransSprite2(dX, dY, cTempFrame, dwTime);
			break;

		case 12: // Burst
			cTempFrame = (rand() % 6) + 10;
			if (cTempFrame < 0) break;
			iDvalue = (m_pEffectList[i]->m_cFrame - 4)*(-3);
			dX = (m_pEffectList[i]->m_mX) - m_sViewPointX;
			dY = (m_pEffectList[i]->m_mY) - m_sViewPointY;
			if (cTempFrame < 4)
				m_pEffectSpr[11]->PutTransSprite_NoColorKey(dX, dY, cTempFrame, dwTime);
			else //m_pEffectSpr[11]->PutTransSpriteRGB(dX, dY, cTempFrame, iDvalue, iDvalue, iDvalue, dwTime); // RGB2
				//
				m_pEffectSpr[11]->PutTransSprite(dX, dY, cTempFrame, dwTime);
			break;

		case 13:
			cTempFrame = m_pEffectList[i]->m_cFrame;
			if (cTempFrame < 0) break;
			dX = (m_pEffectList[i]->m_mX) - m_sViewPointX;
			dY = (m_pEffectList[i]->m_mY) - m_sViewPointY;
			if (cTempFrame < 13)
			{
				m_pEffectSpr[11]->PutTransSprite_NoColorKey(dX, dY, 25 + (cTempFrame / 5), dwTime);
			}
			else
			{
				m_pEffectSpr[11]->PutTransSprite_NoColorKey(dX, dY, (8 + cTempFrame), dwTime);
			}
			break;

		case 14: // Traces de pas (terrain sec)
			if (m_pEffectList[i]->m_cFrame < 0) break;
			dX = m_pEffectList[i]->m_mX - m_sViewPointX;
			dY = m_pEffectList[i]->m_mY - m_sViewPointY;
			m_pEffectSpr[11]->PutTransSprite50_NoColorKey(dX, dY, (28 + m_pEffectList[i]->m_cFrame), dwTime);
			break;

		case 15: // petits nuages rouges
			cTempFrame = m_pEffectList[i]->m_cFrame;
			if (cTempFrame < 0) break;
			dX = m_pEffectList[i]->m_mX - m_sViewPointX;
			dY = m_pEffectList[i]->m_mY - m_sViewPointY;
			m_pEffectSpr[11]->PutTransSprite50_NoColorKey(dX, dY, (33 + cTempFrame), dwTime);
			break;

		case 16: //
			dX = (m_pEffectList[i]->m_mX) - m_sViewPointX;
			dY = (m_pEffectList[i]->m_mY) - m_sViewPointY;
			m_pEffectSpr[0]->PutTransSprite_NoColorKey(dX, dY, 0, dwTime);
			break;

		case 17: //test
			dX = (m_pEffectList[i]->m_mX) - m_sViewPointX;
			dY = (m_pEffectList[i]->m_mY) - m_sViewPointY;
			cTempFrame = 39 + (rand() % 3) * 3 + (rand() % 3);
			if (cTempFrame < 0) break;
			m_pEffectSpr[11]->PutTransSprite_NoColorKey(dX, dY, cTempFrame, dwTime);
			dX = (m_pEffectList[i]->m_mX2) - m_sViewPointX;
			dY = (m_pEffectList[i]->m_mY2) - m_sViewPointY;
			m_pEffectSpr[11]->PutTransSprite50_NoColorKey(dX, dY, cTempFrame, dwTime);
			break;

		case 18: //
			dX = (m_pEffectList[i]->m_mX) - m_sViewPointX;
			dY = (m_pEffectList[i]->m_mY) - m_sViewPointY;
			cTempFrame = m_pEffectList[i]->m_cFrame;
			if (cTempFrame < 0) break;
			m_pEffectSpr[18]->PutTransSprite70_NoColorKey(dX, dY, cTempFrame, dwTime);
			break;

		case 20: // critical hit
		case 21:
		case 22:
		case 23:
		case 24:
		case 25:
		case 26:
		case 27: // Critical strike with a weapon
			dX = (m_pEffectList[i]->m_mX) - m_sViewPointX;
			dY = (m_pEffectList[i]->m_mY) - m_sViewPointY;
			m_pEffectSpr[8]->PutTransSprite_NoColorKey(dX, dY, 1, dwTime);
			break;

		case 30: // Mass-Fire-Strike
			dX = (m_pEffectList[i]->m_mX) - m_sViewPointX;
			dY = (m_pEffectList[i]->m_mY) - m_sViewPointY;
			cTempFrame = m_pEffectList[i]->m_cFrame;
			if (cTempFrame < 0) break;
			m_pEffectSpr[14]->PutTransSprite_NoColorKey(dX, dY, cTempFrame, dwTime);
			break;

		case 31: // Mass-Fire-Strike
			dX = (m_pEffectList[i]->m_mX) - m_sViewPointX;
			dY = (m_pEffectList[i]->m_mY) - m_sViewPointY;
			cTempFrame = m_pEffectList[i]->m_cFrame;
			if (cTempFrame < 0) break;
			m_pEffectSpr[15]->PutTransSprite_NoColorKey(dX, dY, cTempFrame, dwTime);
			break;

		case 32: // Trace de pas  (raining weather)
			dX = (m_pEffectList[i]->m_mX) - m_sViewPointX;
			dY = (m_pEffectList[i]->m_mY) - m_sViewPointY;
			cTempFrame = m_pEffectList[i]->m_cFrame + 20;
			if (cTempFrame < 0) break;
			m_pEffectSpr[11]->PutTransSprite_NoColorKey(dX, dY, cTempFrame, dwTime);
			break;

		case 33: //
			dX = (m_pEffectList[i]->m_mX) - m_sViewPointX;
			dY = (m_pEffectList[i]->m_mY) - m_sViewPointY;
			cTempFrame = m_pEffectList[i]->m_cFrame;
			if (cTempFrame < 0) break;
			iDvalue = 0;
			m_pEffectSpr[19]->PutTransSpriteRGB(dX, dY, cTempFrame, iDvalue, iDvalue, iDvalue, dwTime); // RGB2
			break;

		case 34: // absent (220 et 351)
			break;

		case 35: // Snoopy: Ajout
			if (m_pEffectList[i]->m_cFrame < 0) break;
			dX = (m_pEffectList[i]->m_mX) - m_sViewPointX;
			dY = (m_pEffectList[i]->m_mY) - m_sViewPointY;
			cTempFrame = m_pEffectList[i]->m_cFrame;
			m_pEffectSpr[6]->PutTransSprite_NoColorKey(dX - 30, dY - 18, cTempFrame, dwTime);
			break;

		case 36: // Snoopy: Ajout
			if (m_pEffectList[i]->m_cFrame < 0) break;
			dX = (m_pEffectList[i]->m_mX) - m_sViewPointX;
			dY = (m_pEffectList[i]->m_mY) - m_sViewPointY;
			cTempFrame = m_pEffectList[i]->m_cFrame;
			m_pEffectSpr[97]->PutTransSprite_NoColorKey(dX, dY, cTempFrame, dwTime);
			break;

		case 40:
			cTempFrame = m_pEffectList[i]->m_cFrame;
			if (cTempFrame < 0) break;
			dX = (m_pEffectList[i]->m_mX) - m_sViewPointX;
			dY = (m_pEffectList[i]->m_mY) - m_sViewPointY;
			m_pEffectSpr[20]->PutTransSprite50_NoColorKey(dX, dY, cTempFrame, dwTime); // 20
			break;

		case 41: // Large Type 1, 2, 3, 4
		case 42:
		case 43:
		case 44:
		case 45: // Small Type 1, 2
		case 46:
			dX = (m_pEffectList[i]->m_sX) - m_sViewPointX;
			dY = (m_pEffectList[i]->m_sY) - m_sViewPointY;
			m_pEffectSpr[21]->PutFadeSprite(dX, dY, 48, dwTime);
			cTempFrame = m_pEffectList[i]->m_cFrame;
			if (cTempFrame < 0) break;
			dX = (m_pEffectList[i]->m_mX) - m_sViewPointX;
			dY = (m_pEffectList[i]->m_mY) - m_sViewPointY;
			if ((8 * (m_pEffectList[i]->m_sType - 41) + cTempFrame) < (8 * (m_pEffectList[i]->m_sType - 41) + 7))
			{
				iDvalue = -8 * (6 - cTempFrame);
				m_pEffectSpr[21]->PutTransSpriteRGB(dX, dY, 8 * (m_pEffectList[i]->m_sType - 41) + cTempFrame, iDvalue, iDvalue, iDvalue, dwTime); // RGB2
			}
			else
			{
				if ((cTempFrame - 5) >= 8) cTempFrame = ((cTempFrame - 5) - 8) + 5;
				m_pEffectSpr[21]->PutSpriteFast(dX, dY, 8 * (m_pEffectList[i]->m_sType - 41) + (cTempFrame - 5), dwTime);
			}
			break;

		case 47:
		case 48:
		case 49: // Blizzard
			dX = (m_pEffectList[i]->m_sX) - m_sViewPointX;
			dY = (m_pEffectList[i]->m_sY) - m_sViewPointY;
			m_pEffectSpr[m_pEffectList[i]->m_sType - 1]->PutRevTransSprite(dX, dY, 0, dwTime);
			cTempFrame = m_pEffectList[i]->m_cFrame;
			if (cTempFrame < 0) break;
			dX = (m_pEffectList[i]->m_mX) - m_sViewPointX;
			dY = (m_pEffectList[i]->m_mY) - m_sViewPointY;
			//PutString(dX, dY, "*", RGB(255,255,255));
			if (cTempFrame < 7) {
				iDvalue = -8 * (6 - cTempFrame);
				m_pEffectSpr[m_pEffectList[i]->m_sType - 1]->PutTransSpriteRGB(dX, dY, cTempFrame + 1, iDvalue, iDvalue, iDvalue, dwTime);
			}
			else {
				if (cTempFrame >= 8) cTempFrame = cTempFrame % 8;
				m_pEffectSpr[m_pEffectList[i]->m_sType - 1]->PutSpriteFast(dX, dY, cTempFrame + 1, dwTime);
			}
			break;

		case 50:
			cTempFrame = m_pEffectList[i]->m_cFrame;
			if (cTempFrame < 0) break;
			dX = (m_pEffectList[i]->m_mX) - m_sViewPointX;
			dY = (m_pEffectList[i]->m_mY) - m_sViewPointY;

			if (cTempFrame <= 6) {
				iDvalue = 0;
				m_pEffectSpr[22]->PutTransSpriteRGB(dX, dY, cTempFrame, iDvalue, iDvalue, iDvalue, dwTime);	// RGB2
			}
			else {
				iDvalue = -5 * (cTempFrame - 6);
				m_pEffectSpr[22]->PutTransSpriteRGB(dX, dY, 6, iDvalue, iDvalue, iDvalue, dwTime); // RGB2
			}
			break;

		case 51: //
			cTempFrame = m_pEffectList[i]->m_cFrame + 11; //15
			if (cTempFrame < 0) break;
			dX = (m_pEffectList[i]->m_mX) - m_sViewPointX;
			dY = (m_pEffectList[i]->m_mY) - m_sViewPointY;
			m_pEffectSpr[28]->PutTransSprite25(dX, dY, cTempFrame, dwTime); //20
			break;


		case 52: // Protection Ring commente par siementec, a voir
			/*			cTempFrame = m_pEffectList[i]->m_cFrame;
			dX  = (m_pEffectList[i]->m_mX)  - m_sViewPointX;
			dY  = (m_pEffectList[i]->m_mY)  - m_sViewPointY;
			m_pEffectSpr[24]->PutTransSprite(dX, dY, cTempFrame, dwTime);
			*/
			break;


		case 53: // Hold Twist
			cTempFrame = m_pEffectList[i]->m_cFrame;
			if (cTempFrame < 0) break;
			if (cTempFrame < 0) cTempFrame = 0;
			dX = (m_pEffectList[i]->m_mX) - m_sViewPointX;
			dY = (m_pEffectList[i]->m_mY) - m_sViewPointY;
			m_pEffectSpr[25]->PutTransSprite_NoColorKey(dX, dY, cTempFrame, dwTime); //25
			break;

		case 54: //  star twingkling (effect armes brillantes)
			cTempFrame = m_pEffectList[i]->m_cFrame;
			if (cTempFrame < 0) cTempFrame = 0;
			dX = (m_pEffectList[i]->m_mX) - m_sViewPointX;
			dY = (m_pEffectList[i]->m_mY) - m_sViewPointY;
			m_pEffectSpr[28]->PutTransSprite(dX, dY, cTempFrame, dwTime);
			break;

		case 55: //
			cTempFrame = m_pEffectList[i]->m_cFrame;
			if (cTempFrame < 0) cTempFrame = 0;
			dX = (m_pEffectList[i]->m_mX);
			dY = (m_pEffectList[i]->m_mY);
			m_pEffectSpr[28]->PutTransSprite_NoColorKey(dX, dY, cTempFrame, dwTime);
			break;

		case 56: // Mass-Chill-Wind
			cTempFrame = m_pEffectList[i]->m_cFrame;
			if (cTempFrame < 0) cTempFrame = 0;
			dX = (m_pEffectList[i]->m_mX) - m_sViewPointX;
			dY = (m_pEffectList[i]->m_mY) - m_sViewPointY;
			m_pEffectSpr[29]->PutTransSprite50_NoColorKey(dX, dY, cTempFrame, dwTime);
			break;

		case 57:  // absent (220 et 351)
			break;

		case 60:  //
		case 181: // MS
			cTempFrame = m_pEffectList[i]->m_cFrame;
			if (cTempFrame < 0) break;
			if (cTempFrame > 4)
			{
				cTempFrame = cTempFrame / 4;
			}
			if (cTempFrame >= 0)
			{
				dX = (m_pEffectList[i]->m_mX) - m_sViewPointX;
				dY = (m_pEffectList[i]->m_mY) - m_sViewPointY;
				m_pEffectSpr[31]->PutSpriteFast(dX, dY, 15 + cTempFrame, dwTime);
				m_pEffectSpr[31]->PutTransSprite(dX, dY, cTempFrame, dwTime);
			}
			break;

		case 61: // Fire aura on ground (crueffect1, 1)
			cTempFrame = m_pEffectList[i]->m_cFrame;
			if (cTempFrame < 0) break;
			dX = (m_pEffectList[i]->m_mX) - m_sViewPointX;
			dY = (m_pEffectList[i]->m_mY) - m_sViewPointY;
			m_pEffectSpr[32]->PutTransSprite_NoColorKey(dX, dY, cTempFrame, dwTime);
			break;

		case 62: // MS strike
			cTempFrame = m_pEffectList[i]->m_cFrame;
			if (cTempFrame < 0) break;
			if (cTempFrame > 0)
			{
				cTempFrame = cTempFrame - 1;
				dX = (m_pEffectList[i]->m_mX) - m_sViewPointX;
				dY = (m_pEffectList[i]->m_mY) - m_sViewPointY;
				m_pEffectSpr[31]->PutRevTransSprite(dX, dY, 20 + cTempFrame, dwTime, cTempFrame / 3);
			}
			break;

		case 63: // Fire explosion (crueffect1, 2)
			cTempFrame = m_pEffectList[i]->m_cFrame;
			if (cTempFrame < 0) break;
			dX = (m_pEffectList[i]->m_mX) - m_sViewPointX;
			dY = (m_pEffectList[i]->m_mY) - m_sViewPointY;
			m_pEffectSpr[33]->PutTransSprite_NoColorKey(dX, dY, cTempFrame, dwTime);
			break;

		case 64: // effet halo blancchatre
			cTempFrame = m_pEffectList[i]->m_cFrame;
			if (cTempFrame < 0) break;
			dX = (m_pEffectList[i]->m_mX) - m_sViewPointX;
			dY = (m_pEffectList[i]->m_mY) - m_sViewPointY;
			m_pEffectSpr[34]->PutTransSprite_NoColorKey(dX, dY, cTempFrame, dwTime);
			break;

		case 65: // MS from crusade striking
			cTempFrame = m_pEffectList[i]->m_cFrame;
			if (cTempFrame < 0) break;
			cTempFrame = cTempFrame / 6;
			dX = (m_pEffectList[i]->m_mX) - m_sViewPointX;
			dY = (m_pEffectList[i]->m_mY) - m_sViewPointY;
			m_pEffectSpr[31]->PutRevTransSprite(dX, dY, 20 + cTempFrame, dwTime, cTempFrame >> 2);
			break;

		case 66: // MS explodes on the ground
			cTempFrame = m_pEffectList[i]->m_cFrame;
			if (cTempFrame < 0) break;
			dX = (m_pEffectList[i]->m_mX) - m_sViewPointX;
			dY = (m_pEffectList[i]->m_mY) - m_sViewPointY;
			m_pEffectSpr[39]->PutRevTransSprite(dX, dY, cTempFrame, dwTime);
			m_pEffectSpr[39]->PutTransSprite_NoColorKey(dX, dY, cTempFrame, dwTime);
			break;

		case 67: // MS fire with smoke
			cTempFrame = m_pEffectList[i]->m_cFrame;
			if (cTempFrame < 0) break;
			dX = (m_pEffectList[i]->m_mX) - m_sViewPointX;
			dY = (m_pEffectList[i]->m_mY) - m_sViewPointY;
			switch (rand() % 3) {
			case 0: m_pEffectSpr[0]->PutTransSprite25_NoColorKey(dX, dY + 20, 1, dwTime); break;
			case 1: m_pEffectSpr[0]->PutTransSprite50_NoColorKey(dX, dY + 20, 1, dwTime); break;
			case 2: m_pEffectSpr[0]->PutTransSprite70_NoColorKey(dX, dY + 20, 1, dwTime); break;
			}
			m_pEffectSpr[35]->PutTransSprite70_NoColorKey(dX, dY, cTempFrame / 3, dwTime);
			break;

		case 68: // worm-bite
			cTempFrame = m_pEffectList[i]->m_cFrame;
			if (cTempFrame < 0) break;
			dX = (m_pEffectList[i]->m_mX) - m_sViewPointX;
			dY = (m_pEffectList[i]->m_mY) - m_sViewPointY;
			if (cTempFrame <= 11)
			{
				m_pEffectSpr[40]->PutSpriteFast(dX, dY, cTempFrame, dwTime);
				m_pEffectSpr[41]->PutTransSprite50_NoColorKey(dX, dY, cTempFrame, dwTime);
				m_pEffectSpr[44]->PutRevTransSprite(dX - 2, dY - 3, cTempFrame, dwTime);
				m_pEffectSpr[44]->PutTransSprite_NoColorKey(dX - 4, dY - 3, cTempFrame, dwTime);
			}
			else
			{
				switch (cTempFrame) {
				case 12:
				case 13:
				case 14: m_pEffectSpr[40]->PutSpriteFast(dX, dY, 11, dwTime); break;
				case 15: m_pEffectSpr[40]->PutTransSprite70_NoColorKey(dX, dY, 11, dwTime); break;
				case 16: m_pEffectSpr[40]->PutTransSprite50_NoColorKey(dX, dY, 11, dwTime); break;
				case 17: m_pEffectSpr[40]->PutTransSprite25_NoColorKey(dX, dY, 11, dwTime); break;
				}
			}
			break;

		case 69: // identique au cas 70
			cTempFrame = m_pEffectList[i]->m_cFrame;
			if (cTempFrame < 0) break;
			dX = (m_pEffectList[i]->m_mX) - m_sViewPointX;
			dY = (m_pEffectList[i]->m_mY) - m_sViewPointY;
			m_pEffectSpr[42]->PutTransSprite_NoColorKey(dX, dY, cTempFrame, dwTime);
			break;

		case 70: // identique au cas 69
			cTempFrame = m_pEffectList[i]->m_cFrame;
			if (cTempFrame < 0) break;
			dX = (m_pEffectList[i]->m_mX) - m_sViewPointX;
			dY = (m_pEffectList[i]->m_mY) - m_sViewPointY;
			m_pEffectSpr[43]->PutTransSprite_NoColorKey(dX, dY, cTempFrame, dwTime);
			break;

		case 71: // absent v220 et v351
			break;

		case 72: // Blizzard
			cTempFrame = m_pEffectList[i]->m_cFrame;
			if (cTempFrame < 0) break;
			dX = (m_pEffectList[i]->m_mX) - m_sViewPointX;
			dY = (m_pEffectList[i]->m_mY) - m_sViewPointY;
			if (cTempFrame <= 8)
			{
				iDvalue = 0;
				m_pEffectSpr[51]->PutTransSpriteRGB(dX, dY, cTempFrame, iDvalue, iDvalue, iDvalue, dwTime);
			}
			else
			{
				iDvalue = -1 * (cTempFrame - 8);
				m_pEffectSpr[51]->PutTransSpriteRGB(dX, dY, 8, iDvalue, iDvalue, iDvalue, dwTime);	// RGB2
			}
			break;

		case 73: // absent v220 et v351
		case 74: // absent v220 et v351
		case 75: // absent v220 et v351
		case 76: // absent v220 et v351
		case 77: // absent v220 et v351
			break;

		case 80:
		case 196: // Earth-Shock-Wave
			cTempFrame = m_pEffectList[i]->m_cFrame;
			if (cTempFrame < 0) break;
			dX = (m_pEffectList[i]->m_mX) - m_sViewPointX;
			dY = (m_pEffectList[i]->m_mY) - m_sViewPointY;
			m_pEffectSpr[91]->PutSpriteFast(dX, dY, cTempFrame, dwTime); //Nbe d'arguments modifiÈs ds la 351....
			m_pEffectSpr[92]->PutTransSprite(dX, dY, cTempFrame, dwTime);
			break;

		case 81: // Snoopy: Ajout StormBlade
			dX = (m_pEffectList[i]->m_mX) - m_sViewPointX;
			dY = (m_pEffectList[i]->m_mY) - m_sViewPointY;
			cTempFrame = m_pEffectList[i]->m_cFrame;
			m_pEffectSpr[100]->PutTransSprite_NoColorKey(dX + 70, dY + 70, cTempFrame, dwTime);
			break;

		case 82: // Gate (apocalypse)
			cTempFrame = m_pEffectList[i]->m_cFrame;
			m_pEffectSpr[101]->PutTransSprite_NoColorKey(320, 480, cTempFrame, dwTime);
			break;

		case 100: // Magic Missile
			dX = (m_pEffectList[i]->m_mX) - m_sViewPointX;
			dY = (m_pEffectList[i]->m_mY) - m_sViewPointY;
			m_pEffectSpr[0]->PutTransSprite_NoColorKey(dX, dY, 0, dwTime);
			break;

		case 101: // Heal
		case 121: // Great-Heal
			cTempFrame = m_pEffectList[i]->m_cFrame;
			if (cTempFrame < 0) break;
			dX = (m_pEffectList[i]->m_dX * 32) - m_sViewPointX;
			dY = (m_pEffectList[i]->m_dY * 32) - m_sViewPointY;
			iDvalue = (cTempFrame - 5)*(-5);
			m_pEffectSpr[50]->PutTransSprite_NoColorKey(dX, dY, m_pEffectList[i]->m_cFrame, dwTime);
			break;

		case 102: // Create Food
		case 124: // Protection from N.M
		case 125: // Hold-Person
		case 126: // Possession
		case 127: // Poison
		case 133: // Protect-From-Magic
		case 134: // Detect-Invisibility
		case 135: // Paralyze
		case 136: // Cure
		case 142: // Confuse Language
		case 152: // Polymorph
		case 153: // Mass-Poison
		case 162: // Confusion
		case 171: // Mass-Confusion
			cTempFrame = m_pEffectList[i]->m_cFrame;
			if (cTempFrame < 0) break;
			dX = (m_pEffectList[i]->m_dX * 32) - m_sViewPointX;
			dY = (m_pEffectList[i]->m_dY * 32) - m_sViewPointY;
			iDvalue = (cTempFrame - 5)*(-5);
			if (cTempFrame < 5)
				m_pEffectSpr[4]->PutTransSprite_NoColorKey(dX, dY, m_pEffectList[i]->m_cFrame, dwTime);
			else m_pEffectSpr[4]->PutTransSpriteRGB(dX, dY, m_pEffectList[i]->m_cFrame, iDvalue, iDvalue, iDvalue, dwTime); // RGB2
			break;

		case 110: // Energy-Bolt
			dX = (m_pEffectList[i]->m_mX) - m_sViewPointX;
			dY = (m_pEffectList[i]->m_mY) - m_sViewPointY;
			m_pEffectSpr[0]->PutTransSprite_NoColorKey(dX, dY, 2 + (rand() % 4), dwTime);
			break;

		case 111: // Staminar Drain
			cTempFrame = m_pEffectList[i]->m_cFrame;
			if (cTempFrame < 0) break;
			dX = (m_pEffectList[i]->m_dX * 32) - m_sViewPointX;
			dY = (m_pEffectList[i]->m_dY * 32) - m_sViewPointY;
			iDvalue = (cTempFrame - 5)*(-5);
			m_pEffectSpr[49]->PutTransSprite_NoColorKey(dX, dY, m_pEffectList[i]->m_cFrame, dwTime);
			break;

		case 112: // Recall
		case 131: // Summon-Creature
		case 132: // Invisibility
			cTempFrame = m_pEffectList[i]->m_cFrame;
			if (cTempFrame < 0) break;
			dX = (m_pEffectList[i]->m_dX * 32) - m_sViewPointX;
			dY = (m_pEffectList[i]->m_dY * 32) - m_sViewPointY;
			iDvalue = (cTempFrame - 5)*(-5);
			m_pEffectSpr[52]->PutTransSprite_NoColorKey(dX, dY, m_pEffectList[i]->m_cFrame, dwTime);
			break;

		case 113: // Defense Shield
			cTempFrame = m_pEffectList[i]->m_cFrame;
			if (cTempFrame < 0) break;
			dX = (m_pEffectList[i]->m_dX * 32) - m_sViewPointX;
			dY = (m_pEffectList[i]->m_dY * 32) - m_sViewPointY;
			iDvalue = (cTempFrame - 5)*(-5);
			if (cTempFrame < 6)
				m_pEffectSpr[62]->PutTransSprite_NoColorKey(dX, dY, m_pEffectList[i]->m_cFrame, dwTime);
			else m_pEffectSpr[62]->PutTransSpriteRGB(dX, dY, m_pEffectList[i]->m_cFrame, iDvalue, iDvalue, iDvalue, dwTime); // RGB2
			break;

		case 120: // Fire Ball
		case 130: // Fire Strike
		case 161: // Mass-Fire-Strike
		case 251: //
			dX = (m_pEffectList[i]->m_mX) - m_sViewPointX;
			dY = (m_pEffectList[i]->m_mY) - m_sViewPointY;
			cTempFrame = (m_pEffectList[i]->m_cDir - 1) * 4 + (rand() % 4);
			if (cTempFrame < 0) break;
			m_pEffectSpr[5]->PutTransSprite_NoColorKey(dX, dY, cTempFrame, dwTime);
			break;

		case 122: // Absent v220 et 351
			break;

		case 123: // Staminar-Recovery
		case 128: // Great-Staminar-Recovery
			cTempFrame = m_pEffectList[i]->m_cFrame;
			if (cTempFrame < 0) break;
			dX = (m_pEffectList[i]->m_dX * 32) - m_sViewPointX;
			dY = (m_pEffectList[i]->m_dY * 32) - m_sViewPointY;
			iDvalue = (cTempFrame - 5)*(-5);
			m_pEffectSpr[56]->PutTransSprite_NoColorKey(dX, dY, m_pEffectList[i]->m_cFrame, dwTime);
			break;

		case 137: // Lightning Arrow
			dX = (m_pEffectList[i]->m_mX) - m_sViewPointX;
			dY = (m_pEffectList[i]->m_mY) - m_sViewPointY;
			tX = (m_pEffectList[i]->m_mX2) - m_sViewPointX;
			tY = (m_pEffectList[i]->m_mY2) - m_sViewPointY;
			iErr = 0;
			m_Misc.GetPoint(dX, dY, tX, tY, &rX, &rY, &iErr, 15);
			m_Misc.GetPoint(dX, dY, tX, tY, &rX2, &rY2, &iErr, 30);
			m_Misc.GetPoint(dX, dY, tX, tY, &rX3, &rY3, &iErr, 45);
			m_Misc.GetPoint(dX, dY, tX, tY, &rX4, &rY4, &iErr, 60);
			m_Misc.GetPoint(dX, dY, tX, tY, &rX5, &rY5, &iErr, 75);
			cTempFrame = (m_pEffectList[i]->m_cDir - 1) * 4 + (rand() % 4);
			if (cTempFrame < 0) break;
			m_pEffectSpr[10]->PutTransSprite25_NoColorKey(rX5, rY5, cTempFrame, dwTime);
			cTempFrame = (m_pEffectList[i]->m_cDir - 1) * 4 + (rand() % 4);
			if (cTempFrame < 0) break;
			m_pEffectSpr[10]->PutTransSprite25_NoColorKey(rX4, rY4, cTempFrame, dwTime);
			cTempFrame = (m_pEffectList[i]->m_cDir - 1) * 4 + (rand() % 4);
			if (cTempFrame < 0) break;
			m_pEffectSpr[10]->PutTransSprite50_NoColorKey(rX3, rY3, cTempFrame, dwTime);
			cTempFrame = (m_pEffectList[i]->m_cDir - 1) * 4 + (rand() % 4);
			if (cTempFrame < 0) break;
			m_pEffectSpr[10]->PutTransSprite50_NoColorKey(rX2, rY2, cTempFrame, dwTime);
			cTempFrame = (m_pEffectList[i]->m_cDir - 1) * 4 + (rand() % 4);
			if (cTempFrame < 0) break;
			m_pEffectSpr[10]->PutTransSprite70_NoColorKey(rX, rY, cTempFrame, dwTime);
			cTempFrame = (m_pEffectList[i]->m_cDir - 1) * 4 + (rand() % 4);
			if (cTempFrame < 0) break;
			m_pEffectSpr[10]->PutTransSprite(dX, dY, cTempFrame, dwTime);
			break;

		case 143: // Lightning
			_DrawThunderEffect(m_pEffectList[i]->m_dX * 32 - m_sViewPointX, m_pEffectList[i]->m_dY * 32 - m_sViewPointY - 800,
				m_pEffectList[i]->m_dX * 32 - m_sViewPointX, m_pEffectList[i]->m_dY * 32 - m_sViewPointY,
				m_pEffectList[i]->m_rX, m_pEffectList[i]->m_rY, 1);
			_DrawThunderEffect(m_pEffectList[i]->m_dX * 32 - m_sViewPointX, m_pEffectList[i]->m_dY * 32 - m_sViewPointY - 800,
				m_pEffectList[i]->m_dX * 32 - m_sViewPointX, m_pEffectList[i]->m_dY * 32 - m_sViewPointY,
				m_pEffectList[i]->m_rX + 4, m_pEffectList[i]->m_rY + 2, 2);
			_DrawThunderEffect(m_pEffectList[i]->m_dX * 32 - m_sViewPointX, m_pEffectList[i]->m_dY * 32 - m_sViewPointY - 800,
				m_pEffectList[i]->m_dX * 32 - m_sViewPointX, m_pEffectList[i]->m_dY * 32 - m_sViewPointY,
				m_pEffectList[i]->m_rX - 2, m_pEffectList[i]->m_rY - 2, 2);
			break;

		case 144: // Great-Defense-Shield
			cTempFrame = m_pEffectList[i]->m_cFrame;
			if (cTempFrame < 0) break;
			dX = (m_pEffectList[i]->m_dX * 32) - m_sViewPointX;
			dY = (m_pEffectList[i]->m_dY * 32) - m_sViewPointY;
			iDvalue = (cTempFrame - 5)*(-5);
			if (cTempFrame < 9)
				m_pEffectSpr[63]->PutTransSprite_NoColorKey(dX, dY, m_pEffectList[i]->m_cFrame, dwTime);
			else m_pEffectSpr[63]->PutTransSpriteRGB(dX, dY, m_pEffectList[i]->m_cFrame, iDvalue, iDvalue, iDvalue, dwTime); // RGB2
			break;

		case 151: // Lightning Bolt
			_DrawThunderEffect(m_pEffectList[i]->m_mX - m_sViewPointX, m_pEffectList[i]->m_mY - m_sViewPointY,
				m_pEffectList[i]->m_dX * 32 - m_sViewPointX, m_pEffectList[i]->m_dY * 32 - m_sViewPointY,
				m_pEffectList[i]->m_rX, m_pEffectList[i]->m_rY, 1);

			_DrawThunderEffect(m_pEffectList[i]->m_mX - m_sViewPointX, m_pEffectList[i]->m_mY - m_sViewPointY,
				m_pEffectList[i]->m_dX * 32 - m_sViewPointX, m_pEffectList[i]->m_dY * 32 - m_sViewPointY,
				m_pEffectList[i]->m_rX + 2, m_pEffectList[i]->m_rY - 2, 2);

			_DrawThunderEffect(m_pEffectList[i]->m_mX - m_sViewPointX, m_pEffectList[i]->m_mY - m_sViewPointY,
				m_pEffectList[i]->m_dX * 32 - m_sViewPointX, m_pEffectList[i]->m_dY * 32 - m_sViewPointY,
				m_pEffectList[i]->m_rX - 2, m_pEffectList[i]->m_rY - 2, 2);
			break;

		case 165: // Absolute-Magic-Protect
			cTempFrame = m_pEffectList[i]->m_cFrame;
			if (cTempFrame < 0) break;
			dX = (m_pEffectList[i]->m_dX * 32) - m_sViewPointX;
			dY = (m_pEffectList[i]->m_dY * 32) - m_sViewPointY; // 53 = APFM buble
			m_pEffectSpr[53]->PutTransSprite_NoColorKey(dX, dY, m_pEffectList[i]->m_cFrame, dwTime);
			break;

		case 166: // Armor-Break
			cTempFrame = m_pEffectList[i]->m_cFrame;
			if (cTempFrame < 0) break;
			dX = (m_pEffectList[i]->m_dX * 32) - m_sViewPointX;
			dY = (m_pEffectList[i]->m_dY * 32) - m_sViewPointY;
			m_pEffectSpr[55]->PutRevTransSprite(dX, dY + 35, m_pEffectList[i]->m_cFrame, dwTime);
			m_pEffectSpr[54]->PutTransSprite50(dX, dY, m_pEffectList[i]->m_cFrame, dwTime);
			break;

		case 176: // Cancellation
			cTempFrame = m_pEffectList[i]->m_cFrame;
			if (cTempFrame < 0) break;
			dX = (m_pEffectList[i]->m_dX * 32) - m_sViewPointX;
			dY = (m_pEffectList[i]->m_dY * 32) - m_sViewPointY;
			m_pEffectSpr[90]->PutTransSprite_NoColorKey(dX + 50, dY + 85, cTempFrame, dwTime);
			break;

		case 177: // Illusion-Movement
		case 180: // Illusion
			cTempFrame = m_pEffectList[i]->m_cFrame;
			if (cTempFrame < 0) break;
			dX = (m_pEffectList[i]->m_dX * 32) - m_sViewPointX;
			dY = (m_pEffectList[i]->m_dY * 32) - m_sViewPointY;
			iDvalue = (cTempFrame - 5)*(-3);
			if (cTempFrame < 9)	m_pEffectSpr[60]->PutTransSprite_NoColorKey(dX, dY, m_pEffectList[i]->m_cFrame, dwTime);
			else m_pEffectSpr[60]->PutTransSpriteRGB(dX, dY, m_pEffectList[i]->m_cFrame, iDvalue, iDvalue, iDvalue, dwTime); // RGB2
			break;

		case 182: //Mass-Magic-Missile
			cTempFrame = m_pEffectList[i]->m_cFrame;
			dX = (m_pEffectList[i]->m_mX) - m_sViewPointX;
			dY = (m_pEffectList[i]->m_mY) - m_sViewPointY;
			m_pEffectSpr[98]->PutTransSprite(dX, dY, cTempFrame, dwTime, 0);
			break;

		case 183: // Inhibition-Casting
			cTempFrame = m_pEffectList[i]->m_cFrame;
			if (cTempFrame < 0) break;
			dX = (m_pEffectList[i]->m_dX * 32) - m_sViewPointX;
			dY = (m_pEffectList[i]->m_dY * 32) - m_sViewPointY;
			iDvalue = (cTempFrame - 5)*(-3);
			if (cTempFrame < 9) m_pEffectSpr[94]->PutTransSprite_NoColorKey(dX, dY + 40, m_pEffectList[i]->m_cFrame, dwTime);
			else m_pEffectSpr[94]->PutTransSpriteRGB(dX, dY + 40, m_pEffectList[i]->m_cFrame, iDvalue, iDvalue, iDvalue, dwTime);
			break;

		case 244: // Snoopy: dÈplacÈ pour nvx sorts: Aura du casteur de Mass MagicMissile
			//case 184: // Aura du casteur de Mass MagicMissile
			cTempFrame = m_pEffectList[i]->m_cFrame;
			if (cTempFrame < 0) break;
			dX = m_pEffectList[i]->m_mX - m_sViewPointX;
			dY = m_pEffectList[i]->m_mY - m_sViewPointY;
			m_pEffectSpr[96]->PutTransSprite(dX, dY, m_pEffectList[i]->m_cFrame, dwTime, 0);
			break;

		case 190: // Mass-Illusion
		case 195: // Mass-Illusion-Movement
			cTempFrame = m_pEffectList[i]->m_cFrame;
			if (cTempFrame < 0) break;
			dX = (m_pEffectList[i]->m_dX * 32) - m_sViewPointX;
			dY = (m_pEffectList[i]->m_dY * 32) - m_sViewPointY;
			iDvalue = (cTempFrame - 5)*(-3);
			if (cTempFrame < 9) m_pEffectSpr[61]->PutTransSprite_NoColorKey(dX, dY, m_pEffectList[i]->m_cFrame, dwTime);
			else m_pEffectSpr[61]->PutTransSpriteRGB(dX, dY, m_pEffectList[i]->m_cFrame, iDvalue, iDvalue, iDvalue, dwTime); // RGB2
			break;

			//case 192: // Mage Hero set effect
		case 242:
			dX = (m_pEffectList[i]->m_dX * 32) - m_sViewPointX;
			dY = (m_pEffectList[i]->m_dY * 32) - m_sViewPointY;
			m_pEffectSpr[87]->PutTransSprite_NoColorKey(dX + 50, dY + 57, m_pEffectList[i]->m_cFrame, dwTime);
			break;

			//case 193: // War Hero set effect
		case 243:
			dX = (m_pEffectList[i]->m_dX * 32) - m_sViewPointX;
			dY = (m_pEffectList[i]->m_dY * 32) - m_sViewPointY;
			m_pEffectSpr[88]->PutTransSprite_NoColorKey(dX + 65, dY + 80, m_pEffectList[i]->m_cFrame, dwTime);
			break;

		case 194: // Resurrection
			dX = (m_pEffectList[i]->m_dX * 32) - m_sViewPointX;
			dY = (m_pEffectList[i]->m_dY * 32) - m_sViewPointY;
			m_pEffectSpr[99]->PutTransSprite(dX, dY, m_pEffectList[i]->m_cFrame, dwTime, 0);
			break;

		case 200: // shotstar fall on ground
			dX = m_pEffectList[i]->m_mX;
			dY = m_pEffectList[i]->m_mY;
			m_pEffectSpr[133]->PutTransSprite_NoColorKey(dX, dY, (rand() % 15), dwTime);
			break;

		case 201: // shotstar fall on ground
			dX = m_pEffectList[i]->m_mX;
			dY = m_pEffectList[i]->m_mY;
			m_pEffectSpr[134]->PutTransSprite_NoColorKey(dX, dY, (rand() % 15), dwTime);
			break;

		case 202: // shotstar fall on ground
			dX = m_pEffectList[i]->m_mX;
			dY = m_pEffectList[i]->m_mY;
			m_pEffectSpr[135]->PutTransSprite_NoColorKey(dX, dY, (rand() % 15), dwTime);
			break;

		case 203: // explosion feu apoc
			dX = m_pEffectList[i]->m_mX;
			dY = m_pEffectList[i]->m_mY;
			m_pEffectSpr[136]->PutTransSprite_NoColorKey(dX, dY, (rand() % 18), dwTime);
			break;

		case 204: // Faille oblique
			dX = m_pEffectList[i]->m_mX;
			dY = m_pEffectList[i]->m_mY;
			m_pEffectSpr[137]->PutTransSprite_NoColorKey(dX, dY, (rand() % 12), dwTime);
			break;

		case 205: // Faille horizontale
			dX = m_pEffectList[i]->m_mX;
			dY = m_pEffectList[i]->m_mY;
			m_pEffectSpr[138]->PutTransSprite_NoColorKey(dX, dY, (rand() % 12), dwTime);
			break;

		case 206: // steams
			dX = m_pEffectList[i]->m_mX;
			dY = m_pEffectList[i]->m_mY;
			m_pEffectSpr[139]->PutTransSprite_NoColorKey(dX, dY, (rand() % 20), dwTime);
			break;

		case 250: // Gate (round one)
			dX = m_pEffectList[i]->m_mX - m_sViewPointX;
			dY = m_pEffectList[i]->m_mY - m_sViewPointY;
			m_pEffectSpr[103]->PutTransSprite_NoColorKey(dX, dY, (rand() % 3), dwTime);
			break;

		case 252: // burst (lisgt salmon color)
			dX = m_pEffectList[i]->m_mX - m_sViewPointX;
			dY = m_pEffectList[i]->m_mY - m_sViewPointY;
			m_pEffectSpr[104]->PutTransSprite_NoColorKey(dX, dY, (rand() % 3), dwTime);
			break;
		}
	}
}
void cMain::_DrawThunderEffect(int sX, int sY, int dX, int dY, int rX, int rY, char cType)
{
	int j, iErr, pX1, pY1, iX1, iY1, tX, tY;
	char cDir;
	DWORD dwTime;
	WORD  wR1, wG1, wB1, wR2, wG2, wB2, wR3, wG3, wB3, wR4, wG4, wB4;
	dwTime = m_dwCurTime;
	sX = pX1 = iX1 = tX = sX;
	sY = pY1 = iY1 = tY = sY;
	m_Misc.ColorTransfer(m_DDraw.m_cPixelFormat, RGB(50, 50, 100), &wR1, &wG1, &wB1);
	m_Misc.ColorTransfer(m_DDraw.m_cPixelFormat, RGB(30, 30, 100), &wR2, &wG2, &wB2);
	m_Misc.ColorTransfer(m_DDraw.m_cPixelFormat, RGB(0, 0, 30), &wR3, &wG3, &wB3);
	m_Misc.ColorTransfer(m_DDraw.m_cPixelFormat, RGB(50, 50, 200), &wR4, &wG4, &wB4);

	for (j = 0; j < 100; j++)
	{
		switch (cType) {
		case 1:
			DrawLine(pX1, pY1, iX1, iY1, 15, 15, 20);
			DrawLine(pX1 - 1, pY1, iX1 - 1, iY1, wR1, wG1, wB1);
			DrawLine(pX1 + 1, pY1, iX1 + 1, iY1, wR1, wG1, wB1);
			DrawLine(pX1, pY1 - 1, iX1, iY1 - 1, wR1, wG1, wB1);
			DrawLine(pX1, pY1 + 1, iX1, iY1 + 1, wR1, wG1, wB1);

			DrawLine(pX1 - 2, pY1, iX1 - 2, iY1, wR2, wG2, wB2);
			DrawLine(pX1 + 2, pY1, iX1 + 2, iY1, wR2, wG2, wB2);
			DrawLine(pX1, pY1 - 2, iX1, iY1 - 2, wR2, wG2, wB2);
			DrawLine(pX1, pY1 + 2, iX1, iY1 + 2, wR2, wG2, wB2);

			DrawLine(pX1 - 1, pY1 - 1, iX1 - 1, iY1 - 1, wR3, wG3, wB3);
			DrawLine(pX1 + 1, pY1 - 1, iX1 + 1, iY1 - 1, wR3, wG3, wB3);
			DrawLine(pX1 + 1, pY1 - 1, iX1 + 1, iY1 - 1, wR3, wG3, wB3);
			DrawLine(pX1 - 1, pY1 + 1, iX1 - 1, iY1 + 1, wR3, wG3, wB3);
			break;

		case 2:
			DrawLine2(pX1, pY1, iX1, iY1, wR4, wG4, wB4);
			break;
		}
		iErr = 0;
		m_Misc.GetPoint(sX, sY, dX, dY, &tX, &tY, &iErr, j * 10);
		pX1 = iX1;
		pY1 = iY1;
		cDir = m_Misc.cGetNextMoveDir(iX1, iY1, tX, tY);
		switch (cDir) {
		case 1:	rY -= 5; break;
		case 2: rY -= 5; rX += 5; break;
		case 3:	rX += 5; break;
		case 4: rX += 5; rY += 5; break;
		case 5: rY += 5; break;
		case 6: rX -= 5; rY += 5; break;
		case 7: rX -= 5; break;
		case 8: rX -= 5; rY -= 5; break;
		}
		if (rX < -20) rX = -20;
		if (rX >  20) rX = 20;
		if (rY < -20) rY = -20;
		if (rY >  20) rY = 20;
		iX1 = iX1 + rX;
		iY1 = iY1 + rY;
		if ((abs(tX - dX) < 5) && (abs(tY - dY) < 5)) break;
	}
	switch (cType) {
	case 1:
		m_pEffectSpr[6]->PutTransSprite(iX1, iY1, (rand() % 2), dwTime);
		break;
	}
}

void cMain::DrawNewDialogBox(char cType, int sX, int sY, int iFrame, BOOL bIsNoColorKey, BOOL bIsTrans)
{
	DWORD dwTime = G_dwGlobalTime;

	if (m_pSprite[cType] == NULL) return;
	if (bIsNoColorKey == FALSE)
	{
		if (bIsTrans == TRUE)
			m_pSprite[cType]->PutTransSprite2(sX, sY, iFrame, dwTime);
		else m_pSprite[cType]->PutSpriteFast(sX, sY, iFrame, dwTime);
	}
	else m_pSprite[cType]->PutSpriteFastNoColorKey(sX, sY, iFrame, dwTime);
}
void cMain::PlaySound(char cType, int iNum, int iDist, long lPan)
{
	int iVol;

	if (m_bSoundFlag == FALSE) return;
	if (m_bSoundStat == FALSE) return;

	if (iDist > 10) iDist = 10;

	iVol = (m_cSoundVolume - 100) * 20;
	iVol += -200 * iDist;

	if (iVol > 0) iVol = 0;
	if (iVol < -10000) iVol = -10000;

	if (iVol > -2000) {

		switch (cType) {
		case 'C':
			if (m_pCSound[iNum] == NULL) return;
			m_pCSound[iNum]->Play(FALSE, lPan, iVol);
			break;

		case 'M':
			if (m_pMSound[iNum] == NULL) return;
			m_pMSound[iNum]->Play(FALSE, lPan, iVol);
			break;

		case 'E':
			if (m_pESound[iNum] == NULL) return;
			m_pESound[iNum]->Play(FALSE, lPan, iVol);
			break;
		}
	}
}
void cMain::SetCameraShakingEffect(short sDist, int iMul)
{
	int iDegree;

	iDegree = 5 - sDist;
	if (iDegree <= 0) iDegree = 0;
	iDegree *= 2;

	if (iMul != 0) iDegree *= iMul;

	if (iDegree <= 2) return;

	m_iCameraShakingDegree = iDegree;
}
void cMain::PutString(int iX, int iY, char * pString, COLORREF color)
{
	m_DDraw._GetBackBufferDC();
	m_DDraw.TextOut(iX, iY, pString, color);
	m_DDraw._ReleaseBackBufferDC();
}
void cMain::PutString(int iX, int iY, char * pString, COLORREF color, BOOL bHide, char cBGtype, BOOL bIsPreDC)
{
	char * pTmp;
	int i;
	if (strlen(pString) == 0) return;
	if (bIsPreDC == FALSE) m_DDraw._GetBackBufferDC();
	if (bHide == FALSE)
	{
		switch (cBGtype) {
		case 0:
			m_DDraw.TextOut(iX + 1, iY, pString, color);
			break;
		case 1:
			m_DDraw.TextOut(iX, iY + 1, pString, RGB(5, 5, 5));
			m_DDraw.TextOut(iX + 1, iY + 1, pString, RGB(5, 5, 5));
			m_DDraw.TextOut(iX + 1, iY, pString, RGB(5, 5, 5));
			break;
		}
		m_DDraw.TextOut(iX, iY, pString, color);
	}
	else
	{
		pTmp = new char[strlen(pString) + 2];
		ZeroMemory(pTmp, strlen(pString) + 2);
		strcpy(pTmp, pString);
		for (i = 0; i < (int)strlen(pString); i++)
		if (pTmp[i] != NULL) pTmp[i] = '*';

		switch (cBGtype) {
		case 0:
			m_DDraw.TextOut(iX + 1, iY, pTmp, color);
			break;
		case 1:
			m_DDraw.TextOut(iX, iY + 1, pTmp, RGB(5, 5, 5));
			m_DDraw.TextOut(iX + 1, iY + 1, pTmp, RGB(5, 5, 5));
			m_DDraw.TextOut(iX + 1, iY, pTmp, RGB(5, 5, 5));
			break;
		}
		m_DDraw.TextOut(iX, iY, pTmp, color);
		delete[] pTmp;
	}
	if (bIsPreDC == FALSE) m_DDraw._ReleaseBackBufferDC();
}

void cMain::PutString2(int iX, int iY, char * pString, short sR, short sG, short sB)
{
	m_DDraw._GetBackBufferDC();
	m_DDraw.TextOut(iX + 1, iY, pString, RGB(0, 0, 0));
	m_DDraw.TextOut(iX, iY + 1, pString, RGB(0, 0, 0));
	m_DDraw.TextOut(iX + 1, iY + 1, pString, RGB(0, 0, 0));
	m_DDraw.TextOut(iX, iY, pString, RGB(sR, sG, sB));
	m_DDraw._ReleaseBackBufferDC();
}

void cMain::PutAlignedString(int iX1, int iX2, int iY, char * pString, short sR, short sG, short sB)
{
	RECT rt;
	m_DDraw._GetBackBufferDC();
	SetRect(&rt, iX1, iY, iX2, iY + 15);
	m_DDraw.DrawText(&rt, pString, RGB(sR, sG, sB));
	m_DDraw._ReleaseBackBufferDC();
}
void cMain::PutString_SprFont(int iX, int iY, char * pStr, short sR, short sG, short sB)
{
	int iXpos;
	DWORD iCnt;
	DWORD dwTime = G_dwGlobalTime;
	char  cTmpStr[100];

	ZeroMemory(cTmpStr, sizeof(cTmpStr));
	strcpy(cTmpStr, pStr);
	iXpos = iX;
	for (iCnt = 0; iCnt < strlen(cTmpStr); iCnt++) {
		if ((cTmpStr[iCnt] >= 33) && (cTmpStr[iCnt] <= 122)) {
			m_pSprite[SPRID_INTERFACE_FONT1]->PutSpriteRGB(iXpos + 1, iY, cTmpStr[iCnt] - 33, sR + 11, sG + 7, sB + 6, dwTime);
			if ((sR == 0) && (sG == 0) && (sB == 0))
				m_pSprite[SPRID_INTERFACE_FONT1]->PutSpriteFast(iXpos, iY, cTmpStr[iCnt] - 33, dwTime);
			else m_pSprite[SPRID_INTERFACE_FONT1]->PutSpriteRGB(iXpos, iY, cTmpStr[iCnt] - 33, sR, sG, sB, dwTime);
			iXpos += __cSpace[cTmpStr[iCnt] - 33];
		}
		else iXpos += 5;
	}
}

void cMain::PutString_SprFont2(int iX, int iY, char * pStr, short sR, short sG, short sB)
{
	int iXpos, iR, iG, iB;
	DWORD iCnt;
	DWORD dwTime = G_dwGlobalTime;
	char  cTmpStr[200];

	m_DDraw.ColorTransferRGB(RGB(sR, sG, sB), &iR, &iG, &iB);

	ZeroMemory(cTmpStr, sizeof(cTmpStr));
	strcpy(cTmpStr, pStr);

	iXpos = iX;
	for (iCnt = 0; iCnt < strlen(cTmpStr); iCnt++) {
		if ((cTmpStr[iCnt] >= 33) && (cTmpStr[iCnt] <= 122)) {
			m_pSprite[SPRID_INTERFACE_FONT1]->PutSpriteFast(iXpos + 1, iY, cTmpStr[iCnt] - 33, dwTime);
			m_pSprite[SPRID_INTERFACE_FONT1]->PutSpriteFast(iXpos + 1, iY + 1, cTmpStr[iCnt] - 33, dwTime);
			if ((sR == 0) && (sG == 0) && (sB == 0))
				m_pSprite[SPRID_INTERFACE_FONT1]->PutSpriteFast(iXpos, iY, cTmpStr[iCnt] - 33, dwTime);
			else m_pSprite[SPRID_INTERFACE_FONT1]->PutSpriteRGB(iXpos, iY, cTmpStr[iCnt] - 33, iR, iG, iB, dwTime);
			iXpos += __cSpace[cTmpStr[iCnt] - 33];
		}
		else iXpos += 5;
	}
}

void cMain::PutString_SprFont3(int iX, int iY, char * pStr, short sR, short sG, short sB, BOOL bTrans, int iType)
{
	int iXpos, iAdd;
	DWORD iCnt;
	DWORD dwTime = G_dwGlobalTime;
	char  cTmpStr[128];

	ZeroMemory(cTmpStr, sizeof(cTmpStr));
	strcpy(cTmpStr, pStr);

	if (iType != -1) {
		iAdd = 95 * iType;
		iXpos = iX;
		for (iCnt = 0; iCnt < strlen(cTmpStr); iCnt++) {
			if ((cTmpStr[iCnt] >= 32) && (cTmpStr[iCnt] <= 126)) {

				if (bTrans == FALSE) {
					m_pSprite[SPRID_INTERFACE_SPRFONTS2]->PutSpriteFast(iXpos, iY + 1, cTmpStr[iCnt] - 32 + iAdd, dwTime);
					m_pSprite[SPRID_INTERFACE_SPRFONTS2]->PutSpriteFast(iXpos + 1, iY + 1, cTmpStr[iCnt] - 32 + iAdd, dwTime);
					if ((sR == 0) && (sG == 0) && (sB == 0))
						m_pSprite[SPRID_INTERFACE_SPRFONTS2]->PutSpriteFast(iXpos, iY, cTmpStr[iCnt] - 32 + iAdd, dwTime);
					else m_pSprite[SPRID_INTERFACE_SPRFONTS2]->PutSpriteRGB(iXpos, iY, cTmpStr[iCnt] - 32 + iAdd, sR, sG, sB, dwTime);

				}
				else m_pSprite[SPRID_INTERFACE_SPRFONTS2]->PutTransSpriteRGB(iXpos, iY, cTmpStr[iCnt] - 32 + iAdd, sR, sG, sB, dwTime);

				iXpos += (m_pSprite[SPRID_INTERFACE_SPRFONTS2]->m_stBrush[cTmpStr[iCnt] - 32 + iAdd].szx);
			}
			else iXpos += 5;
		}
	}
	else {
		iAdd = 0;
		iXpos = iX;
		for (iCnt = 0; iCnt < strlen(cTmpStr); iCnt++) {
			if ((cTmpStr[iCnt] >= 32) && (cTmpStr[iCnt] <= 126)) {

				if (bTrans == FALSE) {
					m_pSprite[SPRID_INTERFACE_FONT2]->PutSpriteFast(iXpos, iY + 1, cTmpStr[iCnt] - 32 + iAdd, dwTime);
					m_pSprite[SPRID_INTERFACE_FONT2]->PutSpriteFast(iXpos + 1, iY + 1, cTmpStr[iCnt] - 32 + iAdd, dwTime);
					if ((sR == 0) && (sG == 0) && (sB == 0))
						m_pSprite[SPRID_INTERFACE_FONT2]->PutSpriteFast(iXpos, iY, cTmpStr[iCnt] - 32 + iAdd, dwTime);
					else m_pSprite[SPRID_INTERFACE_FONT2]->PutSpriteRGB(iXpos, iY, cTmpStr[iCnt] - 32 + iAdd, sR, sG, sB, dwTime);

				}
				else m_pSprite[SPRID_INTERFACE_FONT2]->PutTransSpriteRGB(iXpos, iY, cTmpStr[iCnt] - 32 + iAdd, sR, sG, sB, dwTime);

				iXpos += (m_pSprite[SPRID_INTERFACE_FONT2]->m_stBrush[cTmpStr[iCnt] - 32 + iAdd].szx);
			}
			else iXpos += 5;
		}
	}
}


void cMain::PutString_SprNum(int iX, int iY, char * pStr, short sR, short sG, short sB)
{
	int iXpos;
	unsigned char iCnt;
	DWORD dwTime = G_dwGlobalTime;
	char  cTmpStr[200];
	WORD  wR, wG, wB;
	ZeroMemory(cTmpStr, sizeof(cTmpStr));
	strcpy(cTmpStr, pStr);
	m_Misc.ColorTransfer(m_DDraw.m_cPixelFormat, RGB(sR, sG, sB), &wR, &wG, &wB);
	iXpos = iX;
	for (iCnt = 0; iCnt < strlen(cTmpStr); iCnt++)
	{
		if ((cTmpStr[iCnt] >= 0x30) && (cTmpStr[iCnt] <= 0x39))
		{
			m_pSprite[SPRID_INTERFACE_ADDINTERFACE]->PutTransSprite(iXpos + 2, iY, cTmpStr[iCnt] - 0x30 + 6, dwTime);
			m_pSprite[SPRID_INTERFACE_ADDINTERFACE]->PutTransSprite(iXpos + 1, iY + 1, cTmpStr[iCnt] - 0x30 + 6, dwTime);
			if ((sR == 0) && (sG == 0) && (sB == 0))
				m_pSprite[SPRID_INTERFACE_ADDINTERFACE]->PutTransSprite(iXpos, iY, cTmpStr[iCnt] - 0x30 + 6, dwTime);
			else m_pSprite[SPRID_INTERFACE_ADDINTERFACE]->PutTransSpriteRGB(iXpos, iY, cTmpStr[iCnt] - 0x30 + 6, wR, wG, wB, dwTime);
			iXpos += __cSpace2[cTmpStr[iCnt] - 0x30];
		}
	}
}
void cMain::DrawLine(int x0, int y0, int x1, int y1, int iR, int iG, int iB)
{
	int dx, dy, x_inc, y_inc, error, index, dstR, dstG, dstB;
	int iResultX, iResultY;
	WORD * pDst;

	if ((x0 == x1) && (y0 == y1)) return;
	error = 0;
	iResultX = x0;
	iResultY = y0;
	dx = x1 - x0;
	dy = y1 - y0;
	if (dx >= 0)
	{
		x_inc = 1;
	}
	else
	{
		x_inc = -1;
		dx = -dx;
	}
	if (dy >= 0)
	{
		y_inc = 1;
	}
	else
	{
		y_inc = -1;
		dy = -dy;
	}
	if (dx>dy)
	{
		for (index = 0; index <= dx; index++)
		{
			error += dy;
			if (error > dx)
			{
				error -= dx;
				iResultY += y_inc;
			}
			iResultX += x_inc;
			if ((iResultX >= 0) && (iResultX < 640) && (iResultY >= 0) && (iResultY < 480)) {
				pDst = (WORD *)m_DDraw.m_pBackB4Addr + iResultX + ((iResultY)*m_DDraw.m_sBackB4Pitch);
				switch (m_DDraw.m_cPixelFormat) {
				case 1:
					dstR = (int)m_DDraw.m_lTransRB100[(pDst[0] & 0xF800) >> 11][iR];
					dstG = (int)m_DDraw.m_lTransG100[(pDst[0] & 0x7E0) >> 5][iG];
					dstB = (int)m_DDraw.m_lTransRB100[(pDst[0] & 0x1F)][iB];
					*pDst = (WORD)((dstR << 11) | (dstG << 5) | dstB);
					break;

				case 2:
					dstR = (int)m_DDraw.m_lTransRB100[(pDst[0] & 0x7C00) >> 10][iR];
					dstG = (int)m_DDraw.m_lTransG100[(pDst[0] & 0x3E0) >> 5][iG];
					dstB = (int)m_DDraw.m_lTransRB100[(pDst[0] & 0x1F)][iB];
					*pDst = (WORD)((dstR << 10) | (dstG << 5) | dstB);
					break;
				}
			}
		}
	}
	else
	{
		for (index = 0; index <= dy; index++)
		{
			error += dx;
			if (error > dy)
			{
				error -= dy;
				iResultX += x_inc;
			}
			iResultY += y_inc;
			if ((iResultX >= 0) && (iResultX < 640) && (iResultY >= 0) && (iResultY < 480)) {
				pDst = (WORD *)m_DDraw.m_pBackB4Addr + iResultX + ((iResultY)*m_DDraw.m_sBackB4Pitch);
				switch (m_DDraw.m_cPixelFormat) {
				case 1:
					dstR = (int)m_DDraw.m_lTransRB100[(pDst[0] & 0xF800) >> 11][iR];
					dstG = (int)m_DDraw.m_lTransG100[(pDst[0] & 0x7E0) >> 5][iG];
					dstB = (int)m_DDraw.m_lTransRB100[(pDst[0] & 0x1F)][iB];
					*pDst = (WORD)((dstR << 11) | (dstG << 5) | dstB);
					break;

				case 2:
					dstR = (int)m_DDraw.m_lTransRB100[(pDst[0] & 0x7C00) >> 10][iR];
					dstG = (int)m_DDraw.m_lTransG100[(pDst[0] & 0x3E0) >> 5][iG];
					dstB = (int)m_DDraw.m_lTransRB100[(pDst[0] & 0x1F)][iB];
					*pDst = (WORD)((dstR << 10) | (dstG << 5) | dstB);
					break;
				}
			}
		}
	}
}


void cMain::DrawLine2(int x0, int y0, int x1, int y1, int iR, int iG, int iB)
{
	int dx, dy, x_inc, y_inc, error, index, dstR, dstG, dstB;
	int iResultX, iResultY;
	WORD * pDst;
	if ((x0 == x1) && (y0 == y1)) return;

	error = 0;
	iResultX = x0;
	iResultY = y0;
	dx = x1 - x0;
	dy = y1 - y0;
	if (dx >= 0)
	{
		x_inc = 1;
	}
	else
	{
		x_inc = -1;
		dx = -dx;
	}
	if (dy >= 0)
	{
		y_inc = 1;
	}
	else
	{
		y_inc = -1;
		dy = -dy;
	}
	if (dx>dy)
	{
		for (index = 0; index <= dx; index++)
		{
			error += dy;
			if (error > dx)
			{
				error -= dx;
				iResultY += y_inc;
			}
			iResultX += x_inc;
			if ((iResultX >= 0) && (iResultX < 640) && (iResultY >= 0) && (iResultY < 480)) {
				pDst = (WORD *)m_DDraw.m_pBackB4Addr + iResultX + ((iResultY)*m_DDraw.m_sBackB4Pitch);
				switch (m_DDraw.m_cPixelFormat) {
				case 1:
					dstR = (int)m_DDraw.m_lTransRB50[(pDst[0] & 0xF800) >> 11][iR];
					dstG = (int)m_DDraw.m_lTransG50[(pDst[0] & 0x7E0) >> 5][iG];
					dstB = (int)m_DDraw.m_lTransRB50[(pDst[0] & 0x1F)][iB];
					*pDst = (WORD)((dstR << 11) | (dstG << 5) | dstB);
					break;

				case 2:
					dstR = (int)m_DDraw.m_lTransRB50[(pDst[0] & 0x7C00) >> 10][iR];
					dstG = (int)m_DDraw.m_lTransG50[(pDst[0] & 0x3E0) >> 5][iG];
					dstB = (int)m_DDraw.m_lTransRB50[(pDst[0] & 0x1F)][iB];
					*pDst = (WORD)((dstR << 10) | (dstG << 5) | dstB);
					break;
				}
			}
		}
	}
	else
	{
		for (index = 0; index <= dy; index++)
		{
			error += dx;
			if (error > dy)
			{
				error -= dy;
				iResultX += x_inc;
			}
			iResultY += y_inc;
			if ((iResultX >= 0) && (iResultX < 640) && (iResultY >= 0) && (iResultY < 480)) {
				pDst = (WORD *)m_DDraw.m_pBackB4Addr + iResultX + ((iResultY)*m_DDraw.m_sBackB4Pitch);
				switch (m_DDraw.m_cPixelFormat) {
				case 1:
					dstR = (int)m_DDraw.m_lTransRB50[(pDst[0] & 0xF800) >> 11][iR];
					dstG = (int)m_DDraw.m_lTransG50[(pDst[0] & 0x7E0) >> 5][iG];
					dstB = (int)m_DDraw.m_lTransRB50[(pDst[0] & 0x1F)][iB];
					*pDst = (WORD)((dstR << 11) | (dstG << 5) | dstB);
					break;

				case 2:
					dstR = (int)m_DDraw.m_lTransRB50[(pDst[0] & 0x7C00) >> 10][iR];
					dstG = (int)m_DDraw.m_lTransG50[(pDst[0] & 0x3E0) >> 5][iG];
					dstB = (int)m_DDraw.m_lTransRB50[(pDst[0] & 0x1F)][iB];
					*pDst = (WORD)((dstR << 10) | (dstG << 5) | dstB);
					break;
				}
			}
		}
	}
}
void cMain::RequestFullObjectData(WORD wObjectID)
{
	char    cMsg[256];
	DWORD * dwp;
	WORD  * wp;

	ZeroMemory(cMsg, sizeof(cMsg));

	dwp = (DWORD *)(cMsg + 0);
	//*dwp = MSGID_REQUEST_FULLOBJECTDATA;
	wp = (WORD *)(cMsg + 4);
	*wp = wObjectID;

	//iRet = m_pGSock->iSendMsg((char *)cMsg, 6);
	/*
	switch (iRet) {
	case XSOCKEVENT_SOCKETCLOSED:
	case XSOCKEVENT_SOCKETERROR:
	case XSOCKEVENT_QUENEFULL:
	ChangeGameMode(GAMEMODE_ONCONNECTIONLOST);

	//delete m_pGSock;
	//m_pGSock = NULL;
	break;

	case XSOCKEVENT_CRITICALERROR:
	//delete m_pGSock;
	//m_pGSock = NULL;

	if (G_pCalcSocket != NULL) {
	delete G_pCalcSocket;
	G_pCalcSocket = NULL;
	}
	SendMessage(m_hWnd, WM_DESTROY, NULL, NULL);
	break;
	}
	*/
}
int cMain::bHasHeroSet(short Appr3, short Appr4, char OwnerType)
{
	char cArmor, cLeg, cBerk, cHat;
	cArmor = (Appr3 & 0xF000) >> 12;
	cLeg = (Appr3 & 0x0F00) >> 8;
	cHat = (Appr3 & 0x00F0) >> 4;
	cBerk = Appr3 & 0x000F;
	switch (OwnerType){
	case 1:
	case 2:
	case 3:
		if ((cArmor == 8) && (cLeg == 5) && (cHat == 9) && (cBerk == 3)) return (1); // Warr elv M
		if ((cArmor == 9) && (cLeg == 6) && (cHat == 10) && (cBerk == 4)) return (1); // Warr ares M
		if ((cArmor == 10) && (cLeg == 5) && (cHat == 11) && (cBerk == 3)) return (2); // Mage elv M
		if ((cArmor == 11) && (cLeg == 6) && (cHat == 12) && (cBerk == 4)) return (2); // Mage ares M
		break;
	case 4:
	case 5:
	case 6: // fixed
		if ((cArmor == 9) && (cLeg == 6) && (cHat == 9) && (cBerk == 4)) return (1); //warr elv W
		if ((cArmor == 10) && (cLeg == 7) && (cHat == 10) && (cBerk == 5)) return (1); //warr ares W
		if ((cArmor == 11) && (cLeg == 6) && (cHat == 11) && (cBerk == 4)) return (2); //mage elv W
		if ((cArmor == 12) && (cLeg == 7) && (cHat == 12) && (cBerk == 5)) return (2); //mage ares W
		break;
	}
	return 0;
}
void cMain::RestoreSprites()
{
	for (int i = 0; i < MAXSPRITES; i++)
	if (m_pSprite[i] != NULL) m_pSprite[i]->iRestore();
}
void cMain::ShowReceivedString(BOOL bIsHide)
{
	ZeroMemory(G_cTxt, sizeof(G_cTxt));

#ifdef USING_WIN_IME
	if (G_hEditWnd != NULL) GetWindowText(G_hEditWnd, m_pInputBuffer, (int)m_cInputMaxLen);
	strcpy(G_cTxt, m_pInputBuffer);
#else
	strcpy(G_cTxt, m_pInputBuffer);
	if ((m_cEdit[0] != 0) && (strlen(m_pInputBuffer) + strlen(m_cEdit) + 1 <= m_cInputMaxLen))
	{
		strcpy(G_cTxt + strlen(m_pInputBuffer), m_cEdit);
	}
#endif
	if (bIsHide == TRUE)
	{
		for (unsigned char i = 0; i < strlen(G_cTxt); i++)
		if (G_cTxt[i] != NULL) G_cTxt[i] = '*';
	}

	if ((G_dwGlobalTime % 400) < 210) G_cTxt[strlen(G_cTxt)] = '_';

	PutString(m_iInputX + 1, m_iInputY + 1, G_cTxt, RGB(0, 0, 0));
	PutString(m_iInputX, m_iInputY + 1, G_cTxt, RGB(0, 0, 0));
	PutString(m_iInputX + 1, m_iInputY, G_cTxt, RGB(0, 0, 0));
	PutString(m_iInputX, m_iInputY, G_cTxt, RGB(255, 255, 255));
}

void cMain::ClearInputString()
{
	if (m_pInputBuffer != NULL)	ZeroMemory(m_pInputBuffer, sizeof(m_pInputBuffer));
	ZeroMemory(m_cEdit, sizeof(m_cEdit));
#ifdef USING_WIN_IME
	if (G_hEditWnd != NULL)	SetWindowText(G_hEditWnd, "");
#endif
}
void cMain::StartInputString(int sX, int sY, unsigned char iLen, char * pBuffer, BOOL bIsHide)
{
	m_bInputStatus = TRUE;
	m_iInputX = sX;
	m_iInputY = sY;
	m_pInputBuffer = pBuffer;
	ZeroMemory(m_cEdit, sizeof(m_cEdit));
	m_cInputMaxLen = iLen;
#ifdef USING_WIN_IME
	if (bIsHide == FALSE) G_hEditWnd = CreateWindow( RICHEDIT_CLASS, NULL, WS_POPUP | ES_SELFIME, sX-5 , sY -1, iLen*12, 16, hWnd, (HMENU)0, G_hInstance, NULL);
	else G_hEditWnd = CreateWindow( RICHEDIT_CLASS, NULL, WS_POPUP | ES_PASSWORD | ES_SELFIME, sX-5 , sY -1, iLen*12, 16, hWnd, (HMENU)0, G_hInstance, NULL);
	SetWindowText(G_hEditWnd, m_pInputBuffer);
	SendMessage(G_hEditWnd, EM_EXLIMITTEXT, 0, iLen - 1);
	SendMessage(G_hEditWnd, EM_SETLANGOPTIONS, 0, ~IMF_AUTOFONT);
	COMPOSITIONFORM composform;
	composform.dwStyle = CFS_POINT;
	composform.ptCurrentPos.x = sX;
	composform.ptCurrentPos.y = sY;
	HIMC hImc = ImmGetContext(hWnd);
	ImmSetCompositionWindow(hImc, &composform);
	int StrLen = strlen(m_pInputBuffer);
	SendMessage(G_hEditWnd, EM_SETSEL, StrLen, StrLen);
#endif
}

void cMain::EndInputString()
{
	m_bInputStatus = FALSE;
#ifdef USING_WIN_IME
	if (G_hEditWnd != NULL)
	{
		GetWindowText(G_hEditWnd, m_pInputBuffer, (int)m_cInputMaxLen);
		CANDIDATEFORM candiform;
		SendMessage(G_hEditWnd, WM_IME_CONTROL, IMC_GETCANDIDATEPOS, (LPARAM)&candiform);
		DestroyWindow(G_hEditWnd);
		G_hEditWnd = NULL;
	}
#else
	int len = strlen(m_cEdit);
	if (len > 0)
	{
		m_cEdit[len] = 0;
		strcpy(m_pInputBuffer + strlen(m_pInputBuffer), m_cEdit);
		ZeroMemory(m_cEdit, sizeof(m_cEdit));
	}
#endif
}

void cMain::ReceiveString(char *pString)
{
	strcpy(pString, m_pInputBuffer);

#ifdef USING_WIN_IME
	if (G_hEditWnd != NULL) GetWindowText(G_hEditWnd, pString, (int)m_cInputMaxLen);
#endif
}
void cMain::UpdateScreen_OnMainMenu()
{
	short msX, msY, msZ;
	char cLB, cRB, cMIresult;
	int  iMIbuttonNum;
	static class CMouseInterface * pMI;
	DWORD dwTime = G_dwGlobalTime;

	//m_iItemDropCnt = 0;
	//m_bItemDrop = FALSE;

	if (m_cGameModeCount == 0)
	{
		//if (G_pCalcSocket != NULL)
		//{	delete G_pCalcSocket;
		//	G_pCalcSocket = NULL;
		//	}
		if (m_pSprite[SPRID_INTERFACE_ND_LOADING] != NULL)
		{
			delete m_pSprite[SPRID_INTERFACE_ND_LOADING];
			m_pSprite[SPRID_INTERFACE_ND_LOADING] = NULL;
		}
		EndInputString();
		pMI = new class CMouseInterface;

		// CLEROTH - INTERFACE
		pMI->AddRect(384, 177, 548, 198);
		pMI->AddRect(384, 215, 548, 236);
		pMI->AddRect(384, 254, 548, 275);
		m_DInput.m_sX = 400;
		m_DInput.m_sY = 240;

		m_cCurFocus = 1;
		m_cMaxFocus = 3;

		m_bEnterPressed = FALSE;
		m_cArrowPressed = 0;
	}
	m_cGameModeCount++;
	if (m_cGameModeCount > 100) m_cGameModeCount = 100;

	m_DDraw.ClearBackB4();

	DrawNewDialogBox(SPRID_INTERFACE_ND_MAINMENU, -1, -1, 0, TRUE);

	// CLEROTH
	m_DInput.UpdateMouseState(&msX, &msY, &msZ, &cLB, &cRB);

	if ((msX >= 384) && (msY >= 177) && (msX <= 548) && (msY <= 198)) m_cCurFocus = 1;
	if ((msX >= 384) && (msY >= 215) && (msX <= 548) && (msY <= 236)) m_cCurFocus = 2;
	if ((msX >= 384) && (msY >= 254) && (msX <= 548) && (msY <= 275)) m_cCurFocus = 3;

	switch (m_cCurFocus) {

	case 1:
		m_pSprite[SPRID_INTERFACE_ND_MAINMENU]->PutSpriteFast(384, 177, 1, dwTime);
		break;
	case 2:
		m_pSprite[SPRID_INTERFACE_ND_MAINMENU]->PutSpriteFast(384, 215, 2, dwTime);
		break;

	case 3:
		m_pSprite[SPRID_INTERFACE_ND_MAINMENU]->PutSpriteFast(384, 254, 3, dwTime);
		break;
	}

	m_pSprite[SPRID_MOUSECURSOR]->PutSpriteFast(msX, msY, 0, dwTime);
	//m_pSprite[SPRID_INTERFACE_ADDINTERFACE]->PutTransSpriteRGB(msX, msY, 1, 0, 0, 0, dwTime);

	if (m_cArrowPressed != 0) {
		switch (m_cArrowPressed) {
		case 1:
			m_cCurFocus--;
			if (m_cCurFocus <= 0) m_cCurFocus = m_cMaxFocus;
			break;
		case 3:
			m_cCurFocus++;
			if (m_cCurFocus > m_cMaxFocus) m_cCurFocus = 1;
			break;
		}
		m_cArrowPressed = 0;
	}

	if (m_bEnterPressed == TRUE) {
		// Enter
		PlaySound('E', 14, 5);
		m_bEnterPressed = FALSE;
		switch (m_cCurFocus) {
		case 1:
			delete pMI;
			ChangeGameMode(GAMEMODE_ONSELECTSERVER);
			return;
		case 2:
#ifdef MAKE_ACCOUNT
			//ClearContents_OnSelectCharacter();
			delete pMI;
			//ChangeGameMode(GAMEMODE_ONAGREEMENT);
			ChangeGameMode(GAMEMODE_ONCREATENEWACCOUNT);
#else
			GoHomepage();
#endif
			return;
		case 3:
			delete pMI;
			ChangeGameMode(GAMEMODE_ONQUIT);
			return;
		}
	}

	DrawVersion(TRUE);

	iMIbuttonNum = pMI->iGetStatus(msX, msY, cLB, &cMIresult);
	if (cMIresult == MIRESULT_CLICK) {
		PlaySound('E', 14, 5);
		m_cCurFocus = iMIbuttonNum;
		switch (iMIbuttonNum)
		{
		case 1:
			ChangeGameMode(GAMEMODE_ONSELECTSERVER);
			delete pMI;
			break;

		case 2:
#ifdef MAKE_ACCOUNT
			//ClearContents_OnSelectCharacter();
			delete pMI;
			//ChangeGameMode(GAMEMODE_ONAGREEMENT);
			ChangeGameMode(GAMEMODE_ONCREATENEWACCOUNT);
#else
			GoHomepage();
#endif
			return;
		case 3:
			delete pMI;
			ChangeGameMode(GAMEMODE_ONQUIT);
			return;
		}
	}
	//	if ((msX >= 112) && (msY >= 150) && (msX <= 247) && (msY <= 191)) m_cCurFocus = 1;
	//	if ((msX >= 73) && (msY >= 192) && (msX <= 281) && (msY <= 230)) m_cCurFocus = 2;
	//	if ((msX >= 120) && (msY >= 231) && (msX <= 239) && (msY <= 262)) m_cCurFocus = 3;

	//	if (m_cGameModeCount < 6) m_DDraw.DrawShadowBox(0,0,639,479);
	//	if (m_cGameModeCount < 2) m_DDraw.DrawShadowBox(0,0,639,479);

	if (m_DDraw.iFlip() == DDERR_SURFACELOST) RestoreSprites();
}
bool cMain::_bGetIsStringIsNumber(char * pStr)
{
	int i;
	for (i = 0; i < (int)strlen(pStr); i++)
	if ((pStr[i] != '-') && ((pStr[i] < (char)'0') || (pStr[i] > (char)'9'))) return FALSE;

	return TRUE;
}

bool cMain::bInitMagicCfgList()
{
	char cFn[255], cTemp[255];
	char * pContents, *token;
	char seps[] = "= ,\t\n";
	char cReadModeA = 0;
	char cReadModeB = 0;
	int  iMagicCfgListIndex = 0;
	class CStrTok * pStrTok;
	HANDLE hFile;
	FILE * pFile;
	DWORD  dwFileSize;

	ZeroMemory(cTemp, sizeof(cTemp));
	ZeroMemory(cFn, sizeof(cFn));

	// CLEROTH - MAGIC CFG
	strcpy(cTemp, "magiccfg.txt");

	strcat(cFn, "contents");
	strcat(cFn, "\\");
	strcat(cFn, "\\");
	strcat(cFn, cTemp);

	hFile = CreateFile(cFn, GENERIC_READ, NULL, NULL, OPEN_EXISTING, NULL, NULL);
	dwFileSize = GetFileSize(hFile, NULL);
	if (hFile != INVALID_HANDLE_VALUE) CloseHandle(hFile);

	pFile = fopen(cFn, "rt");
	if (pFile == NULL) return FALSE;
	else {
		pContents = new char[dwFileSize + 1];
		ZeroMemory(pContents, dwFileSize + 1);
		fread(pContents, dwFileSize, 1, pFile);
		fclose(pFile);
	}

	pStrTok = new class CStrTok(pContents, seps);
	token = pStrTok->pGet();
	while (token != NULL) {
		if (cReadModeA != 0) {
			switch (cReadModeA) {
			case 1:
				switch (cReadModeB) {
				case 1:
					if (_bGetIsStringIsNumber(token) == FALSE)
					{
						delete[] pContents;
						delete pStrTok;
						return FALSE;
					}
					if (m_pMagicCfgList[atoi(token)] != NULL)
					{
						delete[] pContents;
						delete pStrTok;
						return FALSE;
					}
					m_pMagicCfgList[atoi(token)] = new class CMagic;
					iMagicCfgListIndex = atoi(token);

					cReadModeB = 2;
					break;

				case 2:
					ZeroMemory(m_pMagicCfgList[iMagicCfgListIndex]->m_cName, sizeof(m_pMagicCfgList[iMagicCfgListIndex]->m_cName));
					memcpy(m_pMagicCfgList[iMagicCfgListIndex]->m_cName, token, strlen(token));
					cReadModeB = 3;
					break;

				case 3: // m_sValue1
					if (_bGetIsStringIsNumber(token) == FALSE) {
						delete[] pContents;
						delete pStrTok;
						return FALSE;
					}
					m_pMagicCfgList[iMagicCfgListIndex]->m_sValue1 = atoi(token);
					cReadModeB = 4;
					break;

				case 4: // m_sValue2	// INT
					if (_bGetIsStringIsNumber(token) == FALSE) {
						delete[] pContents;
						delete pStrTok;
						return FALSE;
					}
					m_pMagicCfgList[iMagicCfgListIndex]->m_sValue2 = atoi(token);
					cReadModeB = 5;
					break;

				case 5: // m_sValue3	// COST
					if (_bGetIsStringIsNumber(token) == FALSE) {
						delete[] pContents;
						delete pStrTok;
						return FALSE;
					}
					m_pMagicCfgList[iMagicCfgListIndex]->m_sValue3 = atoi(token);
					cReadModeB = 6;
					break;

					// CLEROTH MAGIC CFG
				case 6: // m_sValue4	// STR
					if (_bGetIsStringIsNumber(token) == FALSE)
					{
						delete[] pContents;
						delete pStrTok;
						return FALSE;
					}
					m_pMagicCfgList[iMagicCfgListIndex]->m_sValue4 = atoi(token);
					cReadModeB = 7;
					break;

				case 7: // m_sValue5
					cReadModeB = 8;
					break;

				case 8: // m_sValue6
					cReadModeB = 9;
					break;

				case 9: // m_bIsVisible
					if (_bGetIsStringIsNumber(token) == FALSE)
					{
						delete[] pContents;
						delete pStrTok;
						return FALSE;
					}
					if (atoi(token) == 0) m_pMagicCfgList[iMagicCfgListIndex]->m_bIsVisible = FALSE;
					else m_pMagicCfgList[iMagicCfgListIndex]->m_bIsVisible = TRUE;
					cReadModeA = 0;
					cReadModeB = 0;
					break;
				}
				break;

			default:
				break;
			}
		}
		else {
			if (memcmp(token, "magic", 5) == 0) {
				cReadModeA = 1;
				cReadModeB = 1;
			}
		}
		token = pStrTok->pGet();
	}

	delete pStrTok;
	delete[] pContents;

	if ((cReadModeA != 0) || (cReadModeB != 0)) {
		return FALSE;
	}

	return TRUE;
}
void cMain::_LoadGameMsgTextContents()
{
	char * pContents, *token, cTemp[120], cFileName[120];
	char   seps[] = ";\n";
	int    iIndex = 0, i;
	class  CStrTok * pStrTok;
	DWORD  dwFileSize;
	HANDLE hFile;
	FILE * pFile;

	for (i = 0; i < MAXGAMEMSGS; i++) {
		if (m_pGameMsgList[i] != NULL)
			delete m_pGameMsgList[i];
		m_pGameMsgList[i] = NULL;
	}

	ZeroMemory(cTemp, sizeof(cTemp));
	ZeroMemory(cFileName, sizeof(cFileName));

	strcpy(cTemp, "GameMsgList");

	strcat(cFileName, "contents");
	strcat(cFileName, "\\");
	strcat(cFileName, "\\");
	strcat(cFileName, cTemp);
	strcat(cFileName, ".txt");

	hFile = CreateFile(cFileName, GENERIC_READ, NULL, NULL, OPEN_EXISTING, NULL, NULL);
	dwFileSize = GetFileSize(hFile, NULL);
	if (hFile != INVALID_HANDLE_VALUE) CloseHandle(hFile);

	pFile = fopen(cFileName, "rt");
	if (pFile == NULL) return;
	else {
		pContents = new char[dwFileSize + 1];
		ZeroMemory(pContents, dwFileSize + 1);
		fread(pContents, dwFileSize, 1, pFile);
	}

	fclose(pFile);

	pStrTok = new class CStrTok(pContents, seps);
	token = pStrTok->pGet();
	while (token != NULL) {
		m_pGameMsgList[iIndex] = new class CMsg(NULL, token, NULL);
		token = pStrTok->pGet();
		iIndex++;
	}

	delete pStrTok;
	delete[] pContents;
}

bool cMain::bCheckImportantFile()
{
	HANDLE hFile;

#ifndef _DEBUG
	hFile = CreateFile("CONTENTS\\badword.txt", GENERIC_READ, NULL, NULL, OPEN_EXISTING, NULL, NULL);
	if (hFile == INVALID_HANDLE_VALUE) return FALSE;
	CloseHandle(hFile);
#endif

	hFile = CreateFile("SPRITES\\TREES1.PAK", GENERIC_READ, NULL, NULL, OPEN_EXISTING, NULL, NULL);
	if (hFile == INVALID_HANDLE_VALUE) return FALSE;

	//	// FileSize : 1846406.... Anti Tree1.pak hack....inutile ca peut se modifier sans changer la taille!
	//	if( GetFileSize( hFile, NULL ) != 2945524 )
	//	{	CloseHandle( hFile );
	//		return FALSE;
	//	}

	CloseHandle(hFile);
	return TRUE;
}

bool cMain::bInitSkillCfgList()
{
	char cFn[255], cTemp[255];
	char * pContents, *token;
	char seps[] = "= ,\t\n";
	char cReadModeA = 0;
	char cReadModeB = 0;
	int  iSkillCfgListIndex = 0;
	class CStrTok * pStrTok;
	HANDLE hFile;
	FILE * pFile;
	DWORD  dwFileSize;

	ZeroMemory(cTemp, sizeof(cTemp));
	ZeroMemory(cFn, sizeof(cFn));

	strcpy(cTemp, "Skillcfg.txt");
	strcat(cFn, "contents");
	strcat(cFn, "\\");
	strcat(cFn, "\\");
	strcat(cFn, cTemp);

	hFile = CreateFile(cFn, GENERIC_READ, NULL, NULL, OPEN_EXISTING, NULL, NULL);
	dwFileSize = GetFileSize(hFile, NULL);
	if (hFile != INVALID_HANDLE_VALUE) CloseHandle(hFile);

	pFile = fopen(cFn, "rt");
	if (pFile == NULL) return FALSE;
	else {
		pContents = new char[dwFileSize + 1];
		ZeroMemory(pContents, dwFileSize + 1);
		fread(pContents, dwFileSize, 1, pFile);
		fclose(pFile);
	}

	pStrTok = new class CStrTok(pContents, seps);
	token = pStrTok->pGet();
	while (token != NULL) {
		if (cReadModeA != 0) {
			switch (cReadModeA) {
			case 1:
				switch (cReadModeB) {
				case 1:
					if (_bGetIsStringIsNumber(token) == FALSE)
					{
						delete[] pContents;
						delete pStrTok;
						return FALSE;
					}
					if (m_pSkillCfgList[atoi(token)] != NULL)
					{
						delete[] pContents;
						delete pStrTok;
						return FALSE;
					}
					m_pSkillCfgList[atoi(token)] = new class CSkill;
					iSkillCfgListIndex = atoi(token);
					cReadModeB = 2;
					break;

				case 2:
					ZeroMemory(m_pSkillCfgList[iSkillCfgListIndex]->m_cName, sizeof(m_pSkillCfgList[iSkillCfgListIndex]->m_cName));
					memcpy(m_pSkillCfgList[iSkillCfgListIndex]->m_cName, token, strlen(token));
					cReadModeB = 3;
					break;

				case 3: // m_bIsUseable
					if (_bGetIsStringIsNumber(token) == FALSE) {
						delete[] pContents;
						delete pStrTok;
						return FALSE;
					}
					m_pSkillCfgList[iSkillCfgListIndex]->m_bIsUseable = (BOOL)atoi(token);
					cReadModeB = 4;
					break;

				case 4: // m_cUseMethod
					if (_bGetIsStringIsNumber(token) == FALSE) {
						delete[] pContents;
						delete pStrTok;
						return FALSE;
					}
					m_pSkillCfgList[iSkillCfgListIndex]->m_cUseMethod = atoi(token);
					cReadModeA = 0;
					cReadModeB = 0;
					break;
				}
				break;

			default:
				break;
			}
		}
		else {
			if (memcmp(token, "skill", 5) == 0) {
				cReadModeA = 1;
				cReadModeB = 1;
			}
		}
		token = pStrTok->pGet();
	}

	delete pStrTok;
	delete[] pContents;

	if ((cReadModeA != 0) || (cReadModeB != 0)) {
		return FALSE;
	}

	return TRUE;
}
bool cMain::_bDecodeBuildItemContents()
{
	char cFileName[255], cTemp[255];
	HANDLE hFile;
	FILE * pFile;
	DWORD  dwFileSize;
	char * pBuffer;
	BOOL   bRet;
	int    i;

	for (i = 0; i < MAXBUILDITEMS; i++)
	if (m_pBuildItemList[i] != NULL)
	{
		delete m_pBuildItemList[i];
		m_pBuildItemList[i] = NULL;
	}

	ZeroMemory(cTemp, sizeof(cTemp));
	ZeroMemory(cFileName, sizeof(cFileName));

	strcpy(cTemp, "BItemcfg");
	strcat(cFileName, "contents");
	strcat(cFileName, "\\");
	strcat(cFileName, "\\");
	strcat(cFileName, cTemp);
	strcat(cFileName, ".txt");

	hFile = CreateFile(cFileName, GENERIC_READ, NULL, NULL, OPEN_EXISTING, NULL, NULL);
	dwFileSize = GetFileSize(hFile, NULL);
	if (hFile != INVALID_HANDLE_VALUE) CloseHandle(hFile);

	pFile = fopen(cFileName, "rt");
	if (pFile == NULL) return FALSE;
	else
	{
		pBuffer = new char[dwFileSize + 1];
		ZeroMemory(pBuffer, dwFileSize + 1);
		fread(pBuffer, dwFileSize, 1, pFile);
		bRet = __bDecodeBuildItemContents(pBuffer);
		delete[] pBuffer;
	}
	fclose(pFile);
//	return bRet;
	return true;
}
bool cMain::__bDecodeBuildItemContents(char *pBuffer)
{
	char * pContents, *token;
	char seps[] = "= ,\t\n";
	char cReadModeA = 0;
	char cReadModeB = 0;
	int  iIndex = 0;
	class CStrTok * pStrTok;
	pContents = pBuffer;
	pStrTok = new class CStrTok(pContents, seps);
	token = pStrTok->pGet();

	while (token != NULL)
	{
		if (cReadModeA != 0)
		{
			switch (cReadModeA) {
			case 1:
				switch (cReadModeB) {
				case 1:
					ZeroMemory(m_pBuildItemList[iIndex]->m_cName, sizeof(m_pBuildItemList[iIndex]->m_cName));
					memcpy(m_pBuildItemList[iIndex]->m_cName, token, strlen(token));
					cReadModeB = 2;
					break;
				case 2:
					m_pBuildItemList[iIndex]->m_iSkillLimit = atoi(token);
					cReadModeB = 3;
					break;
				case 3: // m_cElementName1
					ZeroMemory(m_pBuildItemList[iIndex]->m_cElementName1, sizeof(m_pBuildItemList[iIndex]->m_cElementName1));
					memcpy(m_pBuildItemList[iIndex]->m_cElementName1, token, strlen(token));
					cReadModeB = 4;
					break;
				case 4: // m_iElementCount1
					m_pBuildItemList[iIndex]->m_iElementCount[1] = atoi(token);
					cReadModeB = 5;
					break;
				case 5: // m_cElementName2
					ZeroMemory(m_pBuildItemList[iIndex]->m_cElementName2, sizeof(m_pBuildItemList[iIndex]->m_cElementName2));
					memcpy(m_pBuildItemList[iIndex]->m_cElementName2, token, strlen(token));
					cReadModeB = 6;
					break;
				case 6: // m_iElementCount2
					m_pBuildItemList[iIndex]->m_iElementCount[2] = atoi(token);
					cReadModeB = 7;
					break;
				case 7: // m_cElementName3
					ZeroMemory(m_pBuildItemList[iIndex]->m_cElementName3, sizeof(m_pBuildItemList[iIndex]->m_cElementName3));
					memcpy(m_pBuildItemList[iIndex]->m_cElementName3, token, strlen(token));
					cReadModeB = 8;
					break;
				case 8: // m_iElementCount3
					m_pBuildItemList[iIndex]->m_iElementCount[3] = atoi(token);
					cReadModeB = 9;
					break;
				case 9: // m_cElementName4
					ZeroMemory(m_pBuildItemList[iIndex]->m_cElementName4, sizeof(m_pBuildItemList[iIndex]->m_cElementName4));
					memcpy(m_pBuildItemList[iIndex]->m_cElementName4, token, strlen(token));
					cReadModeB = 10;
					break;
				case 10: // m_iElementCount4
					m_pBuildItemList[iIndex]->m_iElementCount[4] = atoi(token);
					cReadModeB = 11;
					break;
				case 11: // m_cElementName5
					ZeroMemory(m_pBuildItemList[iIndex]->m_cElementName5, sizeof(m_pBuildItemList[iIndex]->m_cElementName5));
					memcpy(m_pBuildItemList[iIndex]->m_cElementName5, token, strlen(token));
					cReadModeB = 12;
					break;
				case 12: // m_iElementCount5
					m_pBuildItemList[iIndex]->m_iElementCount[5] = atoi(token);
					cReadModeB = 13;
					break;
				case 13: // m_cElementName6
					ZeroMemory(m_pBuildItemList[iIndex]->m_cElementName6, sizeof(m_pBuildItemList[iIndex]->m_cElementName6));
					memcpy(m_pBuildItemList[iIndex]->m_cElementName6, token, strlen(token));
					cReadModeB = 14;
					break;
				case 14: // m_iElementCount6
					m_pBuildItemList[iIndex]->m_iElementCount[6] = atoi(token);
					cReadModeB = 15;
					break;

				case 15:
					m_pBuildItemList[iIndex]->m_iSprH = atoi(token);
					cReadModeB = 16;
					break;

				case 16:
					m_pBuildItemList[iIndex]->m_iSprFrame = atoi(token);
					cReadModeB = 17;
					break;

				case 17:
					m_pBuildItemList[iIndex]->m_iMaxSkill = atoi(token);

					cReadModeA = 0;
					cReadModeB = 0;
					iIndex++;
					break;
				}
				break;

			default:
				break;
			}
		}
		else
		{
			if (memcmp(token, "BuildItem", 9) == 0)
			{
				cReadModeA = 1;
				cReadModeB = 1;
				m_pBuildItemList[iIndex] = new class CBuildItem;
			}
		}
		token = pStrTok->pGet();
	}
	delete pStrTok;
	if ((cReadModeA != 0) || (cReadModeB != 0)) return FALSE;
	return TRUE;
}


bool cMain::bReadItemNameConfigFile()
{
	FILE * pFile;
	HANDLE hFile;
	DWORD  dwFileSize;
	char * cp, *token, cReadModeA, cReadModeB;
	char seps[] = "=\n";
	int iIndex;

	cReadModeA = 0;
	cReadModeB = 0;
	iIndex = 0;

	hFile = CreateFile("contents\\ItemName.cfg", GENERIC_READ, NULL, NULL, OPEN_EXISTING, NULL, NULL);
	dwFileSize = GetFileSize(hFile, NULL);
	if (hFile != INVALID_HANDLE_VALUE) CloseHandle(hFile);
	pFile = fopen("contents\\ItemName.cfg", "rt");
	if (pFile == NULL) return FALSE;
	else {
		cp = new char[dwFileSize + 2];
		ZeroMemory(cp, dwFileSize + 2);
		fread(cp, dwFileSize, 1, pFile);

		token = strtok(cp, seps);
		while (token != NULL)   {

			if (cReadModeA != 0) {
				switch (cReadModeA) {
				case 1:
					switch (cReadModeB) {
					case 1:
						m_pItemNameList[iIndex] = new class CItemName;
						strcpy(m_pItemNameList[iIndex]->m_cOriginName, token);
						cReadModeB = 2;
						break;

					case 2:
						strcpy(m_pItemNameList[iIndex]->m_cName, token);
						cReadModeA = 0;
						cReadModeB = 0;
						iIndex++;
						break;
					}
				}
			}
			else {
				if (memcmp(token, "Item", 4) == 0) {
					cReadModeA = 1;
					cReadModeB = 1;
				}
			}
			token = strtok(NULL, seps);
		}
		delete[] cp;
	}
	if (pFile != NULL) fclose(pFile);

	return TRUE;
}
bool cMain::bReadLoginConfigFile(char * cFn)
{
	FILE * pFile;
	HANDLE hFile;
	DWORD  dwFileSize;
	char * cp, *token, cReadMode;
	char seps[] = "= ,\t\n";
	cReadMode = 0;
	// Get file size only
	hFile = CreateFile(cFn, GENERIC_READ, NULL, NULL, OPEN_EXISTING, NULL, NULL);
	dwFileSize = GetFileSize(hFile, NULL);
	if (hFile != INVALID_HANDLE_VALUE) CloseHandle(hFile);
	pFile = fopen(cFn, "rt");
	if (pFile == NULL) goto DEFAULT_IP;
	cp = new char[dwFileSize + 2];
	ZeroMemory(cp, dwFileSize + 2);
	fread(cp, dwFileSize, 1, pFile);
	token = strtok(cp, seps);
	while (token != NULL)
	{
		if (cReadMode != 0)
		{
			switch (cReadMode) {
			case 1: // log-server-address
				if (strlen(token) > 15)
				{
					delete[] cp;
					if (pFile != NULL) fclose(pFile);
					goto DEFAULT_IP;
				}
				ZeroMemory(m_cLogServerAddr, sizeof(m_cLogServerAddr));
				strcpy(m_cLogServerAddr, token);
				cReadMode = 0;
				break;
			case 2: // log-server-port
				m_iLogServerPort = atoi(token);
				if (m_iLogServerPort == 0)
				{
					delete[] cp;
					if (pFile != NULL) fclose(pFile);
					goto DEFAULT_IP;
				}
				cReadMode = 0;
				break;
			case 3: // game-server-mode
				if ((memcmp(token, "lan", 3) == 0) || (memcmp(token, "LAN", 3) == 0))
				{
					m_iGameServerMode = 1;  // Will no connect multi-hoster HB
					// but will connect from LAN even with bad router
				}
				if ((memcmp(token, "internet", 8) == 0) || (memcmp(token, "INTERNET", 8) == 0))
				{
					m_iGameServerMode = 2; // Default HB mode
				}
				cReadMode = 0;
				break;
			}
		}
		else
		{
			if (memcmp(token, "log-server-address", 18) == 0) cReadMode = 1;
			if (memcmp(token, "log-server-port", 15) == 0)    cReadMode = 2;
			if (memcmp(token, "game-server-mode", 16) == 0)   cReadMode = 3;
		}
		token = strtok(NULL, seps);
	}
	delete[] cp;
	if (pFile != NULL) fclose(pFile);
	return TRUE;
DEFAULT_IP:
	ZeroMemory(m_cLogServerAddr, sizeof(m_cLogServerAddr));
	strcpy(m_cLogServerAddr, SERVER_IP);
	m_iLogServerPort = SERVER_PORT;
	return TRUE;
}
void cMain::ReleaseTimeoverChatMsg()
{
	int i;
	DWORD dwTime;
	dwTime = G_dwGlobalTime;
	for (i = 1; i < MAXCHATMSGS; i++)
	if (m_pChatMsgList[i] != NULL) {

		if ((m_pChatMsgList[i]->m_cType >= 1) && (m_pChatMsgList[i]->m_cType <= 20)) {
			if ((dwTime - m_pChatMsgList[i]->m_dwTime) > CHATTIMEOUT_A) {
				delete m_pChatMsgList[i];
				m_pChatMsgList[i] = NULL;
			}
		}
		else
		if ((m_pChatMsgList[i]->m_cType >= 21) && (m_pChatMsgList[i]->m_cType <= 40)) {
			if ((dwTime - m_pChatMsgList[i]->m_dwTime) > CHATTIMEOUT_B) {
				delete m_pChatMsgList[i];
				m_pChatMsgList[i] = NULL;
			}
		}
		else
		if ((m_pChatMsgList[i]->m_cType >= 41) && (m_pChatMsgList[i]->m_cType <= 60)) {
			if ((dwTime - m_pChatMsgList[i]->m_dwTime) > CHATTIMEOUT_C) {
				delete m_pChatMsgList[i];
				m_pChatMsgList[i] = NULL;
			}
		}
		else if ((dwTime - m_pChatMsgList[i]->m_dwTime) > CHATTIMEOUT_A) {
			delete m_pChatMsgList[i];
			m_pChatMsgList[i] = NULL;
		}
	}
}
void cMain::ReleaseUnusedSprites()
{
	int i;
	for (i = 0; i < MAXSPRITES; i++)
	if ((m_pSprite[i] != NULL))
	{
		if ((m_pSprite[i]->m_bIsSurfaceEmpty == FALSE) && (m_pSprite[i]->m_bOnCriticalSection == FALSE))
		{
			if ((G_dwGlobalTime - m_pSprite[i]->m_dwRefTime) > 60000) m_pSprite[i]->_iCloseSprite();

		}
	}
	for (i = 0; i < MAXTILES; i++)
	if ((m_pTileSpr[i] != NULL))
	{
		if ((m_pTileSpr[i]->m_bIsSurfaceEmpty == FALSE) && (m_pTileSpr[i]->m_bOnCriticalSection == FALSE))
		{
			if ((G_dwGlobalTime - m_pTileSpr[i]->m_dwRefTime) > 60000) m_pTileSpr[i]->_iCloseSprite();
		}
	}
	for (i = 0; i < MAXEFFECTSPR; i++)
	if ((m_pEffectSpr[i] != NULL))
	{
		if ((m_pEffectSpr[i]->m_bIsSurfaceEmpty == FALSE) && (m_pEffectSpr[i]->m_bOnCriticalSection == FALSE))
		{
			if ((G_dwGlobalTime - m_pEffectSpr[i]->m_dwRefTime) > 60000) m_pEffectSpr[i]->_iCloseSprite();
		}
	}

	for (i = 0; i < MAXSOUNDEFFECTS; i++)
	{
		if (m_pCSound[i] != NULL)
		{
			if (((G_dwGlobalTime - m_pCSound[i]->m_dwTime) > 30000) && (m_pCSound[i]->m_bIsLooping == FALSE)) m_pCSound[i]->_ReleaseSoundBuffer();
		}
		if (m_pMSound[i] != NULL)
		{
			if (((G_dwGlobalTime - m_pMSound[i]->m_dwTime) > 30000) && (m_pMSound[i]->m_bIsLooping == FALSE)) m_pMSound[i]->_ReleaseSoundBuffer();
		}
		if (m_pESound[i] != NULL)
		{
			if (((G_dwGlobalTime - m_pESound[i]->m_dwTime) > 30000) && (m_pESound[i]->m_bIsLooping == FALSE)) m_pESound[i]->_ReleaseSoundBuffer();
		}
	}
}
void cMain::UpdateScreen_OnQuit()
{
	short msX, msY, msZ;
	char cLB, cRB;
	char cMIresult;
	int  iMIbuttonNum;

	static class CMouseInterface * pMI;

	DWORD dwTime = timeGetTime();

	if (m_cGameModeCount == 0) {
		//if (G_pCalcSocket != NULL) 
		//{	delete G_pCalcSocket;
		//	G_pCalcSocket = NULL;
		//}
		//if (m_pGSock != NULL) 
		//{	//delete m_pGSock;
		//m_pGSock = NULL;
		//}
		m_bEscPressed = FALSE;
		m_bEnterPressed = FALSE;
		pMI = new class CMouseInterface;
		pMI->AddRect(0, 0, 640, 480);
		m_bEnterPressed = FALSE;
	}

	m_cGameModeCount++;
	if (m_cGameModeCount > 120) m_cGameModeCount = 120;

	m_DDraw.ClearBackB4();

	if (m_bEscPressed == TRUE || m_bEnterPressed == TRUE) {
		m_bEscPressed = FALSE;
		m_bEnterPressed = FALSE;
		delete pMI;
		ChangeGameMode(GAMEMODE_NULL);
		SendMessage(m_hWnd, WM_DESTROY, NULL, NULL);
		return;
	}
	DrawNewDialogBox(SPRID_INTERFACE_ND_QUIT, 0, 0, 0, TRUE);
	if (m_cGameModeCount > 20) DrawNewDialogBox(SPRID_INTERFACE_ND_QUIT, 255, 123, 1, TRUE);
	else if ((m_cGameModeCount >= 15) && (m_cGameModeCount <= 20)) m_pSprite[SPRID_INTERFACE_ND_QUIT]->PutTransSprite25(255, 123, 1, TRUE);
	DrawVersion(TRUE);
	if (m_cGameModeCount == 100)
	{
		ChangeGameMode(GAMEMODE_NULL);
		delete pMI;
		SendMessage(m_hWnd, WM_DESTROY, NULL, NULL);
		return;
	}
	m_DInput.UpdateMouseState(&msX, &msY, &msZ, &cLB, &cRB);
	m_pSprite[SPRID_MOUSECURSOR]->PutSpriteFast(msX, msY, 0, dwTime);

	iMIbuttonNum = pMI->iGetStatus(msX, msY, cLB, &cMIresult);
	if ((cMIresult == MIRESULT_CLICK) && (iMIbuttonNum == 1)) {
		ChangeGameMode(GAMEMODE_NULL);
		SendMessage(m_hWnd, WM_DESTROY, NULL, NULL);
		delete pMI;
		return;
	}

	//	if (m_cGameModeCount < 6) m_DDraw.DrawShadowBox(0,0,639,479);
	//	if (m_cGameModeCount < 2) m_DDraw.DrawShadowBox(0,0,639,479);

	if (m_DDraw.iFlip() == DDERR_SURFACELOST) RestoreSprites();
}
void cMain::UpdateScreen_OnAgreement()
{
	short sX, sY, msX, msY, msZ;
	char  cLB, cRB;
	char  cMIresult;
	static class CMouseInterface * pMI;
	int i, iTotalLines, iPointerLoc;
	DWORD dwTime = timeGetTime();
	double d1, d2, d3;
	int iMIbuttonNum;

	sX = 121;
	sY = 22;

	if (m_cGameModeCount == 0) {
		m_iAgreeView = 0;
		_LoadAgreementTextContents(0);

		pMI = new class CMouseInterface;
		pMI->AddRect(sX + 82 - 105, sY + 355, sX + 131 - 105, sY + 374);
		pMI->AddRect(sX + 235 - 105, sY + 355, sX + 303 - 105, sY + 375);
	}

	m_cGameModeCount++;
	if (m_cGameModeCount > 100) m_cGameModeCount = 100;
	if (m_bEnterPressed == TRUE) {
		PlaySound('E', 14, 5);
		ChangeGameMode(GAMEMODE_ONCREATENEWACCOUNT);
		//ClearContents_OnCreateNewAccount();
		delete pMI;
		return;
	}
	if (m_bEscPressed == TRUE) {
		PlaySound('E', 14, 5);
		ChangeGameMode(GAMEMODE_ONMAINMENU);
		delete pMI;
		m_bEscPressed = FALSE;
		return;
	}
	m_DInput.UpdateMouseState(&msX, &msY, &msZ, &cLB, &cRB);

	iMIbuttonNum = pMI->iGetStatus(msX, msY, cLB, &cMIresult);
	if (cMIresult == MIRESULT_CLICK) {
		switch (iMIbuttonNum) {
		case 1: // Agree
			PlaySound('E', 14, 5);
			ChangeGameMode(GAMEMODE_ONCREATENEWACCOUNT);
			//ClearContents_OnCreateNewAccount();
			delete pMI;
			return;

		case 2:	// Disagree
			PlaySound('E', 14, 5);
			ChangeGameMode(GAMEMODE_ONMAINMENU);
			delete pMI;
			return;
		}
	}

	DrawNewDialogBox(SPRID_INTERFACE_ND_AGREEMENT, 0, 0, 0, TRUE);
	DrawNewDialogBox(SPRID_INTERFACE_ND_TEXT, 43, 38, 12, FALSE);

	iTotalLines = 0;
	for (i = 0; i < TEXTDLGMAXLINES; i++)
	if (m_pAgreeMsgTextList[i] != NULL) iTotalLines++;

	d1 = (double)m_iAgreeView;
	d2 = (double)(iTotalLines - 20);
	d3 = (double)d1 / d2;
	d1 = 338.0f * d3;
	iPointerLoc = (int)d1;
	m_pSprite[SPRID_INTERFACE_ND_GAME2]->PutSpriteFast(sX + 361 - 112, sY + 37 + 13 + iPointerLoc, 7, dwTime);

	for (i = 0; i < 20; i++)
	if (m_pAgreeMsgTextList[i + m_iAgreeView] != NULL) {
		PutAlignedString(60, 360, sY + 65 + i * 13, m_pAgreeMsgTextList[i + m_iAgreeView]->m_pMsg, 45, 25, 25);
	}

	if (msZ != 0)
	{
		m_iAgreeView = m_iAgreeView - msZ / 60;
		m_DInput.m_sZ = 0;
	}
	if (cLB != 0 && iTotalLines > 20)
	{
		if ((msX >= sX + 345 - 112) && (msX <= sX + 380 - 112) && (msY >= sY + 50) && (msY <= sY + 395))
		{
			d1 = (double)(msY - (sY + 37 + 13));
			d2 = (double)(iTotalLines - 17);
			d3 = (double)(d1 * d2) / (338.0f);
			m_iAgreeView = (int)d3;
			m_pSprite[SPRID_INTERFACE_ND_GAME2]->PutTransSprite(sX + 361 - 112, sY + 37 + 13 + iPointerLoc, 4, dwTime);
		}
	}
	if (m_iAgreeView < 0) m_iAgreeView = 0;
	if (iTotalLines > 20 && m_iAgreeView > iTotalLines - 20) m_iAgreeView = iTotalLines - 20;

	if ((msX > sX + 82 - 105) && (msX < sX + 131 - 105) && (msY > sY + 355 - 3) && (msY < sY + 374 + 3))
		DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + 43 + 20 - 23 + 45 - 105, sY + 265 + 90, 13);
	else DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + 43 + 20 - 23 + 45 - 105, sY + 265 + 90, 12);

	if ((msX > sX + 235 - 105) && (msX < sX + 303 - 105) && (msY > sY + 355 - 3) && (msY < sY + 375 + 3))
		DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + 158 + 57 - 23 + 45 - 105, sY + 265 + 90, 15);
	else DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, sX + 158 + 57 - 23 + 45 - 105, sY + 265 + 90, 14);

	DrawVersion();
	m_pSprite[SPRID_MOUSECURSOR]->PutSpriteFast(msX, msY, 0, dwTime);

	//	if (m_cGameModeCount < 6) m_DDraw.DrawShadowBox(0,0,639,479);
	//	if (m_cGameModeCount < 2) m_DDraw.DrawShadowBox(0,0,639,479);

	if (m_DDraw.iFlip() == DDERR_SURFACELOST) RestoreSprites();
}

void cMain::UpdateScreen_OnCreateNewAccount()
{
	short msX, msY, msZ;
	char cLB, cRB;
	char cMIresult;
	int  iMIbuttonNum;
	static class CMouseInterface * pMI;
	static char cName[12], cPassword[12], cConfirm[12], cPrevFocus, cSSN_A[8], cSSN_B[8], cQuiz[44], cAnswer[20], cTempQuiz[44];
	DWORD dwTime = timeGetTime();
	int iFlag = 0;

	if (m_cGameModeCount == 0)
	{
		pMI = new class CMouseInterface;
		pMI->AddRect(310, 80, 560, 100);
		pMI->AddRect(310, 101, 558, 122);
		pMI->AddRect(310, 123, 558, 145);
		//pMI->AddRect(310,146,558,167); // real name
		//pMI->AddRect(310,168,477,191); // ssn
		//pMI->AddRect(488,168,557,191); // ssn
		pMI->AddRect(300, 192, 560, 231);
		pMI->AddRect(300, 232, 560, 268);   // Quiz 8  // +41
		pMI->AddRect(300, 269, 560, 306);   // Answer 9
		pMI->AddRect(297, 399, 367, 417);  // Create 10
		pMI->AddRect(392, 399, 462, 417);  // Clear  11
		pMI->AddRect(488, 399, 558, 417);  // Cancel  12   Size 73 x 15
		cPrevFocus = 1;
		m_cCurFocus = 1;
		m_cMaxFocus = 9; //12
		m_bEnterPressed = FALSE;
		m_cArrowPressed = 0;
		ZeroMemory(m_cEmailAddr, sizeof(m_cEmailAddr));
		ZeroMemory(cName, sizeof(cName));
		ZeroMemory(cPassword, sizeof(cPassword));
		ZeroMemory(cConfirm, sizeof(cConfirm));
		ZeroMemory(cSSN_A, sizeof(cSSN_A));
		ZeroMemory(cSSN_B, sizeof(cSSN_B));
		ZeroMemory(m_cAccountSSN, sizeof(m_cAccountSSN));
		ZeroMemory(cQuiz, sizeof(cQuiz));
		ZeroMemory(cTempQuiz, sizeof(cTempQuiz));
		ZeroMemory(cAnswer, sizeof(cAnswer));
		StartInputString(427, 84, 11, cName);
		ClearInputString();
	}

	m_cGameModeCount++;
	if (m_cGameModeCount > 100) m_cGameModeCount = 100;
	if (m_cArrowPressed != 0)
	{
		switch (m_cArrowPressed) {
		case 1:
			m_cCurFocus--;
			if (m_cCurFocus <= 0) m_cCurFocus = m_cMaxFocus;
			break;
		case 3:
			m_cCurFocus++;
			if (m_cCurFocus > m_cMaxFocus) m_cCurFocus = 1;
			break;
		}
		m_cArrowPressed = 0;
	}

	if (m_bEscPressed == TRUE)
	{
		ChangeGameMode(GAMEMODE_ONMAINMENU);
		delete pMI;
		m_bEscPressed = FALSE;
		return;
	}

	if (cPrevFocus != m_cCurFocus)
	{
		EndInputString();
		switch (m_cCurFocus) {
		case 1:
			StartInputString(427, 84, 11, cName);
			break;
		case 2:
			StartInputString(427, 106, 11, cPassword);
			break;
		case 3:
			StartInputString(427, 129, 11, cConfirm);
			break;
		case 4:
			StartInputString(311, 215, 31, m_cEmailAddr);
			break;
		case 5:
			StartInputString(311, 253, 44, cQuiz);
			break;
		case 6:
			StartInputString(311, 291, 19, cAnswer);
			break;
		}
		cPrevFocus = m_cCurFocus;
	}

	m_DDraw.ClearBackB4();
	DrawNewDialogBox(SPRID_INTERFACE_ND_NEWACCOUNT, 0, 0, 0, TRUE);
	PutString(377, 84, "Account:", RGB(100, 100, 200));
	PutString(372, 106, "Password:", RGB(100, 100, 200));
	PutString(372, 129, "(confirm)", RGB(100, 100, 200));
	PutString(271, 215, "eMail:", RGB(100, 100, 200));
	PutString(276, 253, "Quiz:", RGB(100, 100, 200));
	PutString(266, 291, "Answer:", RGB(100, 100, 200));

	if ((m_cCurFocus == 2) || (m_cCurFocus == 3))
		ShowReceivedString(TRUE);
	else if ((m_cCurFocus == 1) || (m_cCurFocus == 4) || (m_cCurFocus == 5) || (m_cCurFocus == 6))
		ShowReceivedString();

	if (m_cCurFocus != 1) {
		if (m_Misc.bCheckValidName(cName) != FALSE)
			PutString2(427, 84, cName, 100, 200, 100);
		else PutString2(427, 84, cName, 200, 100, 100);
	}
	if (m_cCurFocus != 2) {
		if (m_Misc.bCheckValidName(cPassword) != FALSE)
			PutString(427, 106, cPassword, RGB(100, 200, 100), TRUE, 1);
		else PutString(427, 106, cPassword, RGB(200, 100, 100), TRUE, 1);
	}

	if (m_cCurFocus != 3) {
		if (memcmp(cPassword, cConfirm, 10) == 0)
			PutString(427, 129, cConfirm, RGB(100, 200, 100), TRUE, 1);
		else PutString(427, 129, cConfirm, RGB(200, 100, 100), TRUE, 1);
	}
	if (memcmp(cPassword, cConfirm, 10) != 0) iFlag = 9;

	if (m_cCurFocus != 4) {
		if (m_Misc.bIsValidEmail(m_cEmailAddr))
			PutString2(311, 48 + 190 - 25 + 2, m_cEmailAddr, 100, 200, 100);
		else PutString2(311, 48 + 190 - 25 + 2, m_cEmailAddr, 200, 100, 100);
	}

	wsprintf(cTempQuiz, "%s", cQuiz);
	m_Misc.ReplaceString(cTempQuiz, ' ', '_');

	if (m_cCurFocus != 5) {
		//if(m_Misc.bCheckValidName(cTempQuiz) != FALSE)
		PutString2(311, 48 + 226 - 25 + 4, cQuiz, 100, 200, 100);
		//else PutString2(311, 48 + 226 -25 +4, cQuiz, 200,100,100);
	}

	if (m_cCurFocus != 6) {
		//if (m_Misc.bCheckValidName(cAnswer) != FALSE)
		PutString2(311, 291, cAnswer, 100, 200, 100);
		//else PutString2(311, 291, cAnswer, 200,100,100);
	}

	//if (m_Misc.bCheckValidName(cAnswer) == FALSE)		iFlag = 13;
	//if (m_Misc.bCheckValidName(cTempQuiz) == FALSE)		iFlag = 12;
	if (strlen(cAnswer) == 0)							iFlag = 11;
	if (strlen(cTempQuiz) == 0)							iFlag = 10;
	if (m_Misc.bCheckValidName(cPassword) == FALSE)		iFlag = 7;
	if (m_Misc.bCheckValidName(cName) == FALSE)			iFlag = 6;
	if (m_Misc.bIsValidEmail(m_cEmailAddr) == FALSE)	iFlag = 5;
	if (strlen(cConfirm) == 0)							iFlag = 3;
	if (strlen(cPassword) == 0)							iFlag = 2;
	if ((strlen(cName) == 0))							iFlag = 1;


	switch (m_cCurFocus) {
	case 1:
		PutAlignedString(290, 575, 330, UPDATE_SCREEN_ON_CREATE_NEW_ACCOUNT1);
		PutAlignedString(290, 575, 345, UPDATE_SCREEN_ON_CREATE_NEW_ACCOUNT2);
		break;

	case 2:
		PutAlignedString(290, 575, 330, UPDATE_SCREEN_ON_CREATE_NEW_ACCOUNT4);
		break;

	case 3:
		PutAlignedString(290, 575, 330, UPDATE_SCREEN_ON_CREATE_NEW_ACCOUNT8);
		break;

	case 4:
		PutAlignedString(290, 575, 330, UPDATE_SCREEN_ON_CREATE_NEW_ACCOUNT21);
		PutAlignedString(290, 575, 345, UPDATE_SCREEN_ON_CREATE_NEW_ACCOUNT22);
		PutAlignedString(290, 575, 360, UPDATE_SCREEN_ON_CREATE_NEW_ACCOUNT23);
		break;

	case 5:
		PutAlignedString(290, 575, 330, UPDATE_SCREEN_ON_CREATE_NEW_ACCOUNT25);
		PutAlignedString(290, 575, 345, UPDATE_SCREEN_ON_CREATE_NEW_ACCOUNT26);
		break;

	case 6:
		PutAlignedString(290, 575, 330, UPDATE_SCREEN_ON_CREATE_NEW_ACCOUNT29);
		break;

	case 7:
		switch (iFlag) {
		case 0:
			PutAlignedString(290, 575, 330, UPDATE_SCREEN_ON_CREATE_NEW_ACCOUNT33);
			break;

		case 1:
			PutAlignedString(290, 575, 330, UPDATE_SCREEN_ON_CREATE_NEW_ACCOUNT35);
			break;

		case 2:
			PutAlignedString(290, 575, 330, UPDATE_SCREEN_ON_CREATE_NEW_ACCOUNT38);
			break;

		case 3:
			PutAlignedString(290, 575, 330, UPDATE_SCREEN_ON_CREATE_NEW_ACCOUNT42);
			break;

		case 5:
			PutAlignedString(290, 575, 330, UPDATE_SCREEN_ON_CREATE_NEW_ACCOUNT50);
			break;

		case 6:
			PutAlignedString(290, 575, 330, UPDATE_SCREEN_ON_CREATE_NEW_ACCOUNT52);
			PutAlignedString(290, 575, 345, UPDATE_SCREEN_ON_CREATE_NEW_ACCOUNT53);
			break;

		case 7:
			PutAlignedString(290, 575, 330, UPDATE_SCREEN_ON_CREATE_NEW_ACCOUNT56);
			PutAlignedString(290, 575, 345, UPDATE_SCREEN_ON_CREATE_NEW_ACCOUNT57);
			break;

		case 9:
			PutAlignedString(290, 575, 330, UPDATE_SCREEN_ON_CREATE_NEW_ACCOUNT63);
			PutAlignedString(290, 575, 345, UPDATE_SCREEN_ON_CREATE_NEW_ACCOUNT64);
			PutAlignedString(290, 575, 360, UPDATE_SCREEN_ON_CREATE_NEW_ACCOUNT65);
			break;
		case 10:
			PutAlignedString(290, 575, 330, UPDATE_SCREEN_ON_CREATE_NEW_ACCOUNT67);
			break;
		case 11:
			PutAlignedString(290, 575, 330, UPDATE_SCREEN_ON_CREATE_NEW_ACCOUNT69);
			break;
		case 12:
			PutAlignedString(290, 575, 330, UPDATE_SCREEN_ON_CREATE_NEW_ACCOUNT73);
			PutAlignedString(290, 575, 345, UPDATE_SCREEN_ON_CREATE_NEW_ACCOUNT74);
			break;

		case 13:
			PutAlignedString(290, 575, 330, UPDATE_SCREEN_ON_CREATE_NEW_ACCOUNT77);
			PutAlignedString(290, 575, 345, UPDATE_SCREEN_ON_CREATE_NEW_ACCOUNT78);
			break;

		}
		break;

	case 8:
		PutAlignedString(290, 575, 330, UPDATE_SCREEN_ON_CREATE_NEW_ACCOUNT80);
		break;

	case 9:
		PutAlignedString(290, 575, 330, UPDATE_SCREEN_ON_CREATE_NEW_ACCOUNT81);
		break;
	}


	if ((iFlag == 0) && (m_cCurFocus == 7))
		m_pSprite[SPRID_INTERFACE_ND_BUTTON]->PutSpriteFast(199 + 98, 398, 25, dwTime);
	else m_pSprite[SPRID_INTERFACE_ND_BUTTON]->PutSpriteFast(199 + 98, 398, 24, dwTime);

	if (m_cCurFocus == 8)
		m_pSprite[SPRID_INTERFACE_ND_BUTTON]->PutSpriteFast(294 + 98, 398, 27, dwTime);
	else m_pSprite[SPRID_INTERFACE_ND_BUTTON]->PutSpriteFast(294 + 98, 398, 26, dwTime);

	if (m_cCurFocus == 9)
		m_pSprite[SPRID_INTERFACE_ND_BUTTON]->PutSpriteFast(390 + 98, 398, 17, dwTime);
	else m_pSprite[SPRID_INTERFACE_ND_BUTTON]->PutSpriteFast(390 + 98, 398, 16, dwTime);

	DrawVersion(TRUE);
	m_DInput.UpdateMouseState(&msX, &msY, &msZ, &cLB, &cRB);
	m_pSprite[SPRID_MOUSECURSOR]->PutSpriteFast(msX, msY, 0, dwTime);
	if (m_bEnterPressed == TRUE)
	{
		PlaySound('E', 14, 5);
		switch (m_cCurFocus) {
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
			m_cCurFocus++;
			if (m_cCurFocus > m_cMaxFocus) m_cCurFocus = 1;
			break;

		case 7:
			if (iFlag != 0) return;
			ZeroMemory(m_cAccountName, sizeof(m_cAccountName));
			ZeroMemory(m_cAccountPassword, sizeof(m_cAccountPassword));

			ZeroMemory(m_cAccountQuiz, sizeof(m_cAccountQuiz));
			ZeroMemory(m_cAccountAnswer, sizeof(m_cAccountAnswer));

			strcpy(m_cAccountName, cName);
			strcpy(m_cAccountPassword, cPassword);

			strcpy(m_cAccountQuiz, cTempQuiz);
			strcpy(m_cAccountAnswer, cAnswer);
			m_cAccountQuiz[45] = ' ';
			m_cAccountAnswer[20] = ' ';

			ZeroMemory(m_cAccountSSN, sizeof(m_cAccountSSN));
			wsprintf(m_cAccountSSN, "%s-%s", cSSN_A, cSSN_B);

			if (memcmp(cPassword, cConfirm, 10) != 0)
			{
				ChangeGameMode(GAMEMODE_ONMSG);
				ZeroMemory(m_cMsg, sizeof(m_cMsg));
				strcpy(m_cMsg, UPDATE_SCREEN_ON_CREATE_NEW_ACCOUNT82);
				//"Cannot create account! - password not match!"
				delete pMI;
				return;
			}
			//m_pLSock = new class XSocket(m_hWnd, SOCKETBLOCKLIMIT);
			//m_pLSock->bConnect(m_cLogServerAddr, m_iLogServerPort, WM_USER_LOGSOCKETEVENT);
			//m_pLSock->bInitBufferSize(30000);

			ChangeGameMode(GAMEMODE_ONCONNECTING);
			//m_dwConnectMode = MSGID_REQUEST_CREATENEWACCOUNT;
			ZeroMemory(m_cMsg, sizeof(m_cMsg));
			strcpy(m_cMsg, "00");
			delete pMI;
			return;

		case 8:
			ZeroMemory(cName, sizeof(cName));
			ZeroMemory(cPassword, sizeof(cPassword));
			ZeroMemory(cConfirm, sizeof(cConfirm));
			ZeroMemory(m_cAccountAge, sizeof(m_cAccountAge));
			ZeroMemory(m_cAccountCountry, sizeof(m_cAccountCountry));
			ZeroMemory(m_cAccountSSN, sizeof(m_cAccountSSN));
			ZeroMemory(m_cEmailAddr, sizeof(m_cEmailAddr));
			ZeroMemory(cSSN_A, sizeof(cSSN_A));
			ZeroMemory(cSSN_B, sizeof(cSSN_B));
			ZeroMemory(cQuiz, sizeof(cQuiz));
			ZeroMemory(cTempQuiz, sizeof(cTempQuiz));
			ZeroMemory(cAnswer, sizeof(cAnswer));

			break;

		case 9:
			ChangeGameMode(GAMEMODE_ONMAINMENU);
			delete pMI;
			return;
		}
		m_bEnterPressed = FALSE;
	}

	iMIbuttonNum = pMI->iGetStatus(msX, msY, cLB, &cMIresult);
	if (cMIresult == MIRESULT_CLICK) {
		PlaySound('E', 14, 5);

		switch (iMIbuttonNum) {
		default:
			m_cCurFocus = iMIbuttonNum;
			break;

		case 7:
			if (iFlag != 0) return;
			ZeroMemory(m_cAccountName, sizeof(m_cAccountName));
			ZeroMemory(m_cAccountPassword, sizeof(m_cAccountPassword));

			ZeroMemory(m_cAccountQuiz, sizeof(m_cAccountQuiz));
			ZeroMemory(m_cAccountAnswer, sizeof(m_cAccountAnswer));


			strcpy(m_cAccountName, cName);
			strcpy(m_cAccountPassword, cPassword);
			strcpy(m_cAccountQuiz, cTempQuiz);
			strcpy(m_cAccountAnswer, cAnswer);

			ZeroMemory(m_cAccountSSN, sizeof(m_cAccountSSN));
			wsprintf(m_cAccountSSN, "%s-%s", cSSN_A, cSSN_B);

			if (memcmp(cPassword, cConfirm, 10) != 0)
			{
				ChangeGameMode(GAMEMODE_ONMSG);
				ZeroMemory(m_cMsg, sizeof(m_cMsg));
				strcpy(m_cMsg, UPDATE_SCREEN_ON_CREATE_NEW_ACCOUNT82);
				//"Cannot create account! - password not match!"
				delete pMI;
				return;
			}

			//m_pLSock = new class XSocket(m_hWnd, SOCKETBLOCKLIMIT);
			//m_pLSock->bConnect(m_cLogServerAddr, m_iLogServerPort, WM_USER_LOGSOCKETEVENT);
			//m_pLSock->bInitBufferSize(30000);
			ChangeGameMode(GAMEMODE_ONCONNECTING);
			//m_dwConnectMode = MSGID_REQUEST_CREATENEWACCOUNT;
			ZeroMemory(m_cMsg, sizeof(m_cMsg));
			strcpy(m_cMsg, "00");
			delete pMI;
			return;

		case 8:
			ZeroMemory(cName, sizeof(cName));
			ZeroMemory(cPassword, sizeof(cPassword));
			ZeroMemory(cConfirm, sizeof(cConfirm));
			ZeroMemory(m_cAccountAge, sizeof(m_cAccountAge));
			ZeroMemory(m_cAccountCountry, sizeof(m_cAccountCountry));
			ZeroMemory(m_cAccountSSN, sizeof(m_cAccountSSN));
			ZeroMemory(m_cEmailAddr, sizeof(m_cEmailAddr));
			ZeroMemory(cSSN_A, sizeof(cSSN_A));
			ZeroMemory(cSSN_B, sizeof(cSSN_B));
			ZeroMemory(cQuiz, sizeof(cQuiz));
			ZeroMemory(cTempQuiz, sizeof(cTempQuiz));
			ZeroMemory(cAnswer, sizeof(cAnswer));
			break;

		case 9:
			ChangeGameMode(GAMEMODE_ONMAINMENU);
			delete pMI;
			return;
		}
	}

	if ((msX >= 297) && (msX <= 370) && (msY >= 396) && (msY <= 417))  m_cCurFocus = 7;  //12
	if ((msX >= 392) && (msX <= 465) && (msY >= 396) && (msY <= 417)) m_cCurFocus = 8;  //13
	if ((msX >= 488) && (msX <= 561) && (msY >= 396) && (msY <= 417)) m_cCurFocus = 9; //14

	//	if (m_cGameModeCount < 6) m_DDraw.DrawShadowBox(0,0,639,479);
	//	if (m_cGameModeCount < 2) m_DDraw.DrawShadowBox(0,0,639,479);

	if (m_DDraw.iFlip() == DDERR_SURFACELOST) RestoreSprites();
}
void cMain::UpdateScreen_OnVersionNotMatch()
{
	short msX, msY, msZ;
	char cLB, cRB;
	char cMIresult;
	int  iMIbuttonNum;
	static class CMouseInterface * pMI;
	DWORD dwTime = timeGetTime();
	if (m_cGameModeCount == 0)
	{
		//if (G_pCalcSocket != NULL)
		//{	delete G_pCalcSocket;
		//	G_pCalcSocket = NULL;
		//}
		//if (m_pGSock != NULL)
		//{	//delete m_pGSock;
		//m_pGSock = NULL;
		//}
		pMI = new class CMouseInterface;
		pMI->AddRect(0, 0, 640, 480);
		m_bEnterPressed = FALSE;
	}
	m_cGameModeCount++;
	if (m_cGameModeCount > 120) m_cGameModeCount = 120;
	m_DDraw.ClearBackB4();
	if (m_bEscPressed == TRUE || m_bEnterPressed == TRUE)
	{
		m_bEscPressed = FALSE;
		m_bEnterPressed = FALSE;
		delete pMI;
		ChangeGameMode(GAMEMODE_NULL);
		SendMessage(m_hWnd, WM_DESTROY, NULL, NULL);
		return;
	}
	DrawNewDialogBox(SPRID_INTERFACE_ND_QUIT, 0, 0, 0, TRUE);
	DrawNewDialogBox(SPRID_INTERFACE_ND_GAME4, 162, 125, 2);
	PutAlignedString(168, 474, 160, UPDATE_SCREEN_ON_VERSION_NO_MATCH1);
	PutAlignedString(168, 474, 180, UPDATE_SCREEN_ON_VERSION_NO_MATCH2);
	PutAlignedString(168, 474, 250, MSG_HOMEPAGE);
	DrawVersion();
	m_DInput.UpdateMouseState(&msX, &msY, &msZ, &cLB, &cRB);
	m_pSprite[SPRID_MOUSECURSOR]->PutSpriteFast(msX, msY, 0, dwTime);

	iMIbuttonNum = pMI->iGetStatus(msX, msY, cLB, &cMIresult);
	if ((cMIresult == MIRESULT_CLICK) && (iMIbuttonNum == 1))
	{
		ChangeGameMode(GAMEMODE_NULL);
		delete pMI;
		SendMessage(m_hWnd, WM_DESTROY, NULL, NULL);
		return;
	}
	if (m_DDraw.iFlip() == DDERR_SURFACELOST) RestoreSprites();
}
void cMain::DrawVersion(BOOL bAuthor)
{
	DWORD dwTime = timeGetTime();
	WORD  wR, wG, wB;
	m_Misc.ColorTransfer(m_DDraw.m_cPixelFormat, RGB(140, 140, 140), &wR, &wG, &wB);
	// Ver
	m_pSprite[SPRID_INTERFACE_ADDINTERFACE]->PutTransSpriteRGB(14, 463, 19, wR, wG, wB, dwTime);
	// Upper Version
	wsprintf(G_cTxt, "%d", UPPERVERSION);
	PutString_SprNum(36, 463, G_cTxt, 140, 140, 140);
	// .
	m_pSprite[SPRID_INTERFACE_ADDINTERFACE]->PutTransSpriteRGB(42, 463, 18, wR, wG, wB, dwTime);
	// Lower Version
	wsprintf(G_cTxt, "%d", LOWERVERSION);
	PutString_SprNum(46, 463, G_cTxt, 140, 140, 140);
	if (bAuthor == FALSE) return;
	// Of course it's easy to remove those lines, but those people deserve some credit
	// at least for releasing their work....
	PutString2(14, 375, "Client Coded By Lifeplaysu", 220, 200, 200);



}
void cMain::UpdateScreen_OnConnecting()
{
	short sX, sY, msX, msY, msZ;
	char cLB, cRB;
	DWORD dwTime = timeGetTime();
	static class CMouseInterface * pMI;
	static DWORD dwMTime, dwCTime;

	if (m_cGameModeCount == 0) {
		m_bEnterPressed = FALSE;
		m_bEscPressed = FALSE;
		dwCTime = dwMTime = timeGetTime();
	}
	m_cGameModeCount++;
	if (m_cGameModeCount > 100) m_cGameModeCount = 100;

	if (m_bEscPressed == TRUE) {

		if ((dwTime - m_dwTime) > 1000)
		{
			ChangeGameMode(GAMEMODE_ONMAINMENU);
			//if (m_pLSock != NULL)
			//{	//delete m_pLSock;
			//m_pLSock = NULL;
			//}
			//if (m_pGSock != NULL)
			//{	//delete m_pGSock;
			//m_pGSock = NULL;
			//}	
		}
		m_bEscPressed = FALSE;
		return;
	}

	if ((dwTime - dwMTime) > 150) dwMTime = dwTime;

	if ((dwTime - dwCTime) > 100) {
		m_cMenuFrame++;
		dwCTime = dwTime;
	}
	if (m_cMenuFrame >= 8) {
		m_cMenuDirCnt++;
		if (m_cMenuDirCnt > 8) {
			m_cMenuDir++;
			m_cMenuDirCnt = 1;
		}
		m_cMenuFrame = 0;
	}
	if (m_cMenuDir > 8) m_cMenuDir = 1;

	m_DDraw.ClearBackB4();

	m_bIsHideLocalCursor = TRUE;
	switch (m_cMsg[0]) {
	case '0':
		//_Draw_UpdateScreen_OnCreateNewAccount();
		break;

	case '1':
		sX = 146;
		sY = 114;
//		_Draw_OnLogin(m_cAccountName, m_cAccountPassword, 0, 0);
		break;

	case '2':
		//_bDraw_OnCreateNewCharacter(m_cPlayerName, 0, 0, 0);
		break;

	case '3':
		UpdateScreen_OnSelectCharacter(0, 0, 0, 0);
		break;

	case '4':
		// Change Password
		UpdateScreen_OnSelectCharacter(0, 0, 0, 0, TRUE);
		break;

	case '5':
		m_DDraw.ClearBackB4();
		break;
	}
	m_bIsHideLocalCursor = FALSE;

	m_DDraw.DrawShadowBox(0, 0, 639, 479);
	DrawNewDialogBox(SPRID_INTERFACE_ND_GAME4, 162, 125, 2);
	wsprintf(G_cTxt, "Connecting to Server... %3dSec", (dwTime - m_dwTime) / 1000);
	PutString_SprFont(172 + 35, 190, G_cTxt, 7, 0, 0);

	if ((dwTime - m_dwTime) > 7000)
	{
		PutAlignedString(180, 463, 195 + 30, UPDATE_SCREEN_ON_CONNECTING1);//"Press ESC key during long time of no"
		PutAlignedString(180, 463, 195 + 45, UPDATE_SCREEN_ON_CONNECTING2);//"connection and return to the main menu."
	}
	else PutAlignedString(180, 463, 195 + 30, UPDATE_SCREEN_ON_CONNECTING3);//"  Connecting to server. Please wait..."
	DrawVersion();
	m_DInput.UpdateMouseState(&msX, &msY, &msZ, &cLB, &cRB);
	m_pSprite[SPRID_MOUSECURSOR]->PutSpriteFast(msX, msY, 8, dwTime);

	if (m_DDraw.iFlip() == DDERR_SURFACELOST) RestoreSprites();
}

void cMain::UpdateScreen_OnWaitInitData()
{
	short msX, msY, msZ;
	char cLB, cRB;
	DWORD dwTime = timeGetTime();

	if (m_cGameModeCount == 0) {
		m_bEnterPressed = FALSE;
		m_bEscPressed = FALSE;
	}
	m_cGameModeCount++;
	if (m_cGameModeCount > 100) m_cGameModeCount = 100;

	if (m_bEscPressed == TRUE) {
		if ((dwTime - m_dwTime) > 7000)
		{
			ChangeGameMode(GAMEMODE_ONMAINMENU);
			//if (m_pLSock != NULL)
			//{	//delete m_pLSock;
			//m_pLSock = NULL;
			//}
			//if (m_pGSock != NULL)
			//{	//delete m_pGSock;
			//m_pGSock = NULL;
			//}	
		}
		m_bEscPressed = FALSE;
		return;
	}

	m_DDraw.ClearBackB4();
	DrawNewDialogBox(SPRID_INTERFACE_ND_GAME4, 162, 125, 2);

	wsprintf(G_cTxt, "Waiting for response... %dsec", (dwTime - m_dwTime) / 1000);
	PutString_SprFont(172 + 44, 190, G_cTxt, 7, 0, 0);
	if ((dwTime - m_dwTime) > 7000) {
		PutAlignedString(174, 467, 190 + 30, UPDATE_SCREEN_ON_WAIT_INIT_DATA1);//"Press ESC key during long time of no"
		PutAlignedString(174, 467, 190 + 45, UPDATE_SCREEN_ON_WAIT_INIT_DATA2);//"connection and return to the main menu."
	}
	else PutAlignedString(174, 467, 195 + 30, UPDATE_SCREEN_ON_WAIT_INIT_DATA3);//  Connecting to server. Please wait..."

	DrawVersion();
	m_DInput.UpdateMouseState(&msX, &msY, &msZ, &cLB, &cRB);
	m_pSprite[SPRID_MOUSECURSOR]->PutSpriteFast(msX, msY, 8, dwTime);

	if (m_DDraw.iFlip() == DDERR_SURFACELOST) RestoreSprites();
}

void cMain::UpdateScreen_OnConnectionLost()
{
	short msX, msY, msZ;
	char cLB, cRB;
	static DWORD dwTime;
	if (m_cGameModeCount == 0)
	{
		dwTime = timeGetTime();
		if (m_bSoundFlag) m_pESound[38]->bStop();
		if ((m_bSoundFlag) && (m_bMusicStat == TRUE))
		{
			if (m_pBGM != NULL) m_pBGM->bStop();
		}
	}
	m_cGameModeCount++;
	if (m_cGameModeCount > 100) m_cGameModeCount = 100;
	m_DDraw.ClearBackB4();
	DrawNewDialogBox(SPRID_INTERFACE_ND_GAME4, 162, 125, 2);
	PutString_SprFont(172 + 54, 180, "Connection Lost!", 7, 0, 0);
	PutString(172 + 50, 180 + 30, UPDATE_SCREEN_ON_CONNECTION_LOST, RGB(0, 0, 0));//"
	DrawVersion();
	m_DInput.UpdateMouseState(&msX, &msY, &msZ, &cLB, &cRB);
	m_pSprite[SPRID_MOUSECURSOR]->PutSpriteFast(msX, msY, 0, dwTime);

	if (m_DDraw.iFlip() == DDERR_SURFACELOST) RestoreSprites();

	if ((timeGetTime() - m_dwTime) > 5000)
	{
		if (strlen(G_cCmdLineTokenA) != 0)
			ChangeGameMode(GAMEMODE_ONQUIT);
		else
		{
			ChangeGameMode(GAMEMODE_ONMAINMENU);
		}
	}
}
void cMain::UpdateScreen_OnLogin()
{//life work spot
	short msX, msY, msZ, sX, sY;
	char cLB, cRB;
	char cMIresult;
	int  iMIbuttonNum;
	static class CMouseInterface * pMI;
	static char  cPassword[12], cPrevFocus;
	static char cName[12];


	sX = 146;
	sY = 114;
	if (m_cGameModeCount == 0)
	{
		EndInputString();
		pMI = new class CMouseInterface;
		pMI->AddRect(80, 151, 337, 179);
		pMI->AddRect(80, 180, 337, 205);
		pMI->AddRect(80, 280, 163, 302);
		pMI->AddRect(258, 280, 327, 302);
		cPrevFocus = 1;
		m_cCurFocus = 1;
		m_cMaxFocus = 4;
		m_bEnterPressed = FALSE;
		m_cArrowPressed = 0;
		ZeroMemory(cName, sizeof(cName));
		ZeroMemory(cPassword, sizeof(cPassword));
		StartInputString(180, 162, 11, cName);
		ClearInputString();
	}

	m_cGameModeCount++;
	if (m_cGameModeCount > 100) m_cGameModeCount = 100;

	if (m_cArrowPressed != 0)
	{
		switch (m_cArrowPressed) {
		case 1:
			m_cCurFocus--;
			if (m_cCurFocus <= 0) m_cCurFocus = m_cMaxFocus;
			break;

		case 2:
			if (m_cCurFocus == 3) m_cCurFocus = 4;
			else if (m_cCurFocus == 4) m_cCurFocus = 3;
			break;

		case 3:
			m_cCurFocus++;
			if (m_cCurFocus > m_cMaxFocus) m_cCurFocus = 1;
			break;

		case 4:
			if (m_cCurFocus == 3) m_cCurFocus = 4;
			else if (m_cCurFocus == 4) m_cCurFocus = 3;
			break;
		}
		m_cArrowPressed = 0;
	}

	if (m_bEnterPressed == TRUE)
	{
		m_bEnterPressed = FALSE;
		PlaySound('E', 14, 5);

		switch (m_cCurFocus) {
		case 1:
			m_cCurFocus++;
			if (m_cCurFocus > m_cMaxFocus) m_cCurFocus = 1;
			break;
		case 2:
		case 3:
			if ((strlen(cName) == 0) || (strlen(cPassword) == 0)) break;
			ZeroMemory(m_cAccountName, sizeof(m_cAccountName));
			ZeroMemory(m_cAccountPassword, sizeof(m_cAccountPassword));
			strcpy(m_cAccountName, cName);
			strcpy(m_cAccountPassword, cPassword);
			////m_pLSock = new class XSocket(m_hWnd, SOCKETBLOCKLIMIT);
			////m_pLSock->bConnect(m_cLogServerAddr, m_iLogServerPort +(rand()%1), WM_USER_LOGSOCKETEVENT);
			////m_pLSock->bInitBufferSize(30000);

			//Sockets.LoginSocket = Sockets.CreateSocket();
			//Sockets.ConnectSocket(Sockets.LoginSocket, m_cLogServerAddr, m_iLogServerPort, WM_LOGIN_CONECTED);

			ChangeGameMode(GAMEMODE_ONCONNECTING);
			//m_dwConnectMode = MSGID_REQUEST_LOGIN;
			ZeroMemory(m_cMsg, sizeof(m_cMsg));
			strcpy(m_cMsg, "11");
			delete pMI;
			return;
		case 4:	// Cancel
#ifdef SELECTSERVER
			ChangeGameMode(GAMEMODE_ONSELECTSERVER);
#else
			ChangeGameMode(GAMEMODE_ONMAINMENU);
#endif
			delete pMI;
			return;
		}
	}

	if (m_bEscPressed == TRUE)
	{
		EndInputString();
		ChangeGameMode(GAMEMODE_ONMAINMENU);
		delete pMI;
		m_bEscPressed = FALSE;
		return;
	}

	if (cPrevFocus != m_cCurFocus)
	{
		EndInputString();
		switch (m_cCurFocus) {
		case 1:
			StartInputString(180, 162, 11, cName);
			break;
		case 2:
			StartInputString(180, 185, 11, cPassword, TRUE);
			break;
		case 3:
		case 4:
			break;
		}
		cPrevFocus = m_cCurFocus;
	}


	m_DInput.UpdateMouseState(&msX, &msY, &msZ, &cLB, &cRB);
	iMIbuttonNum = pMI->iGetStatus(msX, msY, cLB, &cMIresult);
	if (cMIresult == MIRESULT_CLICK)
	{
		PlaySound('E', 14, 5);
		switch (iMIbuttonNum) {
		case 1:
			m_cCurFocus = 1;
			break;

		case 2:
			m_cCurFocus = 2;
			break;

		case 3:
			if ((strlen(cName) == 0) || (strlen(cPassword) == 0)) break;
			EndInputString();
			ZeroMemory(m_cAccountName, sizeof(m_cAccountName));
			ZeroMemory(m_cAccountPassword, sizeof(m_cAccountPassword));
			strcpy(m_cAccountName, cName);
			strcpy(m_cAccountPassword, cPassword);
			////m_pLSock = new class XSocket(m_hWnd, SOCKETBLOCKLIMIT);
			////m_pLSock->bConnect(m_cLogServerAddr, m_iLogServerPort +(rand()%1), WM_USER_LOGSOCKETEVENT);
			////m_pLSock->bInitBufferSize(30000);

			//Sockets.LoginSocket = Sockets.CreateSocket();
//			Sockets.ConnectSocket(Sockets.LoginSocket, m_cLogServerAddr, m_iLogServerPort, WM_LOGIN_CONECTED);

			ChangeGameMode(GAMEMODE_ONCONNECTING);
			//m_dwConnectMode = MSGID_REQUEST_LOGIN;
			ZeroMemory(m_cMsg, sizeof(m_cMsg));
			strcpy(m_cMsg, "11");
			delete pMI;
			return;

		case 4:
#ifdef SELECTSERVER
			ChangeGameMode(GAMEMODE_ONSELECTSERVER); // ONMAINMENU
#else
			ChangeGameMode(GAMEMODE_ONMAINMENU);
#endif
			delete pMI;
			return;
		}
	}

	if ((msX >= 80) && (msX <= 163) && (msY >= 280) && (msY <= 302)) m_cCurFocus = 3;
	if ((msX >= 258) && (msX <= 327) && (msY >= 280) && (msY <= 302)) m_cCurFocus = 4;

//	_Draw_OnLogin(cName, cPassword, msX, msY, m_cGameModeCount);
	if (m_DDraw.iFlip() == DDERR_SURFACELOST) RestoreSprites();

}

void cMain::UpdateScreen_OnSelectServer()
{
	short msX, msY, msZ, sX, sY;
	char cLB, cRB;
	char cMIresult;
	int  iMIbuttonNum;
	static class CMouseInterface * pMI;
	static char  cPrevFocus;
	DWORD dwTime = timeGetTime();
	BOOL bFlag = TRUE;

	sX = 146;
	sY = 114;
	if (m_cGameModeCount == 0) {
		EndInputString();

		pMI = new class CMouseInterface;
		pMI->AddRect(130, 177, 270, 198);
		pMI->AddRect(130, 199, 270, 225);
		pMI->AddRect(256, 279, 331, 308);

		cPrevFocus = 1;
		m_cCurFocus = 1;
		m_cMaxFocus = 3;

		m_bEnterPressed = FALSE;
		m_cArrowPressed = 0;
	}
	m_cGameModeCount++;
	if (m_cGameModeCount > 100) m_cGameModeCount = 100;

	if (m_cArrowPressed != 0)
	{
		switch (m_cArrowPressed) {
		case 1:
			m_cCurFocus--;
			if (m_cCurFocus <= 0) m_cCurFocus = m_cMaxFocus;
			break;

		case 3:
			m_cCurFocus++;
			if (m_cCurFocus > m_cMaxFocus) m_cCurFocus = 1;
			break;
		}
		m_cArrowPressed = 0;
	}

	if (m_bEnterPressed == TRUE)
	{
		m_bEnterPressed = FALSE;
		PlaySound('E', 14, 5);
		switch (m_cCurFocus) {
		case 1:
			if (strlen(m_cWorldServerName) == 0)
				ZeroMemory(m_cWorldServerName, sizeof(m_cWorldServerName));
			strcpy(m_cWorldServerName, NAME_WORLDNAME1);
			ChangeGameMode(GAMEMODE_ONLOGIN);
			delete pMI;
			return;

		case 2:
			ZeroMemory(m_cWorldServerName, sizeof(m_cWorldServerName));
			strcpy(m_cWorldServerName, "WS2");
			ChangeGameMode(GAMEMODE_ONLOGIN);
			delete pMI;
			return;

		case 3:	// Cancel
			ChangeGameMode(GAMEMODE_ONMAINMENU);
			delete pMI;
			return;
		}
	}

	if (m_bEscPressed == TRUE)
	{
		ChangeGameMode(GAMEMODE_ONMAINMENU);
		delete pMI;
		m_bEscPressed = FALSE;
		return;
	}
	if (cPrevFocus != m_cCurFocus)
	{
		cPrevFocus = m_cCurFocus;
	}
	m_DDraw.ClearBackB4();
	DrawNewDialogBox(SPRID_INTERFACE_ND_LOGIN, 0, 0, 0, TRUE);
	if (m_cGameModeCount > 20) DrawNewDialogBox(SPRID_INTERFACE_ND_LOGIN, 40, 121, 1, TRUE);
	else if ((m_cGameModeCount >= 15) && (m_cGameModeCount <= 20)) m_pSprite[SPRID_INTERFACE_ND_LOGIN]->PutTransSprite25(40, 121, 1, TRUE);

	if (m_cGameModeCount > 20)
	{
		if (m_cCurFocus == 1) DrawNewDialogBox(SPRID_INTERFACE_ND_LOGIN, 138, 177, 5, TRUE);
		if (m_cCurFocus == 2) DrawNewDialogBox(SPRID_INTERFACE_ND_LOGIN, 130, 205, 6, TRUE);
		if (m_cCurFocus == 3) DrawNewDialogBox(SPRID_INTERFACE_ND_LOGIN, 256, 282, 4, TRUE);
	}
	DrawVersion();
	m_DInput.UpdateMouseState(&msX, &msY, &msZ, &cLB, &cRB);
	m_pSprite[SPRID_MOUSECURSOR]->PutSpriteFast(msX, msY, 0, dwTime);

	iMIbuttonNum = pMI->iGetStatus(msX, msY, cLB, &cMIresult);
	if (cMIresult == MIRESULT_CLICK) {
		PlaySound('E', 14, 5);

		switch (iMIbuttonNum) {
		case 1:
			if (m_cCurFocus == 1) {
				ZeroMemory(m_cWorldServerName, sizeof(m_cWorldServerName));
				strcpy(m_cWorldServerName, NAME_WORLDNAME1);
				ChangeGameMode(GAMEMODE_ONLOGIN);
				delete pMI;
				return;
			}
			else m_cCurFocus = 1;
			break;

		case 2:
			if (m_cCurFocus == 2) {
				ZeroMemory(m_cWorldServerName, sizeof(m_cWorldServerName));
				strcpy(m_cWorldServerName, "WS2");
				ChangeGameMode(GAMEMODE_ONLOGIN);
				delete pMI;
				return;
			}
			else m_cCurFocus = 2;
			break;

		case 3:
			ChangeGameMode(GAMEMODE_ONMAINMENU);
			delete pMI;
			return;
		}
	}

	if ((msX >= 130) && (msX <= 295) && (msY >= 175) && (msY <= 198)) m_cCurFocus = 1;
	if ((msX >= 130) && (msX <= 295) && (msY >= 199) && (msY <= 225)) m_cCurFocus = 2;
	if ((msX >= 256) && (msX <= 331) && (msY >= 279) && (msY <= 308)) m_cCurFocus = 3;

	//	if (m_cGameModeCount < 6) m_DDraw.DrawShadowBox(0,0,639,479);
	//	if (m_cGameModeCount < 2) m_DDraw.DrawShadowBox(0,0,639,479);

	if (m_DDraw.iFlip() == DDERR_SURFACELOST) RestoreSprites();
}
void cMain::UpdateScreen_OnSelectCharacter(short sX, short sY, short msX, short msY, BOOL bIgnoreFocus)
{
	int i;
	int iYear, iMonth, iDay, iHour, iMinute;
	__int64 iTemp1, iTemp2;
	char cTotalChar = 0;
	DWORD dwTime = timeGetTime();
	sY = 10;
	m_DDraw.ClearBackB4();
	DrawNewDialogBox(SPRID_INTERFACE_ND_SELECTCHAR, 0, 0, 0);
	DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, 0, 0, 50);

	iTemp1 = 0;
	iTemp2 = 0;
	iYear = iMonth = iDay = iHour = iMinute = 0;
	for (i = 0; i < 4; i++)
	{
		if ((m_cCurFocus - 1 == i) && (bIgnoreFocus == FALSE))
			m_pSprite[SPRID_INTERFACE_ND_BUTTON]->PutSpriteFast(sX + 110 + i * 109 - 7, 63 - 9, 62, dwTime);
		else m_pSprite[SPRID_INTERFACE_ND_BUTTON]->PutSpriteFast(sX + 110 + i * 109 - 7, 63 - 9, 61, dwTime);

		if (m_pCharList[i] != NULL)
		{
			cTotalChar++;
			switch (m_pCharList[i]->m_sSex) {
			case 1:	_tmp_sOwnerType = 1; break;
			case 2:	_tmp_sOwnerType = 4; break;
			}
			_tmp_sOwnerType += m_pCharList[i]->m_sSkinCol - 1;
			_tmp_cDir = m_cMenuDir;
			_tmp_sAppr1 = m_pCharList[i]->m_sAppr1;
			_tmp_sAppr2 = m_pCharList[i]->m_sAppr2;
			_tmp_sAppr3 = m_pCharList[i]->m_sAppr3;
			_tmp_sAppr4 = m_pCharList[i]->m_sAppr4;
			_tmp_iApprColor = m_pCharList[i]->m_iApprColor; // v1.4

			ZeroMemory(_tmp_cName, sizeof(_tmp_cName));
			memcpy(_tmp_cName, m_pCharList[i]->m_cName, 10);
			// CLEROTH - NO USE
			_tmp_cAction = OBJECTMOVE;
			_tmp_cFrame = m_cMenuFrame;

			if (m_pCharList[i]->m_sSex != NULL)
			{
				if (m_Misc.bCheckValidString(m_pCharList[i]->m_cName) == TRUE)
				{
					m_pEffectSpr[0]->PutTransSprite(sX + 157 + i * 109, sY + 138, 1, dwTime);
					DrawObject_OnMove_ForMenu(0, 0, sX + 157 + i * 109, sY + 138, FALSE, dwTime, 0, 0);
					PutString(sX + 112 + i * 109, sY + 179 - 9, m_pCharList[i]->m_cName, RGB(51, 0, 51));//25,35,25);
					int	_sLevel = m_pCharList[i]->m_sLevel;
					wsprintf(G_cTxt, "%d", _sLevel);
					PutString(sX + 138 + i * 109, sY + 196 - 10, G_cTxt, RGB(51, 0, 51)); //25,35,25);

					wsprintf(G_cTxt, "%d", m_pCharList[i]->m_iExp);
					PutString(sX + 138 + i * 109, sY + 211 - 10, G_cTxt, RGB(51, 0, 51)); //25,35,25);
				}
				iTemp2 = m_pCharList[i]->m_iYear * 1000000 + m_pCharList[i]->m_iMonth * 60000 + m_pCharList[i]->m_iDay * 1700 + m_pCharList[i]->m_iHour * 70 + m_pCharList[i]->m_iMinute;
				if (iTemp1 < iTemp2)
				{
					iYear = m_pCharList[i]->m_iYear;
					iMonth = m_pCharList[i]->m_iMonth;
					iDay = m_pCharList[i]->m_iDay;
					iHour = m_pCharList[i]->m_iHour;
					iMinute = m_pCharList[i]->m_iMinute;
					iTemp1 = iTemp2;
				}
			}
		}
	}
	i = 0;

	DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, 0, 0, 51);
	DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, 0, 0, 52);
	DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, 0, 0, 53);
	DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, 0, 0, 54);
	DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, 0, 0, 55);

	if ((msX > 360) && (msY >= 283) && (msX < 545) & (msY <= 315)) {
		DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, 0, 0, 56);
		PutAlignedString(98, 357, 290 + 15, UPDATE_SCREEN_ON_SELECT_CHARACTER1);//"
		PutAlignedString(98, 357, 305 + 15, UPDATE_SCREEN_ON_SELECT_CHARACTER2);//"
		PutAlignedString(98, 357, 320 + 15, UPDATE_SCREEN_ON_SELECT_CHARACTER3);//"
		PutAlignedString(98, 357, 335 + 15, UPDATE_SCREEN_ON_SELECT_CHARACTER4);//"
	}
	else
	if ((msX > 360) && (msY >= 316) && (msX < 545) & (msY <= 345)) {
		DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, 0, 0, 57);
		PutAlignedString(98, 357, 305 + 15, UPDATE_SCREEN_ON_SELECT_CHARACTER5);//"

	}
	else
	if ((msX > 360) && (msY >= 346) && (msX < 545) & (msY <= 375)) {

		DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, 0, 0, 58);
		PutAlignedString(98, 357, 275 + 15, UPDATE_SCREEN_ON_SELECT_CHARACTER6);//"
		PutAlignedString(98, 357, 290 + 15, UPDATE_SCREEN_ON_SELECT_CHARACTER7);//"
		PutAlignedString(98, 357, 305 + 15, UPDATE_SCREEN_ON_SELECT_CHARACTER8);//"
		PutAlignedString(98, 357, 320 + 15, UPDATE_SCREEN_ON_SELECT_CHARACTER9);//"
		PutAlignedString(98, 357, 335 + 15, UPDATE_SCREEN_ON_SELECT_CHARACTER10);//"
		PutAlignedString(98, 357, 350 + 15, UPDATE_SCREEN_ON_SELECT_CHARACTER11);//"

	}
	else if ((msX > 360) && (msY >= 376) && (msX < 545) & (msY <= 405))
	{
		DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, 0, 0, 59);
		PutAlignedString(98, 357, 305 + 15, UPDATE_SCREEN_ON_SELECT_CHARACTER12);//"
	}
	else if ((msX > 360) && (msY >= 406) && (msX < 545) & (msY <= 435)) {
		DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, 0, 0, 60);
		PutAlignedString(98, 357, 305 + 15, UPDATE_SCREEN_ON_SELECT_CHARACTER13);//"
	}
	else
	{
		if (cTotalChar == 0)
		{
			PutAlignedString(98, 357, 275 + 15, UPDATE_SCREEN_ON_SELECT_CHARACTER14);//"
			PutAlignedString(98, 357, 290 + 15, UPDATE_SCREEN_ON_SELECT_CHARACTER15);//"
			PutAlignedString(98, 357, 305 + 15, UPDATE_SCREEN_ON_SELECT_CHARACTER16);//"
			PutAlignedString(98, 357, 320 + 15, UPDATE_SCREEN_ON_SELECT_CHARACTER17);//"New Character
			PutAlignedString(98, 357, 335 + 15, UPDATE_SCREEN_ON_SELECT_CHARACTER18);//"
		}
		else if (cTotalChar < 4)
		{
			PutAlignedString(98, 357, 275 + 15, UPDATE_SCREEN_ON_SELECT_CHARACTER19);//"
			PutAlignedString(98, 357, 290 + 15, UPDATE_SCREEN_ON_SELECT_CHARACTER20);//"Playπ
			PutAlignedString(98, 357, 305 + 15, UPDATE_SCREEN_ON_SELECT_CHARACTER21);//"
			PutAlignedString(98, 357, 320 + 15, UPDATE_SCREEN_ON_SELECT_CHARACTER22);//"
			PutAlignedString(98, 357, 335 + 15, UPDATE_SCREEN_ON_SELECT_CHARACTER23);//"Delete Character
			PutAlignedString(98, 357, 350 + 15, UPDATE_SCREEN_ON_SELECT_CHARACTER24);//"
		}
		if (cTotalChar == 4)
		{
			PutAlignedString(98, 357, 290 + 15, UPDATE_SCREEN_ON_SELECT_CHARACTER25);//"
			PutAlignedString(98, 357, 305 + 15, UPDATE_SCREEN_ON_SELECT_CHARACTER26);//"Play
			PutAlignedString(98, 357, 320 + 15, UPDATE_SCREEN_ON_SELECT_CHARACTER27);//"Delete Character
			PutAlignedString(98, 357, 335 + 15, UPDATE_SCREEN_ON_SELECT_CHARACTER28);//"
		}
	}
	int iTempMon, iTempDay, iTempHour, iTempMin;
	iTempMon = iTempDay = iTempHour = iTempMin = 0;

	if (m_iAccntYear != 0)
	{
		iTempMin = (m_iTimeLeftSecAccount / 60);
		wsprintf(G_cTxt, UPDATE_SCREEN_ON_SELECT_CHARACTER37, m_iAccntYear, m_iAccntMonth, m_iAccntDay, iTempMin);
	}
	else
	{
		if (m_iTimeLeftSecAccount > 0)
		{
			iTempDay = (m_iTimeLeftSecAccount / (60 * 60 * 24));
			iTempHour = (m_iTimeLeftSecAccount / (60 * 60)) % 24;
			iTempMin = (m_iTimeLeftSecAccount / 60) % 60;
			wsprintf(G_cTxt, UPDATE_SCREEN_ON_SELECT_CHARACTER38, iTempDay, iTempHour, iTempMin);
		}
		else strcpy(G_cTxt, UPDATE_SCREEN_ON_SELECT_CHARACTER39);
	}
	PutAlignedString(98, 357, 385 + 10, G_cTxt);

	if (m_iIpYear != 0)
	{
		iTempHour = (m_iTimeLeftSecIP / (60 * 60));
		iTempMin = (m_iTimeLeftSecIP / 60) % 60;
		wsprintf(G_cTxt, UPDATE_SCREEN_ON_SELECT_CHARACTER40, m_iIpYear, m_iIpMonth, m_iIpDay, iTempHour, iTempMin);
	}
	else
	{
		if (m_iTimeLeftSecIP > 0)
		{
			iTempDay = (m_iTimeLeftSecIP / (60 * 60 * 24));
			iTempHour = (m_iTimeLeftSecIP / (60 * 60)) % 24;
			iTempMin = (m_iTimeLeftSecIP / 60) % 60;
			wsprintf(G_cTxt, UPDATE_SCREEN_ON_SELECT_CHARACTER41, iTempDay, iTempHour, iTempMin);
		}
		else strcpy(G_cTxt, UPDATE_SCREEN_ON_SELECT_CHARACTER42);
	}
	PutAlignedString(98, 357, 400 + 10, G_cTxt);
	if (iYear != 0)
	{
		wsprintf(G_cTxt, UPDATE_SCREEN_ON_SELECT_CHARACTER43, iYear, iMonth, iDay, iHour, iMinute);
		PutAlignedString(98, 357, 415 + 10, G_cTxt);
	}

#ifdef _DEBUG
	PutAlignedString(122, 315, 456, UPDATE_SCREEN_ON_SELECT_CHARACTER36);//"Test Server"
#else
	if (strcmp(m_cWorldServerName, NAME_WORLDNAME1) == 0)
		PutAlignedString(129, 321, 456, MSG_WORLDNAME1);//"ABADDON Server"
	else if (strcmp(m_cWorldServerName, "WS2") == 0)
		PutAlignedString(129, 321, 456, MSG_WORLDNAME2);//"APOCALYPSE Server"
	else if (strcmp(m_cWorldServerName, "WS3") == 0)
		PutAlignedString(129, 321, 456, MSG_WORLDNAME3);//"3rd Server"
	else if (strcmp(m_cWorldServerName, "WS4") == 0)
		PutAlignedString(129, 321, 456, MSG_WORLDNAME4);//"4th Server"
	else if (strcmp(m_cWorldServerName, "WS5") == 0)
		PutAlignedString(129, 321, 456, MSG_WORLDNAME5);//"5th Server"
	else if (strcmp(m_cWorldServerName, "WS6") == 0)
		PutAlignedString(129, 321, 456, MSG_WORLDNAME6);//"6th Server"
	else if (strcmp(m_cWorldServerName, "WS7") == 0)
		PutAlignedString(129, 321, 456, MSG_WORLDNAME7);//"7th Server"
	else if (strcmp(m_cWorldServerName, "WS8") == 0)
		PutAlignedString(129, 321, 456, MSG_WORLDNAME8);//"8th Server"
	else if (strcmp(m_cWorldServerName, "WS9") == 0)
		PutAlignedString(129, 321, 456, MSG_WORLDNAME9);//"9th Server"
	else if (strcmp(m_cWorldServerName, "WS10") == 0)
		PutAlignedString(129, 321, 456, MSG_WORLDNAME10);//"10th Server"
	else if (strcmp(m_cWorldServerName, "WS11") == 0)
		PutAlignedString(129, 321, 456, MSG_WORLDNAME11);//"11th Server"
	else if (strcmp(m_cWorldServerName, "WS12") == 0)
		PutAlignedString(129, 321, 456, MSG_WORLDNAME12);//"12th Server"
	else if (strcmp(m_cWorldServerName, "WS13") == 0)
		PutAlignedString(129, 321, 456, MSG_WORLDNAME13);//"13th Server"
	else if (strcmp(m_cWorldServerName, "WS14") == 0)
		PutAlignedString(129, 321, 456, MSG_WORLDNAME14);//"14th Server"
	else if (strcmp(m_cWorldServerName, "WS15") == 0)
		PutAlignedString(129, 321, 456, MSG_WORLDNAME15);//"15th Server"
	else if (strcmp(m_cWorldServerName, "WS16") == 0)
		PutAlignedString(129, 321, 456, MSG_WORLDNAME16);//"16th Server"
#endif



}

void cMain::UpdateScreen_OnWaitingResponse()
{
	short sX, sY, msX, msY, msZ;
	char cLB, cRB;

	DWORD dwTime = timeGetTime();
	//static class CMouseInterface * pMI;
	static DWORD dwCTime;

	if (m_cGameModeCount == 0)
	{
		m_bEnterPressed = FALSE;
		m_bEscPressed = FALSE;
		dwCTime = timeGetTime();
	}
	m_cGameModeCount++;
	if (m_cGameModeCount > 100) m_cGameModeCount = 100;

	if (m_bEscPressed == TRUE)
	{
		if ((dwTime - m_dwTime) > 7000)
		{
			ChangeGameMode(GAMEMODE_ONMAINMENU);
			//if (m_pLSock != NULL)
			//{	//delete m_pLSock;
			//	//m_pLSock = NULL;
			//}
			//if (m_pGSock != NULL)
			//{	//delete m_pGSock;
			//m_pGSock = NULL;
			//}	
		}
		m_bEscPressed = FALSE;
		return;
	}

	if ((dwTime - dwCTime) > 100)
	{
		m_cMenuFrame++;
		dwCTime = dwTime;
	}
	if (m_cMenuFrame >= 8)
	{
		m_cMenuDirCnt++;
		if (m_cMenuDirCnt > 8)
		{
			m_cMenuDir++;
			m_cMenuDirCnt = 1;
		}
		m_cMenuFrame = 0;
	}
	if (m_cMenuDir > 8) m_cMenuDir = 1;

	m_bIsHideLocalCursor = TRUE;
	m_DDraw.ClearBackB4();
	switch (m_cMsg[0]) {
	case '0':
		//_Draw_UpdateScreen_OnCreateNewAccount();
		break;
	case '1':
		sX = 146;
		sY = 114;
		//_Draw_OnLogin(m_cAccountName, m_cAccountPassword, 0, 0);
		break;
	case '2':
		//_bDraw_OnCreateNewCharacter(m_cPlayerName, 0, 0, 0);
		break;
	case '3':
		UpdateScreen_OnSelectCharacter(0, 0, 0, 0);
		break;
	case '4':// Change Password
		UpdateScreen_OnSelectCharacter(0, 0, 0, 0, TRUE);
		break;
	case '5':
		m_DDraw.ClearBackB4();
		break;
	}
	m_bIsHideLocalCursor = FALSE;

	m_DDraw.DrawShadowBox(0, 0, 639, 479);
	DrawNewDialogBox(SPRID_INTERFACE_ND_GAME4, 162, 125, 2);
	PutString_SprFont(172 + 44 - 17, 190, "Connected. Waiting for response...", 7, 0, 0);

	if ((dwTime - m_dwTime) > 7000)
	{
		PutAlignedString(180, 463, 195 + 30, UPDATE_SCREEN_ON_WATING_RESPONSE1);
		PutAlignedString(180, 463, 195 + 45, UPDATE_SCREEN_ON_WATING_RESPONSE2);
	}
	else PutAlignedString(180, 463, 195 + 30, UPDATE_SCREEN_ON_WATING_RESPONSE3);

	DrawVersion();
	m_DInput.UpdateMouseState(&msX, &msY, &msZ, &cLB, &cRB);
	m_pSprite[SPRID_MOUSECURSOR]->PutSpriteFast(msX, msY, 8, dwTime);

	if (m_DDraw.iFlip() == DDERR_SURFACELOST) RestoreSprites();
}

void cMain::UpdateScreen_OnQueryDeleteCharacter()
{
	short msX, msY, msZ;
	char cLB, cRB;
	char cMIresult;
	int  iMIbuttonNum;

	static class CMouseInterface * pMI;
	static DWORD dwCTime;
	DWORD dwTime = timeGetTime();

	if (m_cGameModeCount == 0)
	{
		pMI = new class CMouseInterface;
		pMI->AddRect(200, 244, 200 + BTNSZX, 244 + BTNSZY);
		pMI->AddRect(370, 244, 370 + BTNSZX, 244 + BTNSZY);
		//m_cCurFocus = 1;
		//m_cMaxFocus = 2;
		m_bEnterPressed = FALSE;
		m_cArrowPressed = 0;

		dwCTime = timeGetTime();

		PlaySound('E', 25, 0);
	}
	m_cGameModeCount++;
	if (m_cGameModeCount > 100) m_cGameModeCount = 100;

	if (m_bEscPressed == TRUE)
	{
		ChangeGameMode(GAMEMODE_ONSELECTCHARACTER);
		delete pMI;
		m_bEscPressed = FALSE;
		return;
	}

	m_DInput.UpdateMouseState(&msX, &msY, &msZ, &cLB, &cRB);
	UpdateScreen_OnSelectCharacter(0, 0, 500, 70);
	if ((m_cGameModeCount >= 0) && (m_cGameModeCount < 6))
	{
		m_DDraw.DrawShadowBox(0, 0, 639, 479);
	}
	else if (m_cGameModeCount >= 6)
	{
		m_DDraw.DrawShadowBox(0, 0, 639, 479);
		m_DDraw.DrawShadowBox(0, 0, 639, 479);
	}

	DrawNewDialogBox(SPRID_INTERFACE_ND_GAME4, 162, 125, 2);

	PutString_SprFont(172 + 86, 160, "Delete Character", 7, 0, 0);
	PutString(215, 195, UPDATE_SCREEN_ON_QUERY_DELETE_CHARACTER1, RGB(5, 5, 5));//"Character Name"
	//"Character Name"
	PutString(335, 199, "__________", RGB(5, 5, 5));
	PutString(335, 195, m_pCharList[m_wEnterGameType - 1]->m_cName, RGB(25, 35, 25));
	PutAlignedString(178, 453, 220, UPDATE_SCREEN_ON_QUERY_DELETE_CHARACTER2);//"Do you want to delete the character above?"

	// v2.05
	if ((msX >= 200) && (msX <= 200 + BTNSZX) && (msY >= 244) && (msY <= 244 + BTNSZY))
		DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, 200, 244, 19);
	else DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, 200, 244, 18);

	if ((msX >= 370) && (msX <= 370 + BTNSZX) && (msY >= 244) && (msY <= 244 + BTNSZY))
		DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, 370, 244, 3);
	else DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, 370, 244, 2);

	if ((dwTime - dwCTime) > 100)
	{
		m_cMenuFrame++;
		dwCTime = dwTime;
	}
	if (m_cMenuFrame >= 8)
	{
		m_cMenuDirCnt++;
		if (m_cMenuDirCnt > 8)
		{
			m_cMenuDir++;
			m_cMenuDirCnt = 1;
		}
		m_cMenuFrame = 0;
	}
	if (m_cMenuDir > 8) m_cMenuDir = 1;

	iMIbuttonNum = pMI->iGetStatus(msX, msY, cLB, &cMIresult);
	if (cMIresult == MIRESULT_CLICK)
	{
		PlaySound('E', 14, 5);
		switch (iMIbuttonNum) {
		case 1:
			//m_pLSock = new class XSocket(m_hWnd, SOCKETBLOCKLIMIT);
			//m_pLSock->bConnect(m_cLogServerAddr, m_iLogServerPort, WM_USER_LOGSOCKETEVENT);
			//m_pLSock->bInitBufferSize(30000);
			ChangeGameMode(GAMEMODE_ONCONNECTING);
			//m_dwConnectMode  = MSGID_REQUEST_DELETECHARACTER;
			ZeroMemory(m_cMsg, sizeof(m_cMsg));
			strcpy(m_cMsg, "33");
			delete pMI;
			return;

		case 2:
			ChangeGameMode(GAMEMODE_ONSELECTCHARACTER);
			delete pMI;
			break;
		}
	}
	DrawVersion();
	m_pSprite[SPRID_MOUSECURSOR]->PutSpriteFast(msX, msY, 0, dwTime);
	if (m_DDraw.iFlip() == DDERR_SURFACELOST) RestoreSprites();
}
void cMain::UpdateScreen_OnCreateNewCharacter()
{
	static class CMouseInterface * pMI;
	int i = 0;
	int iMIbuttonNum;
	static int iPoint;
	char cLB, cRB, cMIresult;
	static char cName[12];
	static char cPrevFocus;
	short msX, msY, msZ;
	BOOL bFlag;
	static DWORD dwMTime;
	DWORD dwTime = timeGetTime();

	if (m_cGameModeCount == 0)
	{
		pMI = new class CMouseInterface;
		pMI->AddRect(65 + 4, 65 + 45, 275 + 4, 82 + 45);

		pMI->AddRect(232 + 4, 111 + 45, 274 + 4 - 21, 124 + 45);
		pMI->AddRect(255 + 4, 111 + 45, 289 + 4 - 13, 124 + 45);

		pMI->AddRect(232 + 4, 126 + 45, 274 + 4 - 21, 139 + 45);
		pMI->AddRect(255 + 4, 126 + 45, 289 + 4 - 13, 139 + 45);

		pMI->AddRect(232 + 4, 141 + 45, 274 + 4 - 21, 154 + 45);
		pMI->AddRect(255 + 4, 141 + 45, 289 + 4 - 13, 154 + 45);

		pMI->AddRect(232 + 4, 156 + 45, 274 + 4 - 21, 169 + 45);
		pMI->AddRect(255 + 4, 156 + 45, 289 + 4 - 13, 169 + 45);

		pMI->AddRect(232 + 4, 171 + 45, 274 + 4 - 21, 184 + 45);
		pMI->AddRect(255 + 4, 171 + 45, 289 + 4 - 13, 184 + 45);


		pMI->AddRect(232 + 4, 231 + 45, 253 + 4, 244 + 45);
		pMI->AddRect(255 + 4, 231 + 45, 276 + 4, 244 + 45);

		pMI->AddRect(232 + 4, 246 + 45, 253 + 4, 259 + 45);
		pMI->AddRect(255 + 4, 246 + 45, 276 + 4, 259 + 45);

		pMI->AddRect(232 + 4, 261 + 45, 253 + 4, 274 + 45);
		pMI->AddRect(255 + 4, 261 + 45, 276 + 4, 274 + 45);

		pMI->AddRect(232 + 4, 276 + 45, 253 + 4, 289 + 45);
		pMI->AddRect(255 + 4, 276 + 45, 276 + 4, 289 + 45);

		pMI->AddRect(232 + 4, 291 + 45, 253 + 4, 304 + 45);
		pMI->AddRect(255 + 4, 291 + 45, 276 + 4, 304 + 45);

		pMI->AddRect(232 + 4, 306 + 45, 253 + 4, 319 + 45);
		pMI->AddRect(255 + 4, 306 + 45, 276 + 4, 319 + 45);

		pMI->AddRect(384, 445, 384 + 72, 445 + 15);
		pMI->AddRect(500, 445, 500 + 72, 445 + 15);

		pMI->AddRect(60, 445, 60 + 72, 445 + 15);
		pMI->AddRect(145, 445, 145 + 72, 445 + 15);
		pMI->AddRect(230, 445, 230 + 72, 445 + 15);

		iPoint = m_ccStr + m_ccVit + m_ccDex + m_ccInt + m_ccMag + m_ccChr;
		iPoint = 70 - iPoint;
		cPrevFocus = 1;
		m_cCurFocus = 1;
		m_cMaxFocus = 6;
		m_bEnterPressed = FALSE;
		m_cArrowPressed = 0;
		dwMTime = timeGetTime();
		StartInputString(193 + 4, 65 + 45, 11, cName);
		ClearInputString();
	}
	m_cGameModeCount++;
	if (m_cGameModeCount > 100) m_cGameModeCount = 100;

	if (m_cArrowPressed != 0)
	{
		switch (m_cArrowPressed) {
		case 1:
			m_cCurFocus--;
			if (m_cCurFocus <= 0) m_cCurFocus = m_cMaxFocus;
			break;

		case 3:
			m_cCurFocus++;
			if (m_cCurFocus > m_cMaxFocus) m_cCurFocus = 1;
			break;
		}
		m_cArrowPressed = 0;
	}

	if (cPrevFocus != m_cCurFocus) {
		EndInputString();
		switch (m_cCurFocus) {
		case 1:
			StartInputString(193 + 4, 65 + 45, 11, cName);
			break;
		}
		cPrevFocus = m_cCurFocus;
	}

	if (m_bEscPressed == TRUE) {
		ChangeGameMode(GAMEMODE_ONSELECTCHARACTER);
		delete pMI;
		m_bEscPressed = FALSE;
		return;
	}

	m_DInput.UpdateMouseState(&msX, &msY, &msZ, &cLB, &cRB);
	bFlag = _bDraw_OnCreateNewCharacter(cName, msX, msY, iPoint);

	if ((dwTime - dwMTime) > 100)
	{
		m_cMenuFrame++;
		dwMTime = dwTime;
	}
	if (m_cMenuFrame >= 8)
	{
		m_cMenuDirCnt++;
		if (m_cMenuDirCnt > 8)
		{
			m_cMenuDir++;
			m_cMenuDirCnt = 1;
		}
		m_cMenuFrame = 0;
	}
	if (m_cMenuDir > 8) m_cMenuDir = 1;

	DrawVersion();
	m_pSprite[SPRID_MOUSECURSOR]->PutSpriteFast(msX, msY, 0, dwTime);
	iMIbuttonNum = pMI->iGetStatus(msX, msY, cLB, &cMIresult);
	if (cMIresult == MIRESULT_CLICK)
	{
		PlaySound('E', 14, 5);
		switch (iMIbuttonNum) {
		case 1:
			m_cCurFocus = 1;
			break;
		case 2:
			m_cGender--;
			if (m_cGender < 1) m_cGender = 2;
			break;
		case 3:
			m_cGender++;
			if (m_cGender > 2) m_cGender = 1;
			break;
		case 4:
			m_cSkinCol--;
			if (m_cSkinCol < 1) m_cSkinCol = 3;
			break;
		case 5:
			m_cSkinCol++;
			if (m_cSkinCol > 3) m_cSkinCol = 1;
			break;
		case 6:
			m_cHairStyle--;
			if (m_cHairStyle < 0) m_cHairStyle = 7;
			break;
		case 7:
			m_cHairStyle++;
			if (m_cHairStyle > 7) m_cHairStyle = 0;
			break;
		case 8:
			m_cHairCol--;
			if (m_cHairCol < 0) m_cHairCol = 15;
			break;
		case 9:
			m_cHairCol++;
			if (m_cHairCol > 15) m_cHairCol = 0;
			break;
		case 10:
			m_cUnderCol--;
			if (m_cUnderCol < 0) m_cUnderCol = 7;
			break;
		case 11:
			m_cUnderCol++;
			if (m_cUnderCol > 7) m_cUnderCol = 0;
			break;
		case 12:
			if (iPoint > 0) {
				if (m_ccStr < 14) {
					m_ccStr++;
					iPoint--;
				}
			}
			break;
		case 13:
			if (m_ccStr > 10) {
				m_ccStr--;
				iPoint++;
			}
			break;
		case 14:
			if (iPoint > 0) {
				if (m_ccVit < 14) {
					m_ccVit++;
					iPoint--;
				}
			}
			break;
		case 15:
			if (m_ccVit > 10) {
				m_ccVit--;
				iPoint++;
			}
			break;
		case 16:
			if (iPoint > 0) {
				if (m_ccDex < 14) {
					m_ccDex++;
					iPoint--;
				}
			}
			break;
		case 17:
			if (m_ccDex > 10) {
				m_ccDex--;
				iPoint++;
			}
			break;
		case 18:
			if (iPoint > 0) {
				if (m_ccInt < 14) {
					m_ccInt++;
					iPoint--;
				}
			}
			break;
		case 19:
			if (m_ccInt > 10) {
				m_ccInt--;
				iPoint++;
			}
			break;
		case 20:
			if (iPoint > 0) {
				if (m_ccMag < 14) {
					m_ccMag++;
					iPoint--;
				}
			}
			break;
		case 21:
			if (m_ccMag > 10) {
				m_ccMag--;
				iPoint++;
			}
			break;
		case 22:
			if (iPoint > 0) {
				if (m_ccChr < 14) {
					m_ccChr++;
					iPoint--;
				}
			}
			break;
		case 23:
			if (m_ccChr > 10)
			{
				m_ccChr--;
				iPoint++;
			}
			break;

		case 24:
			if (m_cCurFocus != 2)
			{
				m_cCurFocus = 2;
				return;
			}
			if (bFlag == FALSE) return;
			//if (m_Misc.bCheckValidName(m_cPlayerName) == FALSE) break;
			if (m_Misc.bCheckValidName(cName) == FALSE) break;
			ZeroMemory(m_cPlayerName, sizeof(m_cPlayerName));
			strcpy(m_cPlayerName, cName);
			//m_pLSock = new class XSocket(m_hWnd, SOCKETBLOCKLIMIT);
			//m_pLSock->bConnect(m_cLogServerAddr, m_iLogServerPort, WM_USER_LOGSOCKETEVENT);
			//m_pLSock->bInitBufferSize(30000);
			ChangeGameMode(GAMEMODE_ONCONNECTING);
			//m_dwConnectMode = MSGID_REQUEST_CREATENEWCHARACTER;
			ZeroMemory(m_cMsg, sizeof(m_cMsg));
			strcpy(m_cMsg, "22");
			delete pMI;
			return;


		case 25:
			if (m_cCurFocus != 3)
			{
				m_cCurFocus = 3;
				return;
			}
			ChangeGameMode(GAMEMODE_ONSELECTCHARACTER);
			delete pMI;
			return;


		case 26: // WARRIOR
			if (m_cCurFocus != 4)
			{
				m_cCurFocus = 4;
				return;
			}

			m_ccMag = 10;
			m_ccInt = 10;
			m_ccChr = 10;
			m_ccStr = 14;
			m_ccVit = 12;
			m_ccDex = 14;
			iPoint = m_ccStr + m_ccVit + m_ccDex + m_ccInt + m_ccMag + m_ccChr;
			iPoint = 70 - iPoint;
			break;

		case 27: // MAGE
			if (m_cCurFocus != 5) {
				m_cCurFocus = 5;
				return;
			}

			m_ccMag = 14;
			m_ccInt = 14;
			m_ccChr = 10;
			m_ccStr = 10;
			m_ccVit = 12;
			m_ccDex = 10;
			iPoint = m_ccStr + m_ccVit + m_ccDex + m_ccInt + m_ccMag + m_ccChr;
			iPoint = 70 - iPoint;
			break;

		case 28: // PRIEST
			if (m_cCurFocus != 6) {
				m_cCurFocus = 6;
				return;
			}

			m_ccMag = 12;
			m_ccInt = 10;
			m_ccChr = 14;
			m_ccStr = 14;
			m_ccVit = 10;
			m_ccDex = 10;
			iPoint = m_ccStr + m_ccVit + m_ccDex + m_ccInt + m_ccMag + m_ccChr;
			iPoint = 70 - iPoint;
			break;
		}


	}

	if ((msX >= 65 + 4 - 127) && (msX <= 275 + 4) && (msY >= 65 + 45) && (msY <= 82 + 45)) {
		PutAlignedString(370, 580, 345, UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER1);
	}
	else
	if ((msX >= 261 + 4 - 212) && (msX <= 289 + 4) && (msY >= 111 + 45) && (msY <= 124 + 45)) {
		PutAlignedString(370, 580, 345, UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER2);
	}
	else
	if ((msX >= 261 + 4 - 212) && (msX <= 289 + 4) && (msY >= 126 + 45) && (msY <= 139 + 45)) {
		PutAlignedString(370, 580, 345, UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER3);
	}
	else
	if ((msX >= 261 + 4 - 212) && (msX <= 289 + 4) && (msY >= 141 + 45) && (msY <= 154 + 45)) {
		PutAlignedString(370, 580, 345, UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER4);
	}
	else
	if ((msX >= 261 + 4 - 212) && (msX <= 289 + 4) && (msY >= 156 + 45) && (msY <= 169 + 45)) {
		PutAlignedString(370, 580, 345, UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER5);
	}
	else
	if ((msX >= 261 + 4 - 212) && (msX <= 289 + 4) && (msY >= 171 + 45) && (msY <= 184 + 45)) {
		PutAlignedString(370, 580, 345, UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER6);
	}
	else
	if ((msX >= 240 + 4 - 175) && (msX <= 268 + 4) && (msY >= 231 + 45) && (msY <= 244 + 45)) {
		// Str
		i = 0;
		PutAlignedString(370, 580, 345 + 16 * i++, UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER7);
		PutAlignedString(370, 580, 345 + 16 * i++, UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER8);
		PutAlignedString(370, 580, 345 + 16 * i++, UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER9);
		PutAlignedString(370, 580, 345 + 16 * i++, UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER10);
		PutAlignedString(370, 580, 345 + 16 * i++, UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER11);
	}
	else if ((msX >= 240 + 4 - 175) && (msX <= 268 + 4) && (msY >= 246 + 45) && (msY <= 259 + 45)) {
		// Vit
		i = 0;
		PutAlignedString(370, 580, 345 + 16 * i++, UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER12);//"
		PutAlignedString(370, 580, 345 + 16 * i++, UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER13);//"
		PutAlignedString(370, 580, 345 + 16 * i++, UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER14);//"
		PutAlignedString(370, 580, 345 + 16 * i++, UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER15);//"
		PutAlignedString(370, 580, 345 + 16 * i++, UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER16);//"
	}
	else if ((msX >= 240 + 4 - 175) && (msX <= 268 + 4) && (msY >= 261 + 45) && (msY <= 274 + 45)) {
		// Dex
		i = 0;
		PutAlignedString(370, 580, 345 + 16 * i++, UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER17);//"
		PutAlignedString(370, 580, 345 + 16 * i++, UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER18);//"
		PutAlignedString(370, 580, 345 + 16 * i++, UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER19);//"
		PutAlignedString(370, 580, 345 + 16 * i++, UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER20);//"
	}
	else if ((msX >= 240 + 4 - 175) && (msX <= 268 + 4) && (msY >= 276 + 45) && (msY <= 289 + 45)) {
		// Int
		i = 0;
		PutAlignedString(370, 580, 345 + 16 * i++, UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER21);//"
		PutAlignedString(370, 580, 345 + 16 * i++, UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER22);//"
		PutAlignedString(370, 580, 345 + 16 * i++, UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER23);//"
		PutAlignedString(370, 580, 345 + 16 * i++, UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER24);
	}
	else if ((msX >= 240 + 4 - 175) && (msX <= 268 + 4) && (msY >= 291 + 45) && (msY <= 304 + 45)) {
		// Mag
		i = 0;
		PutAlignedString(370, 580, 345 + 16 * i++, UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER25);
		PutAlignedString(370, 580, 345 + 16 * i++, UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER26);
		PutAlignedString(370, 580, 345 + 16 * i++, UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER27);
		PutAlignedString(370, 580, 345 + 16 * i++, UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER28);
	}
	else if ((msX >= 240 + 4 - 175) && (msX <= 268 + 4) && (msY >= 306 + 45) && (msY <= 319 + 45)) {
		// Charisma
		i = 0;
		PutAlignedString(370, 580, 345 + 16 * i++, UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER29);
		PutAlignedString(370, 580, 345 + 16 * i++, UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER30);
		PutAlignedString(370, 580, 345 + 16 * i++, UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER31);
		PutAlignedString(370, 580, 345 + 16 * i++, UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER32);
	}
	else if ((msX >= 384) && (msX <= 384 + 72) && (msY >= 445) && (msY <= 445 + 15))  {
		m_cCurFocus = 2;
		if (strlen(cName) <= 0)
		{
			i = 0;
			PutAlignedString(370, 580, 345 + 16 * i++, UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER35);//"Please enter a character name."
		}
		else if (iPoint > 0)
		{
			i = 0;
			PutAlignedString(370, 580, 345 + 16 * i++, UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER36);//"You need to select your character class."
		}
		else if (m_Misc.bCheckValidName(cName) == FALSE)
		{
			i = 0;
			PutAlignedString(370, 580, 345 + 16 * i++, UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER39);//"Cannot use special characters "
			PutAlignedString(370, 580, 345 + 16 * i++, UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER40);//"in your character's name. Please"
			PutAlignedString(370, 580, 345 + 16 * i++, UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER41);//"type another name."
		}
		else if (_bCheckBadWords(cName) == TRUE)
		{
			i = 0;
			PutAlignedString(370, 580, 345 + 16 * i++, UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER42);//"The character's name is not appropriate."
			PutAlignedString(370, 580, 345 + 16 * i++, UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER43);//"Please retry with another name."
		}
		else
		{
			i = 0;
			PutAlignedString(370, 580, 345 + 16 * i++, UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER44);//"
			PutAlignedString(370, 580, 345 + 16 * i++, UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER45);//"
			PutAlignedString(370, 580, 345 + 16 * i++, UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER46);//"
			PutAlignedString(370, 580, 345 + 16 * i++, UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER47);//"
			PutAlignedString(370, 580, 345 + 16 * i++, UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER48);//"
		}
	}
	else if ((msX >= 500) && (msX <= 500 + 72) && (msY >= 445) && (msY <= 445 + 15))
	{
		m_cCurFocus = 3;
		PutAlignedString(370, 580, 345, UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER49);
	}

	if ((msX >= 60) && (msX <= 60 + 72) && (msY >= 445) && (msY <= 445 + 15)) {
		m_cCurFocus = 4;
		PutAlignedString(370, 580, 345, UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER50);
	}

	if ((msX >= 145) && (msX <= 145 + 72) && (msY >= 445) && (msY <= 445 + 15)) {
		m_cCurFocus = 5;
		PutAlignedString(370, 580, 345, UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER51);
	}

	if ((msX >= 230) && (msX <= 230 + 72) && (msY >= 445) && (msY <= 445 + 15)) {
		m_cCurFocus = 6;
		PutAlignedString(370, 580, 345, UPDATE_SCREEN_ON_CREATE_NEW_CHARACTER52);
	}

	//	if (m_cGameModeCount < 6) m_DDraw.DrawShadowBox(0,0,639,479);
	//	if (m_cGameModeCount < 2) m_DDraw.DrawShadowBox(0,0,639,479);

	if (m_DDraw.iFlip() == DDERR_SURFACELOST) RestoreSprites();
}
BOOL cMain::_bDraw_OnCreateNewCharacter(char * pName, short msX, short msY, int iPoint)
{
	BOOL bFlag = TRUE;
	DWORD dwTime = timeGetTime();
	int i = 0;

	m_DDraw.ClearBackB4();

	DrawNewDialogBox(SPRID_INTERFACE_ND_NEWCHAR, 0, 0, 0, TRUE);
	DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, 0, 0, 69, TRUE);
	PutAlignedString(64, 282, 90, _BDRAW_ON_CREATE_NEW_CHARACTER1, 5, 5, 5);//"
	PutAlignedString(57, 191, 110, MSG_CHARACTERNAME, 5, 5, 5);//"Character Name"
	if (m_cCurFocus != 1) PutString(197, 112, pName, RGB(25, 35, 25));
	PutAlignedString(64, 282, 140, _BDRAW_ON_CREATE_NEW_CHARACTER2, 5, 5, 5);//"
	PutString(100, 160, MSG_GENDER, RGB(5, 5, 5));//"Gender"
	PutString(100, 175, MSG_SKINCOLOR, RGB(5, 5, 5));//"Skin Color"
	PutString(100, 190, MSG_HAIRSTYLE, RGB(5, 5, 5));//"Hair Style"
	PutString(100, 205, MSG_HAIRCOLOR, RGB(5, 5, 5));//"Hair Color"
	PutString(100, 220, MSG_UNDERWEARCOLOR, RGB(5, 5, 5));//"Underwear Color"
	//PutAlignedString(64, 282, 245, _BDRAW_ON_CREATE_NEW_CHARACTER3, 5,5,5);
	//wsprintf(G_cTxt, _BDRAW_ON_CREATE_NEW_CHARACTER4,  iPoint);//" %d points"
	//PutAlignedString(64, 282, 260, G_cTxt, 15,10,10);
	PutString(100, 275, MSG_STRENGTH, RGB(5, 5, 5));//"Strength"
	PutString(100, 292, MSG_VITALITY, RGB(5, 5, 5));//"Vitality"
	PutString(100, 309, MSG_DEXTERITY, RGB(5, 5, 5));//"Dexterity"
	PutString(100, 326, MSG_INTELLIGENCE, RGB(5, 5, 5));//"Intelligence"
	PutString(100, 343, MSG_MAGIC, RGB(5, 5, 5));//"Magic"
	PutString(100, 360, MSG_CHARISMA, RGB(5, 5, 5));//"Charisma"

	wsprintf(G_cTxt, "%d", m_ccStr);
	PutString(204, 277 + 16 * i++, G_cTxt, RGB(25, 35, 25));
	wsprintf(G_cTxt, "%d", m_ccVit);
	PutString(204, 277 + 16 * i++, G_cTxt, RGB(25, 35, 25));
	wsprintf(G_cTxt, "%d", m_ccDex);
	PutString(204, 277 + 16 * i++, G_cTxt, RGB(25, 35, 25));
	wsprintf(G_cTxt, "%d", m_ccInt);
	PutString(204, 277 + 16 * i++, G_cTxt, RGB(25, 35, 25));
	wsprintf(G_cTxt, "%d", m_ccMag);
	PutString(204, 277 + 16 * i++, G_cTxt, RGB(25, 35, 25));
	wsprintf(G_cTxt, "%d", m_ccChr);
	PutString(204, 277 + 16 * i++, G_cTxt, RGB(25, 35, 25));

	if (strlen(pName) <= 0) bFlag = FALSE;
	if (iPoint > 0) bFlag = FALSE;
	if (m_Misc.bCheckValidName(pName) == FALSE) bFlag = FALSE;
	if (_bCheckBadWords(pName) == TRUE) bFlag = FALSE;

	if ((bFlag == TRUE) && (m_cCurFocus == 2)) m_pSprite[SPRID_INTERFACE_ND_BUTTON]->PutSpriteFast(384, 445, 25, dwTime);
	else m_pSprite[SPRID_INTERFACE_ND_BUTTON]->PutSpriteFast(384, 445, 24, dwTime);
	if (m_cCurFocus == 3)
		m_pSprite[SPRID_INTERFACE_ND_BUTTON]->PutSpriteFast(500, 445, 17, dwTime);
	else m_pSprite[SPRID_INTERFACE_ND_BUTTON]->PutSpriteFast(500, 445, 16, dwTime);
	if (m_cCurFocus == 4)
		m_pSprite[SPRID_INTERFACE_ND_BUTTON]->PutSpriteFast(60, 445, 68, dwTime);
	else m_pSprite[SPRID_INTERFACE_ND_BUTTON]->PutSpriteFast(60, 445, 67, dwTime);
	if (m_cCurFocus == 5)
		m_pSprite[SPRID_INTERFACE_ND_BUTTON]->PutSpriteFast(145, 445, 66, dwTime);
	else m_pSprite[SPRID_INTERFACE_ND_BUTTON]->PutSpriteFast(145, 445, 65, dwTime);
	if (m_cCurFocus == 6)
		m_pSprite[SPRID_INTERFACE_ND_BUTTON]->PutSpriteFast(230, 445, 64, dwTime);
	else m_pSprite[SPRID_INTERFACE_ND_BUTTON]->PutSpriteFast(230, 445, 63, dwTime);
	/*if (m_cCurFocus == 4)
	m_pSprite[SPRID_INTERFACE_ND_BUTTON]->PutSpriteFast(60, 245, 68, dwTime);
	else m_pSprite[SPRID_INTERFACE_ND_BUTTON]->PutSpriteFast(60, 245, 67, dwTime);
	if (m_cCurFocus == 5)
	m_pSprite[SPRID_INTERFACE_ND_BUTTON]->PutSpriteFast(145, 245, 66, dwTime);
	else m_pSprite[SPRID_INTERFACE_ND_BUTTON]->PutSpriteFast(145, 245, 65, dwTime);
	if (m_cCurFocus == 6)
	m_pSprite[SPRID_INTERFACE_ND_BUTTON]->PutSpriteFast(230, 245, 64, dwTime);
	else m_pSprite[SPRID_INTERFACE_ND_BUTTON]->PutSpriteFast(230, 245, 63, dwTime);*/

	ShowReceivedString();

	switch (m_cGender) {
	case 1:	_tmp_sOwnerType = 1; break;
	case 2:	_tmp_sOwnerType = 4; break; //@@@@@@@@@@@@@@@@@!!!!!!!!!!!!!!!!!
	}
	_tmp_sOwnerType += m_cSkinCol - 1;
	_tmp_cDir = m_cMenuDir;
	_tmp_sAppr1 = 0;
	_tmp_sAppr1 = _tmp_sAppr1 | (m_cUnderCol);
	_tmp_sAppr1 = _tmp_sAppr1 | (m_cHairStyle << 8);
	_tmp_sAppr1 = _tmp_sAppr1 | (m_cHairCol << 4);
	_tmp_sAppr2 = 0;
	_tmp_sAppr3 = 0;
	_tmp_sAppr4 = 0;
	ZeroMemory(_tmp_cName, sizeof(_tmp_cName));
	memcpy(_tmp_cName, m_cPlayerName, 10);
	_tmp_cAction = OBJECTMOVE;
	_tmp_cFrame = m_cMenuFrame;

	_Draw_CharacterBody(507, 267, _tmp_sOwnerType);

	DrawObject_OnMove_ForMenu(0, 0, 500, 174, FALSE, dwTime, msX, msY);

	i = 0;

	PutString(445, 192, MSG_HITPOINT, RGB(5, 5, 5));//"Hit Point"
	wsprintf(G_cTxt, "%d", m_ccVit * 3 + 2 + m_ccStr / 2);
	PutString(550, 192 + 16 * i++, G_cTxt, RGB(25, 35, 25));

	PutString(445, 208, MSG_MANAPOINT, RGB(5, 5, 5));//"Mana Point"
	wsprintf(G_cTxt, "%d", m_ccMag * 2 + 2 + m_ccInt / 2);
	PutString(550, 192 + 16 * i++, G_cTxt, RGB(25, 35, 25));

	PutString(445, 224, MSG_STAMINARPOINT, RGB(5, 5, 5));//"Staminar Point"
	wsprintf(G_cTxt, "%d", m_ccStr * 2 + 2);
	PutString(550, 192 + 16 * i++, G_cTxt, RGB(25, 35, 25));

	return bFlag;
}
void cMain::_Draw_CharacterBody(short sX, short sY, short sType)
{
	DWORD dwTime = m_dwCurTime;
	int  iR, iG, iB;

	if (sType <= 3)
	{
		m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + 0]->PutSpriteFast(sX, sY, sType - 1, dwTime);
		_GetHairColorRGB(((_tmp_sAppr1 & 0x00F0) >> 4), &iR, &iG, &iB);
		m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + 18]->PutSpriteRGB(sX, sY, (_tmp_sAppr1 & 0x0F00) >> 8, iR, iG, iB, dwTime);

		m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + 19]->PutSpriteFast(sX, sY, (_tmp_sAppr1 & 0x000F), dwTime);
	}
	else
	{
		m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + 40]->PutSpriteFast(sX, sY, sType - 4, dwTime);
		_GetHairColorRGB(((_tmp_sAppr1 & 0x00F0) >> 4), &iR, &iG, &iB);
		m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + 18 + 40]->PutSpriteRGB(sX, sY, (_tmp_sAppr1 & 0x0F00) >> 8, iR, iG, iB, dwTime);
		m_pSprite[SPRID_ITEMEQUIP_PIVOTPOINT + 19 + 40]->PutSpriteFast(sX, sY, (_tmp_sAppr1 & 0x000F), dwTime);
	}
}
void cMain::UpdateScreen_OnChangePassword()
{
	short msX, msY, msZ;
	char cLB, cRB;
	char cMIresult;
	int  iMIbuttonNum;
	static class CMouseInterface * pMI;
	static char  cName[12], cPassword[12], cNewPassword[12], cNewPassConfirm[12], cPrevFocus;
	static DWORD dwCTime;
	DWORD dwTime = timeGetTime();
	BOOL bFlag = TRUE;

	if (m_cGameModeCount == 0) {
		EndInputString();

		pMI = new class CMouseInterface;
		pMI->AddRect(300, 148, 425, 170);
		pMI->AddRect(300, 172, 425, 194);
		pMI->AddRect(300, 196, 425, 218);
		pMI->AddRect(300, 220, 425, 242);

		pMI->AddRect(197, 320, 197 + BTNSZX, 320 + BTNSZY);
		pMI->AddRect(370, 320, 370 + BTNSZX, 320 + BTNSZY);

		cPrevFocus = 2; //1
		m_cCurFocus = 2; //1
		m_cMaxFocus = 6;
		m_bEnterPressed = FALSE;
		m_cArrowPressed = 0;

		ZeroMemory(cName, sizeof(cName));
		ZeroMemory(cPassword, sizeof(cPassword));
		ZeroMemory(cNewPassword, sizeof(cNewPassword));
		ZeroMemory(cNewPassConfirm, sizeof(cNewPassConfirm));

		strcpy(cName, m_cAccountName);
		//StartInputString(314, 155, 11, cName);
		StartInputString(314, 179, 11, cPassword);
		ClearInputString();
		dwCTime = dwTime;
	}
	m_cGameModeCount++;
	if (m_cGameModeCount > 100) m_cGameModeCount = 100;

	if ((dwTime - dwCTime) > 100) {
		m_cMenuFrame++;
		dwCTime = dwTime;
	}
	if (m_cMenuFrame >= 8)
	{
		m_cMenuDirCnt++;
		if (m_cMenuDirCnt > 8)
		{
			m_cMenuDir++;
			m_cMenuDirCnt = 1;
		}
		m_cMenuFrame = 0;
	}
	if (m_cMenuDir > 8) m_cMenuDir = 1;

	if (m_cArrowPressed != 0)
	{
		switch (m_cArrowPressed) {
		case 1:
			m_cCurFocus--;
			if (m_cCurFocus <= 0) m_cCurFocus = m_cMaxFocus;
			break;

		case 2:
			if (m_cCurFocus == 3) m_cCurFocus = 4;
			else if (m_cCurFocus == 4) m_cCurFocus = 3;
			break;

		case 3:
			m_cCurFocus++;
			if (m_cCurFocus > m_cMaxFocus) m_cCurFocus = 1;
			break;

		case 4:
			if (m_cCurFocus == 3) m_cCurFocus = 4;
			else if (m_cCurFocus == 4) m_cCurFocus = 3;
			break;
		}
		m_cArrowPressed = 0;
	}

	if (m_bEnterPressed == TRUE)
	{
		PlaySound('E', 14, 5);
		switch (m_cCurFocus) {
		case 1:
		case 2:
		case 3:
		case 4:
			m_cCurFocus++;
			if (m_cCurFocus > m_cMaxFocus) m_cCurFocus = 1;
			break;

		case 5:	// Connect
			if ((m_Misc.bCheckValidString(cPassword) == FALSE) || (strlen(cPassword) == 0) ||
				(m_Misc.bCheckValidName(cNewPassword) == FALSE) || (m_Misc.bCheckValidName(cNewPassConfirm) == FALSE) ||
				(strlen(cNewPassword) == 0) || (memcmp(cNewPassword, cNewPassConfirm, 10) != 0)) break;

			ZeroMemory(m_cAccountName, sizeof(m_cAccountName));
			ZeroMemory(m_cAccountPassword, sizeof(m_cAccountPassword));
			ZeroMemory(m_cNewPassword, sizeof(m_cNewPassword));
			ZeroMemory(m_cNewPassConfirm, sizeof(m_cNewPassConfirm));
			strcpy(m_cAccountName, cName);
			strcpy(m_cAccountPassword, cPassword);
			strcpy(m_cNewPassword, cNewPassword);
			strcpy(m_cNewPassConfirm, cNewPassConfirm);
			//m_pLSock = new class XSocket(m_hWnd, SOCKETBLOCKLIMIT);
			//m_pLSock->bConnect(m_cLogServerAddr, m_iLogServerPort, WM_USER_LOGSOCKETEVENT);
			//m_pLSock->bInitBufferSize(30000);
			ChangeGameMode(GAMEMODE_ONCONNECTING);
			//m_dwConnectMode = MSGID_REQUEST_CHANGEPASSWORD;
			ZeroMemory(m_cMsg, sizeof(m_cMsg));
			strcpy(m_cMsg, "41");
			delete pMI;
			return;

		case 6:	// Cancel
			ChangeGameMode(GAMEMODE_ONSELECTCHARACTER);
			//m_dwLoginMode = MSGID_REQUEST_LOGIN;
			delete pMI;
			return;
		}
		m_bEnterPressed = FALSE;
	}

	if (m_bEscPressed == TRUE)
	{
		ChangeGameMode(GAMEMODE_ONMAINMENU);
		delete pMI;
		m_bEscPressed = FALSE;
		return;
	}

	if (cPrevFocus != m_cCurFocus)
	{
		EndInputString();
		switch (m_cCurFocus) {
		case 1:
			StartInputString(314, 155, 11, cName);
			break;
		case 2:
			StartInputString(314, 179, 11, cPassword);
			break;
		case 3:
			StartInputString(314, 203, 11, cNewPassword);
			break;
		case 4:
			StartInputString(314, 227, 11, cNewPassConfirm);
			break;
		}
		cPrevFocus = m_cCurFocus;
	}

	m_DDraw.ClearBackB4();

	UpdateScreen_OnSelectCharacter(0, 0, 0, 0, TRUE);
	m_DDraw.DrawShadowBox(0, 0, 639, 479);//SelectCharacter

	DrawNewDialogBox(SPRID_INTERFACE_ND_GAME4, 153, 112, 0);
	DrawNewDialogBox(SPRID_INTERFACE_ND_TEXT, 153, 112, 13);
	DrawNewDialogBox(SPRID_INTERFACE_ND_GAME4, 153 + 157, 112 + 109, 7);//

	PutString(206, 155, UPDATE_SCREEN_ON_CHANGE_PASSWORD1, RGB(25, 35, 25));
	PutString(206, 179, UPDATE_SCREEN_ON_CHANGE_PASSWORD2, RGB(25, 35, 25));
	PutString(206, 203, UPDATE_SCREEN_ON_CHANGE_PASSWORD3, RGB(25, 35, 25));
	PutString(206, 227, UPDATE_SCREEN_ON_CHANGE_PASSWORD4, RGB(25, 35, 25));

	if (m_cCurFocus != 1) {
		if (m_Misc.bCheckValidString(cName) != FALSE)
			PutString(314, 155, cName, RGB(25, 35, 25));
		else PutString(314, 155, cName, RGB(55, 18, 13));
	}
	if ((m_Misc.bCheckValidString(cName) == FALSE) || (strlen(cName) == 0)) bFlag = FALSE;

	if (m_cCurFocus != 2) {
		if ((m_Misc.bCheckValidString(cPassword) != FALSE))
			PutString(314, 179, cPassword, RGB(25, 35, 25), TRUE, 3);
		else PutString(314, 179, cPassword, RGB(55, 18, 13), TRUE, 3);
	}

	if (m_cCurFocus != 3) {
		if ((m_Misc.bCheckValidName(cNewPassword) != FALSE))
			PutString(314, 203, cNewPassword, RGB(25, 35, 25), TRUE, 3);
		else PutString(314, 203, cNewPassword, RGB(55, 18, 13), TRUE, 3);
	}

	if (m_cCurFocus != 4) {
		if ((m_Misc.bCheckValidName(cNewPassConfirm) != FALSE))
			PutString(314, 227, cNewPassConfirm, RGB(25, 35, 25), TRUE, 3);
		else PutString(314, 227, cNewPassConfirm, RGB(55, 18, 13), TRUE, 3);
	}

	if ((m_Misc.bCheckValidString(cPassword) == FALSE) || (strlen(cPassword) == 0) ||
		(strlen(cNewPassword) < 8) || (memcmp(cNewPassword, cNewPassConfirm, 10) != 0) ||
		(memcmp(cPassword, cNewPassword, 10) == 0)) bFlag = FALSE;


	if (m_cCurFocus == 1) ShowReceivedString();
	else if ((m_cCurFocus == 2) || (m_cCurFocus == 3) || (m_cCurFocus == 4)) ShowReceivedString(TRUE);

	PutAlignedString(153, 487, 258, UPDATE_SCREEN_ON_CHANGE_PASSWORD5);//"
	PutAlignedString(153, 487, 273, UPDATE_SCREEN_ON_CHANGE_PASSWORD6);//"
	PutAlignedString(153, 487, 288, UPDATE_SCREEN_ON_CHANGE_PASSWORD7);//"

	if ((bFlag == TRUE) && (m_cCurFocus == 5))
		m_pSprite[SPRID_INTERFACE_ND_BUTTON]->PutSpriteFast(197, 320, 21, dwTime);
	else m_pSprite[SPRID_INTERFACE_ND_BUTTON]->PutSpriteFast(197, 320, 20, dwTime);

	if (m_cCurFocus == 6)
		m_pSprite[SPRID_INTERFACE_ND_BUTTON]->PutSpriteFast(370, 320, 17, dwTime);
	else m_pSprite[SPRID_INTERFACE_ND_BUTTON]->PutSpriteFast(370, 320, 16, dwTime);

	DrawVersion();
	m_DInput.UpdateMouseState(&msX, &msY, &msZ, &cLB, &cRB);
	m_pSprite[SPRID_MOUSECURSOR]->PutSpriteFast(msX, msY, 0, dwTime);
	iMIbuttonNum = pMI->iGetStatus(msX, msY, cLB, &cMIresult);
	if (cMIresult == MIRESULT_CLICK)
	{
		PlaySound('E', 14, 5);

		switch (iMIbuttonNum) {
		case 1:
		case 2:
		case 3:
		case 4:
			m_cCurFocus = iMIbuttonNum;
			break;

		case 5:
			if ((m_Misc.bCheckValidString(cPassword) == FALSE) || (strlen(cPassword) == 0) ||
				(m_Misc.bCheckValidName(cNewPassword) == FALSE) || (m_Misc.bCheckValidName(cNewPassConfirm) == FALSE) ||
				(strlen(cNewPassword) == 0) || (memcmp(cNewPassword, cNewPassConfirm, 10) != 0)) break;

			EndInputString();
			ZeroMemory(m_cAccountName, sizeof(m_cAccountName));
			ZeroMemory(m_cAccountPassword, sizeof(m_cAccountPassword));
			ZeroMemory(m_cNewPassword, sizeof(m_cNewPassword));
			ZeroMemory(m_cNewPassConfirm, sizeof(m_cNewPassConfirm));
			strcpy(m_cAccountName, cName);
			strcpy(m_cAccountPassword, cPassword);
			strcpy(m_cNewPassword, cNewPassword);
			strcpy(m_cNewPassConfirm, cNewPassConfirm);
			//m_pLSock = new class XSocket(m_hWnd, SOCKETBLOCKLIMIT);
			//m_pLSock->bConnect(m_cLogServerAddr, m_iLogServerPort, WM_USER_LOGSOCKETEVENT);
			//m_pLSock->bInitBufferSize(30000);
			ChangeGameMode(GAMEMODE_ONCONNECTING);
			//m_dwConnectMode = MSGID_REQUEST_CHANGEPASSWORD;
			ZeroMemory(m_cMsg, sizeof(m_cMsg));
			strcpy(m_cMsg, "41");
			delete pMI;
			return;

		case 6:
			// Cancel
			ChangeGameMode(GAMEMODE_ONSELECTCHARACTER);
			delete pMI;
			return;
		}
	}

	if ((msX >= 197) && (msX <= 197 + BTNSZX) && (msY >= 320) && (msY <= 320 + BTNSZY)) m_cCurFocus = 5;
	if ((msX >= 370) && (msX <= 370 + BTNSZX) && (msY >= 320) && (msY <= 320 + BTNSZY)) m_cCurFocus = 6;

	if (m_DDraw.iFlip() == DDERR_SURFACELOST) RestoreSprites();
}
void cMain::UpdateScreen_OnMsg()
{
	short msX, msY, msZ;
	char cLB, cRB;
	DWORD dwTime = G_dwGlobalTime;
	m_DDraw.ClearBackB4();
	PutString(10, 10, m_cMsg, RGB(255, 155, 155), FALSE, 1);
	DrawVersion();
	m_DInput.UpdateMouseState(&msX, &msY, &msZ, &cLB, &cRB);
	m_pSprite[SPRID_MOUSECURSOR]->PutSpriteFast(msX, msY, 0, dwTime);
	if (m_DDraw.iFlip() == DDERR_SURFACELOST) RestoreSprites();
	if ((G_dwGlobalTime - m_dwTime) > 1500)
	{
		ChangeGameMode(GAMEMODE_ONMAINMENU);
	}
}
void cMain::UpdateScreen_OnQueryForceLogin()
{
	short msX, msY, msZ;
	char cLB, cRB;
	char cMIresult;
	int  iMIbuttonNum;

	static class CMouseInterface * pMI;
	static DWORD dwCTime;
	DWORD dwTime = timeGetTime();

	if (m_cGameModeCount == 0) {
		pMI = new class CMouseInterface;
		pMI->AddRect(200, 244, 200 + BTNSZX, 244 + BTNSZY);
		pMI->AddRect(370, 244, 370 + BTNSZX, 244 + BTNSZY);
		m_bEnterPressed = FALSE;
		m_bEscPressed = FALSE;
		m_cArrowPressed = 0;

		dwCTime = timeGetTime();

		PlaySound('E', 25, 0);
	}
	m_cGameModeCount++;
	if (m_cGameModeCount > 100) m_cGameModeCount = 100;

	if (m_bEscPressed == TRUE) {
		ChangeGameMode(GAMEMODE_ONSELECTCHARACTER);
		delete pMI;
		m_bEscPressed = FALSE;
		return;
	}

	m_DInput.UpdateMouseState(&msX, &msY, &msZ, &cLB, &cRB);
	UpdateScreen_OnSelectCharacter(0, 0, 0, 0);
	if ((m_cGameModeCount >= 0) && (m_cGameModeCount < 6)) {
		m_DDraw.DrawShadowBox(0, 0, 639, 479);
	}
	else if (m_cGameModeCount >= 6) {
		m_DDraw.DrawShadowBox(0, 0, 639, 479);
		m_DDraw.DrawShadowBox(0, 0, 639, 479);
	}

	DrawNewDialogBox(SPRID_INTERFACE_ND_GAME4, 162, 130, 2);

	PutString_SprFont(172 + 86, 160, "Character on Use", 7, 0, 0);
	PutAlignedString(178, 453, 195, UPDATE_SCREEN_ON_QUERY_FORCE_LOGIN1);
	PutAlignedString(178, 453, 215, UPDATE_SCREEN_ON_QUERY_FORCE_LOGIN2);

	if ((msX >= 200) && (msX <= 200 + BTNSZX) && (msY >= 244) && (msY <= 244 + BTNSZY))
		DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, 200, 244, 19);
	else DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, 200, 244, 18);

	if ((msX >= 370) && (msX <= 370 + BTNSZX) && (msY >= 244) && (msY <= 244 + BTNSZY))
		DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, 370, 244, 3);
	else DrawNewDialogBox(SPRID_INTERFACE_ND_BUTTON, 370, 244, 2);

	if ((dwTime - dwCTime) > 100) {
		m_cMenuFrame++;
		dwCTime = dwTime;
	}
	if (m_cMenuFrame >= 8) {
		m_cMenuDirCnt++;
		if (m_cMenuDirCnt > 8) {
			m_cMenuDir++;
			m_cMenuDirCnt = 1;
		}
		m_cMenuFrame = 0;
	}
	if (m_cMenuDir > 8) m_cMenuDir = 1;

	iMIbuttonNum = pMI->iGetStatus(msX, msY, cLB, &cMIresult);
	if (cMIresult == MIRESULT_CLICK)
	{
		PlaySound('E', 14, 5);
		switch (iMIbuttonNum) {
		case 1:
			//m_pLSock = new class XSocket(m_hWnd, SOCKETBLOCKLIMIT);
			//m_pLSock->bConnect(m_cLogServerAddr, m_iLogServerPort, WM_USER_LOGSOCKETEVENT);
			//m_pLSock->bInitBufferSize(30000);
			ChangeGameMode(GAMEMODE_ONCONNECTING);
			//m_dwConnectMode  = MSGID_REQUEST_ENTERGAME;
			//m_wEnterGameType = ENTERGAMEMSGTYPE_NOENTER_FORCEDISCONN;
			ZeroMemory(m_cMsg, sizeof(m_cMsg));
			strcpy(m_cMsg, "33");
			delete pMI;
			return;

		case 2:
			ChangeGameMode(GAMEMODE_ONSELECTCHARACTER);
			delete pMI;
			break;
		}
	}
	DrawVersion();
	m_pSprite[SPRID_MOUSECURSOR]->PutSpriteFast(msX, msY, 0, dwTime);
	if (m_DDraw.iFlip() == DDERR_SURFACELOST) RestoreSprites();
}
void cMain::_LoadAgreementTextContents(char cType)
{
	char * pContents, *token, cTemp[120], cFileName[120];
	char   seps[] = "\n";
	int    iIndex = 0, i;
	class  CStrTok * pStrTok;
	DWORD  dwFileSize;
	HANDLE hFile;
	FILE * pFile;

	for (i = 0; i < TEXTDLGMAXLINES; i++) {
		if (m_pAgreeMsgTextList[i] != NULL)
			delete m_pAgreeMsgTextList[i];
		m_pAgreeMsgTextList[i] = NULL;
	}

	ZeroMemory(cTemp, sizeof(cTemp));
	ZeroMemory(cFileName, sizeof(cFileName));

	wsprintf(cTemp, "contents%d", cType);

	strcat(cFileName, "contents");
	strcat(cFileName, "\\");
	strcat(cFileName, "\\");
	strcat(cFileName, cTemp);
	strcat(cFileName, ".txt");

	hFile = CreateFile(cFileName, GENERIC_READ, NULL, NULL, OPEN_EXISTING, NULL, NULL);
	dwFileSize = GetFileSize(hFile, NULL);
	if (hFile != INVALID_HANDLE_VALUE) CloseHandle(hFile);

	pFile = fopen(cFileName, "rt");
	if (pFile == NULL) return;
	else {
		pContents = new char[dwFileSize + 1];
		ZeroMemory(pContents, dwFileSize + 1);
		fread(pContents, dwFileSize, 1, pFile);
	}

	fclose(pFile);
	pStrTok = new class CStrTok(pContents, seps);
	token = pStrTok->pGet();
	while (token != NULL ) {
		m_pAgreeMsgTextList[iIndex] = new class CMsg(NULL, token, NULL);
		token = pStrTok->pGet();
		iIndex++;
	}
	delete pStrTok;
	delete[] pContents;
}
BOOL cMain::DrawObject_OnMove_ForMenu(int indexX, int indexY, int sX, int sY, BOOL bTrans, DWORD dwTime, int msX, int msY)
{
	short dx, dy;
	int iBodyIndex, iHairIndex, iUndiesIndex, iArmArmorIndex, iBodyArmorIndex, iPantsIndex, iBootsIndex, iHelmIndex, iR, iG, iB;
	int iWeaponIndex, iShieldIndex, iAdd, iMantleIndex;
	BOOL bInv = FALSE;
	int iWeaponColor, iShieldColor, iArmorColor, iMantleColor, iArmColor, iPantsColor, iBootsColor, iHelmColor;
	int iSkirtDraw = 0;

	iWeaponColor = (_tmp_iApprColor & 0xF0000000) >> 28;
	iShieldColor = (_tmp_iApprColor & 0x0F000000) >> 24;
	iArmorColor = (_tmp_iApprColor & 0x00F00000) >> 20;
	iMantleColor = (_tmp_iApprColor & 0x000F0000) >> 16;
	iArmColor = (_tmp_iApprColor & 0x0000F000) >> 12;
	iPantsColor = (_tmp_iApprColor & 0x00000F00) >> 8;
	iBootsColor = (_tmp_iApprColor & 0x000000F0) >> 4;
	iHelmColor = (_tmp_iApprColor & 0x0000000F);

	switch (_tmp_sOwnerType) {
	case 1:
	case 2:
	case 3:
		if ((_tmp_sAppr2 & 0xF000) != 0)
		{
			iAdd = 3;
			iBodyIndex = 500 + (_tmp_sOwnerType - 1) * 8 * 15 + (iAdd * 8);
			iUndiesIndex = SPRID_UNDIES_M + (_tmp_sAppr1 & 0x000F) * 15 + iAdd;
			iHairIndex = SPRID_HAIR_M + ((_tmp_sAppr1 & 0x0F00) >> 8) * 15 + iAdd;
			if ((_tmp_sAppr4 & 0x80) == 0)
			{
				if (((_tmp_sAppr3 & 0xF000) >> 12) == 0)
					iBodyArmorIndex = -1;
				else
				{
					iBodyArmorIndex = SPRID_BODYARMOR_M + ((_tmp_sAppr3 & 0xF000) >> 12) * 15 + iAdd;
				}
			}
			else iBodyArmorIndex = -1;
			if ((_tmp_sAppr3 & 0x000F) == 0)
				iArmArmorIndex = -1;
			else iArmArmorIndex = SPRID_BERK_M + (_tmp_sAppr3 & 0x000F) * 15 + iAdd;
			if ((_tmp_sAppr3 & 0x0F00) == 0)
				iPantsIndex = -1;
			else iPantsIndex = SPRID_LEGG_M + ((_tmp_sAppr3 & 0x0F00) >> 8) * 15 + iAdd;

			if (((_tmp_sAppr4 & 0xF000) >> 12) == 0)
				iBootsIndex = -1;
			else iBootsIndex = SPRID_BOOT_M + ((_tmp_sAppr4 & 0xF000) >> 12) * 15 + iAdd;
			if (((_tmp_sAppr2 & 0x0FF0) >> 4) == 0)
				iWeaponIndex = -1;
			else iWeaponIndex = SPRID_WEAPON_M + ((_tmp_sAppr2 & 0x0FF0) >> 4) * 64 + 8 * 3 + (_tmp_cDir - 1);
			if ((_tmp_sAppr2 & 0x000F) == 0)
				iShieldIndex = -1;
			else iShieldIndex = SPRID_SHIELD_M + (_tmp_sAppr2 & 0x000F) * 8 + 3;
			if ((_tmp_sAppr4 & 0x0F00) == 0)
				iMantleIndex = -1;
			else iMantleIndex = SPRID_MANTLE_M + ((_tmp_sAppr4 & 0x0F00) >> 8) * 15 + iAdd;
			if ((_tmp_sAppr3 & 0x00F0) == 0)
				iHelmIndex = -1;
			else iHelmIndex = SPRID_HEAD_M + ((_tmp_sAppr3 & 0x00F0) >> 4) * 15 + iAdd;
		}
		else
		{
			iBodyIndex = 500 + (_tmp_sOwnerType - 1) * 8 * 15 + (2 * 8);
			iUndiesIndex = SPRID_UNDIES_M + (_tmp_sAppr1 & 0x000F) * 15 + 2;
			iHairIndex = SPRID_HAIR_M + ((_tmp_sAppr1 & 0x0F00) >> 8) * 15 + 2;
			if ((_tmp_sAppr4 & 0x80) == 0)
			{
				if (((_tmp_sAppr3 & 0xF000) >> 12) == 0)
					iBodyArmorIndex = -1;
				else iBodyArmorIndex = SPRID_BODYARMOR_M + ((_tmp_sAppr3 & 0xF000) >> 12) * 15 + 2;
			}
			else iBodyArmorIndex = -1;
			if ((_tmp_sAppr3 & 0x000F) == 0)
				iArmArmorIndex = -1;
			else iArmArmorIndex = SPRID_BERK_M + (_tmp_sAppr3 & 0x000F) * 15 + 2;
			if ((_tmp_sAppr3 & 0x0F00) == 0)
				iPantsIndex = -1;
			else iPantsIndex = SPRID_LEGG_M + ((_tmp_sAppr3 & 0x0F00) >> 8) * 15 + 2;
			if (((_tmp_sAppr4 & 0xF000) >> 12) == 0)
				iBootsIndex = -1;
			else iBootsIndex = SPRID_BOOT_M + ((_tmp_sAppr4 & 0xF000) >> 12) * 15 + 2;
			if (((_tmp_sAppr2 & 0x0FF0) >> 4) == 0)
				iWeaponIndex = -1;
			else iWeaponIndex = SPRID_WEAPON_M + ((_tmp_sAppr2 & 0x0FF0) >> 4) * 64 + 8 * 2 + (_tmp_cDir - 1);
			if ((_tmp_sAppr2 & 0x000F) == 0)
				iShieldIndex = -1;
			else iShieldIndex = SPRID_SHIELD_M + (_tmp_sAppr2 & 0x000F) * 8 + 2;
			if ((_tmp_sAppr4 & 0x0F00) == 0)
				iMantleIndex = -1;
			else iMantleIndex = SPRID_MANTLE_M + ((_tmp_sAppr4 & 0x0F00) >> 8) * 15 + 2;
			if ((_tmp_sAppr3 & 0x00F0) == 0)
				iHelmIndex = -1;
			else iHelmIndex = SPRID_HEAD_M + ((_tmp_sAppr3 & 0x00F0) >> 4) * 15 + 2;
		}
		break;
	case 4:
	case 5:
	case 6:
		if (((_tmp_sAppr3 & 0x0F00) >> 8) == 1) iSkirtDraw = 1;
		if ((_tmp_sAppr2 & 0xF000) != 0)
		{
			iAdd = 3;
			iBodyIndex = 500 + (_tmp_sOwnerType - 1) * 8 * 15 + (iAdd * 8);
			iUndiesIndex = SPRID_UNDIES_W + (_tmp_sAppr1 & 0x000F) * 15 + iAdd;
			iHairIndex = SPRID_HAIR_W + ((_tmp_sAppr1 & 0x0F00) >> 8) * 15 + iAdd;
			if ((_tmp_sAppr4 & 0x80) == 0)
			{
				if (((_tmp_sAppr3 & 0xF000) >> 12) == 0)
					iBodyArmorIndex = -1;
				else iBodyArmorIndex = SPRID_BODYARMOR_W + ((_tmp_sAppr3 & 0xF000) >> 12) * 15 + iAdd;
			}
			else  iBodyArmorIndex = -1;
			if ((_tmp_sAppr3 & 0x000F) == 0)
				iArmArmorIndex = -1;
			else iArmArmorIndex = SPRID_BERK_W + (_tmp_sAppr3 & 0x000F) * 15 + iAdd;
			if ((_tmp_sAppr3 & 0x0F00) == 0)
				iPantsIndex = -1;
			else iPantsIndex = SPRID_LEGG_W + ((_tmp_sAppr3 & 0x0F00) >> 8) * 15 + iAdd;
			if (((_tmp_sAppr4 & 0xF000) >> 12) == 0)
				iBootsIndex = -1;
			else iBootsIndex = SPRID_BOOT_W + ((_tmp_sAppr4 & 0xF000) >> 12) * 15 + iAdd;
			if (((_tmp_sAppr2 & 0x0FF0) >> 4) == 0)
				iWeaponIndex = -1;
			else iWeaponIndex = SPRID_WEAPON_W + ((_tmp_sAppr2 & 0x0FF0) >> 4) * 64 + 8 * 3 + (_tmp_cDir - 1);
			if ((_tmp_sAppr2 & 0x000F) == 0)
				iShieldIndex = -1;
			else iShieldIndex = SPRID_SHIELD_W + (_tmp_sAppr2 & 0x000F) * 8 + 3;
			if ((_tmp_sAppr4 & 0x0F00) == 0)
				iMantleIndex = -1;
			else iMantleIndex = SPRID_MANTLE_W + ((_tmp_sAppr4 & 0x0F00) >> 8) * 15 + iAdd;
			if ((_tmp_sAppr3 & 0x00F0) == 0)
				iHelmIndex = -1;
			else iHelmIndex = SPRID_HEAD_W + ((_tmp_sAppr3 & 0x00F0) >> 4) * 15 + iAdd;
		}
		else
		{
			iBodyIndex = 500 + (_tmp_sOwnerType - 1) * 8 * 15 + (2 * 8);
			iUndiesIndex = SPRID_UNDIES_W + (_tmp_sAppr1 & 0x000F) * 15 + 2;
			iHairIndex = SPRID_HAIR_W + ((_tmp_sAppr1 & 0x0F00) >> 8) * 15 + 2;
			if ((_tmp_sAppr4 & 0x80) == 0)
			{
				if (((_tmp_sAppr3 & 0xF000) >> 12) == 0)
					iBodyArmorIndex = -1;
				else iBodyArmorIndex = SPRID_BODYARMOR_W + ((_tmp_sAppr3 & 0xF000) >> 12) * 15 + 2;
			}
			else iBodyArmorIndex = -1;
			if ((_tmp_sAppr3 & 0x000F) == 0)
				iArmArmorIndex = -1;
			else iArmArmorIndex = SPRID_BERK_W + (_tmp_sAppr3 & 0x000F) * 15 + 2;
			if ((_tmp_sAppr3 & 0x0F00) == 0)
				iPantsIndex = -1;
			else iPantsIndex = SPRID_LEGG_W + ((_tmp_sAppr3 & 0x0F00) >> 8) * 15 + 2;
			if (((_tmp_sAppr4 & 0xF000) >> 12) == 0)
				iBootsIndex = -1;
			else iBootsIndex = SPRID_BOOT_W + ((_tmp_sAppr4 & 0xF000) >> 12) * 15 + 2;
			if (((_tmp_sAppr2 & 0x0FF0) >> 4) == 0)
				iWeaponIndex = -1;
			else iWeaponIndex = SPRID_WEAPON_W + ((_tmp_sAppr2 & 0x0FF0) >> 4) * 64 + 8 * 2 + (_tmp_cDir - 1);
			if ((_tmp_sAppr2 & 0x000F) == 0)
				iShieldIndex = -1;
			else iShieldIndex = SPRID_SHIELD_W + (_tmp_sAppr2 & 0x000F) * 8 + 2;
			if ((_tmp_sAppr4 & 0x0F00) == 0)
				iMantleIndex = -1;
			else iMantleIndex = SPRID_MANTLE_W + ((_tmp_sAppr4 & 0x0F00) >> 8) * 15 + 2;
			if ((_tmp_sAppr3 & 0x00F0) == 0)
				iHelmIndex = -1;
			else iHelmIndex = SPRID_HEAD_W + ((_tmp_sAppr3 & 0x00F0) >> 4) * 15 + 2;
		}
		break;
	default:
		iBodyIndex = SPRID_MOB + (_tmp_sOwnerType - 10) * 8 * 7 + (1 * 8);
		iUndiesIndex = -1;
		iHairIndex = -1;
		iBodyArmorIndex = -1;
		iArmArmorIndex = -1;
		iBootsIndex = -1;
		iPantsIndex = -1;
		iWeaponIndex = -1;
		iShieldIndex = -1;
		iHelmIndex = -1;
		break;
	}
	dx = 0;
	dy = 0;
	if (_cDrawingOrder[_tmp_cDir] == 1)
	{
		if (iWeaponIndex != -1)
		{
			if (iWeaponColor == 0)
				m_pSprite[iWeaponIndex]->PutSpriteFast(sX + dx, sY + dy, _tmp_cFrame, dwTime);
			else m_pSprite[iWeaponIndex]->PutSpriteRGB(sX + dx, sY + dy, _tmp_cFrame, m_wWR[iWeaponColor] - m_wR[0], m_wWG[iWeaponColor] - m_wG[0], m_wWB[iWeaponColor] - m_wB[0], dwTime);
		}
		switch (_tmp_sOwnerType) { // Pas d'ombre pour ces mobs
		case 10: // Slime
		case 35: // Energy Sphere
		case 50: // TW
		case 51: // CP
		case 60: // Plant
		case 65: // IceGolem
		case 66: // Wyvern
		case 73: // Fire Wyvern
		case 81: // Abaddon
		case 91: // Gate
			break;
		default:
			if (m_cDetailLevel != 0)
			{
				if (sX < 50)
					m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutShadowSpriteClip(sX + dx, sY + dy, _tmp_cFrame, dwTime);
				else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutShadowSprite(sX + dx, sY + dy, _tmp_cFrame, dwTime);
			}
			break;
		}
		if (bInv == TRUE)
			m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutTransSprite(sX + dx, sY + dy, _tmp_cFrame, dwTime);
		else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutSpriteFast(sX + dx, sY + dy, _tmp_cFrame, dwTime);


		if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 0))
		{
			if (iMantleColor == 0)
				m_pSprite[iMantleIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
			else m_pSprite[iMantleIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
		}
		if (iUndiesIndex != -1)
		{
			if (bInv) m_pSprite[iUndiesIndex]->PutTransSprite2(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
			else m_pSprite[iUndiesIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
		}

		if ((iHairIndex != -1) && (iHelmIndex == -1))
		{
			_GetHairColorRGB(((_tmp_sAppr1 & 0x00F0) >> 4), &iR, &iG, &iB);
			m_pSprite[iHairIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, iR, iG, iB, dwTime);
		}
		if ((iBootsIndex != -1) && (iSkirtDraw == 1))
		{
			if (iBootsColor == 0)
				m_pSprite[iBootsIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
			else m_pSprite[iBootsIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iBootsColor] - m_wR[0], m_wG[iBootsColor] - m_wG[0], m_wB[iBootsColor] - m_wB[0], dwTime);
		}
		if (iPantsIndex != -1)
		{
			if (iPantsColor == 0)
				m_pSprite[iPantsIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
			else m_pSprite[iPantsIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iPantsColor] - m_wR[0], m_wG[iPantsColor] - m_wG[0], m_wB[iPantsColor] - m_wB[0], dwTime);
		}
		if (iArmArmorIndex != -1)
		{
			if (iArmColor == 0)
				m_pSprite[iArmArmorIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
			else m_pSprite[iArmArmorIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iArmColor] - m_wR[0], m_wG[iArmColor] - m_wG[0], m_wB[iArmColor] - m_wB[0], dwTime);
		}
		if ((iBootsIndex != -1) && (iSkirtDraw == 0))
		{
			if (iBootsColor == 0)
				m_pSprite[iBootsIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
			else m_pSprite[iBootsIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iBootsColor] - m_wR[0], m_wG[iBootsColor] - m_wG[0], m_wB[iBootsColor] - m_wB[0], dwTime);
		}
		if (iBodyArmorIndex != -1)
		{
			if (iArmorColor == 0)
				m_pSprite[iBodyArmorIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
			else m_pSprite[iBodyArmorIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iArmorColor] - m_wR[0], m_wG[iArmorColor] - m_wG[0], m_wB[iArmorColor] - m_wB[0], dwTime);
		}
		if (iHelmIndex != -1)
		{
			if (iHelmColor == 0)
				m_pSprite[iHelmIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
			else m_pSprite[iHelmIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iHelmColor] - m_wR[0], m_wG[iHelmColor] - m_wG[0], m_wB[iHelmColor] - m_wB[0], dwTime);
		}
		if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 2))
		{
			if (iMantleColor == 0)
				m_pSprite[iMantleIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
			else m_pSprite[iMantleIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
		}

		if (iShieldIndex != -1)
		{
			if (iShieldColor == 0)
				m_pSprite[iShieldIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
			else m_pSprite[iShieldIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iShieldColor] - m_wR[0], m_wG[iShieldColor] - m_wG[0], m_wB[iShieldColor] - m_wB[0], dwTime);
		}
		if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 1))
		{
			if (iMantleColor == 0)
				m_pSprite[iMantleIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
			else m_pSprite[iMantleIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
		}
	}
	else
	{
		switch (_tmp_sOwnerType) { // Pas d'ombre pour ces mobs
		case 10: // Slime
		case 35: // Energy Sphere
		case 50: // TW
		case 51: // CP
		case 60: // Plant
		case 65: // IceGolem
		case 66: // Wyvern
		case 73: // Fire Wyvern
		case 81: // Abaddon
		case 91: // Gate
			break;
		default:
			if (m_cDetailLevel != 0)
			{
				if (sX < 50)
					m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutShadowSpriteClip(sX + dx, sY + dy, _tmp_cFrame, dwTime);
				else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutShadowSprite(sX + dx, sY + dy, _tmp_cFrame, dwTime);
			}
			break;
		}

		if (bInv == TRUE)
			m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutTransSprite(sX + dx, sY + dy, _tmp_cFrame, dwTime);
		else m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->PutSpriteFast(sX + dx, sY + dy, _tmp_cFrame, dwTime);

		if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 0))
		{
			if (iMantleColor == 0)
				m_pSprite[iMantleIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
			else m_pSprite[iMantleIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
		}
		if (iUndiesIndex != -1) m_pSprite[iUndiesIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);

		if ((iHairIndex != -1) && (iHelmIndex == -1))
		{
			_GetHairColorRGB(((_tmp_sAppr1 & 0x00F0) >> 4), &iR, &iG, &iB);
			m_pSprite[iHairIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, iR, iG, iB, dwTime);
		}
		if ((iBootsIndex != -1) && (iSkirtDraw == 1))
		{
			if (iBootsColor == 0)
				m_pSprite[iBootsIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
			else m_pSprite[iBootsIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iBootsColor] - m_wR[0], m_wG[iBootsColor] - m_wG[0], m_wB[iBootsColor] - m_wB[0], dwTime);
		}
		if (iPantsIndex != -1)
		{
			if (iPantsColor == 0)
				m_pSprite[iPantsIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
			else m_pSprite[iPantsIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iPantsColor] - m_wR[0], m_wG[iPantsColor] - m_wG[0], m_wB[iPantsColor] - m_wB[0], dwTime);
		}
		if (iArmArmorIndex != -1)
		{
			if (iArmColor == 0)
				m_pSprite[iArmArmorIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
			else m_pSprite[iArmArmorIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iArmColor] - m_wR[0], m_wG[iArmColor] - m_wG[0], m_wB[iArmColor] - m_wB[0], dwTime);
		}
		if ((iBootsIndex != -1) && (iSkirtDraw == 0))
		{
			if (iBootsColor == 0)
				m_pSprite[iBootsIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
			else m_pSprite[iBootsIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iBootsColor] - m_wR[0], m_wG[iBootsColor] - m_wG[0], m_wB[iBootsColor] - m_wB[0], dwTime);
		}
		if (iBodyArmorIndex != -1)
		{
			if (iArmorColor == 0)
				m_pSprite[iBodyArmorIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
			else m_pSprite[iBodyArmorIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iArmorColor] - m_wR[0], m_wG[iArmorColor] - m_wG[0], m_wB[iArmorColor] - m_wB[0], dwTime);
		}
		if (iHelmIndex != -1)
		{
			if (iHelmColor == 0)
				m_pSprite[iHelmIndex]->PutSpriteFast(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
			else m_pSprite[iHelmIndex]->PutSpriteRGB(sX, sY, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iHelmColor] - m_wR[0], m_wG[iHelmColor] - m_wG[0], m_wB[iHelmColor] - m_wB[0], dwTime);
		}
		if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 2))
		{
			if (iMantleColor == 0)
				m_pSprite[iMantleIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
			else m_pSprite[iMantleIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
		}

		if (iShieldIndex != -1)
		{
			if (iShieldColor == 0)
				m_pSprite[iShieldIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
			else m_pSprite[iShieldIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iShieldColor] - m_wR[0], m_wG[iShieldColor] - m_wG[0], m_wB[iShieldColor] - m_wB[0], dwTime);
		}
		if ((iMantleIndex != -1) && (_cMantleDrawingOrder[_tmp_cDir] == 1))
		{
			if (iMantleColor == 0)
				m_pSprite[iMantleIndex]->PutSpriteFast(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, dwTime);
			else m_pSprite[iMantleIndex]->PutSpriteRGB(sX + dx, sY + dy, (_tmp_cDir - 1) * 8 + _tmp_cFrame, m_wR[iMantleColor] - m_wR[0], m_wG[iMantleColor] - m_wG[0], m_wB[iMantleColor] - m_wB[0], dwTime);
		}
		if (iWeaponIndex != -1)
		{
			if (iWeaponColor == 0)
				m_pSprite[iWeaponIndex]->PutSpriteFast(sX + dx, sY + dy, _tmp_cFrame, dwTime);
			else m_pSprite[iWeaponIndex]->PutSpriteRGB(sX + dx, sY + dy, _tmp_cFrame, m_wWR[iWeaponColor] - m_wR[0], m_wWG[iWeaponColor] - m_wG[0], m_wWB[iWeaponColor] - m_wB[0], dwTime);
		}
	}

	if (_tmp_iChatIndex != NULL)
	{
		if (m_pChatMsgList[_tmp_iChatIndex] != NULL)
		{
			DrawChatMsgBox(sX + dx, sY + dy, _tmp_iChatIndex, FALSE);
		}
		else
		{
			m_pMapData->ClearChatMsg(indexX, indexY);
		}
	}
	_tmp_dx = dx;
	_tmp_dy = dy;
	if ((m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top != -1) &&
		(m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.top < msY) &&
		(m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.bottom > msY) &&
		(m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.left < msX) &&
		(m_pSprite[iBodyIndex + (_tmp_cDir - 1)]->m_rcBound.right > msX)) return TRUE;
	return FALSE;
}
void cMain::UpdateScreen_OnSelectCharacter()
{
	short sX, sY, msX, msY, msZ;
	char  cLB, cRB, cTotalChar;
	char  cMIresult;
	static class CMouseInterface * pMI;
	DWORD dwTime;
	static DWORD dwCTime;

	int iMIbuttonNum;

	dwTime = timeGetTime();
	sX = 0;
	sY = 0;
	cTotalChar = 0;

	if (m_cGameModeCount == 0)
	{
		G_cSpriteAlphaDegree = 1;
		InitGameSettings();
		pMI = new class CMouseInterface;
		pMI->AddRect(100, 50, 210, 250);
		pMI->AddRect(211, 50, 321, 250);
		pMI->AddRect(322, 50, 431, 250);
		pMI->AddRect(432, 50, 542, 250);

		pMI->AddRect(360, 283, 545, 315);
		pMI->AddRect(360, 316, 545, 345);
		pMI->AddRect(360, 346, 545, 375);
		pMI->AddRect(360, 376, 545, 405);
		pMI->AddRect(360, 406, 545, 435);

		m_cMaxFocus = 4;
		if (m_cCurFocus > m_cMaxFocus) m_cCurFocus = 1;
		if (m_cCurFocus < 1)		   m_cCurFocus = 1;

		m_cArrowPressed = 0;
		m_bEnterPressed = FALSE;

		dwCTime = timeGetTime();
	}

	m_cGameModeCount++;
	if (m_cGameModeCount > 100) m_cGameModeCount = 100;

	if (m_cArrowPressed != 0)
	{
		switch (m_cArrowPressed) {
		case 2:
			m_cCurFocus++;
			if (m_cCurFocus > m_cMaxFocus) m_cCurFocus = 1;
			break;
		case 4:
			m_cCurFocus--;
			if (m_cCurFocus <= 0) m_cCurFocus = m_cMaxFocus;
			break;
		}
		m_cArrowPressed = 0;
	}

	if (m_bEscPressed == TRUE)
	{
		ChangeGameMode(GAMEMODE_ONMAINMENU);
		delete pMI;
		m_bEscPressed = FALSE;
		return;
	}

	if (m_bEnterPressed == TRUE)
	{
		m_bEnterPressed = FALSE;
		PlaySound('E', 14, 5);

		if (m_pCharList[m_cCurFocus - 1] != NULL)
		{
			if (m_pCharList[m_cCurFocus - 1]->m_sSex != NULL)
			{
				ZeroMemory(m_cPlayerName, sizeof(m_cPlayerName));
				strcpy(m_cPlayerName, m_pCharList[m_cCurFocus - 1]->m_cName);
				m_iLevel = (int)m_pCharList[m_cCurFocus - 1]->m_sLevel;
				if (m_Misc.bCheckValidString(m_cPlayerName) == TRUE)
				{
					m_pSprite[SPRID_INTERFACE_ND_LOGIN]->_iCloseSprite();
					m_pSprite[SPRID_INTERFACE_ND_MAINMENU]->_iCloseSprite();
					//m_pLSock = new class XSocket(m_hWnd, SOCKETBLOCKLIMIT);
					//m_pLSock->bConnect(m_cLogServerAddr, m_iLogServerPort +(rand()%1), WM_USER_LOGSOCKETEVENT);
					//m_pLSock->bInitBufferSize(30000);
					ChangeGameMode(GAMEMODE_ONCONNECTING);
					//m_dwConnectMode  = MSGID_REQUEST_ENTERGAME;
					//m_wEnterGameType = ENTERGAMEMSGTYPE_NEW;
					ZeroMemory(m_cMsg, sizeof(m_cMsg));
					strcpy(m_cMsg, "33");
					ZeroMemory(m_cMapName, sizeof(m_cMapName));
					memcpy(m_cMapName, m_pCharList[m_cCurFocus - 1]->m_cMapName, 10);
					delete pMI;
					return;
				}
			}
		}
		else
		{
			_InitOnCreateNewCharacter();
			ChangeGameMode(GAMEMODE_ONCREATENEWCHARACTER);
			delete pMI;
			return;
		}
	}

	m_DInput.UpdateMouseState(&msX, &msY, &msZ, &cLB, &cRB);
	UpdateScreen_OnSelectCharacter(sX, sY, msX, msY);

	if ((dwTime - dwCTime) > 100)
	{
		m_cMenuFrame++;
		dwCTime = dwTime;
	}
	if (m_cMenuFrame >= 8)
	{
		m_cMenuDirCnt++;
		if (m_cMenuDirCnt > 8)
		{
			m_cMenuDir++;
			m_cMenuDirCnt = 1;
		}
		m_cMenuFrame = 0;
	}
	if (m_cMenuDir > 8) m_cMenuDir = 1;

	DrawVersion();
	m_pSprite[SPRID_MOUSECURSOR]->PutSpriteFast(msX, msY, 0, dwTime);

	iMIbuttonNum = pMI->iGetStatus(msX, msY, cLB, &cMIresult);
	if (cMIresult == MIRESULT_CLICK) {
		PlaySound('E', 14, 5);

		switch (iMIbuttonNum) {
		case 1:
		case 2:
		case 3:
		case 4:
			if (m_cCurFocus != iMIbuttonNum)
				m_cCurFocus = iMIbuttonNum;
			else
			{
				if (m_pCharList[m_cCurFocus - 1] != NULL)
				{
					if (m_pCharList[m_cCurFocus - 1]->m_sSex != NULL)
					{
						ZeroMemory(m_cPlayerName, sizeof(m_cPlayerName));
						strcpy(m_cPlayerName, m_pCharList[m_cCurFocus - 1]->m_cName);
						m_iLevel = (int)m_pCharList[m_cCurFocus - 1]->m_sLevel;
						if (m_Misc.bCheckValidString(m_cPlayerName) == TRUE)
						{
							m_pSprite[SPRID_INTERFACE_ND_LOGIN]->_iCloseSprite();
							m_pSprite[SPRID_INTERFACE_ND_MAINMENU]->_iCloseSprite();
							//m_pLSock = new class XSocket(m_hWnd, SOCKETBLOCKLIMIT);
							//m_pLSock->bConnect(m_cLogServerAddr, m_iLogServerPort +(rand()%1), WM_USER_LOGSOCKETEVENT);
							//m_pLSock->bInitBufferSize(30000);
							ChangeGameMode(GAMEMODE_ONCONNECTING);
							//m_dwConnectMode  = MSGID_REQUEST_ENTERGAME;
							//m_wEnterGameType = ENTERGAMEMSGTYPE_NEW;
							ZeroMemory(m_cMsg, sizeof(m_cMsg));
							strcpy(m_cMsg, "33");
							ZeroMemory(m_cMapName, sizeof(m_cMapName));
							memcpy(m_cMapName, m_pCharList[m_cCurFocus - 1]->m_cMapName, 10);
							delete pMI;
							return;
						}
					}
				}
				else
				{
					_InitOnCreateNewCharacter();
					ChangeGameMode(GAMEMODE_ONCREATENEWCHARACTER);
					delete pMI;
					return;
				}
			}
			break;

		case 5:
			if (m_pCharList[m_cCurFocus - 1] != NULL)
			{
				if (m_pCharList[m_cCurFocus - 1]->m_sSex != NULL)
				{
					ZeroMemory(m_cPlayerName, sizeof(m_cPlayerName));
					strcpy(m_cPlayerName, m_pCharList[m_cCurFocus - 1]->m_cName);
					m_iLevel = (int)m_pCharList[m_cCurFocus - 1]->m_sLevel;

					if (m_Misc.bCheckValidString(m_cPlayerName) == TRUE) {
						m_pSprite[SPRID_INTERFACE_ND_LOGIN]->_iCloseSprite();
						m_pSprite[SPRID_INTERFACE_ND_MAINMENU]->_iCloseSprite();
						//m_pLSock = new class XSocket(m_hWnd, SOCKETBLOCKLIMIT);
						//m_pLSock->bConnect(m_cLogServerAddr, m_iLogServerPort +(rand()%1), WM_USER_LOGSOCKETEVENT);
						//m_pLSock->bInitBufferSize(30000);
						ChangeGameMode(GAMEMODE_ONCONNECTING);
						//m_dwConnectMode  = MSGID_REQUEST_ENTERGAME;
						//m_wEnterGameType = ENTERGAMEMSGTYPE_NEW;
						ZeroMemory(m_cMsg, sizeof(m_cMsg));
						strcpy(m_cMsg, "33");
						ZeroMemory(m_cMapName, sizeof(m_cMapName));
						memcpy(m_cMapName, m_pCharList[m_cCurFocus - 1]->m_cMapName, 10);
						delete pMI;
						return;
					}
				}
			}
			break;

		case 6:
			if (m_iTotalChar < 4)
			{
				_InitOnCreateNewCharacter();
				ChangeGameMode(GAMEMODE_ONCREATENEWCHARACTER);
				delete pMI;
				return;
			}
			break;

		case 7:
			if ((m_pCharList[m_cCurFocus - 1] != NULL) && (m_pCharList[m_cCurFocus - 1]->m_sLevel < 50))
			{
				ChangeGameMode(GAMEMODE_ONQUERYDELETECHARACTER);
				//m_wEnterGameType = m_cCurFocus;
				delete pMI;
				return;
			}
			break;

		case 8:
			ChangeGameMode(GAMEMODE_ONCHANGEPASSWORD);
			delete pMI;
			return;

		case 9:
			ChangeGameMode(GAMEMODE_ONMAINMENU);
			delete pMI;
			return;
		}
	}

	//	if (m_cGameModeCount < 6) m_DDraw.DrawShadowBox(0,0,639,479);
	//	if (m_cGameModeCount < 2) m_DDraw.DrawShadowBox(0,0,639,479);

	if (m_DDraw.iFlip() == DDERR_SURFACELOST) RestoreSprites();
}
void cMain::_InitOnCreateNewCharacter()
{
	m_cGender = rand() % 2 + 1;
	m_cSkinCol = rand() % 3 + 1;
	m_cHairStyle = rand() % 8;
	m_cHairCol = rand() % 16;
	m_cUnderCol = rand() % 8;
	m_ccStr = 10;
	m_ccVit = 10;
	m_ccDex = 10;
	m_ccInt = 10;
	m_ccMag = 10;
	m_ccChr = 10;
}
void cMain::_GetHairColorRGB(int iColorType, int * pR, int * pG, int * pB)
{
	switch (iColorType) {
	case 0: // rouge foncÈ
		*pR = 14; *pG = -5; *pB = -5; break;
	case 1: // Orange
		*pR = 20; *pG = 0; *pB = 0; break;
	case 2: // marron tres clair
		*pR = 22; *pG = 13; *pB = -10; break;
	case 3: // vert
		*pR = 0; *pG = 10; *pB = 0; break;
	case 4: // Bleu flashy
		*pR = 0; *pG = 0; *pB = 22; break;
	case 5: // Bleu foncÈ
		*pR = -5; *pG = -5; *pB = 15; break;
	case 6: //Mauve
		*pR = 15; *pG = -5; *pB = 16; break;
	case 7: // Noir
		*pR = -6; *pG = -6; *pB = -6; break;
	case 8:
		*pR = 10; *pG = 3; *pB = 10; break;
	case 9:
		*pR = 10; *pG = 3; *pB = -10; break;
	case 10:
		*pR = -10; *pG = 3; *pB = 10; break;
	case 11:
		*pR = 10; *pG = 3; *pB = 20; break;
	case 12:
		*pR = 21; *pG = 3; *pB = 3; break;
	case 13:
		*pR = 3; *pG = 3; *pB = 25; break;
	case 14:
		*pR = 3; *pG = 11; *pB = 3; break;
	case 15:
		*pR = 6; *pG = 8; *pB = 0; break;
	}
}
BOOL cMain::_bCheckBadWords(char *pMsg)
{
	char cStr[500];
	int iLen;
	ZeroMemory(cStr, sizeof(cStr));
	strcpy(cStr, pMsg);
	iLen = strlen(cStr);

	//for (i = 0; i < iLen; i++) {
	//	if (m_pCGameMonitor->bCheckBadWord((char *)(cStr + i)) == TRUE) return TRUE;
	//	if ((unsigned char)cStr[i] >= 128) i++;
	//	}

	return FALSE;
}
void cMain::DrawChatMsgBox(short sX, short sY, int iChatIndex, BOOL bIsPreDC)
{
	char cMsg[100], cMsgA[22], cMsgB[22], cMsgC[22], *cp;
	int  iRet, iLines, i, iSize, iSize2, iLoc, iFontSize;
	DWORD dwTime;
	COLORREF rgb;
	BOOL bIsTrans;
	RECT rcRect;
	SIZE Size;

	ZeroMemory(cMsg, sizeof(cMsg));
	ZeroMemory(cMsgA, sizeof(cMsgA));
	ZeroMemory(cMsgB, sizeof(cMsgB));
	ZeroMemory(cMsgC, sizeof(cMsgC));

	dwTime = m_pChatMsgList[iChatIndex]->m_dwTime;
	strcpy(cMsg, m_pChatMsgList[iChatIndex]->m_pMsg);
	cp = (char *)cMsg;
	iLines = 0;

	rgb = RGB(255, 255, 255);
	switch (m_pChatMsgList[iChatIndex]->m_cType) {
	case 1:
		rgb = RGB(255, 255, 255);
		break;
	case 20:
		rgb = RGB(255, 255, 20);
		// ∏ﬁΩ√¡ˆ «•Ω√ø° µÙ∑π¿Ã∞° ∞…∏∞¥Ÿ.
		if ((m_dwCurTime - dwTime) < 650) return;
		else dwTime += 650;
		break;
	case 41:
		rgb = RGB(255, 80, 80);
		break;

	case 42:
		rgb = RGB(255, 80, 80);
		if ((m_dwCurTime - dwTime) < 650) return;
		else dwTime += 650;
		break;
	}

	if (strlen(cp) != 0) {
		memcpy(cMsgA, cp, 20);

		iRet = GetCharKind(cMsgA, 19);
		if (iRet == CODE_HAN1) {
			cMsgA[20] = cp[20];
			cp++;
		}
		cp += 20;
		iLines = 1;
	}

	if (strlen(cp) != 0) {
		memcpy(cMsgB, cp, 20);

		iRet = GetCharKind(cMsgB, 19);
		if (iRet == CODE_HAN1) {
			cMsgB[20] = cp[20];
			cp++;
		}
		cp += 20;
		iLines = 2;
	}

	if (strlen(cp) != 0) {
		memcpy(cMsgC, cp, 20);

		iRet = GetCharKind(cMsgC, 19);
		if (iRet == CODE_HAN1) {
			cMsgC[20] = cp[20];
			cp++;
		}
		cp += 20;
		iLines = 3;
	}

	iSize = 0;
	for (i = 0; i < 20; i++)
	if (cMsgA[i] != 0)

	if ((unsigned char)cMsgA[i] >= 128) {
		iSize += 5;	//6
		i++;
	}
	else iSize += 4;

	iLoc = m_dwCurTime - dwTime;
	switch (m_pChatMsgList[iChatIndex]->m_cType) {
	case 21:
	case 22:
	case 23://...
		if (iLoc > 80) iLoc = 10;
		else iLoc = iLoc >> 3;
		break;
	default://
		if (iLoc > 352) iLoc = 9;
		else if (iLoc > 320) iLoc = 10;
		else iLoc = iLoc >> 5;
		break;
	}

	if (m_cDetailLevel == 0)
		bIsTrans = FALSE;
	else bIsTrans = TRUE;

	switch (m_pChatMsgList[iChatIndex]->m_cType) {
	case 41:
	case 42:
		iSize2 = 0;
		for (i = 0; i < 100; i++)
		if (cMsg[i] != 0)
		if ((unsigned char)cMsg[i] >= 128) {
			iSize2 += 5;
			i++;
		}
		else iSize2 += 4;
		if (m_Misc.bCheckIMEString(cMsg) == FALSE)
		{
			PutString(sX - iSize2, sY - 65 - iLoc, cMsg, RGB(180, 30, 30));
			PutString(sX - iSize2 + 1, sY - 65 - iLoc, cMsg, RGB(180, 30, 30));
		}
		else PutString_SprFont3(sX - iSize2, sY - 65 - iLoc, cMsg, m_wR[14] * 4, m_wG[14] * 4, m_wB[14] * 4, FALSE, 0);
		break;

	case 21:
	case 22:
	case 23:
		iFontSize = 23 - (int)m_pChatMsgList[iChatIndex]->m_cType;
		switch (iLines) {
		case 1:
			PutString_SprFont3(sX - iSize, sY - 65 - iLoc, cMsgA, m_wR[13] * 2, m_wG[13] * 2, m_wB[13] * 2, bIsTrans, iFontSize);
			break;
		case 2:
			PutString_SprFont3(sX - iSize, sY - 81 - iLoc, cMsgA, m_wR[13] * 2, m_wG[13] * 2, m_wB[13] * 2, bIsTrans, iFontSize);
			PutString_SprFont3(sX - iSize, sY - 65 - iLoc, cMsgB, m_wR[13] * 2, m_wG[13] * 2, m_wB[13] * 2, bIsTrans, iFontSize);
			break;
		case 3:
			PutString_SprFont3(sX - iSize, sY - 97 - iLoc, cMsgA, m_wR[13] * 2, m_wG[13] * 2, m_wB[13] * 2, bIsTrans, iFontSize);
			PutString_SprFont3(sX - iSize, sY - 81 - iLoc, cMsgB, m_wR[13] * 2, m_wG[13] * 2, m_wB[13] * 2, bIsTrans, iFontSize);
			PutString_SprFont3(sX - iSize, sY - 65 - iLoc, cMsgC, m_wR[13] * 2, m_wG[13] * 2, m_wB[13] * 2, bIsTrans, iFontSize);
			break;
		}
		break;

	case 20:
	default:
		if (bIsPreDC == FALSE)
			m_DDraw._GetBackBufferDC();

		GetTextExtentPoint32(m_DDraw.m_hDC, cMsg, strlen(cMsg), &Size);

		switch (Size.cx / 160) {
		case 0:
			SetRect(&rcRect, sX - 80 + 1, sY - 65 - iLoc, sX + 80 + 1, sY - iLoc);
			m_DDraw.DrawText(&rcRect, cMsg, RGB(0, 0, 0));

			SetRect(&rcRect, sX - 80, sY - 65 - iLoc + 1, sX + 80, sY - iLoc + 1);
			m_DDraw.DrawText(&rcRect, cMsg, RGB(0, 0, 0));

			SetRect(&rcRect, sX - 80, sY - 65 - iLoc, sX + 80, sY - iLoc);
			m_DDraw.DrawText(&rcRect, cMsg, rgb);
			break;

		case 1:
			SetRect(&rcRect, sX - 80 + 1, sY - 83 - iLoc, sX + 80 + 1, sY - iLoc);
			m_DDraw.DrawText(&rcRect, cMsg, RGB(0, 0, 0));

			SetRect(&rcRect, sX - 80, sY - 83 - iLoc + 1, sX + 80, sY - iLoc + 1);
			m_DDraw.DrawText(&rcRect, cMsg, RGB(0, 0, 0));

			SetRect(&rcRect, sX - 80, sY - 83 - iLoc, sX + 80, sY - iLoc);
			m_DDraw.DrawText(&rcRect, cMsg, rgb);
			break;

		case 2:
			SetRect(&rcRect, sX - 80 + 1, sY - 101 - iLoc, sX + 80 + 1, sY - iLoc);
			m_DDraw.DrawText(&rcRect, cMsg, RGB(0, 0, 0));

			SetRect(&rcRect, sX - 80, sY - 101 - iLoc + 1, sX + 80, sY - iLoc + 1);
			m_DDraw.DrawText(&rcRect, cMsg, RGB(0, 0, 0));

			SetRect(&rcRect, sX - 80, sY - 101 - iLoc, sX + 80, sY - iLoc);
			m_DDraw.DrawText(&rcRect, cMsg, rgb);
			break;

		case 3:
			SetRect(&rcRect, sX - 80 + 1, sY - 119 - iLoc, sX + 80 + 1, sY - iLoc);
			m_DDraw.DrawText(&rcRect, cMsg, RGB(0, 0, 0));

			SetRect(&rcRect, sX - 80, sY - 119 - iLoc + 1, sX + 80, sY - iLoc + 1);
			m_DDraw.DrawText(&rcRect, cMsg, RGB(0, 0, 0));

			SetRect(&rcRect, sX - 80, sY - 119 - iLoc, sX + 80, sY - iLoc);
			m_DDraw.DrawText(&rcRect, cMsg, rgb);
			break;
		}

		if (bIsPreDC == FALSE)
			m_DDraw._ReleaseBackBufferDC();
		break;
	}
}
void cMain::InitGameSettings()
{
	int i;

	//m_bForceAttack = FALSE;
	//m_dwCommandTime = 0;

	m_bInputStatus = FALSE;
	m_pInputBuffer = NULL;

	//m_iPDBGSdivX = 0;
	//m_iPDBGSdivY = 0;
	//m_bIsRedrawPDBGS = TRUE;

	m_iCameraShakingDegree = 0;

	//m_cCommand = OBJECTSTOP;
	m_cCommandCount = 0;

	m_bIsGetPointingMode = FALSE;
	//m_iPointCommandType = -1; //v2.15 NULL -> -1

	//m_bIsCombatMode = FALSE;
	m_bRunningMode = FALSE;

	m_stMCursor.cPrevStatus = CURSORSTATUS_NULL;
	m_stMCursor.dwSelectClickTime = NULL;

	//m_bSkillUsingStatus = FALSE;
	//m_bItemUsingStatus = FALSE;
	//m_bUsingSlate = FALSE;

	//m_bIsWhetherEffect = FALSE;
	m_cWhetherEffectType = NULL;

//	m_iDownSkillIndex = -1;
	m_stDialogBoxInfo[15].bFlag = FALSE;

//	m_bIsConfusion = FALSE;

//	m_iIlusionOwnerH = NULL;
	//m_cIlusionOwnerType = NULL;

	//m_iDrawFlag = 0;
	//m_bDrawFlagDir = FALSE;
	///m_bIsCrusadeMode = FALSE;
	//m_iCrusadeDuty = NULL;
	//m_bIsAvatarMode = FALSE;
	//m_bIsAvatarMessenger = FALSE;

	m_iNetLagCount = NULL;

	//m_dwEnvEffectTime = timeGetTime();

	for (i = 0; i < MAXGUILDNAMES; i++) {
		m_stGuildName[i].dwRefTime = 0;
		m_stGuildName[i].iGuildRank = -1;
		ZeroMemory(m_stGuildName[i].cCharName, sizeof(m_stGuildName[i].cCharName));
		ZeroMemory(m_stGuildName[i].cGuildName, sizeof(m_stGuildName[i].cGuildName));
	}
	//Snoopy: 61
	for (i = 0; i < 61; i++)
		m_bIsDialogEnabled[i] = FALSE;

	//Snoopy: 58 because 2 last ones alreaddy defined
	for (i = 0; i < 58; i++)
		m_cDialogBoxOrder[i] = NULL;

	for (i = 0; i < MAXEFFECTS; i++) {
		if (m_pEffectList[i] != NULL) delete m_pEffectList[i];
		m_pEffectList[i] = NULL;
	}

	for (i = 0; i < MAXCHATMSGS; i++) {
		if (m_pChatMsgList[i] != NULL) delete m_pChatMsgList[i];
		m_pChatMsgList[i] = NULL;
	}

	for (i = 0; i < MAXCHATSCROLLMSGS; i++) {
		if (m_pChatScrollList[i] != NULL) delete m_pChatScrollList[i];
		m_pChatScrollList[i] = NULL;
	}

	for (i = 0; i < MAXWHISPERMSG; i++) {
		if (m_pWhisperMsg[i] != NULL) delete m_pWhisperMsg[i];
		m_pWhisperMsg[i] = NULL;
	}

	ZeroMemory(m_cLocation, sizeof(m_cLocation));

	ZeroMemory(m_cGuildName, sizeof(m_cGuildName));
	//m_iGuildRank = -1;
	//m_iTotalGuildsMan = 0;

	for (i = 0; i < 100; i++) {
		m_stGuildOpList[i].cOpMode = NULL;
		ZeroMemory(m_stGuildOpList[i].cName, sizeof(m_stGuildOpList[i].cName));
	}

	for (i = 0; i < 6; i++) {
		ZeroMemory(m_stEventHistory[i].cTxt, sizeof(m_stEventHistory[i].cTxt));
		m_stEventHistory[i].dwTime = G_dwGlobalTime;

		ZeroMemory(m_stEventHistory2[i].cTxt, sizeof(m_stEventHistory2[i].cTxt));
		m_stEventHistory2[i].dwTime = G_dwGlobalTime;
	}

	for (i = 0; i < MAXMENUITEMS; i++) {
		if (m_pItemForSaleList[i] != NULL) delete m_pItemForSaleList[i];
		m_pItemForSaleList[i] = NULL;
	}

	for (i = 0; i < 41; i++) {
		m_stDialogBoxInfo[i].bFlag = FALSE;
		m_stDialogBoxInfo[i].sView = 0;
		m_stDialogBoxInfo[i].bIsScrollSelected = FALSE;
	}

	for (i = 0; i < MAXITEMS; i++)
	if (m_pItemList[i] != NULL) {
		delete m_pItemList[i];
		m_pItemList[i] = NULL;
	}

	for (i = 0; i < MAXSELLLIST; i++) {
		m_stSellItemList[i].iIndex = -1;
		m_stSellItemList[i].iAmount = 0;
	}

	for (i = 0; i < MAXBANKITEMS; i++)
	if (m_pBankList[i] != NULL) {
		delete m_pBankList[i];
		m_pBankList[i] = NULL;
	}

	for (i = 0; i < MAXMAGICTYPE; i++)
		m_cMagicMastery[i] = NULL;

	for (i = 0; i < MAXSKILLTYPE; i++)
		m_cSkillMastery[i] = NULL;

	for (i = 0; i < TEXTDLGMAXLINES; i++) {
		if (m_pMsgTextList[i] != NULL)
			delete m_pMsgTextList[i];
		m_pMsgTextList[i] = NULL;

		if (m_pMsgTextList2[i] != NULL)
			delete m_pMsgTextList2[i];
		m_pMsgTextList2[i] = NULL;

		if (m_pAgreeMsgTextList[i] != NULL)
			delete m_pAgreeMsgTextList[i];
		m_pAgreeMsgTextList[i] = NULL;
	}

	for (i = 0; i < MAXPARTYMEMBERS; i++) {
		m_stPartyMember[i].cStatus = 0;
		ZeroMemory(m_stPartyMember[i].cName, sizeof(m_stPartyMember[i].cName));
	}

	//m_iLU_Point = 0;
	m_cLU_Str = m_cLU_Vit = m_cLU_Dex = m_cLU_Int = m_cLU_Mag = m_cLU_Char = 0;
	//m_bDialogTrans = FALSE;
	m_cWhetherStatus = NULL;
	//m_cLogOutCount = -1;
	//m_dwLogOutCountTime = NULL;
	//m_iSuperAttackLeft = 0;
	//m_bSuperAttackMode = FALSE;
	//m_iFightzoneNumber = 0;
	//ZeroMemory(m_cBGMmapName, sizeof(m_cBGMmapName));
	//m_dwWOFtime = 0;
	m_stQuest.sWho = NULL;
	m_stQuest.sQuestType = NULL;
	m_stQuest.sContribution = NULL;
	m_stQuest.sTargetType = NULL;
	m_stQuest.sTargetCount = NULL;
	m_stQuest.sCurrentCount = NULL;
	m_stQuest.sX = NULL;
	m_stQuest.sY = NULL;
	m_stQuest.sRange = NULL;
	m_stQuest.bIsQuestCompleted = FALSE;
	ZeroMemory(m_stQuest.cTargetName, sizeof(m_stQuest.cTargetName));
	//m_bIsObserverMode = FALSE;
	//m_bIsObserverCommanded = FALSE;
	//m_bIsPoisoned = FALSE;
	//m_bIsPrevMoveBlocked = FALSE;
	//m_iPrevMoveX = m_iPrevMoveY = -1;
	//m_sDamageMove = 0;
	//m_sDamageMoveAmount = 0;
	//m_bForceDisconn = FALSE;
	//m_bIsSpecialAbilityEnabled = FALSE;
	//m_iSpecialAbilityType = 0;
	//m_dwSpecialAbilitySettingTime = NULL;
	//m_iSpecialAbilityTimeLeftSec = NULL;
	//m_stMCursor.cSelectedObjectType = NULL;
	//m_bIsF1HelpWindowEnabled = FALSE;
	//m_bIsTeleportRequested = FALSE;
	for (i = 0; i < MAXCRUSADESTRUCTURES; i++)
	{
		m_stCrusadeStructureInfo[i].cType = NULL;
		m_stCrusadeStructureInfo[i].cSide = NULL;
		m_stCrusadeStructureInfo[i].sX = NULL;
		m_stCrusadeStructureInfo[i].sY = NULL;
	}
	//ZeroMemory(m_cStatusMapName, sizeof(m_cStatusMapName));
	//m_dwCommanderCommandRequestedTime = NULL;
	//ZeroMemory(m_cTopMsg, sizeof(m_cTopMsg));
	//m_iTopMsgLastSec = NULL;
	//m_dwTopMsgTime = NULL;
	//m_iConstructionPoint = NULL;
	//m_iWarContribution = NULL;
	//ZeroMemory(m_cTeleportMapName, sizeof(m_cTeleportMapName));
	//m_iTeleportLocX = m_iTeleportLocY = -1;
	//ZeroMemory(m_cConstructMapName, sizeof(m_cConstructMapName));
	//m_iConstructLocX = m_iConstructLocY = -1;

	//Snoopy: Apocalypse Gate
	//ZeroMemory(m_cGateMapName, sizeof(m_cGateMapName));
	//m_iGatePositX = m_iGatePositY = -1;
	//m_iHeldenianAresdenLeftTower = -1;
	//m_iHeldenianElvineLeftTower = -1;
	//m_iHeldenianAresdenFlags = -1;
	//m_iHeldenianElvineFlags = -1;
	//m_bIsXmas = FALSE;
	//m_iTotalPartyMember = 0;
	//m_iPartyStatus = 0;
	for (i = 0; i < MAXPARTYMEMBERS; i++) ZeroMemory(m_stPartyMemberNameList[i].cName, sizeof(m_stPartyMemberNameList[i].cName));
	//m_iGizonItemUpgradeLeft = 0;
	//cStateChange1 = 0;
	//cStateChange2 = 0;
	//cStateChange3 = 0;
	EnableDialogBox(9, 0, 0, 0);
}
void cMain::EnableDialogBox(int iBoxID, int cType, int sV1, int sV2, char * pString)
{
	int i;
	short sX, sY;

	switch (iBoxID) {
	case 11:
		if (m_bIsDialogEnabled[11] == FALSE)
		{
			switch (cType) {
			case NULL:
				break;
			default:
				_LoadShopMenuContents(cType);
				m_stDialogBoxInfo[11].sV1 = cType;
				m_stDialogBoxInfo[11].cMode = 0;
				m_stDialogBoxInfo[11].sView = 0;
				m_stDialogBoxInfo[11].bFlag = TRUE;
				m_stDialogBoxInfo[11].sV3 = 1;
				break;
			}
		}
		break;

	case 12: // levelup diag
		if (m_bIsDialogEnabled[12] == FALSE)
		{
			m_stDialogBoxInfo[12].sX = m_stDialogBoxInfo[1].sX + 20;
			m_stDialogBoxInfo[12].sY = m_stDialogBoxInfo[1].sY + 20;
			m_stDialogBoxInfo[12].sV1 = m_iLU_Point;
		}
		break;

	case 3: // Magic Dialog
		break;

	case 4:
		if (m_bIsDialogEnabled[4] == FALSE) {
			m_stDialogBoxInfo[4].sView = cType;
		}
		break;

	case 5:
		if (m_bIsDialogEnabled[5] == FALSE) {
			m_stDialogBoxInfo[5].sView = cType;
		}
		break;

	case 6:
		if (m_bIsDialogEnabled[6] == FALSE) {
			m_stDialogBoxInfo[6].sView = cType;
		}
		break;

	case 7:
		if (m_stDialogBoxInfo[7].cMode == 1) {
			sX = m_stDialogBoxInfo[7].sX;
			sY = m_stDialogBoxInfo[7].sY;
			EndInputString();
			StartInputString(sX + 75, sY + 140, 21, m_cGuildName);
		}
		break;

	case 17: // demande quantitÈ
		if (m_bIsDialogEnabled[17] == FALSE)
		{
			m_stDialogBoxInfo[iBoxID].cMode = 1;
			m_stDialogBoxInfo[17].sView = cType;
			EndInputString();
			ZeroMemory(m_cAmountString, sizeof(m_cAmountString));
			wsprintf(m_cAmountString, "%d", sV1);
			sX = m_stDialogBoxInfo[17].sX;
			sY = m_stDialogBoxInfo[17].sY;
			StartInputString(sX + 40, sY + 57, 11, m_cAmountString, FALSE);
		}
		else
		{
			if (m_stDialogBoxInfo[17].cMode == 1)
			{
				sX = m_stDialogBoxInfo[17].sX;
				sY = m_stDialogBoxInfo[17].sY;
				EndInputString();
				StartInputString(sX + 40, sY + 57, 11, m_cAmountString, FALSE);
			}
		}
		break;

	case 18:
		if (m_bIsDialogEnabled[18] == FALSE)
		{
			switch (cType) {
			case NULL:
				m_stDialogBoxInfo[18].cMode = 0;
				m_stDialogBoxInfo[18].sView = 0;
				break;
			default:
				_LoadTextDlgContents(cType);
				m_stDialogBoxInfo[18].cMode = 0;
				m_stDialogBoxInfo[18].sView = 0;
				break;
			}
		}
		break;

	case 19:
		break;

	case 20: // Talk to npc or unicorn
		m_bIsItemDisabled[m_stDialogBoxInfo[20].sV1] = FALSE;
		if (m_bIsDialogEnabled[20] == FALSE)
		{
			m_stDialogBoxInfo[11].sV1 = m_stDialogBoxInfo[11].sV2 = m_stDialogBoxInfo[11].sV3 =
				m_stDialogBoxInfo[11].sV4 = m_stDialogBoxInfo[11].sV5 = m_stDialogBoxInfo[11].sV6 = NULL;
			m_stDialogBoxInfo[20].cMode = cType;
			m_stDialogBoxInfo[20].sView = 0;
			m_stDialogBoxInfo[20].sV1 = sV1;
			m_stDialogBoxInfo[20].sV2 = sV2;
		}
		break;

	case 21:
		if (m_bIsDialogEnabled[21] == FALSE)
		{
			m_stDialogBoxInfo[21].cMode = cType;
			m_stDialogBoxInfo[21].sView = 0;
			m_stDialogBoxInfo[21].sV1 = _iLoadTextDlgContents2(sV1 + 20);
			m_stDialogBoxInfo[21].sV2 = sV1 + 20;
		}
		break;

	case 22:
		if (m_bIsDialogEnabled[22] == FALSE) {
			m_stDialogBoxInfo[22].sV1 = sV1;
			m_stDialogBoxInfo[22].sV2 = sV2;

			m_stDialogBoxInfo[22].sSizeX = 290;
			m_stDialogBoxInfo[22].sSizeY = 290;
		}
		break;

	case 23:
		if (m_bIsDialogEnabled[23] == FALSE) {
			m_stDialogBoxInfo[23].cMode = cType;
			m_stDialogBoxInfo[23].sV1 = sV1;		// ItemID
			m_stDialogBoxInfo[23].sV2 = sV2;
			if (cType == 2)
			{
				m_stDialogBoxInfo[23].sX = m_stDialogBoxInfo[11].sX;
				m_stDialogBoxInfo[23].sY = m_stDialogBoxInfo[11].sY;
			}
		}
		break;

	case 15:
		break;

	case 24:
		if (m_bIsDialogEnabled[24] == FALSE)
		{
			m_stDialogBoxInfo[24].cMode = cType;
			m_stDialogBoxInfo[24].sV1 = sV1;
			m_stDialogBoxInfo[24].sV2 = sV2;
			m_bSkillUsingStatus = TRUE;
		}
		break;

	case 25:
		if (m_bIsDialogEnabled[25] == FALSE) {
			m_stDialogBoxInfo[25].cMode = cType;
			m_stDialogBoxInfo[25].sV1 = sV1;
			m_stDialogBoxInfo[25].sV2 = sV2;
		}
		break;

	case 26:
		switch (cType) {
		case 1:
		case 2: //
			if (m_bIsDialogEnabled[26] == FALSE)
			{
				m_stDialogBoxInfo[26].cMode = cType;
				m_stDialogBoxInfo[26].sV1 = -1;
				m_stDialogBoxInfo[26].sV2 = -1;
				m_stDialogBoxInfo[26].sV3 = -1;
				m_stDialogBoxInfo[26].sV4 = -1;
				m_stDialogBoxInfo[26].sV5 = -1;
				m_stDialogBoxInfo[26].sV6 = -1;
				m_stDialogBoxInfo[26].cStr[0] = 0;
				m_bSkillUsingStatus = TRUE;
				m_stDialogBoxInfo[26].sSizeX = 195;
				m_stDialogBoxInfo[26].sSizeY = 215;
				DisableDialogBox(17);
				DisableDialogBox(20);
				DisableDialogBox(23);
			}
			break;

		case 3:	//
			if (m_bIsDialogEnabled[26] == FALSE)
			{
				m_stDialogBoxInfo[26].sView = 0;
				m_stDialogBoxInfo[26].cMode = cType;
				m_stDialogBoxInfo[26].sV1 = -1;
				m_stDialogBoxInfo[26].sV2 = -1;
				m_stDialogBoxInfo[26].sV3 = -1;
				m_stDialogBoxInfo[26].sV4 = -1;
				m_stDialogBoxInfo[26].sV5 = -1;
				m_stDialogBoxInfo[26].sV6 = -1;
				m_stDialogBoxInfo[26].cStr[0] = 0;
				m_stDialogBoxInfo[26].cStr[1] = 0;
				m_stDialogBoxInfo[26].cStr[4] = 0;
				m_bSkillUsingStatus = TRUE;
				_bCheckBuildItemStatus();
				//m_stDialogBoxInfo[26].sX = 0;
				//m_stDialogBoxInfo[26].sY = 0;
				m_stDialogBoxInfo[26].sSizeX = 270;
				m_stDialogBoxInfo[26].sSizeY = 381;
				DisableDialogBox(17);
				DisableDialogBox(20);
				DisableDialogBox(23);
			}
			break;

		case 6:
			if (m_bIsDialogEnabled[26] == FALSE)
			{
				m_stDialogBoxInfo[26].cMode = cType;
				m_stDialogBoxInfo[26].cStr[2] = sV1;
				m_stDialogBoxInfo[26].cStr[3] = sV2;
				m_stDialogBoxInfo[26].sSizeX = 270;
				m_stDialogBoxInfo[26].sSizeY = 381;
				m_bSkillUsingStatus = TRUE;
				_bCheckBuildItemStatus();
				DisableDialogBox(17);
				DisableDialogBox(20);
				DisableDialogBox(23);
			}
			break;
			// Crafting
		case 7:
		case 8:
			if (m_bIsDialogEnabled[26] == FALSE)
			{
				m_stDialogBoxInfo[26].cMode = cType;
				m_stDialogBoxInfo[26].sV1 = -1;
				m_stDialogBoxInfo[26].sV2 = -1;
				m_stDialogBoxInfo[26].sV3 = -1;
				m_stDialogBoxInfo[26].sV4 = -1;
				m_stDialogBoxInfo[26].sV5 = -1;
				m_stDialogBoxInfo[26].sV6 = -1;
				m_stDialogBoxInfo[26].cStr[0] = 0;
				m_stDialogBoxInfo[26].cStr[1] = 0;
				m_bSkillUsingStatus = TRUE;
				//_bCheckCraftItemStatus();
				m_stDialogBoxInfo[26].sSizeX = 195;
				m_stDialogBoxInfo[26].sSizeY = 215;
				DisableDialogBox(17);
				DisableDialogBox(20);
				DisableDialogBox(23);
			}
			break;
		}
		break;

	case 27: // Snoopy: 7 mar 06 (multitrade) case rewriten
		if (m_bIsDialogEnabled[27] == FALSE)
		{
			m_stDialogBoxInfo[27].cMode = cType;
			for (i = 0; i<8; i++)
			{
				ZeroMemory(m_stDialogBoxExchangeInfo[i].cStr1, sizeof(m_stDialogBoxExchangeInfo[i].cStr1));
				ZeroMemory(m_stDialogBoxExchangeInfo[i].cStr2, sizeof(m_stDialogBoxExchangeInfo[i].cStr2));
				m_stDialogBoxExchangeInfo[i].sV1 = -1;
				m_stDialogBoxExchangeInfo[i].sV2 = -1;
				m_stDialogBoxExchangeInfo[i].sV3 = -1;
				m_stDialogBoxExchangeInfo[i].sV4 = -1;
				m_stDialogBoxExchangeInfo[i].sV5 = -1;
				m_stDialogBoxExchangeInfo[i].sV6 = -1;
				m_stDialogBoxExchangeInfo[i].sV7 = -1;
				m_stDialogBoxExchangeInfo[i].dwV1 = 0;
			}
			DisableDialogBox(17);
			DisableDialogBox(20);
			DisableDialogBox(23);
			DisableDialogBox(26);
		}
		break;

	case 41: // Snoopy: 7 mar 06 (MultiTrade) Confirmation dialog
		break;

	case 28:
		if (m_bIsDialogEnabled[28] == FALSE) {
			m_stDialogBoxInfo[28].cMode = cType;
			m_stDialogBoxInfo[28].sX = m_stDialogBoxInfo[1].sX + 20;
			m_stDialogBoxInfo[28].sY = m_stDialogBoxInfo[1].sY + 20;
		}
		break;

	case 32:
		if (m_bIsDialogEnabled[32] == FALSE) {
			m_stDialogBoxInfo[32].cMode = cType;
			m_stDialogBoxInfo[32].sX = m_stDialogBoxInfo[1].sX + 20;
			m_stDialogBoxInfo[32].sY = m_stDialogBoxInfo[1].sY + 20;
		}
		break;

	case 33:
		if ((m_iHP <= 0) || (m_bCitizen == FALSE)) return;
		if (m_bIsDialogEnabled[33] == FALSE)
		{
			m_stDialogBoxInfo[33].cMode = cType;
			m_stDialogBoxInfo[33].sX = 360;
			m_stDialogBoxInfo[33].sY = 65;
			m_stDialogBoxInfo[33].sV1 = sV1;
		}
		break;

	case 34:
		if (m_bIsDialogEnabled[34] == FALSE)
		{
			int iSoX, iSoM;
			iSoX = iSoM = 0;
			for (i = 0; i < MAXITEMS; i++)
			if (m_pItemList[i] != NULL)
			{
				if ((m_pItemList[i]->m_sSprite == 6) && (m_pItemList[i]->m_sSpriteFrame == 128)) iSoX++;
				if ((m_pItemList[i]->m_sSprite == 6) && (m_pItemList[i]->m_sSpriteFrame == 129)) iSoM++;
			}
			if ((iSoX > 0) || (iSoM > 0))
			{
				m_stDialogBoxInfo[34].cMode = 6; // Stone upgrade
				m_stDialogBoxInfo[34].sV2 = iSoX;
				m_stDialogBoxInfo[34].sV3 = iSoM;
				m_stDialogBoxInfo[34].sV1 = -1;
				m_stDialogBoxInfo[34].dwV1 = NULL;
			}
			else if (m_iGizonItemUpgradeLeft >0)
			{
				m_stDialogBoxInfo[34].cMode = 1;
				m_stDialogBoxInfo[34].sV2 = -1;
				m_stDialogBoxInfo[34].sV3 = -1;
				m_stDialogBoxInfo[34].sV1 = -1;
				m_stDialogBoxInfo[34].dwV1 = NULL;
			}
			else
			{
				AddEventList(DRAW_DIALOGBOX_ITEMUPGRADE30, 10); // "Stone of Xelima or Merien is not present."
				return;
			}
		}
		break;

	case 16:
		if (m_bIsDialogEnabled[iBoxID] == FALSE) {
			if (m_cSkillMastery[4] == 0) {
				DisableDialogBox(16);
				EnableDialogBox(21, 0, 480, 0);
				return;
			}
			else {
				m_stDialogBoxInfo[iBoxID].cMode = 0;
				m_stDialogBoxInfo[iBoxID].sView = 0;
			}
		}
		break;

	case 14:
		EndInputString();
		if (m_bIsDialogEnabled[iBoxID] == FALSE) {
			m_stDialogBoxInfo[iBoxID].cMode = 0;
			m_stDialogBoxInfo[iBoxID].sView = 0;
			EnableDialogBox(2, NULL, NULL, NULL);
		}
		break;

	case 40: // Slates
		if (m_bIsDialogEnabled[40] == FALSE) {
			m_stDialogBoxInfo[40].sView = 0;
			m_stDialogBoxInfo[40].cMode = cType;
			m_stDialogBoxInfo[40].sV1 = -1;
			m_stDialogBoxInfo[40].sV2 = -1;
			m_stDialogBoxInfo[40].sV3 = -1;
			m_stDialogBoxInfo[40].sV4 = -1;
			m_stDialogBoxInfo[40].sV5 = -1;
			m_stDialogBoxInfo[40].sV6 = -1;
			m_stDialogBoxInfo[40].cStr[0] = 0;
			m_stDialogBoxInfo[40].cStr[1] = 0;
			m_stDialogBoxInfo[40].cStr[4] = 0;

			m_stDialogBoxInfo[40].sSizeX = 180;
			m_stDialogBoxInfo[40].sSizeY = 183;

			DisableDialogBox(17);
			DisableDialogBox(20);
			DisableDialogBox(23);
			DisableDialogBox(26);
		}
		break;
	case 42: // Diuuude: Change stats window
		if (m_bIsDialogEnabled[42] == FALSE) {
			m_stDialogBoxInfo[42].sX = m_stDialogBoxInfo[12].sX + 10;
			m_stDialogBoxInfo[42].sY = m_stDialogBoxInfo[12].sY + 10;
			m_stDialogBoxInfo[42].cMode = 0;
			m_stDialogBoxInfo[42].sView = 0;
			m_bSkillUsingStatus = FALSE;
		}
		break;
	case 50: // Snoopy: Resurection
		if (m_bIsDialogEnabled[50] == FALSE)
		{
			m_stDialogBoxInfo[50].sX = 185;
			m_stDialogBoxInfo[50].sY = 100;
			m_stDialogBoxInfo[50].cMode = 0;
			m_stDialogBoxInfo[50].sView = 0;
			m_bSkillUsingStatus = FALSE;
		}
		break;

	default:
		EndInputString();
		if (m_bIsDialogEnabled[iBoxID] == FALSE) {
			m_stDialogBoxInfo[iBoxID].cMode = 0;
			m_stDialogBoxInfo[iBoxID].sView = 0;
		}
		break;
	}
	if (iBoxID != 30)
	{
		if (m_bIsDialogEnabled[iBoxID] == FALSE)
		{
			if (m_stDialogBoxInfo[iBoxID].sY > 400) m_stDialogBoxInfo[iBoxID].sY = 410;
			if (m_stDialogBoxInfo[iBoxID].sX > 620) m_stDialogBoxInfo[iBoxID].sX = 620;
			if ((m_stDialogBoxInfo[iBoxID].sX + m_stDialogBoxInfo[iBoxID].sSizeX) < 10) m_stDialogBoxInfo[iBoxID].sX += 20;
			if ((m_stDialogBoxInfo[iBoxID].sY + m_stDialogBoxInfo[iBoxID].sSizeY) < 10) m_stDialogBoxInfo[iBoxID].sY += 20;
		}
	}
	m_bIsDialogEnabled[iBoxID] = TRUE;
	if (pString != NULL) strcpy(m_stDialogBoxInfo[iBoxID].cStr, pString);
	//Snoopy: 39->59
	for (i = 0; i < 59; i++)
	if (m_cDialogBoxOrder[i] == iBoxID) m_cDialogBoxOrder[i] = NULL;
	//Snoopy: 39->59
	for (i = 1; i < 59; i++)
	if ((m_cDialogBoxOrder[i - 1] == NULL) && (m_cDialogBoxOrder[i] != NULL)) {
		m_cDialogBoxOrder[i - 1] = m_cDialogBoxOrder[i];
		m_cDialogBoxOrder[i] = NULL;
	}
	//Snoopy: 39->59
	for (i = 0; i < 59; i++)
	if (m_cDialogBoxOrder[i] == NULL) {
		m_cDialogBoxOrder[i] = iBoxID;
		return;
	}
}

void cMain::DisableDialogBox(int iBoxID)
{
	int i;

	switch (iBoxID) {
	case 4:
		m_bIsItemDisabled[m_stDialogBoxInfo[4].sView] = FALSE;
		break;

	case 5:
		m_bIsItemDisabled[m_stDialogBoxInfo[5].sView] = FALSE;
		break;

	case 6:
		m_bIsItemDisabled[m_stDialogBoxInfo[6].sView] = FALSE;
		break;

	case 7:
		if (m_stDialogBoxInfo[7].cMode == 1)
			EndInputString();
		m_stDialogBoxInfo[7].cMode = 0;
		break;

	case 11:
		for (i = 0; i < MAXMENUITEMS; i++)
		if (m_pItemForSaleList[i] != NULL) {
			delete m_pItemForSaleList[i];
			m_pItemForSaleList[i] = NULL;
		}
		m_stDialogBoxInfo[39].sV3 = 0;
		m_stDialogBoxInfo[39].sV4 = 0; // v1.4
		m_stDialogBoxInfo[39].sV5 = 0;
		m_stDialogBoxInfo[39].sV6 = 0;
		break;

	case 14:
		if (m_stDialogBoxInfo[14].cMode < 0) return;
		break;

	case 17:
		if (m_stDialogBoxInfo[17].cMode == 1) {
			EndInputString();
			m_bIsItemDisabled[m_stDialogBoxInfo[17].sView] = FALSE;
		}
		break;

	case 20: // v1.4
		m_bIsItemDisabled[m_stDialogBoxInfo[20].sV1] = FALSE;
		break;

	case 21:
		if (m_stDialogBoxInfo[21].sV2 == 500)
		{	//bSendCommand(MSGID_COMMAND_COMMON, COMMONTYPE_GETMAGICABILITY, NULL, NULL, NULL, NULL, NULL);
		}
		break;

	case 24:
		m_bSkillUsingStatus = FALSE;
		break;

	case 26:
		if (m_stDialogBoxInfo[26].sV1 != -1) m_bIsItemDisabled[m_stDialogBoxInfo[26].sV1] = FALSE;
		if (m_stDialogBoxInfo[26].sV2 != -1) m_bIsItemDisabled[m_stDialogBoxInfo[26].sV2] = FALSE;
		if (m_stDialogBoxInfo[26].sV3 != -1) m_bIsItemDisabled[m_stDialogBoxInfo[26].sV3] = FALSE;
		if (m_stDialogBoxInfo[26].sV4 != -1) m_bIsItemDisabled[m_stDialogBoxInfo[26].sV4] = FALSE;
		if (m_stDialogBoxInfo[26].sV5 != -1) m_bIsItemDisabled[m_stDialogBoxInfo[26].sV5] = FALSE;
		if (m_stDialogBoxInfo[26].sV6 != -1) m_bIsItemDisabled[m_stDialogBoxInfo[26].sV6] = FALSE;
		m_bSkillUsingStatus = FALSE;
		break;

	case 27: //Snoopy: 7 mar 06 (multiTrade) case rewriten
		for (i = 0; i<8; i++)
		{
			ZeroMemory(m_stDialogBoxExchangeInfo[i].cStr1, sizeof(m_stDialogBoxExchangeInfo[i].cStr1));
			ZeroMemory(m_stDialogBoxExchangeInfo[i].cStr2, sizeof(m_stDialogBoxExchangeInfo[i].cStr2));
			m_stDialogBoxExchangeInfo[i].sV1 = -1;
			m_stDialogBoxExchangeInfo[i].sV2 = -1;
			m_stDialogBoxExchangeInfo[i].sV3 = -1;
			m_stDialogBoxExchangeInfo[i].sV4 = -1;
			m_stDialogBoxExchangeInfo[i].sV5 = -1;
			m_stDialogBoxExchangeInfo[i].sV6 = -1;
			m_stDialogBoxExchangeInfo[i].sV7 = -1;
			m_stDialogBoxExchangeInfo[i].dwV1 = 0;
			if (m_bIsItemDisabled[m_stDialogBoxExchangeInfo[i].sItemID] == TRUE)
				m_bIsItemDisabled[m_stDialogBoxExchangeInfo[i].sItemID] = FALSE;
		}
		break;


	case 31:
		for (i = 0; i < MAXSELLLIST; i++)
		{
			if (m_stSellItemList[i].iIndex != -1) m_bIsItemDisabled[m_stSellItemList[i].iIndex] = FALSE;
			m_stSellItemList[i].iIndex = -1;
			m_stSellItemList[i].iAmount = 0;
		}
		break;

	case 34:
		if (m_stDialogBoxInfo[34].sV1 != -1)
			m_bIsItemDisabled[m_stDialogBoxInfo[34].sV1] = FALSE;
		break;

	case 40:
		m_bIsItemDisabled[m_stDialogBoxInfo[40].sV1] = FALSE;
		m_bIsItemDisabled[m_stDialogBoxInfo[40].sV2] = FALSE;
		m_bIsItemDisabled[m_stDialogBoxInfo[40].sV3] = FALSE;
		m_bIsItemDisabled[m_stDialogBoxInfo[40].sV4] = FALSE;

		ZeroMemory(m_stDialogBoxInfo[40].cStr, sizeof(m_stDialogBoxInfo[40].cStr));
		ZeroMemory(m_stDialogBoxInfo[40].cStr2, sizeof(m_stDialogBoxInfo[40].cStr2));
		ZeroMemory(m_stDialogBoxInfo[40].cStr3, sizeof(m_stDialogBoxInfo[40].cStr3));
		ZeroMemory(m_stDialogBoxInfo[40].cStr4, sizeof(m_stDialogBoxInfo[40].cStr4));
		m_stDialogBoxInfo[40].sV1 = -1;
		m_stDialogBoxInfo[40].sV2 = -1;
		m_stDialogBoxInfo[40].sV3 = -1;
		m_stDialogBoxInfo[40].sV4 = -1;
		m_stDialogBoxInfo[40].sV5 = -1;
		m_stDialogBoxInfo[40].sV6 = -1;
		m_stDialogBoxInfo[40].sV9 = -1;
		m_stDialogBoxInfo[40].sV10 = -1;
		m_stDialogBoxInfo[40].sV11 = -1;
		m_stDialogBoxInfo[40].sV12 = -1;
		m_stDialogBoxInfo[40].sV13 = -1;
		m_stDialogBoxInfo[40].sV14 = -1;
		m_stDialogBoxInfo[40].dwV1 = 0;
		m_stDialogBoxInfo[40].dwV2 = 0;
		break;

	case 42:
		cStateChange1 = 0;
		cStateChange2 = 0;
		cStateChange3 = 0;
		/*	m_cLU_Str = 0;
		m_cLU_Vit = 0;
		m_cLU_Dex = 0;
		m_cLU_Int = 0;
		m_cLU_Mag = 0;
		m_cLU_Char = 0;*/
		break;

	}

	m_bIsDialogEnabled[iBoxID] = FALSE;
	// Snoopy: 39->59
	for (i = 0; i < 59; i++)
	if (m_cDialogBoxOrder[i] == iBoxID)
		m_cDialogBoxOrder[i] = NULL;

	// Snoopy: 39->59
	for (i = 1; i < 59; i++)
	if ((m_cDialogBoxOrder[i - 1] == NULL) && (m_cDialogBoxOrder[i] != NULL))
	{
		m_cDialogBoxOrder[i - 1] = m_cDialogBoxOrder[i];
		m_cDialogBoxOrder[i] = NULL;
	}
}


int cMain::iGetTopDialogBoxIndex()
{
	int i;
	//Snoopy: 38->58
	for (i = 58; i >= 0; i--)
	if (m_cDialogBoxOrder[i] != NULL)
		return m_cDialogBoxOrder[i];

	return NULL;
}
int cMain::GetCharKind(char *str, int index)
{
	int kind = 1;
	do
	{
		if (kind == 2) kind = 3;
		else
		{
			if ((unsigned char)*str < 128) kind = 1;
			else kind = 2;
		}
		str++;
		index--;
	} while (index >= 0);
	return kind;
}
BOOL cMain::_bCheckBuildItemStatus()
{
	int iIndex, i, j, iMatch, iCount;
	char cTempName[21];
	int  iItemCount[MAXITEMS];

	for (i = 0; i < MAXBUILDITEMS; i++)
	if (m_pDispBuildItemList[i] != NULL)
	{
		delete m_pDispBuildItemList[i];
		m_pDispBuildItemList[i] = NULL;
	}
	iIndex = 0;
	for (i = 0; i < MAXBUILDITEMS; i++)
	if (m_pBuildItemList[i] != NULL)
	{	// Skill-Limit
		if (m_cSkillMastery[13] >= m_pBuildItemList[i]->m_iSkillLimit)
		{
			iMatch = 0;
			m_pDispBuildItemList[iIndex] = new class CBuildItem;
			memcpy(m_pDispBuildItemList[iIndex]->m_cName, m_pBuildItemList[i]->m_cName, 20);

			memcpy(m_pDispBuildItemList[iIndex]->m_cElementName1, m_pBuildItemList[i]->m_cElementName1, 20);
			memcpy(m_pDispBuildItemList[iIndex]->m_cElementName2, m_pBuildItemList[i]->m_cElementName2, 20);
			memcpy(m_pDispBuildItemList[iIndex]->m_cElementName3, m_pBuildItemList[i]->m_cElementName3, 20);
			memcpy(m_pDispBuildItemList[iIndex]->m_cElementName4, m_pBuildItemList[i]->m_cElementName4, 20);
			memcpy(m_pDispBuildItemList[iIndex]->m_cElementName5, m_pBuildItemList[i]->m_cElementName5, 20);
			memcpy(m_pDispBuildItemList[iIndex]->m_cElementName6, m_pBuildItemList[i]->m_cElementName6, 20);

			m_pDispBuildItemList[iIndex]->m_iElementCount[1] = m_pBuildItemList[i]->m_iElementCount[1];
			m_pDispBuildItemList[iIndex]->m_iElementCount[2] = m_pBuildItemList[i]->m_iElementCount[2];
			m_pDispBuildItemList[iIndex]->m_iElementCount[3] = m_pBuildItemList[i]->m_iElementCount[3];
			m_pDispBuildItemList[iIndex]->m_iElementCount[4] = m_pBuildItemList[i]->m_iElementCount[4];
			m_pDispBuildItemList[iIndex]->m_iElementCount[5] = m_pBuildItemList[i]->m_iElementCount[5];
			m_pDispBuildItemList[iIndex]->m_iElementCount[6] = m_pBuildItemList[i]->m_iElementCount[6];

			m_pDispBuildItemList[iIndex]->m_iSprH = m_pBuildItemList[i]->m_iSprH;
			m_pDispBuildItemList[iIndex]->m_iSprFrame = m_pBuildItemList[i]->m_iSprFrame;
			m_pDispBuildItemList[iIndex]->m_iMaxSkill = m_pBuildItemList[i]->m_iMaxSkill;
			m_pDispBuildItemList[iIndex]->m_iSkillLimit = m_pBuildItemList[i]->m_iSkillLimit;

			// ItemCount
			for (j = 0; j < MAXITEMS; j++)
			if (m_pItemList[j] != NULL)
				iItemCount[j] = m_pItemList[j]->m_dwCount;
			else iItemCount[j] = 0;

			// Element1
			ZeroMemory(cTempName, sizeof(cTempName));
			memcpy(cTempName, m_pBuildItemList[i]->m_cElementName1, 20);
			iCount = m_pBuildItemList[i]->m_iElementCount[1];
			if (iCount == 0) iMatch++;
			else
			{
				for (j = 0; j < MAXITEMS; j++)
				if (m_pItemList[j] != NULL) {
					if ((memcmp(m_pItemList[j]->m_cName, cTempName, 20) == 0) && (m_pItemList[j]->m_dwCount >= (DWORD)(iCount)) &&
						(iItemCount[j] > 0))
					{
						iMatch++;
						m_pDispBuildItemList[iIndex]->m_bElementFlag[1] = TRUE;
						iItemCount[j] -= iCount;
						goto CBIS_STEP2;
					}
				}
			}

		CBIS_STEP2:;
			// Element2
			ZeroMemory(cTempName, sizeof(cTempName));
			memcpy(cTempName, m_pBuildItemList[i]->m_cElementName2, 20);
			iCount = m_pBuildItemList[i]->m_iElementCount[2];
			if (iCount == 0) iMatch++;
			else
			{
				for (j = 0; j < MAXITEMS; j++)
				if (m_pItemList[j] != NULL)
				{
					if ((memcmp(m_pItemList[j]->m_cName, cTempName, 20) == 0) && (m_pItemList[j]->m_dwCount >= (DWORD)(iCount)) &&
						(iItemCount[j] > 0))
					{
						iMatch++;
						m_pDispBuildItemList[iIndex]->m_bElementFlag[2] = TRUE;
						iItemCount[j] -= iCount;
						goto CBIS_STEP3;
					}
				}
			}

		CBIS_STEP3:;
			// Element3
			ZeroMemory(cTempName, sizeof(cTempName));
			memcpy(cTempName, m_pBuildItemList[i]->m_cElementName3, 20);
			iCount = m_pBuildItemList[i]->m_iElementCount[3];
			if (iCount == 0) iMatch++;
			else
			{
				for (j = 0; j < MAXITEMS; j++)
				if (m_pItemList[j] != NULL)
				{
					if ((memcmp(m_pItemList[j]->m_cName, cTempName, 20) == 0) && (m_pItemList[j]->m_dwCount >= (DWORD)(iCount)) &&
						(iItemCount[j] > 0))
					{
						iMatch++;
						m_pDispBuildItemList[iIndex]->m_bElementFlag[3] = TRUE;
						iItemCount[j] -= iCount;
						goto CBIS_STEP4;
					}
				}
			}

		CBIS_STEP4:;
			// Element4 ∞ÀªÁ
			ZeroMemory(cTempName, sizeof(cTempName));
			memcpy(cTempName, m_pBuildItemList[i]->m_cElementName4, 20);
			iCount = m_pBuildItemList[i]->m_iElementCount[4];
			if (iCount == 0) iMatch++;
			else
			{
				for (j = 0; j < MAXITEMS; j++)
				if (m_pItemList[j] != NULL)
				{
					if ((memcmp(m_pItemList[j]->m_cName, cTempName, 20) == 0) && (m_pItemList[j]->m_dwCount >= (DWORD)(iCount)) &&
						(iItemCount[j] > 0))
					{
						iMatch++;
						m_pDispBuildItemList[iIndex]->m_bElementFlag[4] = TRUE;
						iItemCount[j] -= iCount;
						goto CBIS_STEP5;
					}
				}
			}

		CBIS_STEP5:;

			// Element5
			ZeroMemory(cTempName, sizeof(cTempName));
			memcpy(cTempName, m_pBuildItemList[i]->m_cElementName5, 20);
			iCount = m_pBuildItemList[i]->m_iElementCount[5];
			if (iCount == 0) iMatch++;
			else
			{
				for (j = 0; j < MAXITEMS; j++)
				if (m_pItemList[j] != NULL)
				{
					if ((memcmp(m_pItemList[j]->m_cName, cTempName, 20) == 0) && (m_pItemList[j]->m_dwCount >= (DWORD)(iCount)) &&
						(iItemCount[j] > 0))
					{
						iMatch++;
						m_pDispBuildItemList[iIndex]->m_bElementFlag[5] = TRUE;
						iItemCount[j] -= iCount;
						goto CBIS_STEP6;
					}
				}
			}

		CBIS_STEP6:;

			// Element6
			ZeroMemory(cTempName, sizeof(cTempName));
			memcpy(cTempName, m_pBuildItemList[i]->m_cElementName6, 20);
			iCount = m_pBuildItemList[i]->m_iElementCount[6];
			if (iCount == 0) iMatch++;
			else
			{
				for (j = 0; j < MAXITEMS; j++)
				if (m_pItemList[j] != NULL)
				{
					if ((memcmp(m_pItemList[j]->m_cName, cTempName, 20) == 0) && (m_pItemList[j]->m_dwCount >= (DWORD)(iCount)) &&
						(iItemCount[j] > 0))
					{
						iMatch++;
						m_pDispBuildItemList[iIndex]->m_bElementFlag[6] = TRUE;
						iItemCount[j] -= iCount;
						goto CBIS_STEP7;
					}
				}
			}

		CBIS_STEP7:;

			if (iMatch == 6) m_pDispBuildItemList[iIndex]->m_bBuildEnabled = TRUE;
			iIndex++;
		}
	}
	return TRUE;
}

int cMain::_iLoadTextDlgContents2(int iType)
{
	char * pContents, *token, cTemp[120], cFileName[120];
	char   seps[] = "\n";
	int    iIndex = 0, i;
	class  CStrTok * pStrTok;
	DWORD  dwFileSize;
	HANDLE hFile;
	FILE * pFile;
	for (i = 0; i < TEXTDLGMAXLINES; i++)
	{
		if (m_pMsgTextList2[i] != NULL)
			delete m_pMsgTextList2[i];
		m_pMsgTextList2[i] = NULL;
	}
	// cType
	ZeroMemory(cTemp, sizeof(cTemp));
	ZeroMemory(cFileName, sizeof(cFileName));

	wsprintf(cTemp, "contents%d", iType);

	strcat(cFileName, "contents");
	strcat(cFileName, "\\");
	strcat(cFileName, "\\");
	strcat(cFileName, cTemp);
	strcat(cFileName, ".txt");
	hFile = CreateFile(cFileName, GENERIC_READ, NULL, NULL, OPEN_EXISTING, NULL, NULL);
	dwFileSize = GetFileSize(hFile, NULL);
	if (hFile != INVALID_HANDLE_VALUE) CloseHandle(hFile);
	pFile = fopen(cFileName, "rt");
	if (pFile == NULL) return -1;
	else
	{
		pContents = new char[dwFileSize + 1];
		if (pContents == NULL) return -1;
		ZeroMemory(pContents, dwFileSize + 1);
		fread(pContents, dwFileSize, 1, pFile);
	}
	fclose(pFile);
	pStrTok = new class CStrTok(pContents, seps);
	token = pStrTok->pGet();
	while (token != NULL)
	{
		m_pMsgTextList2[iIndex] = new class CMsg(NULL, token, NULL);
		token = pStrTok->pGet();
		iIndex++;
	}
	delete pStrTok;
	delete[] pContents;
	return iIndex;
}

void cMain::AddEventList(char * pTxt, char cColor, BOOL bDupAllow)
{
	int i;
	if ((bDupAllow == FALSE) && (strcmp(m_stEventHistory[5].cTxt, pTxt) == 0)) return;
	if (cColor == 10)
	{
		for (i = 1; i < 6; i++)
		{
			strcpy(m_stEventHistory2[i - 1].cTxt, m_stEventHistory2[i].cTxt);
			m_stEventHistory2[i - 1].cColor = m_stEventHistory2[i].cColor;
			m_stEventHistory2[i - 1].dwTime = m_stEventHistory2[i].dwTime;
		}
		ZeroMemory(m_stEventHistory2[5].cTxt, sizeof(m_stEventHistory2[5].cTxt));
		strcpy(m_stEventHistory2[5].cTxt, pTxt);
		m_stEventHistory2[5].cColor = cColor;
		m_stEventHistory2[5].dwTime = m_dwCurTime;
	}
	else
	{
		for (i = 1; i < 6; i++)
		{
			strcpy(m_stEventHistory[i - 1].cTxt, m_stEventHistory[i].cTxt);
			m_stEventHistory[i - 1].cColor = m_stEventHistory[i].cColor;
			m_stEventHistory[i - 1].dwTime = m_stEventHistory[i].dwTime;
		}
		ZeroMemory(m_stEventHistory[5].cTxt, sizeof(m_stEventHistory[5].cTxt));
		strcpy(m_stEventHistory[5].cTxt, pTxt);
		m_stEventHistory[5].cColor = cColor;
		m_stEventHistory[5].dwTime = m_dwCurTime;
	}
}
void cMain::_LoadShopMenuContents(char cType)
{
	char cFileName[255], cTemp[255];
	HANDLE hFile;
	FILE * pFile;
	DWORD  dwFileSize;
	char * pBuffer;

	ZeroMemory(cTemp, sizeof(cTemp));
	ZeroMemory(cFileName, sizeof(cFileName));
	wsprintf(cTemp, "contents%d", cType);
	strcat(cFileName, "contents");
	strcat(cFileName, "\\");
	strcat(cFileName, "\\");
	strcat(cFileName, cTemp);
	strcat(cFileName, ".txt");

	hFile = CreateFile(cFileName, GENERIC_READ, NULL, NULL, OPEN_EXISTING, NULL, NULL);
	dwFileSize = GetFileSize(hFile, NULL);
	if (hFile != INVALID_HANDLE_VALUE) CloseHandle(hFile);

	pFile = fopen(cFileName, "rt");
	if (pFile == NULL) return;
	else {
		pBuffer = new char[dwFileSize + 1];
		ZeroMemory(pBuffer, dwFileSize + 1);
		fread(pBuffer, dwFileSize, 1, pFile);

		__bDecodeContentsAndBuildItemForSaleList(pBuffer);
		delete[] pBuffer;
	}
	fclose(pFile);
}
BOOL cMain::__bDecodeContentsAndBuildItemForSaleList(char * pBuffer)
{
	char * pContents, *token;
	char seps[] = "= ,\t\n";
	char cReadModeA = 0;
	char cReadModeB = 0;
	int  iItemForSaleListIndex = 0;
	class CStrTok * pStrTok;

	pContents = pBuffer;

	pStrTok = new class CStrTok(pContents, seps);
	token = pStrTok->pGet();
	while (token != NULL) {
		if (cReadModeA != 0) {
			//
			switch (cReadModeA) {
			case 1:
				switch (cReadModeB) {
				case 1:
					ZeroMemory(m_pItemForSaleList[iItemForSaleListIndex]->m_cName, sizeof(m_pItemForSaleList[iItemForSaleListIndex]->m_cName));
					memcpy(m_pItemForSaleList[iItemForSaleListIndex]->m_cName, token, strlen(token));
					cReadModeB = 2;
					break;
				case 2:	// m_cItemType
					m_pItemForSaleList[iItemForSaleListIndex]->m_cItemType = atoi(token);
					cReadModeB = 3;
					break;
				case 3: // m_cEquipPos
					m_pItemForSaleList[iItemForSaleListIndex]->m_cEquipPos = atoi(token);
					cReadModeB = 4;
					break;
				case 4: // m_sItemEffectType
					//m_pItemForSaleList[iItemForSaleListIndex]->m_sItemEffectType = atoi(token);
					cReadModeB = 5;
					break;
				case 5:	// m_sItemEffectValue1
					m_pItemForSaleList[iItemForSaleListIndex]->m_sItemEffectValue1 = atoi(token);
					cReadModeB = 6;
					break;
				case 6: // m_sItemEffectValue2
					m_pItemForSaleList[iItemForSaleListIndex]->m_sItemEffectValue2 = atoi(token);
					cReadModeB = 7;
					break;
				case 7: // m_sItemEffectValue3
					m_pItemForSaleList[iItemForSaleListIndex]->m_sItemEffectValue3 = atoi(token);
					cReadModeB = 8;
					break;
				case 8: // m_sItemEffectValue4
					m_pItemForSaleList[iItemForSaleListIndex]->m_sItemEffectValue4 = atoi(token);
					cReadModeB = 9;
					break;
				case 9: // m_sItemEffectValue5
					m_pItemForSaleList[iItemForSaleListIndex]->m_sItemEffectValue5 = atoi(token);
					cReadModeB = 10;
					break;
				case 10: // m_sItemEffectValue6
					m_pItemForSaleList[iItemForSaleListIndex]->m_sItemEffectValue6 = atoi(token);
					cReadModeB = 11;
					break;
				case 11: // m_wMaxLifeSpan
					m_pItemForSaleList[iItemForSaleListIndex]->m_wMaxLifeSpan = (WORD)atoi(token);
					cReadModeB = 12;
					break;
				case 12: // m_sMaxFixCount
					//m_pItemForSaleList[iItemForSaleListIndex]->m_sMaxFixCount = atoi(token);
					cReadModeB = 13;
					break;
				case 13: // m_sSprite
					m_pItemForSaleList[iItemForSaleListIndex]->m_sSprite = atoi(token);
					cReadModeB = 14;
					break;
				case 14: // m_sSpriteFrame
					m_pItemForSaleList[iItemForSaleListIndex]->m_sSpriteFrame = atoi(token);
					cReadModeB = 15;
					break;
				case 15: // m_wPrice
					m_pItemForSaleList[iItemForSaleListIndex]->m_wPrice = atoi(token);
					cReadModeB = 16;
					break;
				case 16: // m_wWeight
					m_pItemForSaleList[iItemForSaleListIndex]->m_wWeight = atoi(token);
					cReadModeB = 17;
					break;
				case 17: // Appr Value
					//m_pItemForSaleList[iItemForSaleListIndex]->m_cApprValue = atoi(token);
					cReadModeB = 18;
					break;
				case 18: // m_cSpeed
					m_pItemForSaleList[iItemForSaleListIndex]->m_cSpeed = atoi(token);
					cReadModeB = 19;
					break;
				case 19: // Level Limit
					m_pItemForSaleList[iItemForSaleListIndex]->m_sLevelLimit = atoi(token);
					m_pItemForSaleList[iItemForSaleListIndex]->m_dwCount = 1;
					cReadModeA = 0;
					cReadModeB = 0;
					iItemForSaleListIndex++;
					break;
				}
				break;

			default:
				break;
			}
		}
		else
		{
			if (memcmp(token, "ItemForSale", 4) == 0)
			{
				if (iItemForSaleListIndex >= MAXMENUITEMS)
				{
					delete pStrTok;
					return FALSE;
				}
				cReadModeA = 1;
				cReadModeB = 1;
				m_pItemForSaleList[iItemForSaleListIndex] = new class CItem;
			}
		}
		token = pStrTok->pGet();
	}
	delete pStrTok;
	if ((cReadModeA != 0) || (cReadModeB != 0)) return FALSE;
	return TRUE;
}


void cMain::_LoadTextDlgContents(int cType)
{
	char * pContents, *token, cTemp[120], cFileName[120];
	char   seps[] = "\n";
	int    iIndex = 0, i;
	class  CStrTok * pStrTok;
	DWORD  dwFileSize;
	HANDLE hFile;
	FILE * pFile;
	for (i = 0; i < TEXTDLGMAXLINES; i++)
	{
		if (m_pMsgTextList[i] != NULL)
			delete m_pMsgTextList[i];
		m_pMsgTextList[i] = NULL;
	}
	// cType
	ZeroMemory(cTemp, sizeof(cTemp));
	ZeroMemory(cFileName, sizeof(cFileName));

	wsprintf(cTemp, "contents%d", cType);
	strcat(cFileName, "contents");
	strcat(cFileName, "\\");
	strcat(cFileName, "\\");
	strcat(cFileName, cTemp);
	strcat(cFileName, ".txt");

	hFile = CreateFile(cFileName, GENERIC_READ, NULL, NULL, OPEN_EXISTING, NULL, NULL);
	dwFileSize = GetFileSize(hFile, NULL);
	if (hFile != INVALID_HANDLE_VALUE) CloseHandle(hFile);
	pFile = fopen(cFileName, "rt");
	if (pFile == NULL) return;
	else
	{
		pContents = new char[dwFileSize + 1];
		ZeroMemory(pContents, dwFileSize + 1);
		fread(pContents, dwFileSize, 1, pFile);
	}
	fclose(pFile);
	pStrTok = new class CStrTok(pContents, seps);
	token = pStrTok->pGet();
	while (token != NULL)
	{
		m_pMsgTextList[iIndex] = new class CMsg(NULL, token, NULL);
		token = pStrTok->pGet();
		iIndex++;
	}
	delete pStrTok;
	delete[] pContents;
}
