//#pragma once
//#include "SFrustum.h"
//class SBaseObj
//{
//public:
//	S_BOX				m_tBox;
//public:
//	SBaseObj() {}
//	virtual ~SBaseObj() {}
//};
//class S_BoxObject : public SBaseObj
//{
//public:
//	Matrix			m_matWorld;
//	Vector3			m_vPosition;
//	Vector4			m_vColor;
//public:
//	S_BoxObject();
//};
//
//class SNode
//{
//public:
//	DWORD				m_dwDepth;
//	BOOL				m_isLeaf;
//	BOOL				m_isTile;
//	// 바운딩 박스
//	S_BOX				m_tBox;
//	vector<DWORD>		m_CornerIndex;
//	// 노드의 코너 정점 인덱스 및 위치
//	vector<Vector3>		m_CornerList;
//	vector<SNode*>		m_ChildList;
//	vector<SNode*>		m_NeighborList;
//	vector<SBaseObj*>	m_ObjectList;
//	// 추가
//	DWORD				m_dwPositionIndex[2];// X,Y	
//	D3D11_BOX			m_BoxVB;
//	D3D11_BOX			m_BoxIB;
//	UINT				m_iBeginVB;
//	UINT				m_iBeginIB;
//	vector<DWORD>		m_IndexList;
//	vector <PNCT_VERTEX>	m_VertexList;
//	ComPtr<ID3D11Buffer>	m_pVertexBuffer;
//	ComPtr<ID3D11Buffer>	m_pIndexBuffer;
//public:
//	SNode();
//	virtual ~SNode();
//};
//
#pragma once
#include "SFrustum.h"
class SBaseObj
{
public:
	S_BOX				m_tBox;
public:
	SBaseObj() {}
	virtual ~SBaseObj() {}
};
class S_BoxObject : public SBaseObj
{
public:
	Matrix			m_matWorld;
	Vector3			m_vPosition;
	Vector4			m_vColor;
public:
	S_BoxObject();
};

class SNode
{
public:
	DWORD				m_dwDepth;
	BOOL				m_isLeaf;
	BOOL				m_isTile;
	// 바운딩 박스
	S_BOX				m_tBox;
	vector<DWORD>		m_CornerIndex;
	// 노드의 코너 정점 인덱스 및 위치
	vector<Vector3>		m_CornerList;
	vector<SNode*>		m_ChildList;
	vector<SNode*>		m_NeighborList;
	vector<SBaseObj*>	m_ObjectList;
	// 추가
	DWORD				m_dwPositionIndex[2];// X,Y	
	D3D11_BOX			m_BoxVB;
	D3D11_BOX			m_BoxIB;
	UINT				m_iBeginVB;
	UINT				m_iBeginIB;
	vector<DWORD>		m_IndexList;
	vector <PNCT_VERTEX>	m_VertexList;
	ComPtr<ID3D11Buffer>	m_pVertexBuffer;
	ComPtr<ID3D11Buffer>	m_pIndexBuffer;
public:
	SNode();
	virtual ~SNode();
};

