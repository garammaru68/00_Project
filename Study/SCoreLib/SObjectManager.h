#pragma once
#include "SEffect.h"
#include <functional>
struct cmp {
	bool operator()(int i, int j)
	{
		if (i > j) return true;
		else return false;
	}
};
class SObjectComposite : public SObject
{
public:
	std::deque<SObject*> m_Components;
public:
	SObject* Clone();
};

using CollisionFunction = std::function<void(SObject*, DWORD)>;
using SelectFunction = std::function<void(POINT, DWORD)>;

class SObjectManager : public SSingleton<SObjectManager>
{
public:
	friend  class SSingleton<SObjectManager>;
private:
	std::vector<SObjAttribute>  m_ObjAttribute;
	std::vector<SSpriteInfo>	m_rtSpriteList;
private:
	std::map<wstring, SObject*>				m_List;
	std::map<wstring, SObject*>::iterator	m_iter;
	std::wstring				m_szDefaultPath;
private:
	int		m_iExecuteSelectID = 0;
	int		m_iExecuteCollisionID = 0;
	std::map<int, SObject*>	      m_CollisionObjectList;
	std::map<int, SObject*>	      m_SelecSObjectList;

	typedef std::map<int, SelectFunction>::iterator SelectFuncIterator;
	std::map<int, SelectFunction>    m_fnSelectExecute;

	typedef std::map<int, CollisionFunction>::iterator CollisionFuncIterator;
	std::map<int, CollisionFunction>    m_fnCollisionExecute;
public:
	void		AddSelectExecute(SObject* ownder, SelectFunction func);
	void		AddCollisionExecute(SObject* ownder, CollisionFunction func);
	void		DeleteExecute(SObject* owner);
public:
	SObject*	Add(SObject* pAddObject);
	bool		Load(const TCHAR* filename);
	SObject*	GetPtr(wstring szName);
	SObject*	Clone(wstring szName);
	bool		Init();
	bool		Frame();
	bool		Render();
	bool		Release();
public:
	bool  LoadObjectFile(T_STR szFileName, std::vector<SObjAttribute>&);
	bool  LoadEffectFile(T_STR szFileName, std::vector<SSpriteInfo>&);
	void  CreateEffect(std::vector<SSpriteInfo>&	list);
	void  CreateObject(std::vector<SObjAttribute>&	list);
	void  GetBitmapLoad(FILE* fp, wstring& ret);
private:
	SObjectManager();
public:
	~SObjectManager();
};
#define g_ObjectMgr SObjectManager::GetInstance()

