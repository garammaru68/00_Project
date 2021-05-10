
// KGCAToolView.h : CKGCAToolView Ŭ������ �������̽�
//

#pragma once


class CKGCAToolView : public CView
{
protected: // serialization������ ��������ϴ�.
	CKGCAToolView();
	DECLARE_DYNCREATE(CKGCAToolView)

// Ư���Դϴ�.
public:
	CKGCAToolDoc* GetDocument() const;

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
	virtual ~CKGCAToolView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ������ �޽��� �� �Լ�
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // KGCAToolView.cpp�� ����� ����
inline CKGCAToolDoc* CKGCAToolView::GetDocument() const
   { return reinterpret_cast<CKGCAToolDoc*>(m_pDocument); }
#endif

