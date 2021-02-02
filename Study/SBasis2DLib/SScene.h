#pragma once
#include "SUserInterface.h"
#include "SGameUser.h"
#include "SNpcObject.h"
#include "SEffect.h"

struct SEnum
{
	enum  SGameType
	{
		S_BACKGROUND = 0,
		S_PLAYER = 100,
		S_NPC = 200,
		S_INTERFACE = 300,
		S_BUTTON,
		S_EFFECT = 400,
	};
};

struct SSpriteInfo
{
	RECT_ARRAY rtArray;
	wstring    szName;
};
class SScene
{
public:
	static SScene*				m_pCurrentScene;
	std::vector<SObjAttribute>  m_ObjAttribute;
	std::vector<SObject*>		m_UIObjList;
	std::vector<SObject*>		m_ItemObjList;
	std::vector<SObject*>		m_ObjList;
	static SGameUser*			m_pGamePlayer;
	std::vector<SSpriteInfo>	m_rtSpriteList;
public:
	static int	m_iStageCounter;
	SPoint		m_ptInitHeroPos;
	int			m_iNextScene;
	int			m_iNpcCounter;
	int			m_iMaxCounter;
	bool		m_bGameFinish;
	int			m_iSceneID;
	bool		m_bSceneChange;
public:
	std::vector<SEffectInfo>	m_EffectList;
	void  AddEffect(wstring name, SPoint pos);
	bool  GameDataLoad(const TCHAR* pszLoad);
	virtual bool  Reset();
public:
	void  GetBitmapLoad(FILE* fp, wstring& ret);
	bool  Load(const TCHAR* filename);
	virtual bool  Init();
	virtual bool  Frame();
	virtual bool  Render();
	virtual bool  Release();
public:
	virtual SObject*  NewObj(int iType);
	virtual bool  LoadScript(const TCHAR* filename);
	virtual bool  CreateScene();
public:
	SScene();
	virtual ~SScene();

};

