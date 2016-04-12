/****************************************************************
*		 This client was writen by Diuuude & Snoopy81.			*
*					Based on Cleroth work.						*
*																*
*			V3.51 compatibility by Cleroth						*
*			V3.51 dialogs by Diuuude							*
*			Effects, mobs, Apocalypse, Heldenian				*
*			& finalizing by Snoopy81							*
*			V3.82 Crafting & Angels by Snoopy81					*
*****************************************************************/

/****************************************************************
*	Find here all compilation options							*
*	I removed useless ones and tryed to add some explanations	*
*	( Snoopy81 04/2005 )										*
*****************************************************************/

/*** Put here global data for your server ***/
#define NAME_WORLDNAME1			"WS1" // Name for default (1st) server
#define MSG_WORLDNAME1			"ABADDON Server" // Long name for 1st server...
#define MSG_HOMEPAGE			"http://www.helbreath.com/"
#define UPPERVERSION		3
#define LOWERVERSION		82
#define SERVER_IP			"127.0.0.1"
#define SERVER_PORT			2484

#define XSOCKSTATUS_READINGHEADER	        11
#define XSOCKSTATUS_READINGBODY		12



/*** Some more compilation options ***/
//#define USING_WIN_IME		// Used for unicode characters (unchecked)
#define MAKE_ACCOUNT		// Enable acc creation. Missing sprites, fixed!
//#define SELECTSERVER		// Show/Hides server selection screen.

// #define COMMA_GOLD		// Allows to show comma nbe as original HB (ie: 1,200,000)
// #define 4XXP_PER_LVL		// limits xp niv 140, then 4 x XP per lvl (affects XP shown as % on status)

//#define  _DEBUG				// Multi-Client, Instant-logout, Version check, Badwords,
// Mics debug msgs, CtrlQ for admins, Show status, Cursor position
//#define WINDOWED_MODE		// Shows HB in a windowed mode (for debug purpose only, sprite will bug....)


/*** Special conditions   ***/
#if _DEBUG
#define WINDOWED_MODE
#endif
