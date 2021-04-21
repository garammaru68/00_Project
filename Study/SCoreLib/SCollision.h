#pragma once
#include "SStd.h"
#include "SimpleMath.h"
using namespace DirectX::SimpleMath;
struct S_RAY
{
	Vector3 vDirection;
	Vector3 vOrigin;
};
struct S_SPHERE
{
	Vector3		vCenter;
	float		fRadius;
};
struct S_BOX
{
	// Common
	Vector3		vCenter;
	Vector3		vPos[8];
	// AABB
	Vector3		vMax;
	Vector3		vMin;
	// OBB
	Vector3		vAxis[3];
	float		fExtent[3];
};
class SCollision
{
public:
	static bool SphereToSphere(S_SPHERE rt1, S_SPHERE rt2);
	static bool RectInPt(RECT rt, POINT pt);
	static bool Rect2Rect(RECT rt1, RECT rt2);
};

