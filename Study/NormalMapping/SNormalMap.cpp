#include "SNormalMap.h"

DWORD SNormalMap::EncodeVectorAsDWORDColor(Vector3* vVector)
{
	DWORD dwRed		 = (DWORD)(127 * vVector->x + 128);
	DWORD dwGreen	 = (DWORD)(127 * vVector->y + 128);
	DWORD dwBlue	 = (DWORD)(127 * vVector->z + 128);
	return (DWORD)(0xff000000 + (dwRed << 16) + (dwGreen << 8) + dwBlue);
}
void SNormalMap::CreateTangentSpaceVectors(
	Vector3 *v0, Vector3 *v1, Vector3 *v2,
	Vector2 uv0, Vector2 uv1, Vector2 uv2,
	Vector3 *vTangent, Vector3 *vBiNormal, Vector3 *vNormal)
{
	Vector3 vEdge1 = *v1 - *v0;
	Vector3 vEdge2 = *v2 - *v0;
	vEdge1.Normalize();
	vEdge2.Normalize();
	// UV delta
	Vector2 deltaUV1 = uv1 - uv0;
	Vector2 deltaUV2 = uv2 - uv0;
	deltaUV1.Normalize();
	deltaUV2.Normalize();

	Vector3 biNormal;
	float fDenominator = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x); // ºÐ¸ð
	if ( fabsf( fDenominator ) < 1e-6f )
	{
		*vTangent = Vector3(1.0f, 0.0f, 0.0f);
		biNormal  = Vector3(0.0f, 0.0f, 1.0f);
	}
	else
	{
      *vTangent = (vEdge1 * deltaUV2.y - vEdge2 * deltaUV1.y) * fDenominator;
		biNormal	 = (vEdge2 * deltaUV1.x - vEdge1 * deltaUV2.x) * fDenominator;
	}
	vTangent->Normalize();
	biNormal.Normalize();
	
	vNormal->Cross(*vTangent, *vBiNormal);
	float fCrossInv = ( vBiNormal->Dot( biNormal ) < 0.0f ) ? -1.0f : 1.0f;
	*vBiNormal *= fCrossInv;
}
void SNormalMap::CreateTangentSpaceVectors(
	Vector3 *v0, Vector3 *v1, Vector3 *v2,
	float v1u, float v1v,
	float v2u, float v2v,
	float v3u, float v3v,
	Vector3 * vTangent, Vector3 * vBiNormal, Vector3 * vNormal)
{
	Vector3 vEdge1 = *v1 - * v0;
	Vector3 vEdge2 = *v2 - * v0;
	float fEdge1_U = v2u - v1u;
	float fEdge1_V = v2v - v1v;
	float fEdge2_U = v3u - v1u;
	float fEdge2_V = v3v - v1v;

	float fDenominator = fEdge1_U * fEdge2_V - fEdge2_U * fEdge1_V;
}