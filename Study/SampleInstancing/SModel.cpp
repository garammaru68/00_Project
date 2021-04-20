#include "SModel.h"
#include "SInput.h"
bool SModel::Init()
{
	m_pObj = make_shared<SFbxObj>();
	if (m_pObj->Load("../../data/object/SM_Barrel.fbx"))
	{
		CStopwatch stop;	// Load, Convert 시간 측정
		for (auto data : m_pObj->m_tNodeList)
		{
			SModelObj* pObject = data;
			for (int iSub = 0; iSub < pObject->subMesh.size(); iSub++)
			{
				SSubMesh* pSub = &pObject->subMesh[iSub];
				if (pSub->m_iNumFace <= 0) continue;

				ID3D11Buffer* vb =
					SBASIS_CORE_LIB::CreateVertexBuffer(SBASIS_CORE_LIB::g_pd3dDevice,
						&pSub->m_VertexList.at(0),
						pSub->m_VertexList.size(),
						sizeof(PNCT_VERTEX));
				pSub->m_pVertexBuffer.Attach(vb);

				ID3D11Buffer* vbiw =
					SBASIS_CORE_LIB::CreateVertexBuffer(SBASIS_CORE_LIB::g_pd3dDevice,
						&pSub->m_VertexListIW.at(0),
						pSub->m_VertexListIW.size(),
						sizeof(IW_VERTEX));
				pSub->m_pVertexBufferIW.Attach(vbiw);
				
				ID3D11Buffer* ib =
					SBASIS_CORE_LIB::CreateIndexBuffer(g_pd3dDevice,
						&pSub->m_IndexArray.at(0),
						pSub->m_IndexArray.size(),
						sizeof(DWORD));
				pSub->m_pIndexBuffer.Attach(ib);

				// Material Load
				wstring loadTex = L"../../data/object/";
				loadTex += pObject->fbxMaterialList[iSub].c_str();
				pSub->m_pTexture = g_TexMgr.Load(g_pd3dDevice, loadTex.c_str());
			}
		}
		if (!m_pObj->Create(SBASIS_CORE_LIB::g_pd3dDevice,
			L"../../data/shader/objectVS.txt",
			L"../../data/shader/objectPS.txt",
			L""))
		{
			return false;
		}
		stop.Output(L"convert");
	}
	D3D11_BUFFER_DESC vbdesc =
	{
		MAX_BONE_MATRICES * sizeof(Matrix),
		D3D11_USAGE_DYNAMIC,
		D3D11_BIND_CONSTANT_BUFFER,
		D3D11_CPU_ACCESS_WRITE,
		0
	};
	g_pd3dDevice->CreateBuffer(&vbdesc, NULL, m_pObj->m_pBoneBuffer.GetAddressOf());

	return true;
}
bool SModel::Frame()
{
	m_pObj->m_fTick += g_fSecondPerFrame *
		m_pObj->m_Scene.iFrameSpeed *
		m_pObj->m_Scene.iTickPerFrame * 0.0f;

	if (m_pObj->m_fTick >=
		(m_pObj->m_Scene.iLastFrame * m_pObj->m_Scene.iTickPerFrame))
	{

	}
}