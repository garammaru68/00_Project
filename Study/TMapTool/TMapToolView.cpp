
// TMapToolView.cpp : CTMapToolView 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
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
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_SIZE()
END_MESSAGE_MAP()

// CTMapToolView 생성/소멸

CTMapToolView::CTMapToolView()
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CTMapToolView::~CTMapToolView()
{
}

BOOL CTMapToolView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CTMapToolView 그리기

void CTMapToolView::OnDraw(CDC* /*pDC*/)
{
	CTMapToolDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
}


// CTMapToolView 인쇄

BOOL CTMapToolView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CTMapToolView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CTMapToolView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}


// CTMapToolView 진단

#ifdef _DEBUG
void CTMapToolView::AssertValid() const
{
	CView::AssertValid();
}

void CTMapToolView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CTMapToolDoc* CTMapToolView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTMapToolDoc)));
	return (CTMapToolDoc*)m_pDocument;
}
#endif //_DEBUG


// CTMapToolView 메시지 처리기


LRESULT CTMapToolView::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	return CView::WindowProc(message, wParam, lParam);
}


void CTMapToolView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CTMapToolApp* pApp = (CTMapToolApp*)AfxGetApp();
	pApp->m_Tool.ResizeDevice(cx, cy );
}
