#pragma once


// SToolMapDlg 대화 상자
struct SMapInfo
{
	int iTileCnt;
	int iCellCnt;
	float fCellSize;
};
class SToolMapDlg : public CDialogEx
{
	DECLARE_DYNAMIC(SToolMapDlg)

public:
	SToolMapDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~SToolMapDlg();
	void LoadFile();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MAP_DLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

	// 타일 개수
	int m_iNumTile;
	int m_iTileCount;
	// 셀 개수
	int m_iNumCell;
	// 셀 크기
	int m_iCellSize;
	// 텍스쳐
	CString m_szTexture;
	CListBox m_TextureList;
	afx_msg void SelectTextureList();

	// 공간 분할
	CComboBox m_SpaceDivision;
	afx_msg void SpaceDivisionList();
	
	// Radio
	afx_msg void SelectRadioButton();
	int m_iRadio1;
	int m_iRadio2;

	// FileLocation
	CTreeCtrl m_TreeCtrl;
	CListCtrl m_ListCtrl;
	afx_msg void SearchFileLocation1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void SearchFileLocation2(NMHDR *pNMHDR, LRESULT *pResult);
};
