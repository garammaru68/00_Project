#include "Sample.h"
bool Sample::Init()
{
	m_Camera.CreateViewMatrix({ 0,500,-2000 }, { 0,0,0 });
	float fAspect = g_rtClient.right / (float)g_rtClient.bottom;
	m_Camera.CreateProjMatrix(1, 5000, SBASIS_PI / 4.0f, fAspect);

	// FBX LOAD
	const char* fbxobject[] =
	{
		"../../data/object/rockBlobFive.fbx",
		"../../data/object/rockBlobFour.fbx",
		"../../data/object/SM_Tree_Var01.fbx",
		//"../../data/object/ship.fbx",
		"../../data/object/SM_Rock.fbx",
		"../../data/object/SM_House_Var01.fbx",
		"../../data/object/SM_Barrel.fbx",
		//"../../data/object/sphereboxZ.fbx",
	};
	for (int iObj = 0; iObj < ARRAYSIZE(fbxobject); iObj++)
	{
		std::shared_ptr<SFbxObj> obj = make_shared<SFbxObj>();
		if (obj->Load(fbxobject[iObj]))
		{
			for (auto data : obj->m_sMeshMap)
			{
				// unordered_map 에서 SObject Data 가져오기 
				SModelObject* pObject = (SModelObject*)data.second;
				if (pObject->m_TriangleList.size() <= 0 &&
					pObject->subMesh.size() <= 0)
				{
					continue;
				}
				// 서브 Material이 없을 경우
				if (pObject->subMesh.size() == 0)
				{
					pObject->m_VertexList.resize(pObject->m_TriangleList.size() * 3); // Triangle 버텍스 크기 resize
					for (int iFace = 0; iFace < pObject->m_TriangleList.size(); iFace++) // 정점 정보
					{
						int iIndex = iFace * 3;
						pObject->m_VertexList[iIndex + 0] =
							pObject->m_TriangleList[iFace].vVertex[0];
						pObject->m_VertexList[iIndex + 1] =
							pObject->m_TriangleList[iFace].vVertex[1];
						pObject->m_VertexList[iIndex + 2] =
							pObject->m_TriangleList[iFace].vVertex[2];
					}

					T_STR loadTexName = L"../../data/object/";
					if (pObject->fbxMaterialList.size())
					{
						loadTexName += pObject->fbxMaterialList[0];
					}
					else
					{
						loadTexName.clear();
					}
					if (!pObject->Create(SBASIS_CORE_LIB::g_pd3dDevice,
						L"vs.txt",
						L"ps.txt",
						loadTexName))
					{
						return false;
					}
				}
				// 서브 Material이 있을 경우
				else
				{
					for (int iSub = 0; iSub < pObject->subMesh.size(); iSub++)
					{
						SSubMesh* pSub = &pObject->subMesh[iSub];
						pSub->m_VertexList.resize(pSub->m_TriangleList.size() * 3);
						for (int iFace = 0; iFace < pSub->m_TriangleList.size(); iFace++)
						{
							int iIndex = iFace * 3;
							pSub->m_VertexList[iIndex + 0] =
								pSub->m_TriangleList[iFace].vVertex[0];
							pSub->m_VertexList[iIndex + 1] =
								pSub->m_TriangleList[iFace].vVertex[1];
							pSub->m_VertexList[iIndex + 2] =
								pSub->m_TriangleList[iFace].vVertex[2];
						}
						// vb
						ID3D11Buffer* vb =
							CreateVertexBuffer(g_pd3dDevice,
								&pSub->m_VertexList.at(0),
								pSub->m_VertexList.size(),
								sizeof(PNCT_VERTEX));
						pSub->m_pVertexBuffer.Attach(vb);
						wstring loadTex = L"../../data/object/";
						loadTex += pObject->fbxMaterialList[iSub].c_str();
						pSub->m_pTexture = g_TexMgr.Load(g_pd3dDevice, loadTex.c_str());
					}
					if (!pObject->Create(SBASIS_CORE_LIB::g_pd3dDevice,
						L"vs.txt",
						L"ps.txt",
						L""))
					{
						return false;
					}
				}
			}
			m_ObjList.push_back(obj);
		}
	}

	return true;
}
bool Sample::Frame()
{

	return true;
}
bool Sample::Render()
{
	for (int iObj = 0; iObj < m_ObjList.size(); iObj++)
	{
		Vector3 vPos;
		vPos.x += iObj * 300.0f;

		for (auto data : m_ObjList[iObj]->m_sMeshMap)
		{
			SModelObject* pObject = (SModelObject*)data.second;
			if (pObject->subMesh.size() == 0)
			{
				if (pObject->m_TriangleList.size() <= 0) continue;

				pObject->m_matWorld._41 = vPos.x;
				pObject->m_cbData.vColor[0] = m_pMainCamera->m_vLook.x;
				pObject->m_cbData.vColor[1] = m_pMainCamera->m_vLook.y;
				pObject->m_cbData.vColor[2] = m_pMainCamera->m_vLook.z;
				pObject->SetMatrix(&pObject->m_matWorld, &m_pMainCamera->m_matView,
					&m_pMainCamera->m_matProj);
				pObject->Render(g_pImmediateContext);
			}
			else
			{
				pObject->SetMatrix(&pObject->m_matWorld, &m_pMainCamera->m_matView,
					&m_pMainCamera->m_matProj);
				pObject->m_cbData.vColor[0] = m_pMainCamera->m_vLook.x;
				pObject->m_cbData.vColor[1] = m_pMainCamera->m_vLook.y;
				pObject->m_cbData.vColor[2] = m_pMainCamera->m_vLook.z;

				pObject->SDxObject::Update(g_pImmediateContext);
				pObject->PreRender(g_pImmediateContext);
				UINT iStride = sizeof(PNCT_VERTEX);
				UINT iOffset = 0;
				g_pImmediateContext->IASetIndexBuffer(pObject->m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
				g_pImmediateContext->IASetInputLayout(pObject->m_pInputLayout);
				g_pImmediateContext->VSSetConstantBuffers(0, 1, &pObject->m_pConstantBuffer);
				g_pImmediateContext->PSSetConstantBuffers(0, 1, &pObject->m_pConstantBuffer);
				g_pImmediateContext->VSSetShader(pObject->m_pVertexShader, NULL, 0);
				g_pImmediateContext->PSSetShader(pObject->m_pPixelShader, NULL, 0);
				g_pImmediateContext->IASetPrimitiveTopology((D3D11_PRIMITIVE_TOPOLOGY)pObject->m_iTopology);

				for (int iSub = 0; iSub < pObject->subMesh.size(); iSub++)
				{
					SSubMesh* pMesh = &pObject->subMesh[iSub];
					if (pMesh->m_TriangleList.size() <= 0) continue;
					g_pImmediateContext->IASetVertexBuffers(0, 1,
						pMesh->m_pVertexBuffer.GetAddressOf(), &iStride, &iOffset);
					if (pMesh->m_pTexture != nullptr)
					{
						g_pImmediateContext->PSSetShaderResources(0, 1,
							&pMesh->m_pTexture->m_pTextureSRV);
					}
					if (pObject->m_pIndexBuffer == nullptr)
					{
						g_pImmediateContext->Draw(pMesh->m_VertexList.size(), 0);
					}
					else
					{
						PostRender();
					}
				}
			}
		}
	}
	return true;
}
bool Sample::Release()
{
	for (int iObj = 0; iObj < m_ObjList.size(); iObj++)
	{
		for (auto data : m_ObjList[iObj]->m_sMeshMap)
		{
			SModelObject* pObject = (SModelObject*)data.second;
			for (int iSub = 0; iSub < pObject->subMesh.size(); iSub++)
			{
				SSubMesh* pMesh = &pObject->subMesh[iSub];
			}
			pObject->Release();
			delete pObject;
		}
	}
	m_ObjList.clear();
	return true;
}