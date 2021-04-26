#pragma once
#include "SCore.h"

class SNormalMap
{
public:
	DWORD EncodeVectorAsDWORDColor(Vector3* vVector);
	void	CreateTangentSpaceVectors( Vector3 *v0, Vector3 *v1, Vector3 *v2, 
												Vector2 uv0, Vector2 uv1, Vector2 uv2,
												Vector3 *vTangent, Vector3 *vBiNormal, Vector3 *vNormal );
	void	CreateTangentSpaceVectors( Vector3 *v0, Vector3 *v1, Vector3 *v2,
												float v1u, float v1v,
												float v2u, float v2v,
												float v3u, float v3v,
												Vector3 *vTangent, Vector3 *vBiNormal, Vector3 *vNormal );
	void Release();
public:
	SNormalMap(void);
	~SNormalMap(void);
};

