// SMapToolForm.cpp: 구현 파일
//

#include "pch.h"
#include "DemoTools.h"
#include "SMapToolForm.h"


// SMapToolForm

IMPLEMENT_DYNCREATE(SMapToolForm, CFormView)

SMapToolForm::SMapToolForm()
	: CFormView(IDD_SMapToolForm)
	, m_iNumTile(0)
	, m_iNumCell(0)
	, m_fCellSize(0)
{

}
SMapToolForm::~SMapToolForm()
{
}

SMapToolForm* SMapToolForm::CreateOne(CWnd* pParent)
{
	SMapToolForm* pForm = new SMapToolForm;
	pForm->Create(NULL, NULL, WS_CHILD | WS_VISIBLE,
		CRect(0, 0, 500, 500), pParent, 0, NULL);
	return pForm;
}

void SMapToolForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_iNumTile);
	DDV_MinMaxInt(pDX, m_iNumTile, 1, 4);
	DDX_Text(pDX, IDC_EDIT2, m_iNumCell);
	DDV_MinMaxInt(pDX, m_iNumCell, 1, 64);
	DDX_Text(pDX, IDC_EDIT5, m_fCellSize);
}

BEGIN_MESSAGE_MAP(SMapToolForm, CFormView)
	ON_BN_CLICKED(IDOK, &SMapToolForm::OnBnClickedOk)
END_MESSAGE_MAP()


// SMapToolForm 진단

#ifdef _DEBUG
void SMapToolForm::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void SMapToolForm::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// SMapToolForm 메시지 처리기


void SMapToolForm::OnBnClickedOk()
{
	UpdateData(TRUE);

	m_MapInfo.iNumTile = m_iNumTile;
	m_MapInfo.iNumCell = m_iNumCell;
	m_MapInfo.fCellSize = m_fCellSize;

	((CDemoToolsApp*)AfxGetApp())->m_Sample.CreateMap(m_MapInfo);
}
