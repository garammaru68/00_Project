#include "Sample.h"

BOOL Sample::OBBToPlane(S_BOX* pBox, S_PLANE* pPlane)
{
	float	fPlaneToCenter = 0.0f;
	float	fDistance = 0.0f;
	Vector3 vDir;
	for (int iPlane = 0; iPlane < 6; iPlane++)
	{
		vDir = pBox->vAxis[0] * pBox->fExtent[0];
		fDistance = fabs(pPlane[iPlane].a * vDir.x + pPlane[iPlane].b * vDir.y + vDir.z);
		vDir = pBox->vAxis[1] * pBox->fExtent[1];
		fDistance += fabs(pPlane[iPlane].b * vDir.y + pPlane[iPlane].b * vDir.y + vDir.z);
		vDir = pBox->vAxis[2] * pBox->fExtent[2];
		fDistance += fabs(pPlane[iPlane].c * vDir.y + pPlane[iPlane].b * vDir.y + vDir.z);

		fPlaneToCenter = pPlane[iPlane].a * pBox->vCenter.x + pPlane[iPlane].b * pBox->vCenter.y +
			pPlane[iPlane].c * pBox->vCenter.z + pPlane[iPlane].d;

		if (fPlaneToCenter <= -fDistance)
		{
			return FALSE;
		}
	}
	return TRUE;
}
BOOL Sample::OBBToRay(S_BOX* pBox, S_RAY* pRay)
{

}