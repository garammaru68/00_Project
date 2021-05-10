#pragma once
#include "afxcmn.h"


// TObjectDlg ��ȭ �����Դϴ�.

class TObjectDlg : public CDialogEx
{
	DECLARE_DYNAMIC(TObjectDlg)

public:
	CImageList imgList;
	CImageList imgListSmall;
	TObjectDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~TObjectDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TOBJECTDLG };
#endif
public:
	void CheckupChild(HTREEITEM hRoot);
	void ChildCheck(HTREEITEM hRoot);
	void LoadTextureFile();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_ObjList;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnHdnItemdblclickList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnHdnItemclickList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButton4();
	CTreeCtrl m_ObjTree;
	afx_msg void OnTvnSelchangingTree1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButton6();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton8();
	afx_msg void OnBnClickedButton7();
};
