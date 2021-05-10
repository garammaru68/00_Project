#pragma once
#include "afxwin.h"


// TCreateMap 대화 상자입니다.

class TCreateMap : public CDialogEx
{
	DECLARE_DYNAMIC(TCreateMap)

public:
	TCreateMap(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~TCreateMap();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TCREATEMAP };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();

	CString m_iMapSizeX;
	CString m_iMapSizeY;
	afx_msg void OnBnInitialize();
	CListBox m_Cell;
	int      m_iCellDetail;
	int      m_iCellSize;
	CComboBox m_CellSizeCtl;
	BOOL m_iSpacePartision;

	int m_iRadio;
	int m_Radio2;
	virtual BOOL OnInitDialog();
};
