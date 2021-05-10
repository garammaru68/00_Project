#pragma once
// SMapToolForm 폼 보기

struct SMapInfo
{
	int iNumTile;
	int iNumCell;
	float fCellSize;
};

class SMapToolForm : public CFormView
{
	DECLARE_DYNCREATE(SMapToolForm)

protected:
	SMapToolForm();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~SMapToolForm();
public:
	SMapInfo m_MapInfo;
	static SMapToolForm* CreateOne(CWnd* pParent);
public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SMapToolForm };
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	int m_iNumTile;
	int m_iNumCell;
	float m_fCellSize;
	afx_msg void OnBnClickedOk();
};


