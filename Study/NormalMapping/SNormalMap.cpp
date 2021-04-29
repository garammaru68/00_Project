#include "SNormalMap.h"

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
	float fDenominator = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x); // 분모
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

	if (fDenominator < 0.0001f && fDenominator > -0.0001f)
	{
		*vTangent = Vector3(1.0f, 0.0f, 0.0f);
		*vBiNormal = Vector3(0.0f, 1.0f, 0.0f);
		*vNormal = Vector3(0.0f, 0.0f, 1.0f);
	}
	else
	{
		float fScale1 = 1.0f / fDenominator;

		Vector3 T;
		Vector3 B;
		Vector3 N;

		// 접선벡터 계산
		T = Vector3((fEdge2_V * vEdge1.x - fEdge1_V * vEdge2.x) * fScale1,
		(fEdge2_V * vEdge1.y - fEdge1_V * vEdge2.y) * fScale1,
		(fEdge2_V * vEdge1.z - fEdge1_V * vEdge2.z) * fScale1);
		B = Vector3((-fEdge2_U * vEdge1.x + fEdge1_U * vEdge2.x) * fScale1,
		(-fEdge2_U * vEdge1.y + fEdge1_U * vEdge2.y) * fScale1,
		(-fEdge2_U * vEdge1.z + fEdge1_U * vEdge2.z) * fScale1);

		N = T.Cross(B);

		float fScale2 = 1.0f / ((T.x * B.y * N.z - T.z * B.y * N.x) +
			(B.x * N.y * T.z - B.z * N.y * T.x) +
			(N.x * T.y * B.z - N.z * T.y * B.x));

		(*vTangent).x = B.Cross(N).x * fScale2;
		(*vTangent).y = -(N.Cross(T).x * fScale2);
		(*vTangent).y = T.Cross(B).x * fScale2;
		(*vTangent).Normalize(*vTangent);

		(*vBiNormal).x = -(B.Cross(N).y * fScale2);
		(*vBiNormal).y = N.Cross(T).y * fScale2;
		(*vBiNormal).z = -(T.Cross(B).y * fScale2);
		(*vTangent).Normalize(*vTangent);

		(*vNormal).x = B.Cross(N).z * fScale2;
		(*vNormal).y = -(N.Cross(T).z * fScale2);
		(*vNormal).z = T.Cross(B).z * fScale2;
		(*vTangent).Normalize(*vTangent);
		//*vBiNormal = *vBiNormal * -1.0f;
	}
}

void SNormalMap::Release()
{

}

SNormalMap::SNormalMap()
{
}
SNormalMap::~SNormalMap()
{
}