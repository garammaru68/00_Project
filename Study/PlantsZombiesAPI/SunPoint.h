#pragma once
#include "SItemObject.h"
#include "SInput.h"

class SunPoint : public SItemObject
{
public:
	void Initialize();
	bool Frame()	override;
};

