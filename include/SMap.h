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
	virtual float   GetHeightmap(int row, int col);
	virtual bool    CreateVertexData() override;
	virtual bool    CreateIndexData()override;
	virtual bool	Frame() override;
	virtual bool	PostRender(ID3D11DeviceContext*	pd3dContext);
public:
	SMap();
	virtual ~SMap();
};
