
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
	ON_COMMAND( ID_EDIT_SINGLEORBIT, &CLunarOrbitView::OnEditSingleOrbit )
	ON_UPDATE_COMMAND_UI( ID_EDIT_SINGLEORBIT, &CLunarOrbitView::OnUpdateEditSingleorbit )
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
CLunarOrbitView::CLunarOrbitView()
{
	Running = false;
	SingleOrbit = false;
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
void CLunarOrbitView::SetPrintDC
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
// render the lunar orbit 
void CLunarOrbitView::RenderLunarOrbit( CDC * pDC )
{
	// 1 hundredths of an inch
	const int nGrayWidth = InchesToLogical( 0.01 );

	// gray color
	const COLORREF rgbGray = RGB( 128, 128, 128 );

	// create a solid gray pen 0.05 inches wide
	CPen penGray;
	penGray.CreatePen( PS_SOLID, nGrayWidth, rgbGray );

	CPen* pOld = pDC->SelectObject( &penGray );

	// draw historical moon images and orbital path
	const size_t nPoints = m_OrbitPoints.size();
	if ( nPoints != 0 )
	{
		pDC->Polyline( &m_OrbitPoints[ 0 ], (int)nPoints );
	}

	pDC->SelectObject( pOld );

} // RenderLunarOrbit

/////////////////////////////////////////////////////////////////////////////
// render the equations of motion
void CLunarOrbitView::RenderEquations( CDC * pDC )
{
	// pointer to the document information
	CLunarOrbitDoc* pDoc = Document;

	// margin around the document in logical pixels (device independent)
	const int nMargin = LogicalDocumentMargin;

	// text color is blue
	COLORREF rgbBlue( RGB( 0, 0, 255 ) );

	// 0.12 inch size
	const int nTextHeight = InchesToLogical( 0.25 );

	// create a font for text output
	CFont font;
	BuildFont
	(
		_T( "Arial" ), false, false, nTextHeight, false, font
	);

	// labels for information to be displayed on the output device
	CString csText
	(
		_T( "Newton's first equation of motion:\n \n" )
		_T( "    v = u + at\n \n" )
		_T( "Newton's second equation of motion:\n \n" )
		_T( "    s = ut + ½at²\n \n")
		_T( "where v is final velocity, u is initial velocity,\n" )
		_T( "    s is final position, a is acceleration, and t is time.")
	);
	
	// prepare the device context
	CFont* pOldFont = pDC->SelectObject( &font );
	const int nTA = pDC->SetTextAlign( TA_LEFT | TA_BASELINE );
	COLORREF rgbOld = pDC->SetTextColor( rgbBlue );

	// document dimensions
	const int nDocWidth = InchesToLogical( DocumentWidth );
	const int nDocHeight = InchesToLogical( DocumentHeight );

	// left justified to the left margin
	int nX = 2 * nMargin;
	int nY = 2 * nMargin;

	const CString csDelim( _T( "\n" ));
	int nStart = 0;

	// tokenize the string using line feeds as delimiters
	// and exit when the token is empty
	do
	{
		const CString csToken = csText.Tokenize( csDelim, nStart );
		if ( csToken.IsEmpty() )
		{
			break;
		}

		// draw the equation text one line at a time
		pDC->TextOut( nX, nY, csToken );

		// move to next line
		nY += nTextHeight;

	} while ( true );

	// restore the device context
	pDC->SetTextColor( rgbOld );
	pDC->SetTextAlign( nTA );
	pDC->SelectObject( pOldFont );

} // RenderEquations

/////////////////////////////////////////////////////////////////////////////
// render the text information
void CLunarOrbitView::RenderText( CDC * pDC )
{
	// pointer to the document information
	CLunarOrbitDoc* pDoc = Document;

	// margin around the document in logical pixels (device independent)
	const int nMargin = LogicalDocumentMargin;

	// 0.12 inch size
	const int nTextHeight = InchesToLogical( 0.25 );

	// text color
	COLORREF rgbText( RGB( 0, 128, 0 ));

	// create a font for text output
	CFont font;
	BuildFont
	(
		_T( "Arial" ), false, false, nTextHeight, false, font
	);

	// labels for information to be displayed on the output device
	CString 
		csMassOfEarth,
		csGravity, csGravityX, csGravityY,
		csVelocity, csVelocityX, csVelocityY,
		csDistance, csMoonX, csMoonY,
		csSample, csSamplesPerDay, csRunningTime, csAngle;
	
	csMassOfEarth.Format
	(
		_T( "Mass of the earth in kg: %g" ), 
		pDoc->MassOfTheEarth
	);
	csGravity.Format
	(
		_T( "Gravity in m/s²: %0.4e" ), 
		pDoc->AccelerationOfGravity
	);
	csGravityX.Format
	(
		_T( "X-gravity in m/s²: %0.4e" ),
		pDoc->GravityX
	);
	csGravityY.Format
	(
		_T( "Y-gravity in m/s²: %0.4e" ),
		pDoc->GravityY
	);
	csVelocity.Format
	(
		_T( "Initial velocity in m/s: %0.0f" ), Velocity
	);
	csVelocityX.Format
	(
		_T( "X-velocity in m/s: %0.0f" ),
		HorizontalVelocity
	);
	csVelocityY.Format
	(
		_T( "Y-velocity in m/s: %0.0f" ),
		VerticalVelocity
	);
	csDistance.Format
	(
		_T( "Distance to moon in m: %0.0f" ), MoonDistance
	);
	csMoonX.Format
	(
		_T( "X Distance to moon in m: %0.0f" ), pDoc->MoonX
	);
	csMoonY.Format
	(
		_T( "Y Distance to moon in m: %0.0f" ), pDoc->MoonY
	);
	csAngle.Format( _T( "Angle in degA: %0.02f" ), AngleInDegrees );
	csSample.Format
	(
		_T( "Time between samples in s: %0.0f" ), SampleTime
	);
	csSamplesPerDay.Format
	(
		_T( "Samples per day: %0.0f" ), pDoc->SamplesPerDay
	);

	// running time is in seconds, so divide by the number of seconds in a day
	// to display the running time in days
	csRunningTime.Format
	(
		_T( "Running time in days: %0.2f" ), pDoc->RunningTime / 86400
	);

	// prepare the device context
	CFont* pOldFont = pDC->SelectObject( &font );
	const int nTA = pDC->SetTextAlign( TA_RIGHT | TA_BASELINE );
	COLORREF rgbOld = pDC->SetTextColor( rgbText );

	// center of the earth on the document
	CPoint ptEarth = EarthCenter;

	// document dimensions
	const int nDocWidth = InchesToLogical( DocumentWidth );
	const int nDocHeight = InchesToLogical( DocumentHeight );

	// right justified to the right margin
	int nX = LogicalDocumentWidth - 2 * nMargin;
	int nY = 2 * nMargin;

	// draw the textual information one line at a time
	pDC->TextOut( nX, nY, csMassOfEarth );
	nY += nTextHeight;
	pDC->TextOut( nX, nY, csDistance );
	nY += nTextHeight;
	pDC->TextOut( nX, nY, csMoonX );
	nY += nTextHeight;
	pDC->TextOut( nX, nY, csMoonY );
	nY += nTextHeight;
	pDC->TextOut( nX, nY, csVelocity );
	nY += nTextHeight;
	pDC->TextOut( nX, nY, csVelocityX );
	nY += nTextHeight;
	pDC->TextOut( nX, nY, csVelocityY );
	nY += nTextHeight;
	pDC->TextOut( nX, nY, csGravity );
	nY += nTextHeight;
	pDC->TextOut( nX, nY, csGravityX );
	nY += nTextHeight;
	pDC->TextOut( nX, nY, csGravityY );
	nY += nTextHeight;
	pDC->TextOut( nX, nY, csAngle );
	nY += nTextHeight;
	pDC->TextOut( nX, nY, csSample );
	nY += nTextHeight;
	pDC->TextOut( nX, nY, csSamplesPerDay );
	nY += nTextHeight;
	pDC->TextOut( nX, nY, csRunningTime );
	nY += nTextHeight;

	// labels beside the grid
	COLORREF rgbScale( RGB( 255, 0, 0 ));
	const CString csLabelX( _T( "X (Inches)" ));
	const CString csLabelY( _T( "Y (Inches)" ) );

	pDC->SetTextColor( rgbScale );
	pDC->SetTextAlign( TA_CENTER | TA_BOTTOM );
	pDC->TextOut( ptEarth.x, nMargin, csLabelX );
	pDC->TextOut( ptEarth.x, nDocHeight - nMargin, csLabelX );
	
	// create a font for vertical text output
	CFont fontY;
	BuildFont
	(
		_T( "Arial" ), false, false, nTextHeight, true, fontY
	);
	pDC->SelectObject( &fontY );
	pDC->SetTextAlign( TA_CENTER | TA_BOTTOM );

	pDC->TextOut( nMargin - nTextHeight, ptEarth.y, csLabelY );
	pDC->TextOut( nDocWidth - nMargin, ptEarth.y, csLabelY );

	// restore the device context
	pDC->SetTextColor( rgbOld );
	pDC->SetTextAlign( nTA );
	pDC->SelectObject( pOldFont );

} // RenderText

/////////////////////////////////////////////////////////////////////////////
// render the grid
void CLunarOrbitView::RenderGrid( CDC * pDC )
{
	// gray color
	const COLORREF rgbGray = RGB( 128, 128, 128 );

	// 1 hundredths of an inch
	const int nGrayWidth = InchesToLogical( 0.01 );

	// gray pen
	CPen penGray;

	// create a solid gray pen 0.05 inches wide
	penGray.CreatePen( PS_SOLID, nGrayWidth, rgbGray );

	// setup the device context
	CPen* pOld = pDC->SelectObject( &penGray );

	// document dimensions in logical pixels (device independent)
	const int nDocWidth = InchesToLogical( DocumentWidth );
	const int nDocHeight = InchesToLogical( DocumentHeight );
	
	// center point of the earth
	CPoint ptEarth = EarthCenter;
	
	// margin around the document in logical pixels (device independent)
	// based on a 1/4" margin
	const int nMargin = LogicalDocumentMargin;

	// drawing limits
	int nX1 = ptEarth.x - nMargin * 20; // 20 margins left of center
	int nX2 = ptEarth.x + nMargin * 20; // 20 margins right of center
	int nY1 = ptEarth.y - nMargin * 16; // 16 margins above center
	int nY2 = ptEarth.y + nMargin * 16; // 16 margins below center

	// draw vertical grid lines every 4 margins (every inch)
	for ( int nX = nX1; nX <= nX2; nX += 4 * nMargin )
	{
		pDC->MoveTo( nX, nY1 );
		pDC->LineTo( nX, nY2 );
	}

	// draw horizontal grid lines every 4 margins (every inch)
	for ( int nY = nY1; nY <= nY2; nY += 4 * nMargin )
	{
		pDC->MoveTo( nX1, nY );
		pDC->LineTo( nX2, nY );
	}

	// restore the device context
	pDC->SelectObject( pOld );

} // RenderGrid

/////////////////////////////////////////////////////////////////////////////
// render the scale labels in inches
void CLunarOrbitView::RenderScale( CDC * pDC )
{
	// pointer to the document information
	CLunarOrbitDoc* pDoc = Document;

	// margin around the document in logical pixels (device independent)
	const int nMargin = LogicalDocumentMargin;

	// text color
	COLORREF rgbText( RGB( 255, 0, 0 ) );

	// 0.12 inch size
	const int nTextHeight = InchesToLogical( 0.18 );

	// create a font for text output
	CFont font;
	BuildFont
	(
		_T( "Arial" ), false, false, nTextHeight, false, font
	);

	// center of the earth on the document
	CPoint ptEarth = EarthCenter;

	// drawing limits
	int nX1 = ptEarth.x - nMargin * 20; // 20 margins left of center
	int nX2 = ptEarth.x + nMargin * 20; // 20 margins right of center
	int nY1 = ptEarth.y - nMargin * 16; // 16 margins above center
	int nY2 = ptEarth.y + nMargin * 16; // 16 margins below center

	// draw X scale labels every 4 margins (every inch)
	// prepare the device context
	CFont* pOldFont = pDC->SelectObject( &font );
	const int nTA = pDC->SetTextAlign( TA_CENTER | TA_TOP );
	COLORREF rgbOld = pDC->SetTextColor( rgbText );

	CString csValue;
	for ( int nX = nX1; nX <= nX2; nX += 4 * nMargin )
	{
		const double dValue = LogicalToInches( nX - 2 * nMargin );
		csValue.Format( _T( "%0.0f" ), dValue );
		pDC->TextOut( nX, nY2, csValue );
	}

	// draw Y scale labels every 4 margins (every inch)
	// create a font for vertical text output
	CFont fontY;
	BuildFont
	(
		_T( "Arial" ), false, false, nTextHeight, true, fontY
	);
	pDC->SelectObject( &fontY );
	pDC->SetTextAlign( TA_CENTER | TA_TOP );
	for ( int nY = nY1; nY <= nY2; nY += 4 * nMargin )
	{
		const double dValue = LogicalToInches( nY - nMargin );
		csValue.Format( _T( "%0.0f" ), dValue );
		pDC->TextOut( nX1, nY, csValue );
	}

	// restore the device context
	pDC->SetTextColor( rgbOld );
	pDC->SetTextAlign( nTA );
	pDC->SelectObject( pOldFont );

} // RenderScale

/////////////////////////////////////////////////////////////////////////////
// render the moon
void CLunarOrbitView::RenderMoon( CDC * pDC )
{
	// gray color
	const COLORREF rgbGray = RGB( 128, 128, 128 );

	// 1 hundredths of an inch
	const int nGrayWidth = InchesToLogical( 0.01 );

	// gray pen to draw the moon's circumference
	CPen penGray;

	// create a solid gray pen 0.05 inches wide
	penGray.CreatePen( PS_SOLID, nGrayWidth, rgbGray );

	// setup the device context
	CPen* pPenOld = pDC->SelectObject( &penGray );

	// a gray brush to fill the moon's interior
	CBrush brGray;

	// create a gray brush
	brGray.CreateSolidBrush( rgbGray );

	// color the moon gray
	CBrush* pBrOld = pDC->SelectObject( &brGray );

	// create a rectangle representing the moon 
	CRect rectMoon = MoonRectangle;

	// draw the moon as an ellipse that fits into the rectangle
	pDC->Ellipse( &rectMoon );

	CPoint ptMoon = MoonCenterRelativeToEarth;
	CString csCoor;
	csCoor.Format( _T( "%d,%d" ), ptMoon.x, ptMoon.y );
	CPoint ptCenter = MoonCenter;

	// 0.18 inch size
	const int nTextHeight = InchesToLogical( 0.18 );

	// create a font for text output
	CFont font;
	BuildFont
	(
		_T( "Arial" ), false, false, nTextHeight, false, font
	);

	// red color
	const COLORREF rgbRed = RGB( 255, 0, 0 );

	const COLORREF rgbOld = pDC->SetTextColor( rgbRed );
	const int nBM = pDC->SetBkMode( OPAQUE );
	const int nTA = pDC->SetTextAlign( DT_TOP | DT_LEFT );
	CFont* pOldFont = pDC->SelectObject( &font );

	pDC->TextOut( rectMoon.right, ptCenter.y, csCoor );

	// restore the device context
	pDC->SelectObject( pOldFont );
	pDC->SetTextColor( rgbOld );
	pDC->SetBkMode( nBM );
	pDC->SelectObject( pPenOld );
	pDC->SelectObject( pBrOld );

} // RenderMoon

/////////////////////////////////////////////////////////////////////////////
void CLunarOrbitView::RenderEarth( CDC * pDC )
{
	// 5 hundredth of an inch
	const int nBlueWidth = InchesToLogical( 0.05 );

	// green color
	const COLORREF rgbGreen = RGB( 0, 255, 0 );

	// blue color
	const COLORREF rgbBlue = RGB( 0, 0, 255 );

	// create a pen to draw the earth's circumference 
	CPen penBlue;

	// create a solid blue pen 0.05 inches wide
	penBlue.CreatePen( PS_SOLID, nBlueWidth, rgbBlue );

	// create a green brush to fill the earth's shape
	CBrush brGreen;

	// create a green brush
	brGreen.CreateSolidBrush( rgbGreen );

	// colors of the earth
	CPen* pPenOld = pDC->SelectObject( &penBlue );
	CBrush* pBrOld = pDC->SelectObject( &brGreen );

	// create a rectangle representing the earth 
	CRect rectEarth = EarthRectangle;

	// draw the earth as an ellipse that fits into the rectangle
	pDC->Ellipse( &rectEarth );

	// restore the device context
	pDC->SelectObject( pPenOld );
	pDC->SelectObject( pBrOld );

} // RenderEarth

/////////////////////////////////////////////////////////////////////////////
// render the earth / moon triangle - the earth's acceleration is 
// applied toward's the earth along the triangle's hypotenuse, but
// in order to calculate velocity and position of the moon that 
// gravitational attraction needs to be broken into a vertical and
// horizontal vector represented by the sides of the right triangle
void CLunarOrbitView::RenderTriangle( CDC* pDC )
{
	// pointer to the document information
	CLunarOrbitDoc* pDoc = Document;

	// 2 hundredths of an inch
	const int nRedWidth = InchesToLogical( 0.02 );

	// red color
	const COLORREF rgbRed = RGB( 255, 0, 0 );

	// create a pen to draw with
	CPen penRed;

	// create a solid red pen 0.02 inches wide
	penRed.CreatePen( PS_SOLID, nRedWidth, rgbRed );

	CPen* pPenOld = pDC->SelectObject( &penRed );

	// draw hypotenuse of triangle
	CPoint ptEarth = EarthCenter;

	// center point of the moon
	CPoint ptMoon = MoonCenter;

	// angle 
	const double dAngle = TextAngleInDegrees;

	// draw the hypotenuse of the right triangle (radius of orbit)
	pDC->MoveTo( ptMoon );
	pDC->LineTo( ptEarth );

	// draw the X vector
	pDC->LineTo( ptMoon.x, ptEarth.y );

	// draw the Y vector
	pDC->LineTo( ptMoon );

	// restore the device context
	pDC->SelectObject( pPenOld );

	// 0.18 inch size
	const int nTextHeight = InchesToLogical( 0.18 );

	const COLORREF rgbOld = pDC->SetTextColor( rgbRed );
	const int nTA = pDC->SetTextAlign( TA_CENTER | TA_BOTTOM );
	const int nBM = pDC->SetBkMode( OPAQUE );
	
	// create a font for text output
	CString csGravity, csGravityX, csGravityY;
	CFont fontG, fontX, fontY;
	BuildFont
	(
		_T( "Arial" ), false, false, nTextHeight, false, fontG, dAngle
	);
	csGravity.Format
	(
		_T( "Ag: %0.4e" ), pDoc->AccelerationOfGravity
	);
	BuildFont
	(
		_T( "Arial" ), false, false, nTextHeight, false, fontX
	);
	csGravityX.Format
	(
		_T( "Ax: %0.4e" ), pDoc->GravityX
	);
	BuildFont
	(
		_T( "Arial" ), false, false, nTextHeight, true, fontY
	);
	csGravityY.Format
	(
		_T( "Ay: %0.4e" ), pDoc->GravityY
	);

	// acceleration of gravity
	CFont* pOldFont = pDC->SelectObject( &fontG );
	const int Xg = ptEarth.x + ( ptMoon.x - ptEarth.x ) / 2;
	const int Yg = ptEarth.y + ( ptMoon.y - ptEarth.y ) / 2;
	pDC->TextOut( Xg, Yg, csGravity );

	// x vector acceleration of gravity
	pDC->SelectObject( &fontX );
	const int Xx = ptEarth.x + ( ptMoon.x - ptEarth.x ) / 2;
	const int Yx = ptEarth.y;
	pDC->TextOut( Xx, Yx, csGravityX );

	// y vector acceleration of gravity
	pDC->SelectObject( &fontY );
	const int Xy = ptMoon.x;
	const int Yy = ptMoon.y + ( ptEarth.y - ptMoon.y ) / 2;
	pDC->SetTextAlign( TA_CENTER | TA_BOTTOM );
	pDC->TextOut( Xy, Yy, csGravityY );

	// restore the device context
	pDC->SelectObject( pOldFont );
	pDC->SetBkMode( nBM );
	pDC->SetTextAlign( nTA );
	pDC->SetTextColor( rgbOld );

} // RenderTriangle

/////////////////////////////////////////////////////////////////////////////
// render the page or view
void CLunarOrbitView::render
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

	// draw the lunar orbit up to this point in time
	RenderLunarOrbit( pDC );

	// render Newton's equations of motion
	RenderEquations( pDC );

	// draw the textual information 
	RenderText( pDC );

	// draw the document grid
	RenderGrid( pDC );

	// draw the X and Y scale labels
	RenderScale( pDC );

	// draw the moon's shape
	RenderMoon( pDC );

	// draw the earth's shape
	RenderEarth( pDC );

	// draw the earth moon triangle 
	RenderTriangle( pDC );

} // render

/////////////////////////////////////////////////////////////////////////////
// add the current moon position to the historical points of the lunar orbit
void CLunarOrbitView::AddOrbitalPoint()
{
	CLunarOrbitDoc* pDoc = Document;

	CRect rectMoon = MoonRectangle;

	CPoint pt = rectMoon.CenterPoint();

	m_OrbitPoints.push_back( pt );

} // AddOrbitalPoint

/////////////////////////////////////////////////////////////////////////////
// this routine will update the moon's position using time slices
// equal to SampleTime.
void CLunarOrbitView::UpdateMoonPosition()
{
	CLunarOrbitDoc* pDoc = Document;

	// 27 days in seconds
	const double dSeconds = 27 * 86400;

	// starting positions
	double dX = pDoc->MoonX;
	double dY = pDoc->MoonY;

	// starting velocity
	double dVx = pDoc->VelocityX;
	double dVy = pDoc->VelocityY;

	// acceleration of earth's gravity on the moon is
	// calculated using Newton's equation
	const double dA = pDoc->AccelerationOfGravity;

	// starting acceleration of gravity
	double dAx = pDoc->GravityX;
	double dAy = pDoc->GravityY;

	// the time the model has been run in seconds
	double dTime = pDoc->RunningTime;

	// mass of the earth in kilograms
	const double dM = pDoc->MassOfTheEarth;

	// the distance to the moon in meters
	const double dR = MoonDistance;

	// the number of time slices the day is divided into
	const int nSamplesPerDay = (int)pDoc->SamplesPerDay;

	// samples per hour
	const int nSamplesPerHour = nSamplesPerDay / 24;

	// the length of a time slice in seconds
	const double dSt = pDoc->SampleTime;

	// are we done with a complete cycle
	bool bDone = false;

	// loop through the time slices and update positions and velocities
	// using Newton's equations of motion
	for ( int nSample = 0; nSample < nSamplesPerHour; nSample++ )
	{
		// the acceleration of gravity in the X direction using 
		// right triangle proportion
		const double dNewAx = -dA * dX / dR;

		// the acceleration of gravity in the Y direction using 
		// right triangle proportion
		const double dNewAy = -dA * dY / dR;

		// the new X velocity in the X direction is the original X velocity
		// plus X acceleration multiplied by the time increment
		const double dNewVx = dVx + dNewAx * dSt;

		// the new Y velocity in the Y direction is the original Y velocity
		// plus Y acceleration multiplied by the time increment
		const double dNewVy = dVy + dNewAy * dSt;

		// the new X position is the original X position plus the X velocity
		// multiplied by the time increment
		const double dNewX = dX + dNewVx * dSt;

		// the new Y position is the original Y position plus the Y velocity
		// multiplied by the time increment
		const double dNewY = dY + dNewVy * dSt;

		// are we doing a single orbit?
		const bool bSingleOrbit = SingleOrbit;

		// if we are doing a single orbit and we have
		// exceeded 27 days, start testing for the end
		// of the orbit (so the testing only is done when
		// we are close to the expected result)
		if ( bSingleOrbit && dTime > dSeconds )
		{
			// difference between the previous X and the new one
			const double dDelta = dNewX - dX;

			// if the delta is negative, we have reached the beginning
			// of the orbit
			if ( dDelta < 0 )
			{
				KillTimer( 1 );
				Running = false;
				bDone = true;
				break;
			}
		}

		// update the current acceleration, velocity and 
		// position for the next time slice
		dAx = dNewAx;
		dAy = dNewAy;
		dVx = dNewVx;
		dVy = dNewVy;
		dX = dNewX;
		dY = dNewY;

		// update the running time
		dTime += dSt;
	}

	// record the final vector results into the document
	pDoc->MoonX = dX; // X distance
	pDoc->MoonY = dY; // Y distance
	pDoc->VelocityX = dVx; // X velocity
	pDoc->VelocityY = dVy; // Y velocity
	pDoc->GravityX = dAx; // X gravity
	pDoc->GravityY = dAy; // Y gravity
	pDoc->RunningTime = dTime;

	// keep track of orbital points
	AddOrbitalPoint();

	// if we are done, repaint the view
	if ( bDone )
	{
		Invalidate();
	}

} // UpdateMoonPosition

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
	// update the moon's position and velocity
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
	OnEditRun();

} // OnEditPause

/////////////////////////////////////////////////////////////////////////////
// the pause tool bar UI handler (check and gray out)
void CLunarOrbitView::OnUpdateEditPause( CCmdUI *pCmdUI )
{
	const bool bRunning = Running;
	pCmdUI->Enable( TRUE );
	if ( bRunning )
	{
		pCmdUI->SetCheck( FALSE );
	}
	else // paused
	{
		pCmdUI->SetCheck( TRUE );
	}
} // OnUpdateEditPause

/////////////////////////////////////////////////////////////////////////////
// the run tool bar button handler changes the running flag and starts
// the timer
void CLunarOrbitView::OnEditRun()
{
	const bool bRunning = Running;
	if ( bRunning )
	{
		Running = false;
		KillTimer( 1 );
	}
	else // paused
	{
		SetTimer( 1, 10, nullptr );
		Running = true;
	}

} // OnEditRun

/////////////////////////////////////////////////////////////////////////////
// the run tool bar UI handler (check and gray out)
void CLunarOrbitView::OnUpdateEditRun( CCmdUI *pCmdUI )
{
	const bool bRunning = Running;
	pCmdUI->Enable( TRUE );
	if ( bRunning )
	{
		pCmdUI->SetCheck( TRUE );
	}
	else // paused
	{
		pCmdUI->SetCheck( FALSE );
	}
} // OnUpdateEditRun

/////////////////////////////////////////////////////////////////////////////
// toggle single orbit flag
void CLunarOrbitView::OnEditSingleOrbit()
{
	const bool bSingleOrbit = SingleOrbit;
	if ( bSingleOrbit )
	{
		SingleOrbit = false;
	}
	else // false
	{
		SingleOrbit = true;
	}
}

/////////////////////////////////////////////////////////////////////////////
// single orbit UI handler to check / uncheck the menu item
void CLunarOrbitView::OnUpdateEditSingleorbit( CCmdUI *pCmdUI )
{
	const bool bSingleOrbit = SingleOrbit;
	pCmdUI->SetCheck( bSingleOrbit == true );
}

/////////////////////////////////////////////////////////////////////////////
