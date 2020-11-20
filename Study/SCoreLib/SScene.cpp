#include "SScene.h"
#include "SObjectManager.h"

int	SScene::m_iStageCounter = 1;
SScene*	SScene::m_pCurrentScene = nullptr;

SGameUser*	SScene::m_pGamePlayer = nullptr;
bool SScene::GameDataLoad(const TCHAR* pszLoad)
{
	TCHAR pBuffer[256] = { 0 };
	TCHAR pTemp[256] = { 0 };

	int iNumSprite = 0;
	FILE* fp_src;
	_wfopen_s(&fp_src, pszLoad, _T("rt"));
	if (fp_src == NULL) return false;

	_fgetts(pBuffer, _countof(pBuffer), fp_src);
	_stscanf_s(pBuffer, _T("%s%d%s"), pTemp, _countof(pTemp), &iNumSprite);
	m_rtSpriteList.resize(iNumSprite);

	for (int iCnt = 0; iCnt < iNumSprite; iCnt++)
	{
		int iNumFrame = 0;
		_fgetts(pBuffer, _countof(pBuffer), fp_src);
		_stscanf_s(pBuffer, _T("%s %d"), pTemp, _countof(pTemp), &iNumFrame);
		//m_rtSpriteList[iCnt].resize(iNumFrame);
		m_rtSpriteList[iCnt].szName = pTemp;
		RECT rt;
		for (int iFrame = 0; iFrame < iNumFrame; iFrame++)
		{
			_fgetts(pBuffer, _countof(pBuffer), fp_src);
			_stscanf_s(pBuffer, _T("%s %d %d %d %d"), pTemp, _countof(pTemp),
				&rt.left, &rt.top, &rt.right, &rt.bottom);
			m_rtSpriteList[iCnt].rtArray.push_back(rt);
		}
	}
	fclose(fp_src);
	return true;
}
SObject*  SScene::NewObj(int iType)
{
	SObject* pObject = nullptr;
	switch (iType)
	{
	case SEnum::S_BACKGROUND: {pObject = new SObject; } break;
	case SEnum::S_PLAYER: {pObject = new SGameUser; } break;
	case SEnum::S_NPC: {
		pObject = new SNpcObject;
		SNpcObject* pNpc = dynamic_cast<SNpcObject*>(pObject);
		if (pNpc != nullptr)
		{
			pNpc->FSM();
		}
	} break;
	case SEnum::S_INTERFACE: {pObject = new SUserInterface; } break;
	case SEnum::S_BUTTON: {pObject = new SButton; } break;
	}
	return pObject;
}
void  SScene::GetBitmapLoad(FILE* fp, wstring& ret)
{
	TCHAR szBuffer[256] = { 0, };
	TCHAR szTemp[256] = { 0, };
	//	nullptr,
	_fgetts(szBuffer, _countof(szBuffer), fp);
	_stscanf_s(szBuffer, _T("%s"), szTemp, _countof(szTemp));
	wstring maskbmp = szTemp;
	if (maskbmp != L"nullptr")
	{
		ret = maskbmp;
	}
}
bool  SScene::LoadScript(const TCHAR* filename)
{
	//std::vector<TObjAttribute>  m_ObjAttribute;
	FILE* fp = nullptr;
	_wfopen_s(&fp, filename, L"rt");
	TCHAR szBuffer[256] = { 0, };
	TCHAR szTemp[256] = { 0, };

	if (fp != nullptr)
	{
		SObjAttribute objinfo;
		int iMaxCount = 0;
		_fgetts(szBuffer, sizeof(TCHAR) * 256, fp);
		_stscanf_s(szBuffer, _T("%d"), &iMaxCount);
		//m_ObjAttribute.resize(iMaxCount);
		for (int iObj = 0; iObj < iMaxCount; iObj++)
		{
			_fgetts(szBuffer, _countof(szBuffer), fp);
			//0 //BACKGROUND
			_fgetts(szBuffer, _countof(szBuffer), fp);
			_stscanf_s(szBuffer, _T("%d"), &objinfo.iObjType);

			GetBitmapLoad(fp, objinfo.colorbitmap);
			GetBitmapLoad(fp, objinfo.pushbitmap);
			GetBitmapLoad(fp, objinfo.selectbitmap);
			GetBitmapLoad(fp, objinfo.disbitmap);
			GetBitmapLoad(fp, objinfo.maskbitmap);
			//	0, 0,
			_fgetts(szBuffer, sizeof(TCHAR) * 256, fp);
			_stscanf_s(szBuffer, _T("%f%f"),
				&objinfo.pos.x,
				&objinfo.pos.y);
			objinfo.rtDesk.left = objinfo.pos.x;
			objinfo.rtDesk.top = objinfo.pos.y;
			//	-1, -1, -1, -1,
			_fgetts(szBuffer, sizeof(TCHAR) * 256, fp);
			_stscanf_s(szBuffer, _T("%d%d%d%d"),
				&objinfo.rtSrc.left,
				&objinfo.rtSrc.top,
				&objinfo.rtSrc.right,
				&objinfo.rtSrc.bottom);
			//	-1, -1,
			_fgetts(szBuffer, sizeof(TCHAR) * 256, fp);
			_stscanf_s(szBuffer, _T("%d%d"),
				&objinfo.rtDesk.right,
				&objinfo.rtDesk.bottom);
			//	0,255, 0, 255
			int mask = 0;
			int r = 255;
			int g = 0;
			int b = 255;
			_fgetts(szBuffer, sizeof(TCHAR) * 256, fp);
			_stscanf_s(szBuffer, _T("%d%d%d%d"),
				&mask,
				&r, &g, &b);
			if (mask == 0)
				objinfo.bColorKey = false;
			else
				objinfo.bColorKey = true;
			objinfo.dwColor = RGB(r, g, b);
			m_ObjAttribute.push_back(objinfo);
		}
	}
	fclose(fp);

	return true;
}
bool  SScene::CreateScene()
{
	m_bSceneChange = false;
	for (SObjAttribute& info : m_ObjAttribute)
	{
		SObject* pObj = NewObj(info.iObjType);

		pObj->Init();
		if (info.bColorKey == false)
		{
			pObj->Load(info.colorbitmap.c_str(),
				info.maskbitmap.c_str());
		}
		else
		{
			pObj->Load(info.colorbitmap.c_str(),
				nullptr,
				info.dwColor);
		}
		// state
		pObj->m_StateBitmap.resize(5);
		pObj->m_StateBitmap[DEFAULT] =
			pObj->m_pColorBmp;
		if (!info.pushbitmap.empty())
		{
			pObj->m_StateBitmap[PUSH] =
				g_BitmapMgr.Load(info.pushbitmap.c_str());
		}
		if (!info.selectbitmap.empty())
		{
			pObj->m_StateBitmap[SELECT] =
				g_BitmapMgr.Load(info.selectbitmap.c_str());
		}
		if (!info.disbitmap.empty())
		{
			pObj->m_StateBitmap[DISABLE] =
				g_BitmapMgr.Load(info.disbitmap.c_str());
		}

		pObj->Set(info.pos, info.rtSrc);

		if (info.iObjType == 100)
		{
			m_pGamePlayer = (SGameUser*)pObj;
			m_ptInitHeroPos = info.pos;
		}
		else
			if (info.iObjType >= 300)
			{
				m_UIObjList.push_back(pObj);
			}
			else
			{
				m_ObjList.push_back(pObj);
			}
	}
	return true;
}
bool  SScene::Load(const TCHAR* filename)
{
	Init();
	if (!LoadScript(filename))
	{
		return false;
	}
	//CreateScene();

	GameDataLoad(L"rtSprite.txt");

	for (int iEffect = 0; iEffect < m_rtSpriteList.size(); iEffect++)
	{
		SEffect* pEffect = new SEffect;
		pEffect->Init();
		pEffect->m_szName = m_rtSpriteList[iEffect].szName;
		pEffect->Load(L"../../data/API/Zombie.bmp");
		RECT rtSrc = m_rtSpriteList[iEffect].rtArray[0];
		SPoint p = { 0,0 };
		pEffect->Set(p, rtSrc);
		pEffect->SetSprite(m_rtSpriteList[iEffect].rtArray);
		g_ObjectMgr.Add(pEffect);
	}
	return true;
}
bool  SScene::Init()
{
	return true;
}
bool  SScene::Frame()
{
	for (SObject* pObj : m_UIObjList)
	{
		pObj->Frame();
	}
	for (SObject* pObj : m_ObjList)
	{
		pObj->Frame();
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
	return true;
}
bool  SScene::Release()
{
	for (SObject* pObj : m_ObjList)
	{
		pObj->Release();
		delete pObj;
	}
	for (SObject* pObj : m_UIObjList)
	{
		pObj->Release();
		delete pObj;
	}
	delete m_pGamePlayer;
	m_pGamePlayer = nullptr;
	m_ObjList.clear();
	m_UIObjList.clear();
	m_ObjAttribute.clear();
	return true;
}
bool  SScene::Render()
{
	for (SObject* pObj : m_UIObjList)
	{
		pObj->Render();
	}
	for (SObject* pObj : m_ObjList)
	{
		if (pObj->m_bDead == false)
		{
			pObj->Render();
		}
	}
	for (auto& pInfo : m_EffectList)
	{
		SEffect* pEffect = (SEffect*)g_ObjectMgr.GetPtr(pInfo.m_csName);
		pEffect->Set(pInfo.p,
			pEffect->m_rtList[pInfo.m_iRectIndex]);
		pEffect->Render();
	}
	//int iSave = SetStretchBltMode(g_hOffScreenDC, HALFTONE);
	//int iRet = SetStretchBltMode(g_hOffScreenDC, iSave);
	//m_ButtonColor.DrawColorKey(rtDeskBt, rtSrcBt, RGB(255, 0, 255));
	return true;
}
void  SScene::AddEffect(wstring name, SPoint pos)
{
	SEffect* pEffect = (SEffect*)g_ObjectMgr.GetPtr(name);
	if (pEffect == nullptr) return;
	SEffectInfo info;
	info.m_csName = name;
	info.m_bDead = false;
	info.m_fLifeTime = 1.0f;
	info.iMaxCount = 12;
	info.m_fStep = 1.0f / pEffect->m_rtList.size();
	info.m_fStep = 0.1f;
	info.p = { pos.x, pos.y };
	m_EffectList.push_back(info);
}
bool  SScene::Reset()
{
	for (auto obj : m_UIObjList)
	{
		obj->Release();
		delete obj;
	}
	for (auto obj : m_ItemObjList)
	{
		obj->Release();
		delete obj;
	}
	for (auto obj : m_ObjList)
	{
		obj->Release();
		delete obj;
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
SScene::SScene()
{
	m_iStageCounter = 0;
	m_iSceneID = 0;
	m_bSceneChange = false;
	m_iNextScene = 1;
	m_iNpcCounter = 0;
}
SScene::~SScene()
{

}