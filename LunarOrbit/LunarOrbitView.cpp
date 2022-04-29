
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

#define _DOUBLE_BUFFER

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
	ON_WM_SIZE()
	ON_WM_VSCROLL()
	ON_WM_ERASEBKGND()
	ON_WM_TIMER()
	ON_COMMAND( ID_EDIT_PAUSE, &CLunarOrbitView::OnEditPause )
	ON_UPDATE_COMMAND_UI( ID_EDIT_PAUSE, &CLunarOrbitView::OnUpdateEditPause )
	ON_COMMAND( ID_EDIT_RUN, &CLunarOrbitView::OnEditRun )
	ON_UPDATE_COMMAND_UI( ID_EDIT_RUN, &CLunarOrbitView::OnUpdateEditRun )
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
CLunarOrbitView::CLunarOrbitView()
{
	Running = false;
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
void CLunarOrbitView::OnDraw( CDC* pDC )
{
	CLunarOrbitDoc* pDoc = Document;
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
void CLunarOrbitView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	Invalidate();

	m_bInitialUpdate = true;
}

/////////////////////////////////////////////////////////////////////////////
void CLunarOrbitView::OnFilePrintPreview()
{
	AFXPrintPreview(this);
}

/////////////////////////////////////////////////////////////////////////////
BOOL CLunarOrbitView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// turning off multiple copies and collation 
	pInfo->m_pPD->m_pd.Flags &= ~PD_USEDEVMODECOPIES;

	return DoPreparePrinting(pInfo);
}

/////////////////////////////////////////////////////////////////////////////
void CLunarOrbitView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
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
void CLunarOrbitView::OnPrint( CDC* pDC, CPrintInfo* pInfo )
{
	const double dPageHeight = LogicalToInches( m_nLogicalPageHeight );
	const int nPage = (int)pInfo->m_nCurPage;
	const double dTopOfView = ( (float)( nPage - 1 ) ) * dPageHeight;
	const double dBottomOfView = dTopOfView + dPageHeight;

	// the same render method used to draw on the screen
	render( pDC, dTopOfView, dBottomOfView, m_nLogicalPageWidth );

} // OnPrint

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
	theApp.GetContextMenuManager()->ShowPopupMenu
	(
		IDR_POPUP_EDIT, point.x, point.y, this, TRUE
	);
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
void CLunarOrbitView::OnPrepareDC( CDC * pDC, CPrintInfo * pInfo )
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
int CLunarOrbitView::SetDrawDC
(
	CDC* pDC
)
{
	// the document has hard coded the width to be 11 inches
	CLunarOrbitDoc* pDoc = Document;
	const double dDocumentWidth = pDoc->Width;

	// the logical width is 11000 since we are mapping the logical
	// coordinate system to 1000 pixels per inch which means the 
	// code does not have to be concerned about the actual resolution
	// of the output device (image, screen, or printer)
	const int nLogicalWidth = InchesToLogical( dDocumentWidth );
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
void CLunarOrbitView::SetPrintDC
(
	CDC* pDC,
	int& nPhysicalWidth, // in pixels
	int& nPhysicalHeight, // in pixels
	int& nLogicalWidth, // in inches * Map
	int& nLogicalHeight // in inches * Map
)
{
	nPhysicalWidth = pDC->GetDeviceCaps( HORZRES );
	nPhysicalHeight = pDC->GetDeviceCaps( VERTRES );
	const int nPixelsPerInchX = pDC->GetDeviceCaps( LOGPIXELSX );
	const int nPixelsPerInchY = pDC->GetDeviceCaps( LOGPIXELSY );

	CLunarOrbitDoc* pDoc = Document;
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
void CLunarOrbitView::OnSize( UINT nType, int cx, int cy )
{
	CView::OnSize( nType, cx, cy );

	m_sizeClient.cx = cx;
	m_sizeClient.cy = cy;

	SetupScrollBars();
} // OnSize

/////////////////////////////////////////////////////////////////////////////
// generate font characteristics from given properties
void CLunarOrbitView::BuildFont
(
	CString csFace, // name of the font face
	bool bBold, // bold font if true
	bool bItalic, // italic font if true
	int nTextHeight, // text height in pixels
	bool bVertical, // vertical orientation
	CFont& font, // generated font
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
	int nAngle = bVertical ? nUp * 900 : 0;

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
// render the page or view
void CLunarOrbitView::render
(
	CDC* pDC, double dTopOfView, double dBottomOfView, int nLogicalWidth
)
{
	CLunarOrbitDoc* pDoc = Document;
	const UINT nPages = pDoc->Pages;
	double dTopOfPage = 0;

	// create a pen to draw with
	CPen penGray, penBlue;

	// save the entry state
	const int nDC = pDC->SaveDC();

	// 1 hundredths of an inch
	const int nGrayWidth = InchesToLogical( 0.01 );

	// 5 hundredth of an inch
	const int nBlueWidth = InchesToLogical( 0.05 );

	// 0.12 inch size
	const int nTextHeight = InchesToLogical( 0.18 );

	// gray color
	const COLORREF rgbGray = RGB( 128, 128, 128 );

	// blue color
	const COLORREF rgbBlue = RGB( 0, 0, 240 );

	// blue color
	const COLORREF rgbGreen = RGB( 0, 240, 0 );

	// create a solid gray pen 0.05 inches wide
	penGray.CreatePen( PS_SOLID, nGrayWidth, rgbGray );

	// create a solid blue pen 0.05 inches wide
	penBlue.CreatePen( PS_SOLID, nBlueWidth, rgbBlue );

	// create a green and gray brush
	CBrush brGreen, brGray;

	// create a green brush
	brGreen.CreateSolidBrush( rgbGreen );

	// create a gray brush
	brGray.CreateSolidBrush( rgbGray );

	// create a font for text output
	CFont font;
	BuildFont
	(
		_T( "Arial" ), false, false, nTextHeight, false, font
	);

	// setting labels
	CString csAngle, csVelocity, csVelocityX, csVelocityY,
		csDistance, csSample, csSamplesPerDay, csRunningTime;
	csAngle.Format( _T( "Angle in degrees: %0.0f" ), AngleInDegrees );
	csVelocity.Format
	(
		_T( "Initial velocity in meters per second: %0.0f" ), Velocity
	);
	csVelocityX.Format
	(
		_T( "Initial X-velocity in meters per second: %0.0f" ),
		HorizontalVelocity
	);
	csVelocityY.Format
	(
		_T( "Initial Y-velocity in meters per second: %0.0f" ),
		VerticalVelocity
	);
	csDistance.Format
	(
		_T( "Distance to moon in meters: %0.0f" ), MetersToMoon
	);
	csSample.Format
	(
		_T( "Time between samples in seconds: %0.0f" ), SampleTime
	);
	csSamplesPerDay.Format
	(
		_T( "Samples per day: %0.0f" ), pDoc->SamplesPerDay
	);
	csRunningTime.Format
	(
		_T( "Running time in days: %0.02f" ), pDoc->RunningTime / 86400
	);

	const double dPageHeight = PageHeight;
	const double dBottomOfPage = dTopOfPage + dPageHeight;

	// distance from top of view to top of page, where a positive value
	// indicates the page is partially below the view
	const double dPageOffset = dTopOfPage - dTopOfView;
	const int nPageOffset = InchesToLogical( dPageOffset );
	const int nLogicalPageHeight = InchesToLogical( dPageHeight );

	// account for the shift of the view due to scrolling or printed pages
	pDC->SetWindowOrg( 0, -nPageOffset );

	// time between trajectory calculations
	const double dT = SampleTime;

	// draw settings information
	pDC->SelectObject( &font );
	pDC->SetTextAlign( TA_RIGHT | TA_BASELINE );

	// the starting point's coordinates
	int nX = InchesToLogical( DocumentWidth - 1.0 );
	int nY = InchesToLogical( 1.0 );

	pDC->TextOut( nX, nY, csDistance );
	nY += nTextHeight;
	pDC->TextOut( nX, nY, csVelocity );
	nY += nTextHeight;
	pDC->TextOut( nX, nY, csVelocityX );
	nY += nTextHeight;
	pDC->TextOut( nX, nY, csVelocityY );
	nY += nTextHeight;
	pDC->TextOut( nX, nY, csAngle );
	nY += nTextHeight;
	pDC->TextOut( nX, nY, csSample );
	nY += nTextHeight;
	pDC->TextOut( nX, nY, csSamplesPerDay );
	nY += nTextHeight;
	pDC->TextOut( nX, nY, csRunningTime );
	nY += nTextHeight;

	// remember position for writing the total time taken
	const int nTextX = nX;
	const int nTextY = nY;

	// gray line for the axes
	pDC->SelectObject( &penGray );

	// draw the X and Y axes
	const int nDocWidth = InchesToLogical( DocumentWidth );
	const int nDocHeight = InchesToLogical( DocumentHeight );
	CPoint ptEarth = EarthCenter;
	pDC->MoveTo( 0, ptEarth.y );
	pDC->LineTo( nDocWidth, ptEarth.y );
	pDC->MoveTo( ptEarth.x, 0 );
	pDC->LineTo( ptEarth.x, nDocHeight );

	// color the moon gray
	pDC->SelectObject( &brGray );

	// create a rectangle representing the moon 
	CRect rectMoon = MoonRectangle;

	// draw the moon as an ellipse that fits into the rectangle
	pDC->Ellipse( &rectMoon );

	// colors of the earth
	pDC->SelectObject( &penBlue );
	pDC->SelectObject( &brGreen );

	// create a rectangle representing the earth 
	CRect rectEarth = EarthRectangle;

	// draw the earth as an ellipse that fits into the rectangle
	pDC->Ellipse( &rectEarth );

	// restore the device context
	pDC->RestoreDC( nDC );

} // render

/////////////////////////////////////////////////////////////////////////////
// this routine will update the moon's position using time slices
// equal to SampleTime.
void CLunarOrbitView::UpdateMoonPosition()
{
	CLunarOrbitDoc* pDoc = Document;

	// starting positions
	double dX = pDoc->MoonX;
	double dY = pDoc->MoonY;

	// starting velocity
	double dVx = pDoc->VelocityX;
	double dVy = pDoc->VelocityY;

	// acceleration of earth's gravity on the moon
	const double dA = pDoc->AccelerationOfGravity;

	// the time the model has been run in seconds
	double dTime = pDoc->RunningTime;

	// mass of the earth in kilograms
	const double dM = pDoc->MassOfTheEarth;

	// the distance to the moon in meters
	const double dR = MetersToMoon;

	// the number of time slices the day is divided into
	const int nSamplesPerDay = (int)pDoc->SamplesPerDay;

	// the length of a time slice in seconds
	const double dSt = pDoc->SampleTime;

	// the known lunar period in seconds
	const double dLunarPeriod = pDoc->LunarPeriod;

	// are we done with a complete cycle
	bool bDone = false;

	// loop through the time slices and update positions and velocities
	for ( int nSample = 0; nSample < nSamplesPerDay; nSample++ )
	{
		// the acceleration of gravity in the X direction
		const double dAx = -dA * dX / dR;

		// the acceleration of gravity in the Y direction
		const double dAy = -dA * dY / dR;

		// the new velocity in the X direction
		const double dNewVx = dVx + dAx * dSt;

		// the new velocity in the Y direction
		const double dNewVy = dVy + dAy * dSt;

		// the new X position 
		const double dNewX = dX + dVx * dSt;

		// the new Y position
		const double dNewY = dY + dVy * dSt;

		// update the current position for the next time slice
		dVx = dNewVx;
		dVy = dNewVy;
		dX = dNewX;
		dY = dNewY;

		// update the running time
		dTime += dSt;

		// stop when a lunar cycle completes
		if ( dTime >= dLunarPeriod )
		{
			KillTimer( 1 );
			Running = false;
			bDone = true;
			break;
		}
	}

	// record the final result into the document
	pDoc->MoonX = dX;
	pDoc->MoonY = dY;
	pDoc->VelocityX = dVx;
	pDoc->VelocityY = dVy;
	pDoc->RunningTime = dTime;

	// if we are done, repaint the view
	if ( bDone )
	{
		Invalidate();
	}

} // UpdateMoonPosition()

/////////////////////////////////////////////////////////////////////////////
BOOL CLunarOrbitView::OnEraseBkgnd( CDC* pDC )
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
void CLunarOrbitView::OnVScroll( UINT nSBCode, UINT nPos, CScrollBar* pScrollBar )
{
	CLunarOrbitDoc* pDoc = Document;
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
void CLunarOrbitView::OnTimer( UINT_PTR nIDEvent )
{
	// update the moon's position and velocity for a day
	UpdateMoonPosition();

	// redraw the view
	Invalidate();

	CScrollView::OnTimer( nIDEvent );
} // OnTimer

/////////////////////////////////////////////////////////////////////////////
// the pause tool bar button handler changes the running flag and stops
// the timer
void CLunarOrbitView::OnEditPause()
{
	Running = false;
	KillTimer( 1 );

} // OnEditPause

/////////////////////////////////////////////////////////////////////////////
// the pause tool bar UI handler (check and gray out)
void CLunarOrbitView::OnUpdateEditPause( CCmdUI *pCmdUI )
{
	const bool bRunning = Running;
	if ( bRunning )
	{
		pCmdUI->Enable( TRUE );
		pCmdUI->SetCheck( FALSE );
	}
	else // paused
	{
		pCmdUI->Enable( FALSE );
		pCmdUI->SetCheck( TRUE );
	}
} // OnUpdateEditPause

/////////////////////////////////////////////////////////////////////////////
// the run tool bar button handler changes the running flag and starts
// the timer
void CLunarOrbitView::OnEditRun()
{
	Running = true;
	SetTimer( 1, 1000, nullptr );

} // OnEditRun

/////////////////////////////////////////////////////////////////////////////
// the run tool bar UI handler (check and gray out)
void CLunarOrbitView::OnUpdateEditRun( CCmdUI *pCmdUI )
{
	const bool bRunning = Running;
	if ( bRunning )
	{
		pCmdUI->Enable( FALSE );
		pCmdUI->SetCheck( TRUE );
	}
	else // paused
	{
		pCmdUI->Enable( TRUE );
		pCmdUI->SetCheck( FALSE );
	}
} // OnUpdateEditRun

/////////////////////////////////////////////////////////////////////////////
