#pragma once
#include "SStd.h"
struct SVertex3
{
	float x;
	float y;
	float z;
	SVertex3(){}
	SVertex3(float x, float y, float z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}
};

