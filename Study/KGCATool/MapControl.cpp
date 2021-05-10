// MapControl.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "KGCATool.h"
#include "MapControl.h"
#include "afxdialogex.h"


// MapControl 대화 상자입니다.

IMPLEMENT_DYNAMIC(MapControl, CDialogEx)

MapControl::MapControl(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG1, pParent)
{

}

MapControl::~MapControl()
{
}

void MapControl::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE1, m_listInfo);
	DDX_Control(pDX, IDC_LIST1, m_listAttribute);
}


BEGIN_MESSAGE_MAP(MapControl, CDialogEx)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE1, &MapControl::OnTvnSelchangedTree1)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST1, &MapControl::OnLvnItemchangedList1)
	ON_BN_CLICKED(IDC_BUTTON1, &MapControl::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &MapControl::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &MapControl::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &MapControl::OnBnClickedButton4)
END_MESSAGE_MAP()


// MapControl 메시지 처리기입니다.


BOOL MapControl::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	
	CBitmap bmp;
	bmp.LoadBitmapW(IDB_BITMAP2);	
	m_imgList.Create(16, 16, ILC_COLOR24, 6, 0);	
	m_imgListSmall.Create(16, 16, ILC_COLOR24, 6, 0);

	m_imgList.Add(&bmp, RGB(255,0,0));
	m_imgListSmall.Add(&bmp, RGB(255, 0, 0));

	m_listInfo.SetImageList(&m_imgList, LVSIL_NORMAL);
	m_listInfo.SetImageList(&m_imgListSmall, LVSIL_SMALL);

	HTREEITEM hParent, hChild0, hChild1;
	hParent = m_listInfo.InsertItem(L"aaaaa", 0, 2, TVI_ROOT);
	hChild0 = m_listInfo.InsertItem(L"BBBBB", 1, 2, hParent);
	hChild1 = m_listInfo.InsertItem(L"BBBBB", 0, 2, hParent);
	hParent = m_listInfo.InsertItem(L"CCCCC", 1, 2, hChild0);
	hParent = m_listInfo.InsertItem(L"ddddd", 0, 2, hChild0);
	hParent = m_listInfo.InsertItem(L"hChild1", 1, 2, hChild1);
	hParent = m_listInfo.InsertItem(L"hChild1", 0, 2, hChild1);
	m_listInfo.InsertItem(L"DDDD", hParent);
	//hParent = m_listInfo.InsertItem(L"aaaaa", TVI_ROOT);
	//hChild0  = m_listInfo.InsertItem(L"BBBBB", hParent);
	//hChild1 = m_listInfo.InsertItem(L"BBBBB", hParent);
	//hParent =m_listInfo.InsertItem(L"CCCCC", hChild0);
	//hParent = m_listInfo.InsertItem(L"ddddd", hChild0);
	//hParent = m_listInfo.InsertItem(L"hChild1", hChild1);
	//hParent = m_listInfo.InsertItem(L"hChild1", hChild1);
	//m_listInfo.InsertItem(L"DDDD", hParent);
	m_listInfo.EnsureVisible(hParent);

	// 스타일 확장
	DWORD dwStyle = m_listAttribute.GetExtendedStyle();
	m_listAttribute.SetExtendedStyle(
		dwStyle | LVS_EX_FULLROWSELECT
		|LVS_EX_GRIDLINES
	|LVS_EX_BORDERSELECT
	|LVS_EX_CHECKBOXES
	|LVS_EX_AUTOCHECKSELECT);

	m_listAttribute.ModifyStyle(LVS_TYPEMASK, LVS_LIST);
	
	static CImageList img0, img1;
	img0.Create(48, 48, ILC_COLOR32, 5, 0);
	img1.Create(16, 16, ILC_COLOR32, 5, 0);
	
	img0.Add(AfxGetApp()->LoadIcon(IDI_ICON1));
	img0.Add(AfxGetApp()->LoadIcon(IDI_ICON2));
	img0.Add(AfxGetApp()->LoadIcon(IDI_ICON3));
	img0.Add(AfxGetApp()->LoadIcon(IDI_ICON4));
	img0.Add(AfxGetApp()->LoadIcon(IDI_ICON5));
	img1.Add(AfxGetApp()->LoadIcon(IDI_ICON1));
	img1.Add(AfxGetApp()->LoadIcon(IDI_ICON2));
	img1.Add(AfxGetApp()->LoadIcon(IDI_ICON3));
	img1.Add(AfxGetApp()->LoadIcon(IDI_ICON4));
	img1.Add(AfxGetApp()->LoadIcon(IDI_ICON5));


	m_listAttribute.SetImageList(&img0, LVSIL_NORMAL);
	m_listAttribute.SetImageList(&img1, LVSIL_SMALL);

	
	m_listAttribute.InsertColumn(0, L"name", LVCFMT_LEFT, 100);
	m_listAttribute.InsertColumn(1, L"size", LVCFMT_LEFT, 200);
	
	TCHAR path[MAX_PATH] = { 0, };
	GetCurrentDirectory(MAX_PATH, path);
	_tcscat_s(path, _T("\\*.*"));
	LoadListData(path);
	/*m_listAttribute.InsertItem(0, L"aaaa" );
	m_listAttribute.SetItemText(0, 1, L"1234566");

	m_listAttribute.InsertItem(1, L"bbbb");
	m_listAttribute.SetItemText(1, 1, L"66666666");
	m_listAttribute.InsertItem(2, L"cccc");
	m_listAttribute.SetItemText(2, 1, L"888888888");
	m_listAttribute.InsertItem(3, L"dddd");
	m_listAttribute.SetItemText(3, 1, L"888888888");*/
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}
void MapControl::LoadListData(TCHAR* szPath)
{
	WIN32_FIND_DATA data;
	HANDLE hSearch = NULL;

	hSearch = FindFirstFile(szPath, &data);
	BOOL bFind = TRUE;
	int iCnt = 0;
	while (bFind)
	{
		if (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			m_listAttribute.InsertItem(iCnt, data.cFileName);
		}
		else
		{
			m_listAttribute.InsertItem(iCnt, data.cFileName, iCnt%5);
			CString buffer;
			buffer.Format(L"%d", data.nFileSizeLow);
			m_listAttribute.SetItemText(iCnt++, 1, buffer);
			
		}
		bFind = FindNextFile(hSearch, &data);
	}
}

void MapControl::OnTvnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	CString name = m_listInfo.GetItemText(pNMTreeView->itemNew.hItem);
	//AfxMessageBox(name);
	GetDlgItem(IDC_SELECTITEM)->SetWindowText(name);
	*pResult = 0;
}


void MapControl::OnLvnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	CString name = m_listAttribute.GetItemText(pNMLV->iItem, pNMLV->iSubItem);
	//AfxMessageBox(name);
	GetDlgItem(IDC_SELECTITEM)->SetWindowText(name);

	*pResult = 0;
}


void MapControl::OnBnClickedButton1()
{
	m_listAttribute.ModifyStyle(LVS_TYPEMASK, LVS_REPORT);

}


void MapControl::OnBnClickedButton2()
{
	m_listAttribute.ModifyStyle(LVS_TYPEMASK, LVS_SMALLICON);

}


void MapControl::OnBnClickedButton3()
{
	m_listAttribute.ModifyStyle(LVS_TYPEMASK, LVS_LIST);

}


void MapControl::OnBnClickedButton4()
{
	m_listAttribute.ModifyStyle(LVS_TYPEMASK, LVS_ICON);

}
