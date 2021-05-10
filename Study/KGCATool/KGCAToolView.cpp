
// KGCAToolView.cpp : CKGCAToolView Ŭ������ ����
//

#include "stdafx.h"
// SHARED_HANDLERS�� �̸� ����, ����� �׸� �� �˻� ���� ó���⸦ �����ϴ� ATL ������Ʈ���� ������ �� ������
// �ش� ������Ʈ�� ���� �ڵ带 �����ϵ��� �� �ݴϴ�.
#ifndef SHARED_HANDLERS
#include "KGCATool.h"
#endif

#include "KGCAToolDoc.h"
#include "KGCAToolView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CKGCAToolView

IMPLEMENT_DYNCREATE(CKGCAToolView, CView)

BEGIN_MESSAGE_MAP(CKGCAToolView, CView)
	// ǥ�� �μ� ����Դϴ�.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CKGCAToolView ����/�Ҹ�

CKGCAToolView::CKGCAToolView()
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.

}

CKGCAToolView::~CKGCAToolView()
{
}

BOOL CKGCAToolView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.

	return CView::PreCreateWindow(cs);
}

// CKGCAToolView �׸���

void CKGCAToolView::OnDraw(CDC* /*pDC*/)
{
	CKGCAToolDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: ���⿡ ���� �����Ϳ� ���� �׸��� �ڵ带 �߰��մϴ�.
}


// CKGCAToolView �μ�

BOOL CKGCAToolView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// �⺻���� �غ�
	return DoPreparePrinting(pInfo);
}

void CKGCAToolView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ��ϱ� ���� �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
}

void CKGCAToolView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ� �� ���� �۾��� �߰��մϴ�.
}


// CKGCAToolView ����

#ifdef _DEBUG
void CKGCAToolView::AssertValid() const
{
	CView::AssertValid();
}

void CKGCAToolView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CKGCAToolDoc* CKGCAToolView::GetDocument() const // ����׵��� ���� ������ �ζ������� �����˴ϴ�.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CKGCAToolDoc)));
	return (CKGCAToolDoc*)m_pDocument;
}
#endif //_DEBUG


// CKGCAToolView �޽��� ó����
