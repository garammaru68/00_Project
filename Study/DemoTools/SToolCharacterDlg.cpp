// SToolCharacterDlg.cpp: 구현 파일
//

#include "pch.h"
#include "DemoTools.h"
#include "SToolCharacterDlg.h"
#include "afxdialogex.h"


// SToolCharacterDlg 대화 상자

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
END_MESSAGE_MAP()


// SToolCharacterDlg 메시지 처리기
