#pragma once


// MapSaveCtrl ��ȭ �����Դϴ�.

class MapSaveCtrl : public CDialogEx
{
	DECLARE_DYNAMIC(MapSaveCtrl)

public:
	MapSaveCtrl(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~MapSaveCtrl();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MAPSAVECTRL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButtonFile();
	afx_msg void OnBnClickedButtonColor();
	afx_msg void OnBnClickedButtonSave();
	afx_msg void OnBnClickedButtonFont();
};
