// MapCtrl.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "KGCATool.h"
#include "MapCtrl.h"
#include "afxdialogex.h"


// MapCtrl 대화 상자입니다.

IMPLEMENT_DYNAMIC(MapCtrl, CDialogEx)

MapCtrl::MapCtrl(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_MAPCTRL, pParent)
	, m_iTileCnt(0)
	, m_iCellCnt(0)
	, m_iCellSize(0)
	, m_iTileCount(0)
	, m_szTextureName(_T(""))
	, m_bAtt_0(FALSE)
	, m_bAtt_1(FALSE)
	, m_bAtt_2(FALSE)
{
	m_iTileCnt = 100;
	m_iCellCnt = 10;
	m_iCellSize = 1;
	
}

MapCtrl::~MapCtrl()
{
}

void MapCtrl::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_iTileCnt);
	DDV_MinMaxInt(pDX, m_iTileCnt, 1, 1025);
	DDX_Text(pDX, IDC_EDIT2, m_iCellCnt);
	DDV_MinMaxInt(pDX, m_iCellCnt, 1, 65);
	DDX_Text(pDX, IDC_EDIT3, m_iCellSize);
	DDV_MinMaxInt(pDX, m_iCellSize, 1, 100);
	DDX_CBIndex(pDX, IDC_COMBO1, m_iTileCount);
	DDX_Control(pDX, IDC_COMBO1, m_TileCtrl);
	DDX_Control(pDX, IDC_LIST1, m_TextureCtrl);
	DDX_Text(pDX, IDC_SELTEXNAME, m_szTextureName);
	DDX_Check(pDX, IDC_CHECK1, m_bAtt_0);
	DDX_Check(pDX, IDC_CHECK2, m_bAtt_1);
	DDX_Check(pDX, IDC_CHECK3, m_bAtt_2);
}


BEGIN_MESSAGE_MAP(MapCtrl, CDialogEx)
	ON_BN_CLICKED(IDOK, &MapCtrl::OnBnClickedOk)
//	ON_CBN_EDITUPDATE(IDC_COMBO1, &MapCtrl::OnCbnEditupdateCombo1)
//	ON_CBN_SELCHANGE(IDC_COMBO1, &MapCtrl::OnCbnSelchangeCombo1)
ON_LBN_SELCHANGE(IDC_LIST1, &MapCtrl::OnLbnSelchangeTexture)
ON_BN_CLICKED(IDC_BUTTON1, &MapCtrl::OnBnClickedButton1)
END_MESSAGE_MAP()


// MapCtrl 메시지 처리기입니다.


void MapCtrl::OnBnClickedOk()
{	
	UpdateData(TRUE);
	CString rString;

	SetDlgItemText(IDC_COMBO1, rString);

	m_TileCtrl.GetLBText(m_iTileCount, rString);			
	m_MapInfo.iTileCnt = _wtoi(rString.GetBuffer(0));
	m_MapInfo.fCellSize = m_iCellSize;
	m_MapInfo.iCellCnt = m_iCellCnt;

	CDialogEx::OnOK();
}


BOOL MapCtrl::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_TileCtrl.AddString(L"Quadtree");
	m_TileCtrl.AddString(L"Octree");
	m_TileCtrl.AddString(L"bsp");
	m_TileCtrl.SetCurSel(0);

	m_TextureCtrl.InsertString(0, L"base0.dds");
	m_TextureCtrl.InsertString(1, L"base1.dds");
	m_TextureCtrl.InsertString(2, L"base2.dds");
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


//void MapCtrl::OnCbnEditupdateCombo1()
//{
//	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
//}


//void MapCtrl::OnCbnSelchangeCombo1()
//{
//	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
//}


void MapCtrl::OnLbnSelchangeTexture()
{
	CString name;
	int iSel = m_TextureCtrl.GetCurSel();
	m_TextureCtrl.GetText(iSel, name);
	m_szTextureName = name;
	UpdateData(FALSE);
}
////////////////Works for multiple selection //////////////////
//void MapCtrl::OnBnClickedOk()
//{
//	// Works for multiple selection
//	int count = m_TextureCtrl.GetSelCount();
//	int *lp = new int[1025];
//	int count2 = m_TextureCtrl.GetSelItems(1024, lp);
//	for (int i = 0; i < count2; i++)
//	{
//		CString str;
//		m_TextureCtrl.GetText(lp[i], str);
//		MessageBox(str);
//	}//	
//}

//int n = 0;
//CString data = _T("");
//CString str = _T("");
//CString str2 = _T("");
//
//for (int i = 0; i < m_strListBoxNew.GetCount(); i++)
//{
//	n = m_strListBoxNew.GetSel(i);
//	if (n == 1)
//	{
//		n = m_strListBoxNew.GetTextLen(i);
//		m_strListBoxNew.GetText(i, str.GetBuffer(n));
//		str.ReleaseBuffer();
//
//		str2.Format(_T("%s"), str.GetBuffer(0));
//		AfxMessageBox(str2);
//	}
//}

void MapCtrl::OnBnClickedButton1()
{
	CString selFileName;
	CFileDialog dlg(
		TRUE, L"bmp", NULL,
		OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST,
		L"bmp Files(*.bmp)|*.bmp| All Files(*.*)|*.*|", this);

	if (dlg.DoModal() == IDOK)
	{
		selFileName = dlg.GetPathName();
		selFileName += dlg.GetFileName();
		//selFileName += dlg.GetFileExt();
		//AfxMessageBox(selFileName);
		m_szTextureName = selFileName;
		UpdateData(FALSE);
	}
}
