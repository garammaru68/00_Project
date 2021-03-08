#pragma once
#include "SFrustum.h"

class Sample
{
public:
	BOOL OBBToPlane(S_BOX* pBox, S_PLANE* pPlane);
	BOOL OBBToRay(S_BOX* pBox, S_RAY* pRay);
};

