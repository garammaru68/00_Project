// TCreateMap.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "TMapTool.h"
#include "TCreateMap.h"
#include "afxdialogex.h"


// TCreateMap ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(TCreateMap, CDialogEx)

TCreateMap::TCreateMap(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_TCREATEMAP, pParent)	
	, m_iMapSizeX(_T("32"))
	, m_iMapSizeY(_T("32"))
	, m_iCellSize(0)
	, m_iSpacePartision(FALSE)
	, m_iRadio(0)
	, m_Radio2(0)
{

}

TCreateMap::~TCreateMap()
{
}

void TCreateMap::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_iMapSizeX);
	DDX_Text(pDX, IDC_EDIT2, m_iMapSizeY);
	DDX_Control(pDX, IDC_LIST3, m_Cell);
	DDV_MinMaxInt(pDX, m_iCellSize, 0, 1000);
	DDX_Control(pDX, IDC_CELLSIZE2, m_CellSizeCtl);
	DDX_Check(pDX, IDC_CHECK2, m_iSpacePartision);
	DDX_Radio(pDX, IDC_RADIO1, m_iRadio);
	DDX_Radio(pDX, IDC_RADIO4, m_Radio2);
}


BEGIN_MESSAGE_MAP(TCreateMap, CDialogEx)
	ON_BN_CLICKED(IDOK, &TCreateMap::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON1, &TCreateMap::OnBnInitialize)
END_MESSAGE_MAP()


// TCreateMap �޽��� ó�����Դϴ�.


void TCreateMap::OnBnClickedOk()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	//AfxMessageBox(m_iMapSizeX);
	UpdateData(TRUE);//��Ʈ��-> ����
	//UpdateData(FALSE);// ���� -> ��Ʈ��
	int iSell = m_Cell.GetCurSel();
	if (iSell >= 0)
	{
		m_iCellDetail = m_Cell.GetItemData(iSell);
		CString buffer;
		m_Cell.GetText(iSell, buffer);
		int iNum = _tstoi((LPCTSTR)buffer);
	}
	CDialogEx::OnOK();
}


void TCreateMap::OnBnInitialize()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_iMapSizeX = L"32";
	m_iMapSizeY = L"32";
	m_Cell.AddString(L"1");
	m_Cell.AddString(L"2");
	m_Cell.AddString(L"4");
	m_Cell.AddString(L"8");
	m_Cell.AddString(L"16");
	m_Cell.AddString(L"32");

	m_Cell.SetCurSel(0);

	m_CellSizeCtl.AddString(L"1");
	m_CellSizeCtl.AddString(L"2");
	m_CellSizeCtl.AddString(L"4");
	m_CellSizeCtl.AddString(L"8");
	m_CellSizeCtl.AddString(L"16");
	m_CellSizeCtl.AddString(L"32");
	m_CellSizeCtl.SetCurSel(0);
	CComboBox* cb =(CComboBox*)GetDlgItem(IDC_CELLSIZE);
	cb->AddString(L"1");
	cb->AddString(L"2");
	cb->AddString(L"4");
	cb->SetCurSel(0);

	UpdateData(FALSE);
}


BOOL TCreateMap::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_iMapSizeX = L"32";
	m_iMapSizeY = L"32";
	m_Cell.AddString(L"1");
	m_Cell.AddString(L"2");
	m_Cell.AddString(L"4");
	m_Cell.AddString(L"8");
	m_Cell.AddString(L"16");
	m_Cell.AddString(L"32");

	m_Cell.SetCurSel(0);

	m_CellSizeCtl.AddString(L"1");
	m_CellSizeCtl.AddString(L"2");
	m_CellSizeCtl.AddString(L"4");
	m_CellSizeCtl.AddString(L"8");
	m_CellSizeCtl.AddString(L"16");
	m_CellSizeCtl.AddString(L"32");
	m_CellSizeCtl.SetCurSel(0);
	CComboBox* cb = (CComboBox*)GetDlgItem(IDC_CELLSIZE);
	cb->AddString(L"1");
	cb->AddString(L"2");
	cb->AddString(L"4");
	cb->SetCurSel(0);

	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}
