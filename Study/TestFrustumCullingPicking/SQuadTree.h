#pragma once
#include <queue>
#include "SNode.h"
#include "SHeightMap.h"

class SQuadTree
{
public:
	enum QUADTREECORNER { tTL = 0, tTR = 1, tBL = 2, tBR = 3, };

	SMap*			m_pMap;
	SNode*			m_pRootNode;
	vector<SNode*>	m_leafList;
	vector<SNode*>  m_visibleList;
	queue<SNode*>	m_qQuadTree;
	vector<Object*>	m_DrawObjList;
	vector<SNode*>	m_DrawNodeList;

	int				m_iRenderDepth;
	int				m_iMaxDepthLimit;
	void			SetMaxDepthLimit(int iMaxDepth) { m_iMaxDepthLimit = iMaxDepth; }
	int				m_iMaxDepth;
	float			m_fWidth;
	float			m_fHeight;

	float			m_fMinDivideSize;
	void			SetMinDivideSize(int iMinDivideSize) { m_fMinDivideSize = (float)iMinDivideSize; }
public:
	SNode*  CreateNode(SNode* pParentNode, UINT TopLeft, UINT TopRight, UINT BottomLeft, UINT BottomRight);
	bool	SBuildTree();
	bool	CreateQuadtree(SMap* pMap);
	bool    DivideNode(SNode* pNode);
	bool    BuildQuadTree(SNode* pNode);
	bool    Render(ID3D11DeviceContext* pContext);
	void    Draw(SNode* pNode, ID3D11DeviceContext* pd3dContext);

	SNode*	CreateObjectNode(SNode* pParentNode, float fTopLeft, float fTopRight, float fBottomLeft, float fBottomRight);
	bool			Build(float fWidth, float fHeight);
	virtual bool	BuildTree(SNode* pNode);
	virtual bool	SubDivide(SNode* pNode);
	virtual bool	PreFrame();
	virtual bool	Frame();

	int		AddObject(Object* pObj);
	int		CheckRect(SNode* pNode, Object* pObj);
	SNode*	FindNode(SNode* pNode, Object* pObj);

	bool	Release();
};