
/////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2022 by W. T. Block, All Rights Reserved
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "LunarOrbit.h"
#include "LunarOrbitDoc.h"
#include "LunarOrbitView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNCREATE(CLunarOrbitView, CScrollView)

/////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CLunarOrbitView, CScrollView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CLunarOrbitView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
CLunarOrbitView::CLunarOrbitView()
{
	// TODO: add construction code here

}

/////////////////////////////////////////////////////////////////////////////
CLunarOrbitView::~CLunarOrbitView()
{
}

/////////////////////////////////////////////////////////////////////////////
BOOL CLunarOrbitView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CScrollView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
void CLunarOrbitView::OnDraw(CDC* /*pDC*/)
{
	CLunarOrbitDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
void CLunarOrbitView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	// TODO: calculate the total size of this view
	sizeTotal.cx = sizeTotal.cy = 100;
	SetScrollSizes(MM_TEXT, sizeTotal);
}

/////////////////////////////////////////////////////////////////////////////
void CLunarOrbitView::OnFilePrintPreview()
{
	AFXPrintPreview(this);
}

/////////////////////////////////////////////////////////////////////////////
BOOL CLunarOrbitView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

/////////////////////////////////////////////////////////////////////////////
void CLunarOrbitView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

/////////////////////////////////////////////////////////////////////////////
void CLunarOrbitView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
void CLunarOrbitView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

/////////////////////////////////////////////////////////////////////////////
void CLunarOrbitView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
}


/////////////////////////////////////////////////////////////////////////////
#ifdef _DEBUG
void CLunarOrbitView::AssertValid() const
{
	CScrollView::AssertValid();
}

/////////////////////////////////////////////////////////////////////////////
void CLunarOrbitView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

/////////////////////////////////////////////////////////////////////////////
CLunarOrbitDoc* CLunarOrbitView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CLunarOrbitDoc)));
	return (CLunarOrbitDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
