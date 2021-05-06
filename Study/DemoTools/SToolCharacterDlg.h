#pragma once


// SToolCharacterDlg 대화 상자

class SToolCharacterDlg : public CDialogEx
{
	DECLARE_DYNAMIC(SToolCharacterDlg)

public:
	SToolCharacterDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~SToolCharacterDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CHARACTER_DLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonFile();
	afx_msg void OnBnClickedButtonColor();
	afx_msg void OnBnClickedButtonSave();
	afx_msg void OnBnClickedButtonFont();
};
