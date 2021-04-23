#include "Sample.h"
bool Sample::Init()
{
	m_pConstantBufferLight[0].Attach(SBASIS_CORE_LIB::CreateConstantBuffer(
		g_pd3dDevice, &m_cbLight1, 1, sizeof(LIGHT_CONSTANT_BUFFER1)));
	m_pConstantBufferLight[1].Attach(SBASIS_CORE_LIB::CreateConstantBuffer(
		g_pd3dDevice, &m_cbLight2, 1, sizeof(LIGHT_CONSTANT_BUFFER2)));
	m_pConstantBufferLight[2].Attach(SBASIS_CORE_LIB::CreateConstantBuffer(
		g_pd3dDevice, &m_cbLight3, 1, sizeof(LIGHT_CONSTANT_BUFFER3)));

	m_Camera.CreateViewMatrix({ 0,500,-500 }, { 0,0,0 });
	//m_Map.CreateHeightMap(g_pd3dDevice, g_pImmediateContext,
	//	//L"../../data/map/heightMap513.bmp");
	//	L"../../data/map/129.jpg");

	SMapDesc desc;
	desc.iNumCols = 257;// m_Map.m_iNumCols;
	desc.iNumRows = 257; //m_Map.m_iNumRows;
	desc.fCellDistance = 10;
	desc.fScaleHeight = 10.0f;
	desc.szTexFile = L"../../data/map/grass_2.jpg";
	desc.szVS = L"MapObject.hlsl";
	desc.szPS = L"MapObject.hlsl";

	m_Map.CreateMap(g_pd3dDevice, g_pImmediateContext, desc);
	m_Quadtree.CreateQuadtree(&m_Map);
	return true;
}
bool Sample::Frame()
{
	// Material
	m_cbLight1.g_cAmbientMaterial[0] = Vector4(0.1f, 0.1f, 0.1f, 1);
	m_cbLight1.g_cDiffuseMaterial[0] = Vector4(1, 1, 1, 1);
	m_cbLight1.g_cSpecularMaterial[0] = Vector4(1, 1, 1, 1);
	m_cbLight1.g_cEmissionMaterial[0] = Vector4(0, 0, 0, 1);
	// Color
	m_cbLight1.g_cAmbientLightColor[0] = Vector4(0.5f, 0.0f, 0.0f, 1);
	m_cbLight1.g_cSpecularLightColor[0] = Vector4(1, 1, 1, 1);
	m_cbLight1.g_cDiffuseLightColor[0] = Vector4(1, 0, 0, 1.0f);
	m_cbLight1.g_cAmbientLightColor[1] = Vector4(0.0f, 0.5f, 0.0f, 1);
	m_cbLight1.g_cSpecularLightColor[1] = Vector4(1, 1, 1, 1);
	m_cbLight1.g_cDiffuseLightColor[1] = Vector4(0, 1, 0, 1.0f);
	m_cbLight1.g_cAmbientLightColor[2] = Vector4(0.0f, 0.0f, 0.5f, 1);
	m_cbLight1.g_cSpecularLightColor[2] = Vector4(1, 1, 1, 1);
	m_cbLight1.g_cDiffuseLightColor[2] = Vector4(0, 0, 1, 1.0f);
	// Position
	m_cbLight2.g_vLightPos[0] = Vector4(-300, 500, 300, 500);//범위
	m_cbLight2.g_vLightPos[1] = Vector4(300, 1000, -300, 500);//범위
	m_cbLight2.g_vLightPos[2] = Vector4(0, 500, 0, 500);//범위
	// Light Direction
	m_cbLight2.g_vLightDir[0] = -m_cbLight2.g_vLightPos[0];
	m_cbLight2.g_vLightDir[0].w = 1.0f;
	m_cbLight2.g_vLightDir[0].Normalize();
	m_cbLight2.g_vLightDir[1] = -m_cbLight2.g_vLightPos[1];
	m_cbLight2.g_vLightDir[1].w = 1.0f;
	m_cbLight2.g_vLightDir[1].Normalize();
	m_cbLight2.g_vLightDir[2] = -m_cbLight2.g_vLightPos[2];
	m_cbLight2.g_vLightDir[2].w = 1.0f;
	m_cbLight2.g_vLightDir[2].Normalize();
	// Eyes Direction
	for (int iLight = 0; iLight < g_iNumLight; iLight++)
	{
		Matrix matInvWorld;
		m_cbLight2.g_matInvWorld[iLight] = Matrix::Identity;
		m_cbLight2.g_vEyeDir[iLight].x = m_pMainCamera->m_vLook.x;
		m_cbLight2.g_vEyeDir[iLight].y = m_pMainCamera->m_vLook.y;
		m_cbLight2.g_vEyeDir[iLight].z = m_pMainCamera->m_vLook.z;
		m_cbLight2.g_vEyeDir[iLight].w = 50.0f; // 강도
	}

	m_cbLight3.g_vSpotInfo[0].x = 20.0f;// 내부 콘의 각도 범위	
	m_cbLight3.g_vSpotInfo[0].y = 30.0f;// 외부 콘의 각도 범위	
	m_cbLight3.g_vSpotInfo[0].z = 4;	// 내부 콘과 외부 콘의 휘도( Luminance )	
	m_cbLight3.g_vSpotInfo[0].w = 1000;// 범위	
	m_cbLight3.g_vSpotInfo[1].x = 20.0f;// 내부 콘의 각도 범위	
	m_cbLight3.g_vSpotInfo[1].y = 30.0f;// 외부 콘의 각도 범위	
	m_cbLight3.g_vSpotInfo[1].z = 4;	// 내부 콘과 외부 콘의 휘도( Luminance )	
	m_cbLight3.g_vSpotInfo[1].w = 1000;// 범위	
	m_cbLight3.g_vSpotInfo[2].x = 60.0f;// 내부 콘의 각도 범위	
	m_cbLight3.g_vSpotInfo[2].y = 90.0f;// 외부 콘의 각도 범위	
	m_cbLight3.g_vSpotInfo[2].z = 4;	// 내부 콘과 외부 콘의 휘도( Luminance )	
	m_cbLight3.g_vSpotInfo[2].w = 10000;// 범위	

	m_pImmediateContext->UpdateSubresource(m_pConstantBufferLight[0].Get(), 0, NULL, &m_cbLight1, 0, 0);
	m_pImmediateContext->UpdateSubresource(m_pConstantBufferLight[1].Get(), 0, NULL, &m_cbLight2, 0, 0);
	m_pImmediateContext->UpdateSubresource(m_pConstantBufferLight[2].Get(), 0, NULL, &m_cbLight3, 0, 0);

	ID3D11Buffer*               pBuffers[3];
	pBuffers[0] = m_pConstantBufferLight[0].Get();
	pBuffers[1] = m_pConstantBufferLight[1].Get();
	pBuffers[2] = m_pConstantBufferLight[2].Get();
	m_pImmediateContext->PSSetConstantBuffers(1, 3, pBuffers);

	return true;
}
bool Sample::Render()
{
	if (g_Input.GetKey('0') == KEY_PUSH)
	{
		SDxState::g_RasterizerDesc.FillMode = D3D11_FILL_WIREFRAME;
		SDxState::SetRasterizerState(g_pd3dDevice, g_pImmediateContext,
			SDxState::g_RasterizerDesc);
		ApplyRS(g_pImmediateContext, SDxState::g_pRSEdit.Get());
	}
	if (g_Input.GetKey('9') == KEY_PUSH)
	{
		SDxState::g_RasterizerDesc.FillMode = D3D11_FILL_SOLID;
		SDxState::SetRasterizerState(g_pd3dDevice, g_pImmediateContext,
			SDxState::g_RasterizerDesc);
		ApplyRS(g_pImmediateContext, SDxState::g_pRSEdit.Get());
	}

	m_Map.SetMatrix(NULL,
		&m_pMainCamera->m_matView,
		&m_pMainCamera->m_matProj);

	m_Map.m_cbData.vColor[0] = m_pMainCamera->m_vLook.x;
	m_Map.m_cbData.vColor[1] = m_pMainCamera->m_vLook.y;
	m_Map.m_cbData.vColor[2] = m_pMainCamera->m_vLook.z;
	for (auto node : m_Quadtree.m_leafList)
	{
		m_Quadtree.DrawNode(node, g_pImmediateContext);
	}

	m_LineShape.SetMatrix(NULL, &m_pMainCamera->m_matView,
		&m_pMainCamera->m_matProj);
	m_LineShape.Draw(g_pImmediateContext, vRayOrigin, vRayOrigin + vRayDir * 100.0f);
	m_LineShape.Draw(g_pImmediateContext, vList[0], vList[1]);
	m_LineShape.Draw(g_pImmediateContext, vList[1], vList[2]);
	m_LineShape.Draw(g_pImmediateContext, vList[2], vList[0]);
	return true;
}
bool Sample::Release()
{
	m_pObj.Release();
	m_Map.Release();
	m_Quadtree.Release();
	return true;
}