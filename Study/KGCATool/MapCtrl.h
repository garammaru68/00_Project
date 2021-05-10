#pragma once
#include "afxwin.h"


// MapCtrl 대화 상자입니다.
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
	MapCtrl(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~MapCtrl();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MAPCTRL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	// 타일 가로 및 세로 최대 개수를 입력하시오
	int m_iTileCnt;
	// 셀 개수를 입력하시오!
	int m_iCellCnt;
	// 1개의 셀 크기를 입력하시오!
	int m_iCellSize;
	// 공간분할
	int m_iTileCount;
	CComboBox m_TileCtrl;
	virtual BOOL OnInitDialog();
	// 지형의 기본 텍스처 선택!// 지형의 기본 텍스처 선택!
	CListBox m_TextureCtrl;
	afx_msg void OnLbnSelchangeTexture();
	// 현재 선택된 텍스처명
	CString m_szTextureName;
	BOOL m_bAtt_0;
	BOOL m_bAtt_1;
	BOOL m_bAtt_2;
	afx_msg void OnBnClickedButton1();
};
