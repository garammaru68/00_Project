#pragma once
#include "afxcmn.h"


// MapControl 대화 상자입니다.

class MapControl : public CDialogEx
{
	DECLARE_DYNAMIC(MapControl)

public:
	MapControl(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~MapControl();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CTreeCtrl m_listInfo;
	CListCtrl m_listAttribute;
	CImageList m_imgList;
	CImageList m_imgListSmall;
	void LoadListData(TCHAR* szFath);
	virtual BOOL OnInitDialog();
	afx_msg void OnTvnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
};
