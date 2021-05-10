#pragma once
#include "afxwin.h"


// MapCtrl ��ȭ �����Դϴ�.
struct TMapInfo
{
	int iTileCnt;
	int iCellCnt;
	float fCellSize;
};
class MapCtrl : public CDialogEx
{
	DECLARE_DYNAMIC(MapCtrl)
public :
	TMapInfo  m_MapInfo;
public:
	MapCtrl(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~MapCtrl();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MAPCTRL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	// Ÿ�� ���� �� ���� �ִ� ������ �Է��Ͻÿ�
	int m_iTileCnt;
	// �� ������ �Է��Ͻÿ�!
	int m_iCellCnt;
	// 1���� �� ũ�⸦ �Է��Ͻÿ�!
	int m_iCellSize;
	// ��������
	int m_iTileCount;
	CComboBox m_TileCtrl;
	virtual BOOL OnInitDialog();
	// ������ �⺻ �ؽ�ó ����!// ������ �⺻ �ؽ�ó ����!
	CListBox m_TextureCtrl;
	afx_msg void OnLbnSelchangeTexture();
	// ���� ���õ� �ؽ�ó��
	CString m_szTextureName;
	BOOL m_bAtt_0;
	BOOL m_bAtt_1;
	BOOL m_bAtt_2;
	afx_msg void OnBnClickedButton1();
};
