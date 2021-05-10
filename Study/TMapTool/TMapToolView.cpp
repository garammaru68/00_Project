
// TMapToolView.cpp : CTMapToolView Ŭ������ ����
//

#include "stdafx.h"
// SHARED_HANDLERS�� �̸� ����, ����� �׸� �� �˻� ���� ó���⸦ �����ϴ� ATL ������Ʈ���� ������ �� ������
// �ش� ������Ʈ�� ���� �ڵ带 �����ϵ��� �� �ݴϴ�.
#ifndef SHARED_HANDLERS
#include "TMapTool.h"
#endif

#include "TMapToolDoc.h"
#include "TMapToolView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTMapToolView

IMPLEMENT_DYNCREATE(CTMapToolView, CView)

BEGIN_MESSAGE_MAP(CTMapToolView, CView)
	// ǥ�� �μ� ����Դϴ�.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_SIZE()
END_MESSAGE_MAP()

// CTMapToolView ����/�Ҹ�

CTMapToolView::CTMapToolView()
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.

}

CTMapToolView::~CTMapToolView()
{
}

BOOL CTMapToolView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.

	return CView::PreCreateWindow(cs);
}

// CTMapToolView �׸���

void CTMapToolView::OnDraw(CDC* /*pDC*/)
{
	CTMapToolDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: ���⿡ ���� �����Ϳ� ���� �׸��� �ڵ带 �߰��մϴ�.
}


// CTMapToolView �μ�

BOOL CTMapToolView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// �⺻���� �غ�
	return DoPreparePrinting(pInfo);
}

void CTMapToolView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ��ϱ� ���� �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
}

void CTMapToolView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ� �� ���� �۾��� �߰��մϴ�.
}


// CTMapToolView ����

#ifdef _DEBUG
void CTMapToolView::AssertValid() const
{
	CView::AssertValid();
}

void CTMapToolView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CTMapToolDoc* CTMapToolView::GetDocument() const // ����׵��� ���� ������ �ζ������� �����˴ϴ�.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTMapToolDoc)));
	return (CTMapToolDoc*)m_pDocument;
}
#endif //_DEBUG


// CTMapToolView �޽��� ó����


LRESULT CTMapToolView::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.

	return CView::WindowProc(message, wParam, lParam);
}


void CTMapToolView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	CTMapToolApp* pApp = (CTMapToolApp*)AfxGetApp();
	pApp->m_Tool.ResizeDevice(cx, cy );
}
