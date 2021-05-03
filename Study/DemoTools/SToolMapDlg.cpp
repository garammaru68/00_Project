// SToolMapDlg.cpp: 구현 파일
//

#include "pch.h"
#include "DemoTools.h"
#include "SToolMapDlg.h"
#include "afxdialogex.h"


// SToolMapDlg 대화 상자

IMPLEMENT_DYNAMIC(SToolMapDlg, CDialogEx)

SToolMapDlg::SToolMapDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SToolMapDlg, pParent)
{

}

SToolMapDlg::~SToolMapDlg()
{
}

void SToolMapDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(SToolMapDlg, CDialogEx)
END_MESSAGE_MAP()


// SToolMapDlg 메시지 처리기
