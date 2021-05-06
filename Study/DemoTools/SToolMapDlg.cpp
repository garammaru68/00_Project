// SToolMapDlg.cpp: 구현 파일
//

#include "pch.h"
#include "DemoTools.h"
#include "SToolMapDlg.h"
#include "afxdialogex.h"


// SToolMapDlg 대화 상자

IMPLEMENT_DYNAMIC(SToolMapDlg, CDialogEx)

SToolMapDlg::SToolMapDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MAP_DLG, pParent)
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
	ON_EN_CHANGE(IDC_EDIT1, &SToolMapDlg::OnEnChangeEdit1)
	ON_EN_CHANGE(IDC_EDIT2, &SToolMapDlg::OnEnChangeEdit2)
END_MESSAGE_MAP()


// SToolMapDlg 메시지 처리기


void SToolMapDlg::OnEnChangeEdit1()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void SToolMapDlg::OnEnChangeEdit2()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
