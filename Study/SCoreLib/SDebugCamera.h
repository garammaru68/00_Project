#pragma once
#include "SCamera.h"
class SDebugCamera : public SCamera
{

public:
	void Update(Vector4 data);
	bool Init();
	bool Frame();
public:
	SDebugCamera();
	virtual ~SDebugCamera();
};

