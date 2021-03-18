#pragma once
#include "SCore.h"
#include "TFbxObj.h"
class Sample : public SCore
{
	TFbxObj		m_fbxObj;
public:
	bool Init() override;
	bool Frame() override;
	bool Render() override;
	bool Release() override;
};
SGAME_RUN;