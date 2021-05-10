// TObjectDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "TMapTool.h"
#include "TObjectDlg.h"
#include "afxdialogex.h"


// TObjectDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(TObjectDlg, CDialogEx)

TObjectDlg::TObjectDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_TOBJECTDLG, pParent)
{

}

TObjectDlg::~TObjectDlg()
{
}

void TObjectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ObjList);
	DDX_Control(pDX, IDC_TREE1, m_ObjTree);
}


BEGIN_MESSAGE_MAP(TObjectDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &TObjectDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &TObjectDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &TObjectDlg::OnBnClickedButton3)
	ON_NOTIFY(HDN_ITEMDBLCLICK, 0, &TObjectDlg::OnHdnItemdblclickList1)
	ON_NOTIFY(HDN_ITEMCLICK, 0, &TObjectDlg::OnHdnItemclickList1)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST1, &TObjectDlg::OnLvnItemchangedList1)
	ON_BN_CLICKED(IDOK, &TObjectDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON4, &TObjectDlg::OnBnClickedButton4)
	ON_NOTIFY(TVN_SELCHANGING, IDC_TREE1, &TObjectDlg::OnTvnSelchangingTree1)
	ON_BN_CLICKED(IDC_BUTTON6, &TObjectDlg::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON5, &TObjectDlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON8, &TObjectDlg::OnBnClickedButton8)
	ON_BN_CLICKED(IDC_BUTTON7, &TObjectDlg::OnBnClickedButton7)
END_MESSAGE_MAP()


// TObjectDlg 메시지 처리기입니다.
void TObjectDlg::LoadTextureFile()
{
	TCHAR path[MAX_PATH] = { 0, };
	GetCurrentDirectory(MAX_PATH, path);
	_tcscat_s(path, _T("\\res\\*.*"));
	HANDLE hSearch = NULL;

	WIN32_FIND_DATA  data;
	hSearch = FindFirstFile(path, &data);

	int iCnt = 0;
	BOOL bFind = TRUE;
	while (bFind)
	{
		if (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{

		}
		else
		{
			m_ObjList.InsertItem(iCnt, data.cFileName, 0);
			SYSTEMTIME st;
			//SystemTimeToFileTime(&st, &data.ftCreationTime);
			FileTimeToSystemTime(&data.ftCreationTime, &st);

			CString date;
			date.AppendFormat(_T("%ld년%ld월%ld일"), st.wYear, 
				st.wMonth, st.wDay);
			m_ObjList.SetItemText(iCnt++, 1, date);
		}
		bFind = FindNextFile(hSearch, &data);
	}
	FindClose(hSearch);
}

BOOL TObjectDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	if (imgList.m_hImageList == NULL)
	{
		CBitmap bmp;
		bmp.LoadBitmapW(IDB_BITMAP1);
		imgList.Create(16, 16, ILC_COLOR24, 7, 0);
		imgList.Add(&bmp, RGB(255, 255, 255));
		imgListSmall.Create(16, 16, ILC_COLOR24, 7, 0);
		imgListSmall.Add(&bmp, RGB(255, 255, 255));
	
	}
	m_ObjList.SetImageList(&imgList, LVSIL_NORMAL);
	m_ObjList.SetImageList(&imgList, LVSIL_SMALL);


	m_ObjTree.SetImageList(&imgList, LVSIL_NORMAL);

	HTREEITEM hParent =NULL;
	HTREEITEM hSubParent = NULL;
	hParent = m_ObjTree.InsertItem(_T("Bitped 0"), 0, 6, TVI_ROOT);
	hSubParent = m_ObjTree.InsertItem(_T("Bone 0"), 1, 6, hParent);
	hParent = m_ObjTree.InsertItem(_T("Bone 1"), 1,6, hParent);
	hParent = m_ObjTree.InsertItem(_T("Sub node 1"), 2, 6, hParent);
	hParent = m_ObjTree.InsertItem(_T("Sub node 1"), 3, 6, hParent);

	//m_ObjTree.EnsureVisible(hParent);

	hParent = m_ObjTree.InsertItem(_T("Root 1"), 0, 6, TVI_ROOT);
	hSubParent = m_ObjTree.InsertItem(_T("Sub node 0"), 1, 6, hParent);
	hParent = m_ObjTree.InsertItem(_T("Sub node 1"), 1, 6, hParent);

	//m_ObjTree.EnsureVisible(hParent);

	/*DWORD dwTreeStyle = m_ObjTree.GetExtendedStyle();
	DWORD dwTreeMask = m_ObjTree.GetExtendedStyle();

	m_ObjTree.SetExtendedStyle(dwTreeMask,
		dwTreeStyle | LVS_EX_CHECKBOXES |
		LVS_EX_BORDERSELECT | LVS_EX_GRIDLINES
		| LVS_EX_FULLROWSELECT);*/

		DWORD dwStyle = m_ObjList.GetExtendedStyle();
		m_ObjList.SetExtendedStyle(
			dwStyle | LVS_EX_CHECKBOXES |
			LVS_EX_BORDERSELECT | LVS_EX_GRIDLINES
			| LVS_EX_FULLROWSELECT
			| LVS_EX_AUTOCHECKSELECT);

		m_ObjList.InsertColumn(0, _T("name"),
			LVCFMT_LEFT, 200);
		m_ObjList.InsertColumn(1, _T("Description"),
			LVCFMT_LEFT, 300);

		LoadTextureFile();

	/*	m_ObjList.InsertItem(0, _T("Item 0"), 0);
		m_ObjList.SetItemText(0, 1, _T("bmp"));
		m_ObjList.InsertItem(1, _T("Item 1"), 1);
		m_ObjList.SetItemText(1, 1, _T("tga"));
		m_ObjList.InsertItem(2, _T("Item 2"), 2);
		m_ObjList.SetItemText(2, 1, _T("tga"));
		m_ObjList.InsertItem(3, _T("Item 3"), 3);
		m_ObjList.SetItemText(3, 1, _T("dds"));
		m_ObjList.InsertItem(4, _T("Item 4"), 4);
		m_ObjList.SetItemText(4, 1, _T("jpg"));
		m_ObjList.InsertItem(5, _T("Item 5"), 5);
		m_ObjList.SetItemText(5, 1, _T("jpg"));
		m_ObjList.InsertItem(6, _T("Item 6"), 6);
		m_ObjList.SetItemText(6, 1, _T("dds"));*/
		// TODO:  여기에 추가 초기화 작업을 추가합니다.
	
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void TObjectDlg::OnBnClickedButton1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_ObjList.ModifyStyle(LVS_TYPEMASK,
		LVS_REPORT);
}


void TObjectDlg::OnBnClickedButton2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_ObjList.ModifyStyle(LVS_TYPEMASK,
		LVS_ICON);
}


void TObjectDlg::OnBnClickedButton3()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_ObjList.ModifyStyle(LVS_TYPEMASK,
		LVS_LIST);
}


void TObjectDlg::OnHdnItemdblclickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;
}


void TObjectDlg::OnHdnItemclickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;
}


void TObjectDlg::OnLvnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	CString name = m_ObjList.GetItemText(pNMLV->iItem, pNMLV->iSubItem);
	//AfxMessageBox(name);
	GetDlgItem(IDC_SELECTITEM)->SetWindowText(name);
	*pResult = 0;
}


void TObjectDlg::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	CDialogEx::OnOK();
}


void TObjectDlg::OnBnClickedButton4()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int iCnt = m_ObjList.GetItemCount();
	for (int i = 0; i < iCnt; i++)
	{
		if (m_ObjList.GetCheck(i))
		{
			CString name = m_ObjList.GetItemText(i,0);
			//m_ObjList.DeleteItem(i);
			AfxMessageBox(name);
		}
	}

	HTREEITEM hRoot = m_ObjTree.GetRootItem();
	CheckupChild(hRoot);
}
void TObjectDlg::CheckupChild(HTREEITEM hRoot)
{
	HTREEITEM hChild = m_ObjTree.GetChildItem(hRoot);
	while (hChild != NULL)
	{
		if (m_ObjTree.GetCheck(hChild))
		{
			ChildCheck(hChild);
			//AfxMessageBox(m_ObjTree.GetItemText(hChild));
		}
		if (m_ObjTree.ItemHasChildren(hChild))
		{
			CheckupChild(hChild);
		}
		hChild = m_ObjTree.GetNextItem(hChild, TVGN_NEXT);
	}
}
void TObjectDlg::ChildCheck(HTREEITEM hRoot)
{
	HTREEITEM hChild = m_ObjTree.GetChildItem(hRoot);
	while (hChild != NULL)
	{
		m_ObjTree.SetCheck(hChild);
		m_ObjTree.EnsureVisible(hChild);
		if (m_ObjTree.ItemHasChildren(hChild))
		{
			ChildCheck(hChild);
		}
		hChild = m_ObjTree.GetNextItem(hChild, TVGN_NEXT);
	}
}
void TObjectDlg::OnTvnSelchangingTree1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	CString name = m_ObjTree.GetItemText(pNMTreeView->itemNew.hItem);
	//AfxMessageBox(name);
	GetDlgItem(IDC_SELECTITEM)->SetWindowText(name);	
	*pResult = 0;
}


void TObjectDlg::OnBnClickedButton6()
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
		AfxMessageBox(selFileName);
	}
}


void TObjectDlg::OnBnClickedButton5()
{
	CString selFileName;
	CColorDialog dlg(RGB(0, 0, 0), CC_FULLOPEN);

	if (dlg.DoModal() == IDOK)
	{
		COLORREF color = dlg.GetColor();
		selFileName.Format(L"%u, %u, %u",
			GetRValue(color),
			GetGValue(color),
			GetBValue(color));
		AfxMessageBox(selFileName);
	}
}


void TObjectDlg::OnBnClickedButton8()
{
	CString selFileName;
	CFileDialog dlg(
		FALSE, L"bmp", NULL,
		OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST,
		L"bmp Files(*.bmp)|*.bmp| All Files(*.*)|*.*|", this);

	if (dlg.DoModal() == IDOK)
	{
		selFileName = dlg.GetPathName();
		//selFileName += dlg.GetFileName();
		//selFileName += dlg.GetFileExt();
		AfxMessageBox(selFileName);
	}
}


void TObjectDlg::OnBnClickedButton7()
{
	CString selFileName;
	CFontDialog dlg;
	if (dlg.DoModal() == IDOK)
	{
		COLORREF color = dlg.GetColor();
		selFileName.Format(L"%s, %d, %u %u %u",
			dlg.GetFaceName(),
			dlg.GetSize(),
			GetRValue(color),
			GetGValue(color),
			GetBValue(color));
		AfxMessageBox(selFileName);
	}
}
