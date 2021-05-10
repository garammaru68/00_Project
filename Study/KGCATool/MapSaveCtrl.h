#pragma once


// MapSaveCtrl 대화 상자입니다.

class MapSaveCtrl : public CDialogEx
{
	DECLARE_DYNAMIC(MapSaveCtrl)

public:
	MapSaveCtrl(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~MapSaveCtrl();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MAPSAVECTRL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButtonFile();
	afx_msg void OnBnClickedButtonColor();
	afx_msg void OnBnClickedButtonSave();
	afx_msg void OnBnClickedButtonFont();
};
