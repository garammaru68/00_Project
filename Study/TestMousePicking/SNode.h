#pragma once
#include "SFrustum.h"

class Object
{
public:
	S_BOX		m_Box;
public:
	Object() {}
	virtual ~Object() {}
};

class S_BoxObject : public Object
{
public:
	Matrix			m_matWorld;
	Vector3			m_vPosition;
	Vector4			m_vColor;
public:
	S_BoxObject();
	virtual ~S_BoxObject();
};

class SNode
{
public:
	DWORD					m_dwDepth;
	BOOL					m_blLeaf;
	UINT					m_iCornerIndex[4];
	SNode*					m_pChild[4];
	vector<Vector3>			m_vCornerList;
	vector<SNode*>			m_ChildList;
	std::vector<DWORD>		m_vIndexList;
	ComPtr<ID3D11Buffer>	m_pIndexBuffer;

	S_BOX					m_Box;
	vector<Object*>			m_ObjectList;
public:
	SNode();
	~SNode();
};