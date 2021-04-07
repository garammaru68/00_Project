#pragma once
#include "SCore.h"
#include "SFbxObj.h"
#define MAX_BONE_MATRICES 255
class Sample : public SCore
{
	std::shared_ptr<SFbxObj>  m_pObj;
public:

	bool Init() override;
	bool Frame() override;
	bool Render() override;
	bool Release() override;
};
SGAME_RUN;