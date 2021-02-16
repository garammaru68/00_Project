#pragma once
#include "SCamera.h"
class SDebugCamera : public SCamera
{
	Vector4 m_vDirValue;
public:
	void Update(Vector4 data);
	bool Frame();
public:
	SDebugCamera();
	virtual ~SDebugCamera();
};

