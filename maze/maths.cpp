#include "maths.h"

FLOAT BarryCentric(D3DXVECTOR3 p1, D3DXVECTOR3 p2, D3DXVECTOR3 p3, D3DXVECTOR2 v)
{
	FLOAT det = (p2.z - p3.z) * (p1.x - p3.x) + (p3.x - p2.x) * (p1.z - p3.z);
	FLOAT l1 = ((p2.z - p3.z) * (v.x - p3.x) + (p3.x - p2.x) * (v.y - p3.z)) / det;
	FLOAT l2 = ((p3.z - p1.z) * (v.x - p3.x) + (p1.x - p3.x) * (v.y - p3.z)) / det;
	FLOAT l3 = 1.0f - l1 - l2;

	return l1 * p1.y + l2 * p2.y + l3 * p3.y;
}

BOOL IntersectRaySphere(D3DXVECTOR3* pOut, D3DXVECTOR3 p, D3DXVECTOR3 d, D3DXVECTOR3 sc, FLOAT r)
{
	D3DXVECTOR3 m = p - sc;
	FLOAT b = D3DXVec3Dot(&m, &d);
	FLOAT c = D3DXVec3Dot(&m, &m) - r * r;
	
	if (b > 0.0f && c > 0.0f)
		return FALSE;
	
	FLOAT discr = b * b - c;
	if (discr < 0.0f)
		return FALSE;

	if (pOut) {
		FLOAT t = -b - sqrtf(discr);
		t = max(0.0f, t);

		*pOut = p + t * d;
	}

	return TRUE;
}
