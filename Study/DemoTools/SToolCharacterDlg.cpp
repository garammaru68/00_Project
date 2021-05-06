// SToolCharacterDlg.cpp: 구현 파일

#include "pch.h"
#include "DemoTools.h"
#include "SToolCharacterDlg.h"
#include "afxdialogex.h"

IMPLEMENT_DYNAMIC(SToolCharacterDlg, CDialogEx)

SToolCharacterDlg::SToolCharacterDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CHARACTER_DLG, pParent)
{

}
SToolCharacterDlg::~SToolCharacterDlg()
{
}

void SToolCharacterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(SToolCharacterDlg, CDialogEx)
ON_BN_CLICKED(IDC_BUTTON_FILE, &SToolCharacterDlg::OnBnClickedButtonFile)
ON_BN_CLICKED(IDC_BUTTON_SAVE, &SToolCharacterDlg::OnBnClickedButtonSave)
ON_BN_CLICKED(IDC_BUTTON_COLOR, &SToolCharacterDlg::OnBnClickedButtonColor)
ON_BN_CLICKED(IDC_BUTTON_FONT, &SToolCharacterDlg::OnBnClickedButtonFont)
END_MESSAGE_MAP()

void SToolCharacterDlg::OnBnClickedButtonFile()
{
	CFileDialog dlg(TRUE,
						 L"bmp",
						 NULL,
						 OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST,
						 L"bmp files(*.bmp)|*.bmp| All files(*.*)|*.*|",
						 this);
	if (dlg.DoModal() == IDOK)
	{
		CString filename;
		filename = dlg.GetPathName();
		filename += dlg.GetFileName();
		AfxMessageBox(filename);
	}
}
void SToolCharacterDlg::OnBnClickedButtonSave()
{
	CFileDialog dlg(FALSE,
		L"bmp",
		NULL,
		OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST,
		L"bmp files(*.bmp)|*.bmp| All Files(*.*)|*.*|",
		this);
	if (dlg.DoModal() == IDOK)
	{
		CString filename;
		filename = dlg.GetPathName();
		AfxMessageBox(filename);
	}
}
void SToolCharacterDlg::OnBnClickedButtonColor()
{
	CColorDialog dlg(RGB(0, 0, 0), CC_FULLOPEN);
	if (dlg.DoModal() == IDOK)
	{
		COLORREF color = dlg.GetColor();
		CString colordata;
		colordata.Format(L"%u, %u, %u",
			GetRValue(color),
			GetGValue(color),
			GetBValue(color));
		AfxMessageBox(colordata);
	}
}
void SToolCharacterDlg::OnBnClickedButtonFont()
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