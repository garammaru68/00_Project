#pragma once
#include "SObject.h"
struct SMapDesc
{
	int	iNumCols;
	int iNumRows;
	float fScaleHeight;
	float fCellDistance;
	T_STR szTexFile;
	T_STR szVS;
	T_STR szPS;
};
struct SNormalLookupTable
{
	int index[6];
	SNormalLookupTable()
	{
		index[0] = -1;
		index[1] = -1;
		index[2] = -1;
		index[3] = -1;
		index[4] = -1;
		index[5] = -1;
	}
};
class SMap : public SObject
{
public:
	SMapDesc  m_MapDesc;
	UINT m_iNumRows;
	UINT m_iNumCols;
	UINT m_iNumCellCols;
	UINT m_iNumCellRows;
	UINT m_iNumVertices;
	UINT m_iNumFaces;
	float m_fCellDistance;
	std::vector<float> m_fHeightList;
public:
	virtual bool	CreateMap(ID3D11Device* pDevice,
		ID3D11DeviceContext* pContext,
		SMapDesc  desc);
	virtual float   Lerp(float fStart, float fEnd, float fTangent);
	virtual float	GetHeightMap(float fPosX, float fPosZ);
	virtual float   GetHeight(UINT index);
	virtual Vector3	GetNormalOfVertex(UINT index);
	virtual float   GetHeightmap(int row, int col);
	virtual bool    CreateVertexData() override;
	virtual bool    CreateIndexData()override;
	virtual bool	Frame() override;
	virtual bool	PostRender(ID3D11DeviceContext*	pd3dContext);
public:
	std::vector<Vector3> m_FaceNormals;
	std::vector<SNormalLookupTable> m_LookupTable;
	Vector3 ComputeFaceNormal(DWORD i0, DWORD i1, DWORD i2);
	void CalcFaceNormals();
	void GetVertexNormal();
	void InitFaceNormals();
	void GenNormalLookupTable();
	void CalcPerVertexNormalsFastLookup();
public:
	SMap();
	virtual ~SMap();
};

//#pragma once
//#include "SObject.h"
//struct SMapDesc
//{
//	int		iNumCols;
//	int		iNumRows;
//	float	fScaleHeight;
//	float	fCellDistance;
//	T_STR	szTexFile;
//	T_STR	szVS;
//	T_STR	szPS;
//};
//class SMap : public SObject
//{
//public:
//	SMapDesc	m_MapDesc;
//	UINT		m_iNumRows;
//	UINT		m_iNumCols;
//	UINT		m_iNumCellCols;
//	UINT		m_iNumCellRows;
//	UINT		m_iNumVertices;
//	UINT		m_iNumFaces;
//	float		m_fCellDistance;
//	std::vector<float> m_fHeightList;
//	Vector3*	m_pFaceNormals;
//	int			m_iNumFace;
//	int*		m_pNormalLookupTable;
//	Vector3		m_vLightDir;
//	bool		m_bStaticLight;
//public:	
//	virtual bool	Frame() override;
//	virtual bool	PostRender(ID3D11DeviceContext*	pd3dContext);
//	virtual bool	Release() override;
//	virtual bool    CreateVertexData() override;
//	virtual bool    CreateIndexData()override;
//	virtual bool	CreateMap(ID3D11Device* pDevice,
//		ID3D11DeviceContext* pContext,
//		SMapDesc  desc);
//
//	// 초기화 함수 및 페이스 노말 계산
//	virtual bool	GetVertexNormal();
//	virtual	void	CalcVertexColor(Vector3 vLightDir);
//	virtual void	InitFaceNormals();
//	virtual void	CalcFaceNormals();
//	virtual Vector3 ComputeFaceNormal(DWORD dwIndex0, DWORD dwIndex1, DWORD dwIndex2);
//
//	// 정점 노말 룩업 테이블 생성 및 계산
//	virtual void	GetNormalLookupTable();
//	virtual void	CalcPerVertexNormalsFastLookup();
//	virtual float   GetHeightmap(int row, int col);
//	virtual float	GetHeightOfVertex(UINT Index);
//	virtual Vector3	GetNormalOfVertex(UINT Index);
//	virtual Vector4 GetColorOfVertex(UINT Index);
//	virtual	Vector2	GetTextureOfVertex(float fOffsetX, float fOffsetY);
//	virtual float	GetHeight(float fPosX, float fPosZ);
//	virtual float   Lerp(float fStart, float fEnd, float fTangent);
//public:
//	SMap();
//	virtual ~SMap();
//};