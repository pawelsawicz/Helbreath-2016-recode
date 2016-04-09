// Item.h: interface for the CItem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ITEM_H__211A1360_91B9_11D2_B143_00001C7030A6__INCLUDED_)
#define AFX_ITEM_H__211A1360_91B9_11D2_B143_00001C7030A6__INCLUDED_

#define _WINSOCKAPI_    // stops windows.h including winsock.h
#include <windows.h>

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#define MAXITEMEQUIPPOS		15
#define EQUIPPOS_NONE		0
#define EQUIPPOS_HEAD		1
#define EQUIPPOS_BODY		2
#define EQUIPPOS_ARMS		3
#define EQUIPPOS_PANTS		4
#define EQUIPPOS_BOOTS		5
#define EQUIPPOS_NECK		6
#define EQUIPPOS_LHAND		7
#define EQUIPPOS_RHAND		8
#define EQUIPPOS_TWOHAND	9
#define EQUIPPOS_RFINGER	10
#define EQUIPPOS_LFINGER	11
#define EQUIPPOS_BACK		12
#define EQUIPPOS_FULLBODY	13

#define ITEMTYPE_NONE			0
#define ITEMTYPE_EQUIP			1
#define ITEMTYPE_APPLY			2
#define ITEMTYPE_USE_DEPLETE	3
#define ITEMTYPE_INSTALL		4
#define ITEMTYPE_CONSUME		5
#define ITEMTYPE_ARROW			6
#define ITEMTYPE_EAT			7
#define ITEMTYPE_USE_SKILL		8
#define ITEMTYPE_USE_PERM		9
#define ITEMTYPE_USE_SKILL_ENABLEDIALOGBOX	10
#define ITEMTYPE_USE_DEPLETE_DEST			11
#define ITEMTYPE_MATERIAL					12

class CItem  
{
public:
	CItem();
	virtual ~CItem();
	char  m_cName[21];
	char  m_cItemType;
	char  m_cEquipPos;
	char  m_cItemColor;
	char  m_cSpeed;
	char  m_cGenderLimit;
//	short m_sItemEffectType;
	short m_sLevelLimit;
	short m_sSprite;
	short m_sSpriteFrame;
	short m_sX, m_sY;
	short m_sItemSpecEffectValue1, m_sItemSpecEffectValue2, m_sItemSpecEffectValue3;
	short m_sItemEffectValue1, m_sItemEffectValue2, m_sItemEffectValue3, m_sItemEffectValue4, m_sItemEffectValue5, m_sItemEffectValue6; 
	WORD  m_wCurLifeSpan;
	WORD  m_wMaxLifeSpan;
	WORD  m_wPrice, m_wWeight;
	DWORD m_dwCount;
	DWORD m_dwAttribute;
};

#endif // !defined(AFX_ITEM_H__211A1360_91B9_11D2_B143_00001C7030A6__INCLUDED_)
