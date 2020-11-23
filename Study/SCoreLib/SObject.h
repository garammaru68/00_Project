#pragma once
#include "SBitmapManager.h"
#include "SCollision.h"

enum SObjectType {
	BAKCGROUND = 0,
	UICONTROL,
	USER,
	NPC
};
enum SControlState {
	DEFAULT = 0,
	PUSH,
	SELECT,
	DISABLE,
};
struct SObjAttribute
{
	int iObjType;
	wstring colorbitmap;
	wstring pushbitmap;
	wstring selectbitmap;
	wstring disbitmap;
	wstring maskbitmap;
	SPoint	pos;
	RECT	rtSrc;
	RECT	rtDesk;
	bool	bColorKey;
	DWORD	dwColor;
};

class SObject
{
public:
	int			m_iObjectType;
	wstring		m_szName;
public:
	SBitmap*	m_pMaskBmp;
	SBitmap*	m_pColorBmp;
	std::vector<SBitmap*> m_StateBitmap;
	RECT		m_rtSrc;
	RECT		m_rtDesk;
	bool		m_bColorKey;
	DWORD		m_dwColorKey;
public:
	RECT		m_rtCollide;
	SPoint		m_ptPos;
	float		m_fSpeed;
	SPoint		m_ptDirection;
	bool		m_bDead;
	float		m_fLifeTime;
	float		m_fTmpTimer;
public:
	virtual bool Reset();
	virtual bool Init();
	virtual bool Frame();
	virtual bool Render();
	virtual bool Release();
	virtual void SetPos(SPoint p);
	virtual void Set(SPoint p, RECT rtSrc, float fSpeed = 200.0f);

	virtual void SetPosition(SPoint p);
	virtual void Set(RECT rtSrc, RECT rtDesk, float fSpeed = 200.0f);
	virtual bool Load(  const TCHAR* color, 
						const TCHAR* mask = nullptr,
						DWORD dwColor = RGB(255, 0, 255));
	virtual void DrawColorKey();
public:
	virtual void NpcDamage() {};
	virtual void NpcDead() {};
	//virtual void PlayerDamage() {};
	//virtual void PlayerDead() {};
	virtual void Attack(SObject* pPlayer) {};
public:
	virtual void SetTransition(DWORD dwEvent) {};
public:
	SObject();
	virtual ~SObject();
};

