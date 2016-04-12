#ifndef _cMain_H
#define _cMain_H

#define _WINSOCKAPI_   /* Prevent inclusion of winsock.h in windows.h */
#include <windows.h>

#include <windows.h>
#include <windowsx.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <mmsystem.h>
#include <winbase.h>
#include <memory.h>
#include <string.h>
#include <process.h>
#include <direct.h>


#include "Application.h"
#include "cClient.h"
#include "cLoginServer.h"
#include "cSocket.h"
#include "../shared/NetworkMessages.h"
/*
#include "ActionID.h"
#include "BuildItem.h"
#include "CharInfo.h"
#include "Cint.h"
#include "Curse.h"
//#include "DXC_ddraw.h"
#include "DXC_dinput.h"
#include "DXC_dsound.h"
#include "DynamicObjectID.h"
#include "Effect.h"
#include "GameMonitor.h"
#include "GlobalDef.h"
#include "Item.h"
#include "ItemName.h"
#include "DXC_ddraw.h"
#include "LAN_ENG.h"
#include "Magic.h"
#include "MapData.h"
#include "Misc.h"
#include "MouseInterface.h"
#include "Msg.h"
#include "Skill.h"
#include "SoundBuffer.h"
#include "Sprite.h"
#include "SpriteID.h"
#include "StrTok.h"
#include "Tile.h"
#include "TileSpr.h"
#include "YWSound.h"
*/
#include "GlobalDef.h"
#include "DXC_ddraw.h"
#include "DXC_dinput.h"
#include "YWSound.h"
#include "SoundBuffer.h"
#include "Sprite.h"
#include "SpriteID.h"
#include "Misc.h"
#include "StrTok.h"
#include "Msg.h"
#include "Effect.h"
#include "MapData.h"
#include "ActionID.h"
#include "MouseInterface.h"
#include "CharInfo.h"
#include "Item.h"
#include "Magic.h"
#include "Skill.h"
#include "DynamicObjectID.h"
#include "GameMonitor.h"
#include "BuildItem.h"
#include "ItemName.h"
#include "Curse.h"
#include "Cint.h"
#include "LAN_ENG.h"

class cMain
{
public:
	cMain();
	//virtual ~cMain();
	~cMain();

	void StartUp();
	bool bReadConfigFile(char * filename);
	void OnKeyUp(WPARAM wParam, LPARAM lParam);
	void SocketsMessages(UINT message, WPARAM wParam, LPARAM lParam);
	void msgDecode();
	//
	bool bInit(HWND hWnd, HINSTANCE hInst, char * pCmdLine);
	void OnTimer();
	void UpdateScreen();
	void UpdateScreen_OnLoading(bool bActive);
	void UpdateScreen_OnLoading_Progress();
	void MakeSprite(char* FileName, short sStart, short sCount, bool bAlphaEffect = TRUE);
	void MakeTileSpr(char* FileName, short sStart, short sCount, bool bAlphaEffect = TRUE);
	void MakeEffectSpr(char* FileName, short sStart, short sCount, bool bAlphaEffect = TRUE);
	void DrawEffects();
	void bAddNewEffect(short sType, int sX, int sY, int dX, int dY, char cStartFrame, int iV1 = 1);
	void PlaySound(char cType, int iNum, int iDist, long lPan = 0);
	void SetCameraShakingEffect(short sDist, int iMul = 0);
	void DrawNewDialogBox(char cType, int sX, int sY, int iFrame, BOOL bIsNoColorKey = FALSE, BOOL bIsTrans = FALSE);
	void PutString(int iX, int iY, char * pString, COLORREF color);
	void PutString(int iX, int iY, char * pString, COLORREF color, BOOL bHide, char cBGtype, BOOL bIsPreDC = FALSE);
	void PutString2(int iX, int iY, char * pString, short sR, short sG, short sB);
	void PutAlignedString(int iX1, int iX2, int iY, char * pString, short sR = 0, short sG = 0, short sB = 0);
	void PutString_SprFont(int iX, int iY, char * pStr, short sR, short sG, short sB);
	void PutString_SprFont2(int iX, int iY, char * pStr, short sR, short sG, short sB);
	void PutString_SprFont3(int iX, int iY, char * pStr, short sR, short sG, short sB, BOOL bTrans = FALSE, int iType = 0);
	void PutString_SprNum(int iX, int iY, char * pStr, short sR, short sG, short sB);
	void ChangeGameMode(char cMode);
	void _DrawThunderEffect(int sX, int sY, int dX, int dY, int rX, int rY, char cType);
	void DrawLine(int x0, int y0, int x1, int y1, int iR, int iG, int iB);
	void DrawLine2(int x0, int y0, int x1, int y1, int iR, int iG, int iB);
	void RequestFullObjectData(WORD wObjectID);
	int bHasHeroSet(short Appr3, short Appr4, char OwnerType);
	void RestoreSprites();
	void EndInputString();
	void ReceiveString(char *pString);
	void UpdateScreen_OnMainMenu();
	bool _bGetIsStringIsNumber(char * pStr);
	bool bInitMagicCfgList();
	bool bCheckImportantFile();
	bool bInitSkillCfgList();
	bool _bDecodeBuildItemContents();
	bool bReadItemNameConfigFile();
	bool bReadLoginConfigFile(char * cFn);
	void _LoadGameMsgTextContents();
	bool __bDecodeBuildItemContents(char *pBuffer);
	void ReleaseTimeoverChatMsg();
	void ReleaseUnusedSprites();
	void UpdateScreen_OnQuit();
	void UpdateScreen_OnAgreement();
	void UpdateScreen_OnCreateNewAccount();
	void UpdateScreen_OnVersionNotMatch();
	void DrawVersion(BOOL bAuthor = FALSE);
	void UpdateScreen_OnConnecting();
	void UpdateScreen_OnWaitInitData();
	void UpdateScreen_OnConnectionLost();
	void UpdateScreen_OnLogin();
	void UpdateScreen_OnSelectServer();
	void UpdateScreen_OnSelectCharacter();
	void UpdateScreen_OnSelectCharacter(short sX, short sY, short msX, short msY, BOOL bIgnoreFocus = FALSE);
	void UpdateScreen_OnWaitingResponse();
	void UpdateScreen_OnQueryDeleteCharacter();
	void UpdateScreen_OnCreateNewCharacter();
	void UpdateScreen_OnChangePassword();
	void UpdateScreen_OnMsg();
	void UpdateScreen_OnQueryForceLogin();
	void StartInputString(int sX, int sY, unsigned char iLen, char * pBuffer, BOOL bIsHide = FALSE);
	void _LoadAgreementTextContents(char cType);
	void ShowReceivedString(BOOL bIsHide = FALSE);
	void ClearInputString();
	BOOL __fastcall DrawObject_OnMove_ForMenu(int indexX, int indexY, int sX, int sY, BOOL bTrans, DWORD dwTime, int msX, int msY);
	void _InitOnCreateNewCharacter();
	void _GetHairColorRGB(int iColorType, int * pR, int * pG, int * pB);
	BOOL _bCheckBadWords(char *pMsg);
	void DrawChatMsgBox(short sX, short sY, int iChatIndex, BOOL bIsPreDC);
	void InitGameSettings();
	int iGetTopDialogBoxIndex();
	void DisableDialogBox(int iBoxID);
	void EnableDialogBox(int iBoxID, int cType, int sV1, int sV2, char * pString = NULL);
	int GetCharKind(char *str, int index);
	BOOL _bCheckBuildItemStatus();
	int _iLoadTextDlgContents2(int iType);
	void AddEventList(char * pTxt, char cColor = 0, BOOL bDupAllow = TRUE);
	void _LoadShopMenuContents(char cType);
	void _LoadTextDlgContents(int cType);
	BOOL __bDecodeContentsAndBuildItemForSaleList(char * pBuffer);
	BOOL _bDraw_OnCreateNewCharacter(char * pName, short msX, short msY, int iPoint);
	void _Draw_CharacterBody(short sX, short sY, short sType);

	//
	int		G_iLevel;
	char	G_cWorldServerName[32];
	char	G_cPlayerName[12];
	char	G_cMapName[11];
	char	G_cMapMessage[30];
	HWND m_hWnd;
	//
	char m_cWorldServerName[32];
	char cStateChange1;
	char cStateChange2;
	char cStateChange3;

	BOOL m_bHunter;
	BOOL m_bAresden;
	BOOL m_bCitizen;

	CInt m_iHP;//was int			// Hit Point
	CInt m_iMP;//was int			// Mana Point
	CInt m_iSP;//was int			// Staminar Point

	char G_cTxt[180];
	char m_cGameMode;
	char m_cGameModeCount;
	char m_cLoading;
	HANDLE m_hPakFile;
	bool m_bSoundFlag;
	bool m_bSoundStat, m_bMusicStat; // On/Off
	char m_cSoundVolume, m_cMusicVolume;
	char m_cDetailLevel;
	short m_sViewDstX, m_sViewDstY;
	short m_sViewPointX, m_sViewPointY;
	short m_sVDL_X, m_sVDL_Y;
	DWORD m_dwTime;
	DWORD G_dwGlobalTime;
	int m_iCameraShakingDegree;
	short m_sPlayerObjectID;
	short m_sPlayerType;
	short m_sPlayerAppr1, m_sPlayerAppr2, m_sPlayerAppr3, m_sPlayerAppr4;
	int m_iPlayerStatus;
	char m_cPlayerTurn;
	char m_cWhetherEffectType;
	char m_cWhetherStatus;
	char m_cCurFocus, m_cMaxFocus;
	char m_cEnterCheck, m_cTabCheck, m_cLeftArrowCheck;
	char m_cArrowPressed;
	bool m_bEnterPressed, m_bEscPressed, m_bCtrlPressed, m_bRunningMode, m_bShiftPressed;
	CInt m_bIsGetPointingMode;
	
	char m_cEdit[4];
	char * m_pInputBuffer;
	CInt m_bCommandAvailable;
	bool m_bIsHideLocalCursor;
	char m_cMenuDir, m_cMenuDirCnt, m_cMenuFrame;
	DWORD m_dwCurTime;
	DWORD m_dwCheckConnTime, m_dwCheckSprTime, m_dwCheckChatTime;
	char m_cCommandCount;
	bool m_bIsProgramActive;
	int m_iNetLagCount;
	char m_cMsg[200];
	int m_iAgreeView;

	char m_cAccountName[12];
	char m_cAccountPassword[12];
	char m_cAccountAge[12];
	char m_cNewPassword[12];
	char m_cNewPassConfirm[12];
	char m_cAccountCountry[18];
	char m_cAccountSSN[32];
	char m_cEmailAddr[52];
	char m_cAccountQuiz[46];// Quiz
	char m_cAccountAnswer[22];
	char m_cPlayerName[12];
		char m_cLocation[12];
	char m_cCurLocation[12];
	char m_cGuildName[22];
	char m_cMCName[12];
	char m_cMapName[12];
	char m_cMapMessage[32];
	char m_cMapIndex;
		int m_iAccntYear, m_iAccntMonth, m_iAccntDay;
	int m_iIpYear, m_iIpMonth, m_iIpDay;
	int m_iTimeLeftSecAccount, m_iTimeLeftSecIP;
	char m_cGender, m_cSkinCol, m_cHairStyle, m_cHairCol, m_cUnderCol;
	char m_ccStr, m_ccVit, m_ccDex, m_ccInt, m_ccMag, m_ccChr;
	char m_cLU_Str, m_cLU_Vit, m_cLU_Dex, m_cLU_Int, m_cLU_Mag, m_cLU_Char;
	int m_iInputX, m_iInputY;
	unsigned char m_cInputMaxLen;
	int m_iTotalChar;

	int m_iLevel, m_iStr, m_iInt, m_iVit, m_iDex, m_iMag, m_iCharisma, m_iExp, m_iContribution;
	int m_iAngelicStr, m_iAngelicInt, m_iAngelicDex, m_iAngelicMag;

	WORD m_wCommObjectID;
	WORD m_wEnterGameType;
	WORD m_wR[16], m_wG[16], m_wB[16];
	WORD m_wWR[16], m_wWG[16], m_wWB[16];

	//
	char m_cLogServerAddr[16];
	int m_iGameServerMode;
	int m_iLogServerPort;
	//
	CInt m_bIsItemEquipped[MAXITEMS];//was BOOL
	CInt m_bIsItemDisabled[MAXITEMS];//was BOOL

	char m_cItemOrder[MAXITEMS];
	char m_cAmountString[12];

	int m_iLU_Point;

	BOOL m_bDialogTrans;
	BOOL m_bIsCombatMode;
	BOOL m_bIsSafeAttackMode;
	CInt m_bSkillUsingStatus;//was BOOL
	CInt m_bItemUsingStatus;//was BOOL
	BOOL m_bIsWhetherEffect;
	BOOL m_bSuperAttackMode;	//
	BOOL m_bIsObserverMode, m_bIsObserverCommanded;
	CInt m_bIsPoisoned;//was BOOL
	BOOL m_bIsFirstConn;
	BOOL m_bIsConfusion;
	BOOL m_bIsRedrawPDBGS;
	BOOL m_bDrawFlagDir;
	BOOL m_bIsCrusadeMode;
	CInt m_bIsSpecialAbilityEnabled;//was BOOL
	BOOL m_bInputStatus;
	BOOL m_bToggleScreen;
	BOOL m_bIsSpecial;
	int m_iGizonItemUpgradeLeft;
	class YWSound m_DSound;
	class CSoundBuffer *	m_pCSound[MAXSOUNDEFFECTS];
	class CSoundBuffer *	m_pMSound[MAXSOUNDEFFECTS];
	class CSoundBuffer *	m_pESound[MAXSOUNDEFFECTS];
	class CSoundBuffer *    m_pBGM;
	class DXC_ddraw  m_DDraw;
	class DXC_dinput m_DInput;
	class CMisc      m_Misc;
	class CSprite  * m_pSprite[MAXSPRITES];
	class CSprite  * m_pTileSpr[MAXTILES];
	class CSprite  * m_pEffectSpr[MAXEFFECTSPR];
	class CMapData * m_pMapData;
	//class XSocket * m_pGSock;
	//class XSocket * m_pLSock;
	class CMsg    * m_pChatMsgList[MAXCHATMSGS];
	class CMsg    * m_pChatScrollList[MAXCHATSCROLLMSGS];
	class CMsg    * m_pWhisperMsg[MAXWHISPERMSG];
	class CEffect * m_pEffectList[MAXEFFECTS];
	class CItem   * m_pItemList[MAXITEMS];
	class CItem   * m_pBankList[MAXBANKITEMS];
	class CMagic * m_pMagicCfgList[MAXMAGICTYPE];
	class CSkill * m_pSkillCfgList[MAXSKILLTYPE];
	class CMsg * m_pMsgTextList[TEXTDLGMAXLINES];
	class CMsg * m_pMsgTextList2[TEXTDLGMAXLINES];
	class CMsg * m_pAgreeMsgTextList[TEXTDLGMAXLINES];
	class CMsg * m_pExID;
	class CBuildItem * m_pBuildItemList[MAXBUILDITEMS];
	class CBuildItem * m_pDispBuildItemList[MAXBUILDITEMS];


	class CGameMonitor * m_pCGameMonitor;
	class CItem * m_pItemForSaleList[MAXMENUITEMS];
	class CCharInfo * m_pCharList[4];
	class CMsg * m_pGameMsgList[MAXGAMEMSGS];
	class CItemName * m_pItemNameList[MAXITEMNAMES];
	class CCurse m_curse;
	char m_cMagicMastery[MAXMAGICTYPE];
	unsigned char m_cSkillMastery[MAXSKILLTYPE];
	//
	char IpAddress[46], Version[6], ServerName[36];

	class cSocket * G_pListenSock = NULL;
	struct cLoginSocket * G_pLoginSock = NULL;
	struct cClientSocket * G_pClientSock = NULL;

	// Config file read modes.
	enum class ConfigFileReadMode { Undefined, ServerName, Address, ServerPort, LoginPort, Version };

	
	struct {
		short sX;
		short sY;
		short sCursorFrame;
		char  cPrevStatus;
		char  cSelectedObjectType;
		short sSelectedObjectID;
		short sPrevX, sPrevY, sDistX, sDistY;
		DWORD dwSelectClickTime;
		short sClickX, sClickY;
	} m_stMCursor;

	struct {
		int   sV1, sV2, sV3, sV4, sV5, sV6, sV7, sV8, sV9, sV10, sV11, sV12, sV13, sV14; // v1.4 short
		DWORD dwV1, dwV2, dwT1;
		BOOL  bFlag;
		short sX, sY;
		short sSizeX, sSizeY;
		short sView;
		char  cStr[32], cStr2[32], cStr3[32], cStr4[32];
		char  cMode;
		BOOL  bIsScrollSelected;
	} m_stDialogBoxInfo[61];	 // Snoopy passé à 61 (origine 41, Alastor 60), j'ai mis +20 car plus pratique.
	char m_cDialogBoxOrder[61];
	CInt m_bIsDialogEnabled[61];//was BOOL
	//Snoopy=>>>>>>>>>>>>>>>>>>>>>
	struct {
		int   sV1, sV2, sV3, sV4, sV5, sV6, sV7, sItemID;
		DWORD dwV1;
		char  cStr1[32], cStr2[32];
	} m_stDialogBoxExchangeInfo[8];
	//Snoopy end<<<<<<<<<<<<<<<<<<
	struct {
		int iIndex;
		int iAmount;
	} m_stSellItemList[MAXSELLLIST];

	struct {
		char cName[22];
		char cOpMode;
	} m_stGuildOpList[100];

	struct {
		DWORD dwTime;
		char  cColor;
		char  cTxt[96];
	} m_stEventHistory[6];

	struct {
		DWORD dwTime;
		char  cColor;
		char  cTxt[96];
	} m_stEventHistory2[6];

	struct {
		short sX, sY, sBX;
		char cStep;
	} m_stWhetherObject[MAXWHETHEROBJECTS];

	struct {
		BOOL bIsQuestCompleted;
		short sWho, sQuestType, sContribution, sTargetType, sTargetCount, sX, sY, sRange;
		short sCurrentCount; // by Snoopy
		char cTargetName[22];
	} m_stQuest;

	struct {
		char cStatus;
		char cName[12];
	} m_stPartyMember[MAXPARTYMEMBERS];

	struct {
		short sX, sY;
		char cType;
		char cSide;
	} m_stCrusadeStructureInfo[MAXCRUSADESTRUCTURES];

	struct {
		char cName[12];
	} m_stPartyMemberNameList[MAXPARTYMEMBERS + 1];

	// v2.171 2002-6-14
	struct {
		DWORD dwRefTime;
		int iGuildRank;
		char cCharName[12];
		char cGuildName[24];
	} m_stGuildName[MAXGUILDNAMES];

	struct {
		int iIndex;
		char mapname[12];
		int iX;
		int iY;
		int iCost;
	} m_stTeleportList[20];

};

#endif