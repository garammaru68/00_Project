#pragma once
#include "afxcmn.h"


// MapControl ��ȭ �����Դϴ�.

class MapControl : public CDialogEx
{
	DECLARE_DYNAMIC(MapControl)

public:
	MapControl(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~MapControl();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

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
