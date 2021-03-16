#pragma once
#include "SCore.h"
#include "SFbxObject.h"
class Sample : public SCore
{
	SFbxObject	m_fbxObj;
public:
	bool Init() override;
	bool Frame() override;
	bool Render() override;
	bool Release() override;
};
SGAME_RUN;