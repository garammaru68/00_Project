#include "Sample.h"
// WALK			3 ~ 35
// SideStep		36 ~ 101
// Stand		363 ~ 537
// Sitdown		538 ~ 598
// SitHold		599 ~ 783
// Standup		780 ~ 837
// Run			838 ~ 856
// Lookaround	857 ~ 1105
// WALK2		1106 ~ 1135
// Run2			1166 ~ 1185
// Jump			1386 ~ 1398
bool Sample::Init()
{
	m_Camera.CreateViewMatrix(
		{ 0,150,-200 },
		{ 0,0, 0 });
	float fAspect = g_rtClient.right / (float)g_rtClient.bottom;
	m_Camera.CreateProjMatrix(1, 5000, SBASIS_PI / 4.0f, fAspect);

	m_pObj = make_shared<SFbxObj>();
	// FBX 파일 로드
	//if (m_pObj->Load("../../data/object/man.fbx"))
		//if (m_pObj->Load("../../data/object/Turret_Deploy1.fbx"))
		if (m_pObj->Load("../../data/object/Scifi_Model_L2_all_in_one.fbx"))	
	{
		CStopwatch stop;
		for (auto data : m_pObj->m_sNodeList)
		{
			SModelObj* pObject = data;
			for (int iSub = 0; iSub < pObject->subMesh.size(); iSub++)
			{
				SSubMesh* pSub = &pObject->subMesh[iSub];
				if (pSub->m_iNumFace <= 0) continue;

				/*pSub->m_VertexList.resize(	pSub->m_TriangleList.size() * 3);
				for (int iFace = 0; iFace < pSub->m_TriangleList.size(); iFace++)
				{
					int iIndex = iFace * 3;
					pSub->m_VertexList[iIndex + 0] =pSub->m_TriangleList[iFace].vVertex[0];
					pSub->m_VertexList[iIndex + 1] =pSub->m_TriangleList[iFace].vVertex[1];
					pSub->m_VertexList[iIndex + 2] =pSub->m_TriangleList[iFace].vVertex[2];
				}*/
				// vb
				ID3D11Buffer* vb =
					CreateVertexBuffer(g_pd3dDevice,
						&pSub->m_VertexList.at(0),
						pSub->m_VertexList.size(),
						sizeof(PNCT_VERTEX));
				pSub->m_pVertexBuffer.Attach(vb);

				//pSub->m_VertexListIW.resize(pSub->m_TriangleList.size() * 3);
				//for (int iFace = 0; iFace < pSub->m_TriangleList.size(); iFace++)
				//{
				//		int iIndex = iFace * 3;
				//		pSub->m_VertexListIW[iIndex + 0] =pSub->m_TriangleList[iFace].vVertexIW[0];
				//		pSub->m_VertexListIW[iIndex + 1] =pSub->m_TriangleList[iFace].vVertexIW[1];
				//		pSub->m_VertexListIW[iIndex + 2] =pSub->m_TriangleList[iFace].vVertexIW[2];
				//}
				// vb + 가중치
				ID3D11Buffer* vbiw =
					CreateVertexBuffer(g_pd3dDevice,
						&pSub->m_VertexListIW.at(0),
						pSub->m_VertexListIW.size(),
						sizeof(IW_VERTEX));
				pSub->m_pVertexBufferIW.Attach(vbiw);

				// ib
				ID3D11Buffer* ib =
					CreateIndexBuffer(g_pd3dDevice,
						&pSub->m_IndexArray.at(0),
						pSub->m_IndexArray.size(),
						sizeof(DWORD));
				pSub->m_pIndexBuffer.Attach(ib);

				// Sub Material
				wstring loadTex = L"../../data/object/";
				loadTex += pObject->fbxMaterialList[iSub].c_str();
				pSub->m_pTexture = g_TexMgr.Load(g_pd3dDevice, loadTex.c_str());
			}
			/*if (!pObject->Create(SBASIS_CORE_LIB::g_pd3dDevice,
				L"vs.txt",
				L"ps.txt",
				L""))
			{
				return false;
			}*/
		}
		// Object 생성 ( cb, vb, ib, vd, id, shader, texture, InputLayout )
		if (!m_pObj->Create(SBASIS_CORE_LIB::g_pd3dDevice,
			L"vs.txt",
			L"ps.txt",
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
		D3D11_BIND_CONSTANT_BUFFER, //D3D11_BIND_SHADER_RESOURCE,
		D3D11_CPU_ACCESS_WRITE,
		0
	};
	// 
	m_pd3dDevice->CreateBuffer(&vbdesc, NULL, m_pObj->m_pBoneBuffer.GetAddressOf());

	m_pObj->m_Scene.iFirstFrame = 36;
	m_pObj->m_Scene.iLastFrame = 101;
	m_pObj->m_fTick = m_pObj->m_Scene.iFirstFrame * 160;

	//dwKey = KEY_FREE;
	vDirection = m_pObj->m_vLook;
	fDistance = 1.0;
	return true;
}
bool Sample::Frame()
{
	//GetAsyncKeyState(dwKey);
	if (g_Input.GetKey(VK_UP) == KEY_HOLD ||
		g_Input.GetKey(VK_DOWN) == KEY_HOLD || 
		g_Input.GetKey(VK_LEFT) == KEY_HOLD ||
		g_Input.GetKey(VK_RIGHT) == KEY_HOLD)
	{
		m_Input->SetEnable(true);
		if (m_Input->GetEnable() == true)
		m_pObj->m_Scene.iFirstFrame = 5;
		m_pObj->m_Scene.iLastFrame = 35;

		if (dwKey == VK_UP || dwKey == VK_DOWN) vDirection = m_pObj->m_vLook;
		else if (dwKey == VK_LEFT || dwKey == VK_RIGHT) vDirection = m_pObj->m_vRight;

		if (dwKey == VK_DOWN || dwKey == VK_LEFT) fDistance = -1.0f;
		else fDistance = 1.0f;

		Vector3 vMove = vDirection * g_fSecondPerFrame * m_pObj->m_fSpeed * fDistance;
		m_pObj->m_vPos += vMove;

	}
	//if (g_Input.GetKey(dwKey) == KEY_UP)
	//{
	//	m_pObj->m_Scene.iFirstFrame = 36;
	//	m_pObj->m_Scene.iLastFrame = 101;
	//	dwKey = 0;
	//}
	//if (g_Input.GetKey(VK_UP) == KEY_HOLD)
	//{
	//	//SAnimTrack track;
	//	//Quaternion qRotation(0.0f, 10.0f, 0.0f, 1.0f);	
	//	//track.q += qRotation;
	//	//m_Scene.iFirstFrame = 3;
	//	m_pObj->m_Scene.iFirstFrame = 5;
	//	m_pObj->m_Scene.iLastFrame = 35;
	//	Vector3 vMove = m_pObj->m_vLook * g_fSecondPerFrame * m_pObj->m_fSpeed * 1.0f;
	//	m_pObj->m_vPos += vMove;
	//}
	//if (g_Input.GetKey(VK_DOWN) == KEY_HOLD)
	//{
	//	m_pObj->m_Scene.iFirstFrame = 5;
	//	m_pObj->m_Scene.iLastFrame = 35;
	//	Vector3 vMove = m_pObj->m_vLook * g_fSecondPerFrame * m_pObj->m_fSpeed * -1.0f;
	//	m_pObj->m_vPos += vMove;
	//}
	//if (g_Input.GetKey(VK_LEFT) == KEY_HOLD)
	//{
	//	m_pObj->m_Scene.iFirstFrame = 5;
	//	m_pObj->m_Scene.iLastFrame = 35;
	//	Vector3 vMove = m_pObj->m_vRight * g_fSecondPerFrame * m_pObj->m_fSpeed * -1.0f;
	//	m_pObj->m_vPos += vMove;
	//}
	//if (g_Input.GetKey(VK_RIGHT) == KEY_HOLD)
	//{
	//	m_pObj->m_Scene.iFirstFrame = 5;
	//	m_pObj->m_Scene.iLastFrame = 35;
	//	Vector3 vMove = m_pObj->m_vRight * g_fSecondPerFrame * m_pObj->m_fSpeed * 1.0f;
	//	m_pObj->m_vPos += vMove;
	//	//Quaternion vRot = m_matRotation * g_fSecondPerFrame * m_fSpeed * 5.0f;
	//}
	if (g_Input.GetKey(VK_UP) == KEY_UP ||
		g_Input.GetKey(VK_DOWN) == KEY_UP ||
		g_Input.GetKey(VK_RIGHT) == KEY_UP ||
		g_Input.GetKey(VK_LEFT) == KEY_UP)
	{
		m_pObj->m_Scene.iFirstFrame = 36;
		m_pObj->m_Scene.iLastFrame = 101;

		m_Input->SetEnable(false);
	}

	// Tick 계산
	m_pObj->m_fTick += g_fSecondPerFrame *
		m_pObj->m_Scene.iFrameSpeed *
		m_pObj->m_Scene.iTickPerFrame * 1.0f;

	// 마지막 프레임에서 처음으로 돌아온다
	if (m_pObj->m_fTick >=
		(m_pObj->m_Scene.iLastFrame * m_pObj->m_Scene.iTickPerFrame))
	{
		m_pObj->m_fTick = m_pObj->m_Scene.iFirstFrame * 160;
	}

	for (int iNode = 0; iNode < m_pObj->m_sNodeList.size(); iNode++)
	{
		Matrix matWorld = Matrix::Identity;
		SModelObj* pModelObject = m_pObj->m_sNodeList[iNode];
		// 바이패드공간 * global 
		std::string szName;
		szName.assign(pModelObject->m_szName.begin(), pModelObject->m_szName.end());
		Matrix matBiped = Matrix::Identity;
		auto data = m_pObj->m_dxMatrixBindPoseMap.find(szName);
		if (data != m_pObj->m_dxMatrixBindPoseMap.end())
		{
			matBiped = data->second;
		}
		Matrix matParent = Matrix::Identity;
		if (pModelObject->m_pParentObject != nullptr)
		{
			matParent = pModelObject->m_pParentObject->m_matAnim;
		}
		//SAnimTrack sTrack = pModelObject->animlist[0];
		// 보간
		for (int iTick = 1; iTick < pModelObject->animlist.size() - 1; iTick++)
		{
			if (pModelObject->animlist[iTick].iTick >=
				m_pObj->m_fTick)
			{
				int iStart = pModelObject->animlist[iTick - 1].iTick;
				int iEnd = pModelObject->animlist[iTick].iTick;
				int iStepTick = iEnd - iStart;
				float t = (m_pObj->m_fTick - iStart) / iStepTick;
				Vector3 vStart, vEnd, vPos;
				vStart = pModelObject->animlist[iTick - 1].p;
				vEnd = pModelObject->animlist[iTick].p;
				vPos = Vector3::Lerp(vStart, vEnd, t);
				Vector3 vScale;
				vStart = pModelObject->animlist[iTick - 1].s;
				vEnd = pModelObject->animlist[iTick].s;
				vScale = Vector3::Lerp(vStart, vEnd, t);

				Quaternion q1, q2, qRot;
				q1 = pModelObject->animlist[iTick - 1].q;
				q2 = pModelObject->animlist[iTick].q;
				qRot = Quaternion::Slerp(q1, q2, t);

				Matrix matScale = Matrix::CreateScale(vScale);
				Matrix matRotate = Matrix::CreateFromQuaternion(qRot);
				Matrix matTrans = Matrix::CreateTranslation(vPos);
				pModelObject->m_matAnim = matScale * matRotate * matTrans *matParent;
				//pModelObject->m_matAnim = pModelObject->animlist[iTick].mat;
				
				m_pObj->m_pMatrixList[iNode] = /*matBiped **/ pModelObject->m_matAnim;
				break;
			}
		}
	}
	return true;
}
bool Sample::Render()
{
	g_pImmediateContext->VSSetConstantBuffers(1, 1, m_pObj->m_pBoneBuffer.GetAddressOf());

	m_pObj->SetMatrix(NULL, &m_pMainCamera->m_matView, &m_pMainCamera->m_matProj);

	m_pObj->m_cbData.vColor[0] = m_pMainCamera->m_vLook.x;
	m_pObj->m_cbData.vColor[1] = m_pMainCamera->m_vLook.y;
	m_pObj->m_cbData.vColor[2] = m_pMainCamera->m_vLook.z;
	m_pObj->SDxObject::Update(g_pImmediateContext);
	m_pObj->PreRender(g_pImmediateContext);

	//g_pImmediateContext->IASetIndexBuffer(pObject->m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	g_pImmediateContext->IASetInputLayout(m_pObj->m_pInputLayout);
	g_pImmediateContext->VSSetConstantBuffers(0, 1, &m_pObj->m_pConstantBuffer);
	g_pImmediateContext->PSSetConstantBuffers(0, 1, &m_pObj->m_pConstantBuffer);
	g_pImmediateContext->VSSetShader(m_pObj->m_pVertexShader, NULL, 0);
	g_pImmediateContext->PSSetShader(m_pObj->m_pPixelShader, NULL, 0);
	g_pImmediateContext->IASetPrimitiveTopology((D3D11_PRIMITIVE_TOPOLOGY)m_pObj->m_iTopology);


	for (int iNode = 0; iNode < m_pObj->m_sNodeList.size(); iNode++)
	{
		Matrix matWorld = Matrix::Identity;
		SModelObj* pObject = m_pObj->m_sNodeList[iNode];
		if (pObject->m_dxMatrixBindPosMap.size() <= 0)
		{
			continue;
		}

		// 상수버퍼 업데이트
		Matrix* pMatrices;
		D3D11_MAPPED_SUBRESOURCE MappedFaceDest;
		if (SUCCEEDED(g_pImmediateContext->Map((ID3D11Resource*)m_pObj->m_pBoneBuffer.Get(),
					  0, D3D11_MAP_WRITE_DISCARD, 0, &MappedFaceDest)))
		{
			pMatrices = (Matrix*)MappedFaceDest.pData;
			for (int sNode = 0; sNode < m_pObj->m_sNodeList.size(); sNode++)
			{
				SModelObj* bone = m_pObj->m_sNodeList[sNode];
				std::string szName;
				szName.assign(bone->m_szName.begin(), bone->m_szName.end());
				Matrix matBiped = Matrix::Identity;
				auto data = pObject->m_dxMatrixBindPosMap.find(szName);
				if (data != pObject->m_dxMatrixBindPosMap.end())
				{
					matBiped = data->second;
				}
				Matrix matAnim = matBiped * m_pObj->m_pMatrixList[sNode];
				pMatrices[sNode] = matAnim.Transpose();
			}
			g_pImmediateContext->Unmap(m_pObj->m_pBoneBuffer.Get(), 0);
		}

		for (int iSub = 0; iSub < pObject->subMesh.size(); iSub++)
		{
			SSubMesh* pMesh = &pObject->subMesh[iSub];
			if (pMesh->m_iNumFace <= 0) continue;

			ID3D11Buffer* vb[2] = { pMesh->m_pVertexBuffer.Get(),	pMesh->m_pVertexBufferIW.Get() };
			UINT stride[2] = { sizeof(PNCT_VERTEX), sizeof(IW_VERTEX) };
			UINT offset[2] = { 0, 0 };
			g_pImmediateContext->IASetVertexBuffers(0, 2, vb, stride, offset);
			g_pImmediateContext->IASetIndexBuffer(pMesh->m_pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

			if (pMesh->m_pTexture != nullptr)
			{
				g_pImmediateContext->PSSetShaderResources(0, 1, &pMesh->m_pTexture->m_pTextureSRV);
			}
			else
			{
				g_pImmediateContext->PSSetShaderResources(0, 1, &g_TexMgr.m_pWhiteTexture->m_pTextureSRV);
			}
			if (pMesh->m_pIndexBuffer == nullptr)
			{
				g_pImmediateContext->Draw(pMesh->m_VertexList.size(), 0);
			}
			else
			{
				g_pImmediateContext->DrawIndexed(pMesh->m_IndexArray.size(), 0, 0);
			}
		}
	}
	return true;
}
bool Sample::Release()
{
	for (auto data : m_pObj->m_sNodeList)
	{
		SModelObj* pModelObject = data;
		pModelObject->Release();
		delete pModelObject;
	}
	m_pObj->Release();
	return true;
}
bool Sample::Movement(DWORD dwGetKey)
{
	return true;
}