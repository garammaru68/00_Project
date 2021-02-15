#include "Sample.h"
xMatrix* SD3DXMatrixShadow(xMatrix *pout, Vector4 *plight, Vector4 *pplane)
{
	Vector4 Nplane;
	FLOAT dot;
	Nplane = Nplane.Normal();
	//D3DXPlaneNormalize(&Nplane, pplane);
	dot = Nplane | *plight;
	pout->m[0][0] = dot - Nplane.x * plight->x;
	pout->m[0][1] = -Nplane.x * plight->y;
	pout->m[0][2] = -Nplane.x * plight->z;
	pout->m[0][3] = -Nplane.x * plight->w;
	pout->m[1][0] = -Nplane.y * plight->x;
	pout->m[1][1] = dot - Nplane.y * plight->y;
	pout->m[1][2] = -Nplane.y * plight->z;
	pout->m[1][3] = -Nplane.y * plight->w;
	pout->m[2][0] = -Nplane.z * plight->x;
	pout->m[2][1] = -Nplane.z * plight->y;
	pout->m[2][2] = dot - Nplane.z * plight->z;
	pout->m[2][3] = -Nplane.z * plight->w;
	pout->m[3][0] = -Nplane.w * plight->x;
	pout->m[3][1] = -Nplane.w * plight->y;
	pout->m[3][2] = -Nplane.w * plight->z;
	pout->m[3][3] = dot - Nplane.w * plight->w;
	return pout;
}
xMatrix Sample::CreateMatrixShadow(
	Vector4* pPlane,
	Vector4* pLight)
{
	xMatrix mat;
	Vector4 plane, light;
	pPlane->Normal();
	plane.x = pPlane->x * -1.0f;
	plane.y = pPlane->y * -1.0f;
	plane.z = pPlane->z * -1.0f;
	plane.w = pPlane->w * -1.0f;
	light = *pLight;// * -1.0f;
	float D = -(plane | light);
	mat._11 = plane.x * light.x + D;	mat._12 = plane.x * light.y;		mat._13 = plane.x * light.z;		mat._14 = plane.x * light.w;
	mat._21 = plane.y * light.x;		mat._22 = plane.y * light.y + D;	mat._23 = plane.y * light.z; 		mat._24 = plane.y * light.w;
	mat._31 = plane.z * light.x;		mat._32 = plane.z * light.y;		mat._33 = plane.z * light.z + D;	mat._34 = plane.z * light.w;
	mat._41 = plane.w * light.x;		mat._42 = plane.w * light.y;		mat._43 = plane.w * light.z;		mat._44 = plane.w * light.w + D;
}