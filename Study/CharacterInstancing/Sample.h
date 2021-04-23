#pragma once
#include "SCore.h"
#include "SFbxObj.h"
#define MAX_BONE_MATRICES 255
class Sample : public SCore
{
	std::shared_ptr<SFbxObj>  m_pObj;
	DWORD	m_dwKey;
	DWORD	dwKey;
	Vector3 vDirection;
	float	fDistance;
	SInput*	m_Input;

public:
	bool Init() override;
	bool Frame() override;
	bool Render() override;
	bool Release() override;
	bool Movement(DWORD dwGetKey);
};
SGAME_RUN;