#pragma once
#include "TObject.h"
struct TMapDesc
{
	int	iNumCols;
	int iNumRows;
	float fCellDistance;
	T_STR szTexFile;
	T_STR szVS;
	T_STR szPS;
};
class TMap : public TObject
{
public:
	TMapDesc  m_MapDesc;
	UINT m_iNumRows;
	UINT m_iNumCols;
	UINT m_iNumCellCols;
	UINT m_iNumCellRows;
	UINT m_iNumVertices;
	UINT m_iNumFaces;
	float m_fCellDistance;
public:
	bool	  CreateMap(ID3D11Device* pDevice, TMapDesc  desc);
	virtual bool    CreateVertexData() override;
	virtual bool    CreateIndexData()override;
	virtual bool	Frame() override;

public:
	TMap();
	virtual ~TMap();
};

