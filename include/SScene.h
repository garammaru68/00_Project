#pragma once
#include "SUserInterface.h"
#include "SGameUser.h"
#include "SNpcObject.h"
#include "SEffect.h"

struct SEnum
{
	enum  SGameType
	{
		S_MAP = 0,
		S_USER = 100,
		S_NPC = 200,
		S_INTERFACE = 300,
		S_BUTTON,
		S_EDIT,
		S_DIALOGBOX,
		S_EFFECT = 400,
		S_OBJECT = 500,
	};
};


class SScene
{
public:
	enum SSceneID
	{
		SSCENE_LOADING = 0,
		SSCENE_LOBBY = 1,
		SSCENE_ZONE = 2,
	};
public:
	static SScene*				m_pCurrenSScene;
	std::vector<SObjAttribute>  m_ObjAttribute;
	static SGameUser*			m_pGamePlayer;
	std::vector<SSpriteInfo>	m_rtSpriteList;

	std::map<wstring, SObject*>				m_UIObjList;
	std::map<wstring, SObject*>				m_ItemObjList;
	std::map<wstring, SObject*>				m_ObjList;
	std::map<wstring, SObject*>				m_CharacterList;
	std::map<wstring, SObject*>				m_MapList;
	std::map<wstring, SObject*>::iterator	m_iter;

	std::vector<std::pair<wstring, SObject*>>	m_UIDrawObjList;
public:
	virtual SObject*	FindUI(std::wstring szName);
	virtual SObject*	FindItem(std::wstring szName);
	virtual SObject*	FindObject(std::wstring szName);
	virtual SObject*	FindMap(std::wstring szName);
	virtual SObject*	FindNpc(int iIndex);
	virtual SObject*	FindParent(std::wstring szName);
	virtual bool		DelNpc(int iIndex);
public:
	std::map<int, SGameUser*>			m_UserList;
	std::map<int, SObject*>				m_NpcList;
public:
	static int	m_iStageCounter;
	SPoint		m_ptInitHeroPos;
	int			m_iNexSScene;
	int			m_iNpcCounter;
	int			m_iMaxCounter;
	bool		m_bGameFinish;
	int			m_iSceneID;
	bool		m_bSceneChange;
public:
	std::vector<SEffectInfo>	m_EffectList;
	virtual void  AddEffect(wstring name, SPoint pos, SPoint dir);
	virtual void  AddNpc(wstring name, SPoint pos, SPoint dir, int index);
	virtual bool  Reset();
	virtual void  SortZValue(std::map<wstring, SObject*>& srcList,
		std::vector<std::pair<wstring, SObject*>>& destList);
public:
	virtual LRESULT	 MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	virtual bool  Load(const TCHAR* filename);
	virtual bool  Init();
	virtual bool  PreFrame();
	virtual bool  Frame();
	virtual bool  PostFrame();
	virtual bool  PreRender(ID3D11DeviceContext*	pd3dContext);
	virtual bool  Render(ID3D11DeviceContext*	pd3dContext);
	virtual bool  PostRender(ID3D11DeviceContext*	pd3dContext);
	virtual bool  RenderMap(ID3D11DeviceContext*	pd3dContext);
	virtual bool  RenderObject(ID3D11DeviceContext*	pd3dContext);
	virtual bool  RenderCharacter(ID3D11DeviceContext*	pd3dContext);
	virtual bool  RenderEffect(ID3D11DeviceContext*	pd3dContext);
	virtual bool  RenderUI(ID3D11DeviceContext*	pd3dContext);
	virtual bool  Release();
public:
	virtual SObject*  NewObj(SObjAttribute& info);
	virtual bool  CreateScene();
	virtual bool  SeSObject(SObjAttribute& iType, SObject* pObj);
public:
	SScene();
	virtual ~SScene();

};

