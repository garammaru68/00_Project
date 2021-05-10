
// TMapToolView.h : CTMapToolView Ŭ������ �������̽�
//

#pragma once


class CTMapToolView : public CView
{
protected: // serialization������ ��������ϴ�.
	CTMapToolView();
	DECLARE_DYNCREATE(CTMapToolView)

// Ư���Դϴ�.
public:
	CTMapToolDoc* GetDocument() const;

// �۾��Դϴ�.
public:

// �������Դϴ�.
public:
	virtual void OnDraw(CDC* pDC);  // �� �並 �׸��� ���� �����ǵǾ����ϴ�.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// �����Դϴ�.
public:
	virtual ~CTMapToolView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ������ �޽��� �� �Լ�
protected:
	DECLARE_MESSAGE_MAP()
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
};

#ifndef _DEBUG  // TMapToolView.cpp�� ����� ����
inline CTMapToolDoc* CTMapToolView::GetDocument() const
   { return reinterpret_cast<CTMapToolDoc*>(m_pDocument); }
#endif

