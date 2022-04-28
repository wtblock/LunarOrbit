
// LunarOrbitView.h : interface of the CLunarOrbitView class
//

#pragma once


class CLunarOrbitView : public CScrollView
{
protected: // create from serialization only
	CLunarOrbitView();
	DECLARE_DYNCREATE(CLunarOrbitView)

// Attributes
public:
	CLunarOrbitDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CLunarOrbitView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in LunarOrbitView.cpp
inline CLunarOrbitDoc* CLunarOrbitView::GetDocument() const
   { return reinterpret_cast<CLunarOrbitDoc*>(m_pDocument); }
#endif

