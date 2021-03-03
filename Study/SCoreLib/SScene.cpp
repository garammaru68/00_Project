#include "SScene.h"
#include "SObjectManager.h"
int	SScene::m_iStageCounter = 1;
SScene*	SScene::m_pCurrenSScene = nullptr;

SGameUser*	SScene::m_pGamePlayer = nullptr;
LRESULT	 SScene::MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	for (auto iter : m_UIObjList)
	{
		SUserInterface* pUI = (SUserInterface*)iter.second;
		int iRet = pUI->MsgProc(hWnd, msg, wParam, lParam);
		if (iRet >= 0) return iRet;
	}
	return -1;
};
SObject*  SScene::NewObj(SObjAttribute& info)
{
	SObject* pObject = nullptr;
	switch (info.iObjType)
	{
	case SEnum::S_MAP:
	{
		pObject = new SObject;
	}break;
	case SEnum::S_USER:
	{
		pObject = new SGameUser;
	}break;
	case SEnum::S_NPC:
	{
		pObject = new SNpcObject;
		SNpcObject* pNpc = dynamic_cast<SNpcObject*>(pObject);
		if (pNpc != nullptr)
		{
			pNpc->FSM();
		}
	} break;
	case SEnum::S_INTERFACE: {pObject = new SUserInterface; } break;
	case SEnum::S_DIALOGBOX:
	{
		pObject = new SDialogBox;
	} break;
	case SEnum::S_BUTTON: {pObject = new SButton; } break;
	case SEnum::S_EDIT: {pObject = new SEdit; } break;
	}

	return pObject;
}
bool  SScene::CreateScene()
{
	m_bSceneChange = false;
	for (SObjAttribute& info : m_ObjAttribute)
	{
		SObject* pObj = NewObj(info);
		if (pObj == nullptr) continue;

		pObj->m_iObjectType = info.iObjType;
		pObj->m_Attribute = info;
		pObj->m_szName = info.szName;
		pObj->m_szParentName = info.szParentName;
		pObj->Init();

		if (info.bColorKey == false)
		{
			pObj->Load(info.colorbitmap.c_str(), info.maskbitmap.c_str());
		}
		else
		{
			pObj->Load(info.colorbitmap.c_str(), nullptr, info.dwColor);
		}
		//// state
		//pObj->m_StateBitmap.resize(4);
		//pObj->m_StateBitmap[DEFAULT] =	pObj->m_pColorBmp;
		//if (!info.pushbitmap.empty())
		//{
		//	pObj->m_StateBitmap[PUSH] =	g_BitmapMgr.Load(info.pushbitmap.c_str());
		//}
		//if (!info.selectbitmap.empty())
		//{
		//	pObj->m_StateBitmap[SELECT] =g_BitmapMgr.Load(info.selectbitmap.c_str());
		//}
		//if (!info.disbitmap.empty())
		//{
		//	pObj->m_StateBitmap[DISABLE] =	g_BitmapMgr.Load(info.disbitmap.c_str());
		//}

		SeSObject(info, pObj);
		if (info.iObjType == SEnum::S_DIALOGBOX)
		{
			pObj->m_iSortValue += 100;
		}
		if (info.iObjType == SEnum::S_EDIT)
		{
			SEdit* pEdit = (SEdit*)pObj;
			pEdit->m_rtWndCtrl = pObj->m_rtDesk;
			if (pObj->m_pParentObject != nullptr)
			{
				pEdit->m_rtWndCtrl.left = pObj->m_pParentObject->m_rtCollide.left + pObj->m_rtDesk.left;
				pEdit->m_rtWndCtrl.top = pObj->m_pParentObject->m_rtCollide.top + pObj->m_rtDesk.top;
			}
			pEdit->CreateEdit();
		}
	}

	SortZValue(m_UIObjList, m_UIDrawObjList);
	return true;
}
void  SScene::SortZValue(std::map<wstring, SObject*>& srcList, std::vector<std::pair<wstring, SObject*>>& destList)
{
	auto cmp = [](std::pair<wstring, SObject*>  a, std::pair<wstring, SObject*>  b)
	{
		SObject* pA = a.second;
		SObject* pB = b.second;
		return pA->m_iSortValue < pB->m_iSortValue;
	};
	std::copy(srcList.begin(), srcList.end(), std::back_inserter(destList));
	std::sort(destList.begin(), destList.end(), cmp);
}
bool  SScene::SeSObject(SObjAttribute& info, SObject* pObj)
{
	assert(pObj);

	if (pObj && !(info.szParentName == L"nullptr" || info.szParentName == L"NULL"))
	{
		pObj->m_pParentObject = FindParent(info.szParentName);
		if (pObj->m_pParentObject != nullptr)
		{
			pObj->m_pParentObject->m_pChildObjects.push_back(pObj);
		}
	}

	switch (info.iObjType)
	{
	case SEnum::S_USER:
	{
		m_pGamePlayer = (SGameUser*)pObj;
		m_ptInitHeroPos = info.pos;
		return true;
	}break;
	case SEnum::S_NPC:
	{
		pObj->Set(info.pos, info.rtSrc);
		if (pObj->m_pParentObject == nullptr)
		{
			m_CharacterList.insert(make_pair(pObj->m_szName, pObj));
		}
		return true;
	}break;
	case SEnum::S_OBJECT:
	{
		pObj->Set(info.pos, info.rtSrc);
		if (pObj->m_pParentObject == nullptr)
		{
			m_ObjList.insert(make_pair(pObj->m_szName, pObj));
		}
		return true;
	}break;
	case SEnum::S_EDIT:
	{
		pObj->Set(info.rtSrc, info.rtDesk);
		if (pObj->m_pParentObject == nullptr)
		{
			m_UIObjList.insert(make_pair(pObj->m_szName, pObj));
		}
		return true;
	}break;
	case SEnum::S_INTERFACE:
	case SEnum::S_BUTTON:
	case SEnum::S_DIALOGBOX:
	{
		pObj->Set(info.rtSrc, info.rtDesk);
		pObj->Update();
		if (pObj->m_pParentObject == nullptr)
		{
			m_UIObjList.insert(make_pair(pObj->m_szName, pObj));
		}
		return true;
	}break;
	case SEnum::S_MAP:
	{
		pObj->Set(info.rtSrc, info.rtDesk);
		if (pObj->m_pParentObject == nullptr)
		{
			m_MapList.insert(make_pair(pObj->m_szName, pObj));
		}
		return true;
	}break;
	}
	return false;
}
bool  SScene::Load(const TCHAR* filename)
{
	Init();
	if (!g_ObjectMgr.LoadObjectFile(filename, m_ObjAttribute))
	{
		return false;
	}
	return true;
}
bool  SScene::Init()
{
	for (auto iter : m_UIObjList)
	{
		SObject* pObj = (SObject*)iter.second;
		pObj->Init();
	}
	for (auto iter : m_ObjList)
	{
		SObject* pObj = (SObject*)iter.second;
		if (pObj)	pObj->Init();
	}
	return true;
}
bool  SScene::PreFrame()
{
	return true;
}
bool  SScene::Frame()
{
	for (auto iter : m_MapList)
	{
		SObject* pObj = (SObject*)iter.second;
		if (pObj->m_Attribute.iEnable >= 1)
		{
			pObj->Frame();
		}
	}
	for (auto iter : m_ObjList)
	{
		SObject* pObj = (SObject*)iter.second;
		if (pObj->m_Attribute.iEnable >= 1)
		{
			pObj->Frame();
		}
	}
	for (auto iter : m_NpcList)
	{
		SObject* pObj = (SObject*)iter.second;
		if (pObj->m_Attribute.iEnable >= 1)
		{
			pObj->Frame();
		}
	}
	for (auto iter : m_UserList)
	{
		SObject* pObj = (SObject*)iter.second;
		if (pObj->m_Attribute.iEnable >= 1)
		{
			pObj->Frame();
		}
	}
	//effect frame
	for (std::vector<SEffectInfo>::iterator iter = m_EffectList.begin();
		iter != m_EffectList.end();
		)
	{
		iter->Frame();
		if (iter->m_bDead == true)
		{
			iter = m_EffectList.erase(iter);
			continue;
		}
		else
		{
			iter++;
		}
	}
	for (auto iter : m_UIDrawObjList)
	{
		SObject* pObj = (SObject*)iter.second;
		if (pObj->m_Attribute.iEnable >= 1)
		{
			pObj->Frame();
		}
	}
	SButton* pExitBtn = (SButton*)FindUI(L"GAME_EXIT");
	if (pExitBtn &&  pExitBtn->m_bSelect == true)
	{
		SScene::m_pCurrenSScene->m_bGameFinish = true;
	}
	return true;
}
bool  SScene::PostFrame()
{
	return true;
}
bool  SScene::Release()
{
	for (auto iter : m_MapList)
	{
		SObject* pObj = (SObject*)iter.second;
		pObj->Release();
		delete pObj;
	}
	for (auto iter : m_ObjList)
	{
		SObject* pObj = (SObject*)iter.second;
		pObj->Release();
		delete pObj;
	}
	for (auto iter : m_ItemObjList)
	{
		SObject* pObj = (SObject*)iter.second;
		pObj->Release();
		delete pObj;
	}
	for (auto iter : m_UIObjList)
	{
		SObject* pObj = (SObject*)iter.second;
		pObj->Release();
		delete pObj;
	}
	for (auto iter : m_UserList)
	{
		SObject* pObj = (SObject*)iter.second;
		pObj->Release();
		delete pObj;
	}
	for (auto iter : m_NpcList)
	{
		SObject* pObj = (SObject*)iter.second;
		pObj->Release();
		delete pObj;
	}
	delete m_pGamePlayer;
	m_pGamePlayer = nullptr;
	m_MapList.clear();
	m_ObjList.clear();
	m_ItemObjList.clear();
	m_UIObjList.clear();
	m_ObjAttribute.clear();
	m_UserList.clear();
	m_NpcList.clear();
	m_EffectList.clear();
	return true;
}
bool  SScene::PreRender(ID3D11DeviceContext*	pd3dContext)
{
	return true;
}
bool  SScene::Render(ID3D11DeviceContext*	pd3dContext)
{
	PreRender(pd3dContext);
	{
		RenderMap(pd3dContext);
		RenderObject(pd3dContext);
		RenderCharacter(pd3dContext);
		RenderEffect(pd3dContext);
		RenderUI(pd3dContext);
	}
	PostRender(pd3dContext);
	return true;
}
bool  SScene::PostRender(ID3D11DeviceContext*	pd3dContext)
{
	if (SScene::m_pCurrenSScene->m_bGameFinish)
	{
		SendMessage(g_hWnd, WM_DESTROY, 0, 0);
		return false;
	}
	return true;
}
bool  SScene::RenderMap(ID3D11DeviceContext*	pd3dContext)
{
	for (auto iter : m_MapList)
	{
		SObject* pObj = (SObject*)iter.second;
		if (pObj->m_Attribute.iVisible >= 1)
		{
			pObj->Render(pd3dContext);
		}
	}
	return true;
}
bool  SScene::RenderUI(ID3D11DeviceContext*	pd3dContext)
{
	for (auto iter : m_UIDrawObjList)
	{
		SObject* pObj = (SObject*)iter.second;
		if (pObj->m_Attribute.iVisible >= 1)
		{
			pObj->Render(pd3dContext);
		}
	}
	return true;
}
bool  SScene::RenderEffect(ID3D11DeviceContext*	pd3dContext)
{
	for (auto& pInfo : m_EffectList)
	{
		SEffect* pEffect = (SEffect*)g_ObjectMgr.GetPtr(pInfo.m_csName);
		pEffect->Set(pInfo.p,
			pEffect->m_rtList[pInfo.m_iRectIndex]);
		pEffect->Render(pd3dContext);
	}
	return true;
}
bool  SScene::RenderObject(ID3D11DeviceContext*	pd3dContext)
{
	for (auto iter : m_ObjList)
	{
		SObject* pObj = (SObject*)iter.second;
		if (pObj->m_bDead == false)
		{
			pObj->Render(pd3dContext);
		}
	}
	return true;
}
bool  SScene::RenderCharacter(ID3D11DeviceContext*	pd3dContext)
{
	for (auto iter : m_NpcList)
	{
		SObject* pObj = (SObject*)iter.second;
		if (pObj->m_Attribute.iVisible >= 1)
		{
			pObj->Render(pd3dContext);
		}
	}
	for (auto iter : m_UserList)
	{
		SObject* pObj = (SObject*)iter.second;
		if (pObj->m_Attribute.iVisible >= 1)
		{
			pObj->Render(pd3dContext);
		}
	}
	return true;
}
void  SScene::AddEffect(wstring name, SPoint pos, SPoint dir)
{
	SEffect* pEffect = (SEffect*)g_ObjectMgr.GetPtr(name);
	if (pEffect == nullptr) return;
	SEffectInfo info;
	info.m_csName = name;
	info.m_bDead = false;
	info.m_fLifeTime = 1.0f;
	info.iMaxCount = (int)pEffect->m_rtList.size();
	info.m_fStep = 1.0f / pEffect->m_rtList.size();
	info.m_fStep = 0.1f;
	info.d = { dir.x, dir.y };
	info.p = { pos.x, pos.y };
	m_EffectList.push_back(info);
}
void  SScene::AddNpc(wstring name, SPoint pos, SPoint dir, int index)
{
};
bool  SScene::Reset()
{
	for (auto iter : m_UIObjList)
	{
		SObject* pObj = (SObject*)iter.second;
		pObj->Release();
		delete pObj;
	}
	for (auto iter : m_ItemObjList)
	{
		SObject* pObj = (SObject*)iter.second;
		pObj->Release();
		delete pObj;
	}
	for (auto iter : m_ObjList)
	{
		SObject* pObj = (SObject*)iter.second;
		pObj->Release();
		delete pObj;
	}

	m_UIObjList.clear();
	m_ItemObjList.clear();
	m_ObjList.clear();
	if (m_pGamePlayer)
	{
		m_pGamePlayer->Reset();
		m_pGamePlayer->SetPos(m_ptInitHeroPos);
	}
	m_EffectList.clear();

	m_iNpcCounter = 0;
	return true;
}
SObject*	SScene::FindParent(std::wstring szName)
{
	SObject* pParentObj = FindMap(szName);
	if (pParentObj == nullptr)
	{
		pParentObj = FindUI(szName);
		if (pParentObj == nullptr)
		{
			pParentObj = FindItem(szName);
			if (pParentObj == nullptr)
			{
				pParentObj = FindObject(szName);
				if (pParentObj == nullptr)
				{
					return nullptr;
				}
			}
		}
	}
	return pParentObj;
}
SObject*	SScene::FindMap(std::wstring szName)
{
	auto iter = m_MapList.find(szName);
	if (iter == m_MapList.end())
	{
		return nullptr;
	}
	return (*iter).second;
}
SObject*	SScene::FindUI(std::wstring szName)
{
	auto iter = m_UIObjList.find(szName);
	if (iter == m_UIObjList.end())
	{
		return nullptr;
	}
	return (*iter).second;
}
SObject*	SScene::FindItem(std::wstring szName)
{
	auto iter = m_ItemObjList.find(szName);
	if (iter == m_ItemObjList.end())
	{
		return nullptr;
	}
	return (*iter).second;
};
SObject*	SScene::FindObject(std::wstring szName)
{
	auto iter = m_ObjList.find(szName);
	if (iter == m_ObjList.end())
	{
		return nullptr;
	}
	return (*iter).second;
}
SObject*	SScene::FindNpc(int iIndex)
{
	auto iter = m_NpcList.find(iIndex);
	if (iter == m_NpcList.end())
	{
		return nullptr;
	}
	return (*iter).second;
}
bool	SScene::DelNpc(int iIndex)
{
	auto iter = m_NpcList.find(iIndex);
	if (iter != m_NpcList.end())
	{
		g_ObjectMgr.DeleteExecute(iter->second);
		delete iter->second;
		m_NpcList.erase(iter);
		return true;
	}
	return false;
}
SScene::SScene()
{
	m_iStageCounter = 0;
	m_iSceneID = 0;
	m_bSceneChange = false;
	m_iNexSScene = SSCENE_LOADING;
	m_iNpcCounter = 0;
}
SScene::~SScene()
{

}