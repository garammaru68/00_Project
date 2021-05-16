#pragma once
#include "STemplate.h"
#include "SDxHelperEx.h"
// Sub texture types
#define ID_TBASIS_AM 0   // ambient
#define ID_TBASIS_DI 1   // diffuse
#define ID_TBASIS_SP 2   // specular
#define ID_TBASIS_SH 3   // shininesNs
#define ID_TBASIS_SS 4   // shininess strength
#define ID_TBASIS_SI 5   // self-illumination
#define ID_TBASIS_OP 6   // opacity
#define ID_TBASIS_FI 7   // filter color
#define ID_TBASIS_BU 8   // bump 
#define ID_TBASIS_RL 9   // reflection
#define ID_TBASIS_RR 10  // refraction 
#define ID_TBASIS_DP 11  // displacement
#define ID_TBASIS_GN 12  // Generic
#define ID_TBASIS_EV 13  // EnvMap

using namespace SBASIS;

const enum OBJECTCLASSTYPE {
	CLASS_GEOM = 0,
	CLASS_BONE,
	CLASS_DUMMY,
	CLASS_BIPED,
};

const enum FILETYPE {
	NULLFILE = 0,
	ASEFILE,
	TBSFILE,
	SKMFILE,
	MATFILE,
};


struct TScene
{
	int iVersion;		// ����
	int	iFirstFrame;    // ���� ������
	int	iLastFrame;     // ������ ������
	int	iFrameSpeed;    // 1�ʴ� ������ ����(30)
	int	iTickPerFrame;  // 1�������� ƽ ��(160)
	int iNumMesh;		// �޽�������Ʈ ����
	int iMaxWeight;		// ���� �� ����ġ
	int iBindPose;		// ���ε� ���� ���ϸ��̼� ����
};
struct VersionMark
{
	int		iVersion;
	TCHAR	description[128];
};

struct TTextexMap
{
	// ������Ʈ �ؽ��� �����ڿ��� ���͸����� �ؽ������ϸ��� ���� DX�ؽ��� ���� �� �����ε����� �����Ѵ�.
	DWORD		m_dwIndex;
	// �ؽ��� �� Ÿ���ε���
	DWORD		m_dwType;
	// �� �̸�
	T_STR		m_strName;
	// �� Ŭ���� �̸�
	T_STR		m_strClassName;
	// �� �ؽ��� �̸�.
	T_STR		m_strTextureName;
	TTextexMap() : m_dwType(1), m_dwIndex(0) {}
};
struct TMtrl
{
	// ���͸��� �̸�
	T_STR		m_strName;
	// ���͸��� Ŭ���� �̸�
	T_STR		m_strClassName;
	// ��Ÿ�Ե�
	vector<TTextexMap>	m_TexMaps;
	// ���� ���͸��� ���庤��
	vector<TMtrl>		m_SubMaterial;
	//TBS
	DWORD m_dwSubCount;
	DWORD m_dwTexMapCount;
	DWORD m_dwIndex;

	TMtrl() {}
	~TMtrl()
	{
		stl_wipe_vector(m_SubMaterial);
		stl_wipe_vector(m_TexMaps);
	}
};


struct TFaceList
{
	union
	{
		struct { DWORD	_0, _1, _2; };
		DWORD v[3];
	};
	DWORD   dwMtrl;
	TFaceList() : dwMtrl(0), _0(0), _1(0), _2(0)
	{}
};

struct TVertexList
{
	DWORD					dwNumVertex;
	DWORD					dwNumFace;
	vector<Vector3>		pVertexList;
	vector<TFaceList>		pFaceList;
	vector<DWORD>			pSubListMtl;
	TVertexList() : dwNumVertex(0), dwNumFace(0)
	{
	}
	~TVertexList() { stl_wipe_vector(pVertexList); stl_wipe_vector(pVertexList); }
};

struct TAnimTrack
{
	int					iTick;
	Quaternion		qRotate;
	Vector3			vVector;
	TAnimTrack*			pNext;
	TAnimTrack*			pPrev;
};

//--------------------------------------------------------------------------------------
// Structures
//--------------------------------------------------------------------------------------
struct T_PLANE
{
	float	fA, fB, fC, fD;
	bool	CreatePlane(Vector3 v0, Vector3 v1, Vector3 v2);
	bool	CreatePlane(Vector3 vNormal, Vector3 v0);
	void	Normalize()
	{
		float fMag = sqrt(fA*fA + fB*fB + fC*fC);
		fA = fA / fMag;
		fB = fB / fMag;
		fC = fC / fMag;
		fD = fD / fMag;
	}

};
struct T_SPHERE
{
	Vector3		vCenter;
	float			fRadius;
};
struct T_BOX
{
	// Common
	Vector3		vCenter;
	Vector3		vPos[8];
	// AABB
	Vector3		vMax;
	Vector3		vMin;
	// OBB
	Vector3		vAxis[3];
	float			fExtent[3];
};
//--------------------------------------------------------------------------------------
// Structures
//--------------------------------------------------------------------------------------
struct PCT_VERTEX
{
	Vector3 p;
	Vector4 c;
	Vector2 t;
	bool operator == (const PCT_VERTEX & Vertex)
	{
		if (p == Vertex.p  && c == Vertex.c    && t == Vertex.t)
		{
			return true;
		}
		return  false;
	}
	PCT_VERTEX() {}
	PCT_VERTEX(Vector3		vp,
		Vector4		vc,
		Vector2     vt)
	{
		p = vp, c = vc, t = vt;
	}
};
struct PCT2_VERTEX
{
	Vector3 p;
	Vector4 c;
	Vector2 t0;
	Vector2 t1;
	bool operator == (const PCT2_VERTEX & Vertex)
	{
		if (p == Vertex.p  && c == Vertex.c    && t0 == Vertex.t0 && t1 == Vertex.t1)
		{
			return true;
		}
		return  false;
	}
	PCT2_VERTEX() {}
	PCT2_VERTEX(Vector3		vp,
		Vector4		vc,
		Vector2     vt0,
		Vector2     vt1)
	{
		p = vp, c = vc, t0 = vt0, t1 = vt1;
	}
};
struct PC_VERTEX
{
	Vector3 p;
	Vector4 c;
	bool operator == (const PC_VERTEX & Vertex)
	{
		if (p == Vertex.p  && c == Vertex.c)
		{
			return true;
		}
		return  false;
	}
	PC_VERTEX() {}
	PC_VERTEX(Vector3		vp,
		Vector4		vc)
	{
		p = vp, c = vc;
	}
};
struct PNC_VERTEX
{
	Vector3 p;
	Vector3 n;
	Vector4 c;
	bool operator == (const PNC_VERTEX & Vertex)
	{
		if (p == Vertex.p  && n == Vertex.n && 	c == Vertex.c)
		{
			return true;
		}
		return  false;
	}
	PNC_VERTEX() {}
	PNC_VERTEX(Vector3		vp,
		Vector3		vn,
		Vector4		vc)
	{
		p = vp, n = vn, c = vc;
	}
};
struct PNCT_VERTEX
{
	Vector3		p;
	Vector3		n;
	Vector4		c;
	Vector2     t;
	bool operator == (const PNCT_VERTEX & Vertex)
	{
		if (p == Vertex.p  && n == Vertex.n && 	c == Vertex.c  &&	t == Vertex.t)
		{
			return true;
		}
		return  false;
	}
	PNCT_VERTEX() {}
	PNCT_VERTEX(Vector3		vp,
		Vector3		vn,
		Vector4		vc,
		Vector2     vt)
	{
		p = vp, n = vn, c = vc, t = vt;
	}
};
struct PNCT2_VERTEX
{
	Vector3		p;
	Vector3		n;
	Vector4		c;
	Vector2     t;
	Vector3		vTangent;
	bool operator == (const PNCT2_VERTEX & Vertex)
	{
		if (p == Vertex.p  && n == Vertex.n && 	c == Vertex.c   &&	vTangent == Vertex.vTangent)
		{
			return true;
		}
		return  false;
	}
	PNCT2_VERTEX() {}
	PNCT2_VERTEX(Vector3		vp,
		Vector3		vn,
		Vector4		vc,
		Vector2     vt,
		Vector3		tangent )
	{
		p = vp, n = vn, c = vc, t = vt, vTangent = tangent;
	}
};
// ������������������������������������������������������������������
//  ���̴� ����( ����ġ 4�� ���� ��� )
// ������������������������������������������������������������������
struct PNCT3_VERTEX
{
	Vector3		p;
	Vector3		n;
	Vector4		c;
	Vector2		t;
	Vector4		i;	// I0, I1, I2, I3
	Vector4		w;// W0, fW1, fW2, fNumWeight;
	bool operator == (const PNCT3_VERTEX & Vertex)
	{
		if (p == Vertex.p  && n == Vertex.n && 	c == Vertex.c  &&	t == Vertex.t)
		{
			return true;
		}
		return  false;
	}
	PNCT3_VERTEX() {
		w.x = w.y = w.z = w.w = 0.0f;
		i.x = i.y = i.z = i.w = 0.0f;
	}
	PNCT3_VERTEX(Vector3		vp,
		Vector3		vn,
		Vector4		vc,
		Vector2     vt,
		Vector4		vw,
		Vector4		vi)
	{
		p = vp, n = vn, c = vc, t = vt;
		w = vw, i = vi;
	}
};
// ������������������������������������������������������������������
//  ���̴� ����( ����ġ 8�� ���� ��� )
// ������������������������������������������������������������������
struct PNCT5_VERTEX
{
	Vector3		p;
	Vector3		n;
	Vector4		c;
	Vector2		t;
	Vector4		w0;// W0, fW1, fW2, fW3;
	Vector4		i0;	// I0, I1, I2, I3
	Vector4		w1;// W4, fW5, fW6, fNumWeight;	
	Vector4		i1;	// I4, I5, I6, I7
	bool operator == (const PNCT5_VERTEX & Vertex)
	{
		if (p == Vertex.p  && n == Vertex.n && 	c == Vertex.c  &&	t == Vertex.t)
		{
			return true;
		}
		return  false;
	}
	PNCT5_VERTEX() {
		w0.x = w0.y = w0.z = w0.w = 0.0f;
		i0.x = i0.y = i0.z = i0.w = 0.0f;
		w1.x = w1.y = w1.z = w1.w = 0.0f;
		i1.x = i1.y = i1.z = i1.w = 0.0f;
	}
	PNCT5_VERTEX(Vector3		vp,
		Vector3		vn,
		Vector4		vc,
		Vector2     vt,
		Vector4		vw0,
		Vector4		vi0,
		Vector4		vw1,
		Vector4		vi1)
	{
		p = vp, n = vn, c = vc, t = vt;
		w0 = vw0, i0 = vi0, w1 = vw1, i1 = vi1;
	}
};

struct TBipedMesh
{
	T_STR		m_strName;
	int					m_iNumFace;
	Matrix			m_matWorld;
	vector<PNC_VERTEX>	m_VertexList;
	PNC_VERTEX*		m_pDrawVertex;
};

struct VS_CONSTANT_BUFFER
{
	Matrix matWorld;// c0						
	Matrix matView;	// c4						
	Matrix matProj;	// c8						
	Vector4 Color; // 12
};

struct TObjFrame
{
	vector<Matrix>  m_matWorldList;
	int  				m_iIndex;
	TObjFrame(int iNumMesh, int iIndex)
	{
		m_matWorldList.resize(iNumMesh);
		m_iIndex = iIndex;
	}
};

typedef struct
{
	double x, y, z;
} Point3;



typedef struct
{
	/* sample time */
	double t;

	/* sample position */
	Point3 P;

	/* sample parameters for affecting tangential behavior at control point */
	double tension, continuity, bias;
}
PositionKey;

/* private structures to hide details of polynomials */
typedef struct
{
	/* P(u) = C0 + u*C1 + u^2*C2 + u^3*C3,  0 <= u <= 1 */
	Point3 C0, C1, C2, C3;

	/* sample time interval on which polynomial is valid, tmin <= t <= tmax */
	double tmin, tmax, trange;
}
CubicPolynomial;

typedef struct
{
	int numPolys;
	CubicPolynomial* poly;

	/* partial sums of arc length */
	double* length;
	double totalLength;
}
SplineInfo;