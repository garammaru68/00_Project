#pragma once
#include "SFbxObj.h"
#include "SCamera.h"
#define MAX_BONE_MATRICES 255
class SModel : public SFbxObj
{
public:
	std::shared_ptr<SFbxObj>  m_pObj;
	SCamera* m_pMainCamera;
public:
	bool Init() override;
	bool Frame() override;
	bool Render(ID3D11DeviceContext* pd3dContext) override;
	bool Release() override;
public:
	SModel() = default;
	virtual ~SModel() = default;
};

