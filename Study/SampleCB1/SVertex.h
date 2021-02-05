#pragma once
#include "SStd.h"
struct SVertex2
{
	float x;
	float y;
	SVertex2() {}
	SVertex2(float x, float y)
	{
		this->x = x;
		this->y = y;
	}
};
struct SVertex3
{
	float x;
	float y;
	float z;
	SVertex3() {}
	SVertex3(float x, float y, float z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}
};
struct SVertex4
{
	float x;
	float y;
	float z;
	float w;
	SVertex4() {}
	SVertex4(float x, float y, float z, float w)
	{
		this->x = x;
		this->y = y;
		this->z = z;
		this->w = w;
	}
};
