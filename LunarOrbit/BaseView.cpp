
/////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2022 by W. T. Block, All Rights Reserved
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "BaseDoc.h"
#include "BaseView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define _DOUBLE_BUFFER

/////////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNCREATE(CBaseView, CScrollView)

/////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CBaseView, CScrollView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CBaseView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_SIZE()
	ON_WM_VSCROLL()
	ON_WM_ERASEBKGND()
	ON_WM_TIMER()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
CBaseView::CBaseView()
{
	TopOfView = 0;
}

/////////////////////////////////////////////////////////////////////////////
CBaseView::~CBaseView()
{
}

/////////////////////////////////////////////////////////////////////////////
BOOL CBaseView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CScrollView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// render the page or view
void CBaseView::render
(
	CDC* pDC, double dTopOfView, double dBottomOfView, int nLogicalWidth
)
{
	double dTopOfPage = 0;

	const double dPageHeight = PageHeight;
	const double dBottomOfPage = dTopOfPage + dPageHeight;

	// distance from top of view to top of page, where a positive value
	// indicates the page is partially below the view
	const double dPageOffset = dTopOfPage - dTopOfView;

	// logical coordinates allow the drawing to be device independent
	// i.e. rendering works on the screen as well as printing and print
	// preview
	const int nPageOffset = InchesToLogical( dPageOffset );
	const int nLogicalPageHeight = InchesToLogical( dPageHeight );

	// account for the shift of the view due to scrolling or printed pages
	pDC->SetWindowOrg( 0, -nPageOffset );

} // render

/////////////////////////////////////////////////////////////////////////////
void CBaseView::OnDraw( CDC* pDC )
{
	CBaseDoc* pDoc = Document;
	ASSERT_VALID( pDoc );
	if ( !pDoc )
		return;

	CDC* pTargetDC = pDC;

#ifdef _DOUBLE_BUFFER
	// double buffer output by creating a memory bitmap and drawing
	// directly to it and then copy the bitmap to the screen to reduce 
	// screen flicker
	CRect rectClient;
	GetClientRect( &rectClient );
	const int nRectWidth = rectClient.Width();
	const int nRectHeight = rectClient.Height();

	CBitmap bm;
	bm.CreateCompatibleBitmap( pDC, nRectWidth, nRectHeight );

	CDC dcMem;
	dcMem.CreateCompatibleDC( pDC );
	CBitmap* pBmOld = dcMem.SelectObject( &bm );
	dcMem.PatBlt( 0, 0, nRectWidth, nRectHeight, WHITENESS );
	pTargetDC = &dcMem;
	int nDcOrg = pTargetDC->SaveDC();
#endif

	const int nLogicalWidth = SetDrawDC( pTargetDC );
	const double dTopOfView = TopOfView;
	const double dBottomOfView = BottomOfView;
	render( pTargetDC, dTopOfView, dBottomOfView, nLogicalWidth );

#ifdef _DOUBLE_BUFFER
	pTargetDC->RestoreDC( nDcOrg );

	// output the drawing to the screen in a single bitblit
	pDC->BitBlt
	(
		0, 0, nRectWidth, nRectHeight, &dcMem, 0, 0, SRCCOPY
	);

	dcMem.SelectObject( pBmOld );
#endif

} // OnDraw

/////////////////////////////////////////////////////////////////////////////
void CBaseView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	OnVScroll( SB_TOP, 0, nullptr );

	Invalidate();

	m_bInitialUpdate = true;
}

/////////////////////////////////////////////////////////////////////////////
void CBaseView::OnFilePrintPreview()
{
	AFXPrintPreview(this);
}

/////////////////////////////////////////////////////////////////////////////
BOOL CBaseView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// turning off multiple copies and collation 
	pInfo->m_pPD->m_pd.Flags &= ~PD_USEDEVMODECOPIES;

	return DoPreparePrinting(pInfo);
}

/////////////////////////////////////////////////////////////////////////////
void CBaseView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	// setup the device context for our printer
	SetPrintDC
	(
		pDC,
		m_nPhysicalPageWidth,
		m_nPhysicalPageHeight,
		m_nLogicalPageWidth,
		m_nLogicalPageHeight
	);

	// height of the document in inches
	const double dDocumentHeight = DocumentHeight;

	// height of a page in inches
	const double dPageHeight = LogicalToInches( m_nLogicalPageHeight );

	// number of printer pages
	double dPages = dDocumentHeight / dPageHeight;

	// add a page if there is a fraction of a page
	m_nNumPages = (int)dPages;
	if ( !NearlyEqual( double( m_nNumPages ), dPages, 0.05 ) )
	{
		m_nNumPages++; // account for fractional page
	}

	// let the print dialog know
	pInfo->SetMinPage( 1 );
	pInfo->SetMaxPage( m_nNumPages );
}

/////////////////////////////////////////////////////////////////////////////
void CBaseView::OnPrint( CDC* pDC, CPrintInfo* pInfo )
{
	const double dPageHeight = LogicalToInches( m_nLogicalPageHeight );
	const int nPage = (int)pInfo->m_nCurPage;
	const double dTopOfView = ( (float)( nPage - 1 ) ) * dPageHeight;
	const double dBottomOfView = dTopOfView + dPageHeight;

	// the same render method used to draw on the screen
	render( pDC, dTopOfView, dBottomOfView, m_nLogicalPageWidth );

} // OnPrint

/////////////////////////////////////////////////////////////////////////////
void CBaseView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
#ifdef _DEBUG
void CBaseView::AssertValid() const
{
	CScrollView::AssertValid();
}

/////////////////////////////////////////////////////////////////////////////
void CBaseView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

/////////////////////////////////////////////////////////////////////////////
CBaseDoc* CBaseView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CBaseDoc)));
	return (CBaseDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// generate font characteristics from given properties
void CBaseView::BuildFont
(
	CString csFace, // name of the font face
	bool bBold, // bold font if true
	bool bItalic, // italic font if true
	int nTextHeight, // text height in pixels
	bool bVertical, // vertical orientation
	CFont& font, // generated font
	double dAngle /*= 0*/, // angle in degrees
	BYTE nCharSet/* = ANSI_CHARSET*/, // current character set
	bool bFlipX/* = false*/, // flip horizontally
	bool bFlipY/* = false*/, // flip vertically
	short nUp/* = -1*/, // moving up is a negative value
	int nTextWidth/* = 0*/ // default width
)
{
	LOGFONT lf;
	// Populate logical font with defaults
	::GetObject( GetStockObject( SYSTEM_FONT ), sizeof( LOGFONT ), &lf );
	int nAngle = int( dAngle * nUp * 10 );
	nAngle += bVertical ? nUp * 900 : 0;

	// rotate 180 degrees (happens when printing up-side-down)
	if ( bFlipX && bFlipY )
	{
		nAngle += ( nUp * 1800 );
		nAngle = nAngle % 3600;
	}

	// customize our font
	if ( nTextHeight != 0 )
	{
		lf.lfHeight = nTextHeight;
		if ( nTextWidth != 0 )
		{
			lf.lfWidth = nTextWidth;
		}
		else
		{
			lf.lfWidth = lf.lfHeight * 2 / 5;
		}
	}

	lf.lfEscapement = nAngle;
	lf.lfOrientation = nAngle;

	lf.lfWeight = bBold ? FW_BOLD : FW_NORMAL;
	lf.lfItalic = bItalic == true;
	lf.lfCharSet = nCharSet;
	_tcscpy( lf.lfFaceName, csFace );
	font.CreateFontIndirect( &lf );
} // BuildFont

/////////////////////////////////////////////////////////////////////////////
void CBaseView::OnPrepareDC( CDC * pDC, CPrintInfo * pInfo )
{
	if ( pInfo != nullptr ) // printing
	{
		SetPrintDC
		(
			pDC,
			m_nPhysicalPageWidth, m_nPhysicalPageHeight,
			m_nLogicalPageWidth, m_nLogicalPageHeight
		);
	}

} // OnPrepareDC

/////////////////////////////////////////////////////////////////////////////
// prepare the device context for drawing and
// return the logical width
int CBaseView::SetDrawDC
(
	CDC* pDC
)
{
	// the logical width is 11000 since we are mapping the logical
	// coordinate system to 1000 pixels per inch which means the 
	// code does not have to be concerned about the actual resolution
	// of the output device (image, screen, or printer)
	const int nLogicalWidth = LogicalDocumentWidth;
	if ( nLogicalWidth != 0 )
	{
		// isotropic means the values are the same in the X and Y directions
		pDC->SetMapMode( MM_ISOTROPIC );

		// using the width for X and Y so that the image will alway fit
		// horizontally in the window and the vertical dimension will 
		// adjust proportionally
		pDC->SetWindowExt( nLogicalWidth, nLogicalWidth );
		pDC->SetViewportExt( m_sizeClient.cx, m_sizeClient.cx );
	}

	return nLogicalWidth;
} // SetDrawDC

/////////////////////////////////////////////////////////////////////////////
// prepare the device context for printing
void CBaseView::SetPrintDC
(
	CDC* pDC,
	int& nPhysicalWidth, // in pixels
	int& nPhysicalHeight, // in pixels
	int& nLogicalWidth, // in inches * Map
	int& nLogicalHeight // in inches * Map
)
{
	nLogicalWidth = LogicalDocumentWidth;
	nLogicalHeight = LogicalDocumentHeight;

	nPhysicalWidth = pDC->GetDeviceCaps( HORZRES );
	nPhysicalHeight = pDC->GetDeviceCaps( VERTRES );
	const int nPixelsPerInchX = pDC->GetDeviceCaps( LOGPIXELSX );
	const int nPixelsPerInchY = pDC->GetDeviceCaps( LOGPIXELSY );

	CBaseDoc* pDoc = Document;
	const int nMap = pDoc->Map;
	const double dWidth = 
		(double)nPhysicalWidth * nMap / (double)nPixelsPerInchX;
	const int nWidth = (int)( dWidth + 0.5 );
	double dHeight = 
		(double)nPhysicalHeight * nMap / (double)nPixelsPerInchY;
	const int nHeight = (int)( dHeight + 0.5 );

	if ( nWidth < nLogicalWidth && nLogicalWidth != 0 )
	{
		dHeight = (float)nHeight / (float)nWidth;
		dHeight *= (float)nLogicalWidth;
		nLogicalHeight = (int)( dHeight + 0.5 );
	}
	else
	{
		nLogicalHeight = nHeight;
		nLogicalWidth = nWidth;
	}

	// create custom MM_HIENGLISH mapping mode:
	// 	1. maintain constant aspect ratio
	//	2. accept coordinates in 1/Map inches
	//	3. keep entire page width visible at all times
	//	4. vertical dimensions increase from top to bottom
	pDC->SetMapMode( MM_ISOTROPIC );
	//	base horizontal AND VERTICAL extents on page WIDTH
	//                  ============                 =====
	pDC->SetWindowExt( nLogicalWidth, nLogicalWidth );
	//	client WIDTH
	//		   =====
	pDC->SetViewportExt( nPhysicalWidth, nPhysicalWidth );
} // SetPrintDC

/////////////////////////////////////////////////////////////////////////////
// record the client rectangle every time the windows is resized
void CBaseView::OnSize( UINT nType, int cx, int cy )
{
	CView::OnSize( nType, cx, cy );

	m_sizeClient.cx = cx;
	m_sizeClient.cy = cy;

	SetupScrollBars();
} // OnSize

/////////////////////////////////////////////////////////////////////////////
BOOL CBaseView::OnEraseBkgnd( CDC* pDC )
{
#ifdef _DOUBLE_BUFFER
	// double buffer output by creating a memory bitmap and drawing
	// directly to it and then copy the bitmap to the screen to reduce flicker
	// need to prevent the default erase background behavior
	return TRUE;
#else
	return CView::OnEraseBkgnd( pDC );
#endif
} // OnEraseBkgnd

/////////////////////////////////////////////////////////////////////////////
void CBaseView::OnVScroll( UINT nSBCode, UINT nPos, CScrollBar* pScrollBar )
{
	CBaseDoc* pDoc = Document;
	ASSERT_VALID( pDoc );

	double dTop = TopOfView;
	double dLast = Last;
	const double dLineHeight = LineHeight;
	const double dPageHeight = PageHeight;

	switch ( nSBCode )
	{
		case SB_PAGEDOWN: // Scroll one page down.
			if ( NearlyEqual( dTop, dLast ) ) return;
			dTop += dPageHeight;
			if ( dTop > dLast )
			{
				dTop = dLast;
			}
			break;
		case SB_LINEDOWN: // Scroll one line down.
			if ( NearlyEqual( dTop, dLast ) ) return;
			dTop += dLineHeight;
			break;
		case SB_PAGEUP: // Scroll one page up.
			if ( NearlyEqual( dTop, 0.0 ) ) return;
			dTop -= dPageHeight;
			break;
		case SB_LINEUP: // Scroll one line up.
			if ( NearlyEqual( dTop, 0.0 ) ) return;
			dTop -= dLineHeight;
			break;
		case SB_THUMBTRACK: 	// Drag scroll box to specified position. 
								// The current position is provided in nPos.
			dTop = nPos * dLineHeight;
			break;
		case SB_THUMBPOSITION: // Scroll to the absolute position. 
								// The current position is provided in nPos.

			dTop = nPos * dLineHeight;
			break;
		case SB_TOP: // Scroll to top.
			dTop = 0;
			break;
		case SB_BOTTOM: // Scroll to bottom.
			dTop = dLast;
			break;
		case SB_ENDSCROLL: // End scroll.
			return;
	} // switch 

	// cannot scroll above the top
	if ( dTop < 0 )
	{
		dTop = 0;
	}
	// cannot scroll below the bottom
	else if ( dTop > dLast )
	{
		dTop = dLast;
	}

	if ( !NearlyEqual( dLast, 0.0 ) )
	{
		const double dLines = dLast / dLineHeight;
		const int nLines = int( dLines + 0.5f );
		const double dRatio = dTop / dLast;
		const double dLine = dLines * dRatio;
		int nLine = int( dLine + 0.5f );
		SetScrollPos( SB_VERT, nLine );
	}

	SetTopOfView( dTop );
	InvalidateRect( NULL );
} // OnVScroll

/////////////////////////////////////////////////////////////////////////////
