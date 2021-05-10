#include "pch.h"
#include "DemoTools.h"
#include "SToolMapDlg.h"
#include "afxdialogex.h"

// SToolMapDlg 대화 상자
IMPLEMENT_DYNAMIC(SToolMapDlg, CDialogEx)

SToolMapDlg::SToolMapDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MAP_DLG, pParent)
	, m_iNumTile(0)
	, m_szTexture(_T(""))
	, m_iRadio1(0)
	, m_iRadio2(0)
{
}
SToolMapDlg::~SToolMapDlg()
{
}

void SToolMapDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_iNumTile);
	DDV_MinMaxInt(pDX, m_iNumTile, 0, 4096);
	DDX_Control(pDX, IDC_COMBO1, m_SpaceDivision);
	DDX_Text(pDX, IDC_EDIT3, m_szTexture);
	DDX_Control(pDX, IDC_LIST1, m_TextureList);
	DDX_Radio(pDX, IDC_RADIO1, m_iRadio1);
	DDX_Radio(pDX, IDC_RADIO4, m_iRadio2);
	DDX_Control(pDX, IDC_TREE1, m_TreeCtrl);
	DDX_Control(pDX, IDC_LIST4, m_ListCtrl);
}

BEGIN_MESSAGE_MAP(SToolMapDlg, CDialogEx)
	ON_CBN_SELCHANGE(IDC_COMBO1, &SToolMapDlg::SpaceDivisionList)
	ON_LBN_SELCHANGE(IDC_LIST1, &SToolMapDlg::SelectTextureList)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST4, &SToolMapDlg::SearchFileLocation2)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE1, &SToolMapDlg::SearchFileLocation1)
END_MESSAGE_MAP()

BOOL SToolMapDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_iNumTile = 0;

	m_SpaceDivision.AddString(L"Quadtree");
	m_SpaceDivision.AddString(L"Octree");
	m_SpaceDivision.AddString(L"BSP");
	m_SpaceDivision.SetCurSel(0);

	m_TextureList.InsertString(0, L"a.dds");
	m_TextureList.InsertString(1, L"b.dds");
	m_TextureList.InsertString(2, L"c.dds");
	m_TextureList.SetCurSel(0);
	return TRUE;  
}
void SToolMapDlg::SelectTextureList()
{
	CString name;
	int iSel = m_TextureList.GetCurSel();
	m_TextureList.GetText(iSel, m_szTexture);
	UpdateData(FALSE);
}
void SToolMapDlg::SpaceDivisionList()
{
	CString name;
	int iSel = m_SpaceDivision.GetCurSel();
	m_TextureList.GetText(iSel, m_szTexture);
	UpdateData(FALSE);
}
void SToolMapDlg::SelectRadioButton()
{
	UpdateData(TRUE);
	switch (m_iRadio1)
	{
		case 0: AfxMessageBox(L"0"); break;
		case 1: AfxMessageBox(L"1"); break;
		case 2: AfxMessageBox(L"2"); break;
	}
	CDialogEx::OnOK();
}

void SToolMapDlg::SearchFileLocation1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);

	CString name = m_TreeCtrl.GetItemText(pNMTreeView->itemNew.hItem);
	AfxMessageBox(name);
	*pResult = 0;
}
void SToolMapDlg::SearchFileLocation2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	*pResult = 0;
}


