#pragma once


// SToolMapDlg 대화 상자

class SToolMapDlg : public CDialogEx
{
	DECLARE_DYNAMIC(SToolMapDlg)

public:
	SToolMapDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~SToolMapDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MAP_DLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
};
