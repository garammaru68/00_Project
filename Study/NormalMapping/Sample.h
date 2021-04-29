#pragma once
#include "SCore.h"
#include "FbxObj.h"
#define MAX_BONE_MATRICES 255
class Sample : public SCore
{
	STexture* m_pNormalMap = nullptr;
	std::shared_ptr<FbxObj>  m_pObj;
public:
	bool Init() override;
	bool Frame() override;
	bool Render() override;
	bool Release() override;
};
SGAME_RUN;