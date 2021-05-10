// MapSaveCtrl.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "KGCATool.h"
#include "MapSaveCtrl.h"
#include "afxdialogex.h"


// MapSaveCtrl ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(MapSaveCtrl, CDialogEx)

MapSaveCtrl::MapSaveCtrl(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_MAPSAVECTRL, pParent)
{

}

MapSaveCtrl::~MapSaveCtrl()
{
}

void MapSaveCtrl::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(MapSaveCtrl, CDialogEx)
	ON_BN_CLICKED(IDOK, &MapSaveCtrl::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_FILE, &MapSaveCtrl::OnBnClickedButtonFile)
	ON_BN_CLICKED(IDC_BUTTON_COLOR, &MapSaveCtrl::OnBnClickedButtonColor)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, &MapSaveCtrl::OnBnClickedButtonSave)
	ON_BN_CLICKED(IDC_BUTTON_FONT, &MapSaveCtrl::OnBnClickedButtonFont)
END_MESSAGE_MAP()


// MapSaveCtrl �޽��� ó�����Դϴ�.


void MapSaveCtrl::OnBnClickedOk()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CDialogEx::OnOK();
}


void MapSaveCtrl::OnBnClickedButtonFile()
{
	CString selFileName;
	CFileDialog dlg (
		TRUE, L"bmp", NULL, 
		OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST,
		L"bmp Files(*.bmp)|*.bmp| All Files(*.*)|*.*|", this);

	if (dlg.DoModal() == IDOK)
	{
		selFileName= dlg.GetPathName();
		selFileName += dlg.GetFileName();
		//selFileName += dlg.GetFileExt();
		AfxMessageBox(selFileName);
	}
}


void MapSaveCtrl::OnBnClickedButtonColor()
{
	CString selFileName;
	CColorDialog dlg(RGB(0, 0, 0), CC_FULLOPEN);

	if (dlg.DoModal() == IDOK)
	{
		COLORREF color = dlg.GetColor();
		selFileName.Format(L"%u, %u, %u",
		    GetRValue(color),
			GetGValue(color),
			GetBValue(color)	);
		AfxMessageBox(selFileName);
	}
}


void MapSaveCtrl::OnBnClickedButtonSave()
{
	CString selFileName;
	CFileDialog dlg (
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


void MapSaveCtrl::OnBnClickedButtonFont()
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
