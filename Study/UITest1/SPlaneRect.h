#pragma once
#include "SImeUI.h"
#include "SShape.h"
using namespace DX;

enum TUI_TYPE {
	T_BUTTON = 0,
	T_EDIT,
	T_LISTBOX,
	T_STATITEXT,
};

class SControlUI
{
public:	
	SShape*     m_pShape;
	T_STR		m_Text;
	PCT_VERTEX  m_Vertices[4];
	RECT		m_rtSize[8];
	RECT		m_rt;
	Matrix	m_matViewPort;
	Matrix  m_matWorld;
	Matrix  m_matView;
	Matrix  m_matProj;
	Vector3  m_vScale;
	Vector3  m_vRotate;
	Vector3  m_vTrans;
	UINT		 m_iWidthVP;
	UINT		 m_iHeightVP;
public: //event
	virtual void		SetMatrix(Matrix* pWorld, Matrix* pView, Matrix* pProj);
	virtual void		SetAmbientColor(float fR, float fG, float fB, float fA);
	virtual bool		Init();
	virtual HRESULT		Create(ID3D11Device* pDevice, const TCHAR* pLoadShaderFile = nullptr,
							const TCHAR* pLoadTextureString = nullptr) 	{return S_OK;};
	virtual HRESULT		CreateResource(int iRectWidth, int iRectHeight);
	virtual bool		Set(UINT iWidth, UINT iHeight);
	virtual void		Move(float x, float y, float z);
	virtual void		Scale(float x, float y, float z);
	virtual void		Rotate(float x, float y, float z);
	virtual bool		Update();
	virtual bool		Begin(ID3D11DeviceContext* pContext);
	virtual bool		End(ID3D11DeviceContext* pContext);
	virtual bool		Render(ID3D11DeviceContext* pContext);
	virtual bool		Release();
public:
	SControlUI();
	virtual ~SControlUI();
};
class SButtonCtl : public SControlUI
{
public:
	SShapeBox     m_Box;
	HRESULT		Create(ID3D11Device* pDevice,
		const TCHAR* pLoadShaderFile=nullptr,
		const TCHAR* pLoadTextureString = nullptr);
public:
	SButtonCtl() {};
	virtual ~SButtonCtl() {};
};
class SStaticTextCtl : public SButtonCtl
{
public:
	SShapePlane   m_Plane;
	SStaticTextCtl() {};
	virtual ~SStaticTextCtl() {};
};
class SEditCtl : public SButtonCtl
{
public:
	SShapePlane   m_Plane;
	T_STR    m_CompString;
	T_STR    m_CompWord;
	T_STR    m_CurrentString;
public:
	HRESULT		Create(ID3D11Device* pDevice,
		const TCHAR* pLoadShaderFile = nullptr,
		const TCHAR* pLoadTextureString = nullptr);
	SEditCtl() {};
	virtual ~SEditCtl() {};
};
class SListBoxCtl : public SButtonCtl
{
public:
	std::vector<SButtonCtl*>  m_ListBox;
	SListBoxCtl() {};
	virtual ~SListBoxCtl() {};
};
class SImageCtl : public SButtonCtl
{
public:
	std::vector<SButtonCtl*>  m_ListBox;
	HRESULT		Create(ID3D11Device* pDevice,
		const TCHAR* pLoadShaderFile = nullptr,
		const TCHAR* pLoadTextureString = nullptr);
	SImageCtl() {};
	virtual ~SImageCtl() {};
};
class SPanel // 1개의 배경에서 뜨는 대화상자 개념.
{
public:
	std::vector<SButtonCtl*>   m_ButtonCtl;
	std::vector<SImageCtl*>   m_ImageCtl;
	std::vector<SEditCtl*>   m_EditCtl;
	std::vector<SListBoxCtl*>   m_BoxCtl;
	std::vector<SStaticTextCtl*>   m_StaticTextCtl;
	SPanel() {};
	virtual ~SPanel() {};
};
class SSceneUI // 배경 화면이 교체 되는 단위
{
public:
	std::vector<SPanel*>  m_SceneList;
	std::vector<SImageCtl*>   m_ImageCtl;
	std::vector<SButtonCtl*>   m_ButtonCtl;
	std::vector<SEditCtl*>   m_EditCtl;
	std::vector<SListBoxCtl*>   m_listBoxCtl;
	std::vector<SStaticTextCtl*>   m_StaticTextCtl;
	SSceneUI() {};
	virtual ~SSceneUI() {};
};