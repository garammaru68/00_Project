#pragma once
#include "SCamera.h"
class SDebugCamera : public SCamera
{
	Vector4 m_vDirValue;
	POINT	m_ptPrePosition;
public:
	void Update(Vector4 data);
	bool Init();
	bool Frame();
public:
	SDebugCamera();
	virtual ~SDebugCamera();
};
