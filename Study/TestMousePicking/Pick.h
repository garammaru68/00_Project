#pragma once
#include "SShape.h"
#include "SCamera.h"

class Pick
{
public:
	Vector3 m_vIntersection;
public:
	bool Init();
	bool Frame();
	bool Render();
	bool Release();

	bool    GetIntersection(Vector3 vStart, Vector3 vEnd, Vector3 vNormal,
							Vector3 v0, Vector3 v1, Vector3 v2);
	bool    PointInPolygon( Vector3 vert, Vector3 faceNormal,
							Vector3 v0, Vector3 v1, Vector3 v2);

};

