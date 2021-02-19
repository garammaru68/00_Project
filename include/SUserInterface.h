#pragma once
#include "SObject.h"
#include "SInput.h"
struct SOffset
{
	float fScaleX0;
	float fScaleX1;
	float fScaleY0;
	float fScaleY1;
	float fOffsetU0;
	float fOffsetV0;
	float fOffsetU1;
	float fOffsetV1;
};

struct SRect
{
	float    left;
	float    top;
	float    right;
	float    bottom;
	void operator = (const RECT rt)
	{
		left = (float)rt.left;
		top = (float)rt.top;
		right = (float)rt.right;
		bottom = (float)rt.bottom;
	}
};

struct SIndex
{
	int x, y;
	bool operator == (const SIndex& pos)
	{
		return (x == pos.x && y == pos.y);
	}
	SIndex operator + (const SIndex& right_)
	{
		return{ x + right_.x, y + right_.y };
	}
	SIndex(int fX, int fY)
	{
		x = fX;
		y = fY;
	}
	SIndex()
	{
		x = y = 0;
	}
};

struct PT_VERTEX
{
	SPoint p;
	SPoint t;
	bool operator == (const PT_VERTEX & Vertex)
	{
		if (p == Vertex.p  &&  t == Vertex.t)
		{
			return true;
		}
		return  false;
	}
	PT_VERTEX() {}
	PT_VERTEX(SPoint		vp, SPoint     vt)
	{
		p = vp, t = vt;
	}
};

class SUserInterface : public SObject
{
public:

	std::vector<SUserInterface>  m_list;
public:
	virtual LRESULT	 MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	virtual SObject* Clone() override;
	virtual bool	PostFrame() override;
	SUserInterface()
	{
		m_iObjectType = 300;
	}
};
class SActor : public SUserInterface
{
public:
	SRect		m_rtInit;
	SOffset		m_tOffset;
	SPoint		m_vCenter;
	PT_VERTEX	m_VertexList[6];
public:
	virtual SPoint  Gen(float x, float y);
	virtual void    GenCenter();
	virtual void    SetVertexData();
	virtual  bool	SetData(float left, float top, float width, float height);

public:
	SActor() {};
	~SActor() {}
};
class SDialogBox : public SActor
{
public:
	vector<PT_VERTEX> m_tVertexList;
	SActor             m_Actor[9];
	SPoint			   m_cpPoint[16];
public:
	void   SetVertexData();
	bool   SetData(RECT rt, float fScaleX0, float fScaleX1,
		float fScaleY0, float fScaleY1,
		float fOffsetU0, float fOffsetU1,
		float fOffsetV0, float fOffsetV1);
public:
	virtual bool	Init() override;
	virtual bool	Render(ID3D11DeviceContext*	pd3dContext) override;
	virtual void  Set(RECT rtSrc, RECT rtDesk) override;
	virtual void  Set(SPoint p, RECT rtDesk) override;
public:
	SDialogBox() {};
	~SDialogBox() {}
};
class SButton : public SUserInterface
{
public:

public:
	bool   Frame();
	bool   Render(ID3D11DeviceContext*	pd3dContext);
	virtual SObject* Clone() override;
public:
	SButton()
	{
		m_iObjectType = 301;
	}
};
class SEdit : public SUserInterface
{
public:
	T_STR	m_szEdit;
	int		m_iWindowCtrl;
	static int g_iControlID;
	HWND	m_hWndEdit;
	RECT	m_rtWndCtrl;
	bool	m_bEditFocus = false;
public:
	void   CreateEdit();
public:
	virtual SObject* Clone() override;
	bool	Init() override;
	bool	Frame()override;
	bool	Render(ID3D11DeviceContext*	pd3dContext)override;
	LRESULT	 MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
public:
	SEdit()
	{
		m_iObjectType = 302;
	}
};