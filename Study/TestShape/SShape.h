#pragma once
#include "SStd.h"
#include "WICTextureLoader.h" // ../../include/DirectXTK
#include "SMatrix.h"
struct P_VERTEX
{
	Vector3 p;
	Vector3 n;
	Vector4 c;
	Vector2 t;
	P_VERTEX() {}
	P_VERTEX(Vector3 p,
		Vector3 n,
		Vector4 c,
		Vector2 tagABORTPATH)
};
class SShape
{
};

