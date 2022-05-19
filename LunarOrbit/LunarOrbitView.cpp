
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
IMPLEMENT_DYNCREATE(CLunarOrbitView, CBaseView)

/////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CLunarOrbitView, CBaseView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CBaseView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CBaseView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CLunarOrbitView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_TIMER()
	ON_COMMAND( ID_EDIT_PAUSE, &CLunarOrbitView::OnEditPause )
	ON_UPDATE_COMMAND_UI( ID_EDIT_PAUSE, &CLunarOrbitView::OnUpdateEditPause )
	ON_COMMAND( ID_EDIT_RUN, &CLunarOrbitView::OnEditRun )
	ON_UPDATE_COMMAND_UI( ID_EDIT_RUN, &CLunarOrbitView::OnUpdateEditRun )
	ON_COMMAND( ID_EDIT_SINGLEORBIT, &CLunarOrbitView::OnEditSingleOrbit )
	ON_UPDATE_COMMAND_UI( ID_EDIT_SINGLEORBIT, &CLunarOrbitView::OnUpdateEditSingleorbit )
	ON_COMMAND( ID_EDIT_30DEGSTEPS, &CLunarOrbitView::OnEdit30DegSteps )
	ON_UPDATE_COMMAND_UI( ID_EDIT_30DEGSTEPS, &CLunarOrbitView::OnUpdateEdit30DegSteps )
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
CLunarOrbitView::CLunarOrbitView()
{
	Running = false;
	SingleOrbit = false;
	ThirtyDegreeSteps = false;
	TopOfView = 0;
	AngleError = 0.01; // tenth of a degree
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

	return CBaseView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// render the page or view
void CLunarOrbitView::render
(
	CDC* pDC, double dLeftOfView, double dTopOfView
)
{
	CBaseView::render( pDC, dLeftOfView, dTopOfView );

	// draw the lunar orbit up to this point in time
	RenderLunarOrbit( pDC );

	// render Newton's equations of motion
	RenderEquations( pDC );

	// render the initial condition text
	RenderInitialConditions( pDC );

	// render the distance text information
	RenderDistanceText( pDC );

	// render the gravity text information
	RenderGravityText( pDC );

	// render the velocity text information
	RenderVelocityText( pDC );

	// render the grid labels
	RenderGridLabels( pDC );

	//// draw the textual information 
	//RenderText( pDC );

	// draw the document grid
	RenderGrid( pDC );

	// draw the X and Y scale labels
	RenderScale( pDC );

	// draw the moon's shape
	RenderMoon( pDC );

	// draw the earth's shape
	RenderEarth( pDC );

	// render the distance vector
	RenderDistance( pDC );

	// render the acceleration vector
	RenderAcceleration( pDC );

	// render the velocity vector
	RenderVelocity( pDC );

} // render

/////////////////////////////////////////////////////////////////////////////
void CLunarOrbitView::OnDraw( CDC* pDC )
{
	CLunarOrbitDoc* pDoc = Document;
	ASSERT_VALID( pDoc );
	if ( !pDoc )
		return;

	CBaseView::OnDraw( pDC );

} // OnDraw

/////////////////////////////////////////////////////////////////////////////
void CLunarOrbitView::OnInitialUpdate()
{
	CBaseView::OnInitialUpdate();

}

/////////////////////////////////////////////////////////////////////////////
void CLunarOrbitView::OnFilePrintPreview()
{
	AFXPrintPreview(this);
}

/////////////////////////////////////////////////////////////////////////////
BOOL CLunarOrbitView::OnPreparePrinting(CPrintInfo* pInfo)
{
	return CBaseView::DoPreparePrinting(pInfo);
}

/////////////////////////////////////////////////////////////////////////////
void CLunarOrbitView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	CBaseView::OnBeginPrinting( pDC, pInfo );
}

/////////////////////////////////////////////////////////////////////////////
void CLunarOrbitView::OnPrint( CDC* pDC, CPrintInfo* pInfo )
{
	CBaseView::OnPrint( pDC, pInfo );

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
	CBaseView::AssertValid();
}

/////////////////////////////////////////////////////////////////////////////
void CLunarOrbitView::Dump(CDumpContext& dc) const
{
	CBaseView::Dump(dc);
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
	CBaseView::OnPrepareDC( pDC, pInfo );

} // OnPrepareDC

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

	// text color is orange
	COLORREF rgbBlue( RGB( 255, 127, 0 ) );

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
		_T( "Newton's Second Law of Universal Gravitation:\n \n" )
		_T( "    a = GM/r�\n \n" )
		_T( "Newton's first equation of motion:\n \n" )
		_T( "    v = u + at\n \n" )
		_T( "Newton's second equation of motion:\n \n" )
		_T( "    s = ut + 폸t�\n \n")
		_T( "where:\n \n" ) 
		_T( "    a is acceleration,\n")
		_T( "    G is the universal gravitational constant,\n")
		_T( "    M is mass,\n")
		_T( "    r is distance,\n" )
		_T( "    v is final velocity,\n" )
		_T( "    u is initial velocity,\n" )
		_T( "    s is final position, and\n")
		_T( "    t is time." )
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
// render the initial condition text in dark green on the top right
void CLunarOrbitView::RenderInitialConditions( CDC * pDC )
{
	// pointer to the document information
	CLunarOrbitDoc* pDoc = Document;

	// margin around the document in logical pixels (device independent)
	const int nMargin = LogicalDocumentMargin;

	// 0.12 inch size
	const int nTextHeight = InchesToLogical( 0.25 );

	// text color is dark green
	COLORREF rgbText( RGB( 0, 128, 0 ) );

	// create a font for text output
	CFont font;
	BuildFont
	(
		_T( "Arial" ), false, false, nTextHeight, false, font
	);

	// labels for information to be displayed on the output device
	CString
		csMassOfEarth, csSample, csSamplesPerDay, csRunningTime;

	csMassOfEarth.Format
	(
		_T( "Me=%g kg" ), pDoc->MassOfTheEarth
	);
	csSample.Format
	(
		_T( "Time slice=%0.1f s" ), SampleTime
	);
	csSamplesPerDay.Format
	(
		_T( "%0.0f samples/day" ), pDoc->SamplesPerDay
	);

	// running time is in seconds, so divide by the number of seconds in a day
	// to display the running time in days
	csRunningTime.Format
	(
		_T( "Period=%0.2f days" ), pDoc->RunningTime / 86400
	);

	// prepare the device context
	CFont* pOldFont = pDC->SelectObject( &font );
	const int nTA = pDC->SetTextAlign( TA_RIGHT | TA_BASELINE );
	COLORREF rgbOld = pDC->SetTextColor( rgbText );

	// right justified to the right margin
	int nX = LogicalDocumentWidth - 2 * nMargin;
	int nY = 2 * nMargin;

	// draw the textual information one line at a time
	pDC->TextOut( nX, nY, csMassOfEarth );
	nY += nTextHeight;
	pDC->TextOut( nX, nY, csSample );
	nY += nTextHeight;
	pDC->TextOut( nX, nY, csSamplesPerDay );
	nY += nTextHeight;
	pDC->TextOut( nX, nY, csRunningTime );

	// restore the device context
	pDC->SetTextColor( rgbOld );
	pDC->SetTextAlign( nTA );
	pDC->SelectObject( pOldFont );

} // RenderInitialConditions

/////////////////////////////////////////////////////////////////////////////
// render the distance text information
void CLunarOrbitView::RenderDistanceText( CDC* pDC )
{
	// 0.12 inch size
	const int nTextHeight = InchesToLogical( 0.25 );

	// create a font for text output
	CFont font;
	BuildFont
	(
		_T( "Arial" ), false, false, nTextHeight, false, font
	);

	const CString csDistance = DistanceVector.Label;
	const CString csDistanceX = DistanceX.Label;
	const CString csDistanceY = DistanceY.Label;
	CString csAngle;
	csAngle.Format( _T( "S angle=%0.02f �" ), DistanceVector.Degrees );

	// prepare the device context
	CFont* pOldFont = pDC->SelectObject( &font );
	const int nTA = pDC->SetTextAlign( TA_RIGHT | TA_BASELINE );
	COLORREF rgbOld = pDC->SetTextColor( DistanceVector.Color );

	// document dimensions
	const int nDocWidth = InchesToLogical( DocumentWidth );
	const int nDocHeight = InchesToLogical( DocumentHeight );

	// margin around the document in logical pixels (device independent)
	const int nMargin = LogicalDocumentMargin;

	// right justified to these coordinates
	int nX = 10 * nMargin;
	int nY = nDocHeight - 6 * nMargin;

	pDC->TextOut( nX, nY, csDistance );
	nY += nTextHeight;
	pDC->TextOut( nX, nY, csDistanceX );
	nY += nTextHeight;
	pDC->TextOut( nX, nY, csDistanceY );
	nY += nTextHeight;
	pDC->TextOut( nX, nY, csAngle );

	// restore the device context
	pDC->SetTextColor( rgbOld );
	pDC->SetTextAlign( nTA );
	pDC->SelectObject( pOldFont );

} // RenderDistanceText

/////////////////////////////////////////////////////////////////////////////
// render the gravity text information
void CLunarOrbitView::RenderGravityText( CDC* pDC )
{
	// 0.12 inch size
	const int nTextHeight = InchesToLogical( 0.25 );

	// create a font for text output
	CFont font;
	BuildFont
	(
		_T( "Arial" ), false, false, nTextHeight, false, font
	);

	const CString csGravity = GravityVector.Label;
	const CString csGravityX = GravityX.Label;
	const CString csGravityY = GravityY.Label;
	CString csAngle;
	csAngle.Format( _T( "Ag angle=%0.02f �" ), GravityVector.Degrees );


	// prepare the device context
	CFont* pOldFont = pDC->SelectObject( &font );
	const int nTA = pDC->SetTextAlign( TA_RIGHT | TA_BASELINE );
	COLORREF rgbOld = pDC->SetTextColor( GravityVector.Color );

	// document dimensions
	const int nDocWidth = InchesToLogical( DocumentWidth );
	const int nDocHeight = InchesToLogical( DocumentHeight );

	// margin around the document in logical pixels (device independent)
	const int nMargin = LogicalDocumentMargin;

	// right justified to these coordinates
	int nX = 26 * nMargin;
	int nY = nDocHeight - 6 * nMargin;

	pDC->SetTextColor( GravityVector.Color );
	pDC->TextOut( nX, nY, csGravity );
	nY += nTextHeight;
	pDC->TextOut( nX, nY, csGravityX );
	nY += nTextHeight;
	pDC->TextOut( nX, nY, csGravityY );
	nY += nTextHeight;
	pDC->TextOut( nX, nY, csAngle );

	// restore the device context
	pDC->SetTextColor( rgbOld );
	pDC->SetTextAlign( nTA );
	pDC->SelectObject( pOldFont );
} // RenderGravityText

/////////////////////////////////////////////////////////////////////////////
// render the velocity text information
void CLunarOrbitView::RenderVelocityText( CDC* pDC )
{
	// 0.12 inch size
	const int nTextHeight = InchesToLogical( 0.25 );

	// create a font for text output
	CFont font;
	BuildFont
	(
		_T( "Arial" ), false, false, nTextHeight, false, font
	);

	const CString csVelocity = VelocityVector.Label;
	const CString csVelocityX = VelocityX.Label;
	const CString csVelocityY = VelocityY.Label;
	CString csAngle;
	csAngle.Format( _T( "Vg angle=%0.02f �" ), VelocityVector.Degrees );

	// prepare the device context
	CFont* pOldFont = pDC->SelectObject( &font );
	const int nTA = pDC->SetTextAlign( TA_RIGHT | TA_BASELINE );
	COLORREF rgbOld = pDC->SetTextColor( VelocityVector.Color );

	// document dimensions
	const int nDocWidth = InchesToLogical( DocumentWidth );
	const int nDocHeight = InchesToLogical( DocumentHeight );

	// margin around the document in logical pixels (device independent)
	const int nMargin = LogicalDocumentMargin;

	// right justified to the right margin
	int nX = LogicalDocumentWidth - 2 * nMargin;
	int nY = nDocHeight - 6 * nMargin;

	pDC->TextOut( nX, nY, csVelocity );
	nY += nTextHeight;
	pDC->TextOut( nX, nY, csVelocityX );
	nY += nTextHeight;
	pDC->TextOut( nX, nY, csVelocityY );
	nY += nTextHeight;
	pDC->TextOut( nX, nY, csAngle );

	// restore the device context
	pDC->SetTextColor( rgbOld );
	pDC->SetTextAlign( nTA );
	pDC->SelectObject( pOldFont );
} // RenderVelocityText

/////////////////////////////////////////////////////////////////////////////
// render the grid labels
void CLunarOrbitView::RenderGridLabels( CDC* pDC )
{
	// pointer to the document information
	CLunarOrbitDoc* pDoc = Document;

	// margin around the document in logical pixels (device independent)
	const int nMargin = LogicalDocumentMargin;

	// 0.12 inch size
	const int nTextHeight = InchesToLogical( 0.25 );

	// create a font for text output
	CFont font;
	BuildFont
	(
		_T( "Arial" ), false, false, nTextHeight, false, font
	);

	// center of the earth on the document
	CPoint ptEarth = EarthCenter;

	// prepare the device context
	CFont* pOldFont = pDC->SelectObject( &font );

	// document dimensions
	const int nDocWidth = InchesToLogical( DocumentWidth );
	const int nDocHeight = InchesToLogical( DocumentHeight );

	// right justified to the right margin
	int nX = LogicalDocumentWidth - 2 * nMargin;
	int nY = 2 * nMargin;

	// labels beside the grid
	COLORREF rgbScale( RGB( 255, 0, 0 ) );
	const CString csLabelX( _T( "X (Inches)" ) );
	const CString csLabelY( _T( "Y (Inches)" ) );

	COLORREF rgbOld = pDC->SetTextColor( rgbScale );
	int nTA = pDC->SetTextAlign( TA_CENTER | TA_BOTTOM );
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
} // RenderGridLabels

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

	// the coordinates represent the distance to earth from the moon
	// in logical coordinates
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

	// dark cyan color
	const COLORREF rgbCyan = RGB( 0, 128, 128 );

	const COLORREF rgbOld = pDC->SetTextColor( rgbCyan );
	const int nBM = pDC->SetBkMode( TRANSPARENT );
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
// draw the distance vector centered on the moon
void CLunarOrbitView::RenderDistance( CDC * pDC )
{
	// draw all three vectors
	DistanceVector.Draw( pDC );
	DistanceX.Draw( pDC );
	DistanceY.Draw( pDC );

} // RenderDistance

/////////////////////////////////////////////////////////////////////////////
// draw the acceleration vector centered on the moon
void CLunarOrbitView::RenderAcceleration( CDC * pDC )
{
	// draw all three vectors
	GravityVector.Draw( pDC );
	GravityX.Draw( pDC );
	GravityY.Draw( pDC );

} // RenderAcceleration

/////////////////////////////////////////////////////////////////////////////
// render velocity vector
void CLunarOrbitView::RenderVelocity( CDC * pDC )
{
	// draw all three vectors
	VelocityVector.Draw( pDC );
	VelocityX.Draw( pDC );
	VelocityY.Draw( pDC );

} // RenderVelocity

/////////////////////////////////////////////////////////////////////////////
// add the current moon position to the historical points of the lunar orbit
void CLunarOrbitView::AddOrbitalPoint()
{
	CLunarOrbitDoc* pDoc = Document;

	// number of points in the current orbit
	const int nPoints = (int)m_OrbitPoints.size();

	// number of hours in the known lunar period is the number of seconds
	// divided by the number of seconds in an hour
	const int nHours = int( pDoc->LunarPeriod / 3600 );

	// since we are adding one point per hour, we can stop after one orbit's
	// worth of hours
	if ( nPoints < nHours )
	{
		CPoint pt = MoonCenter;

		m_OrbitPoints.push_back( pt );
	}

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
	double dVx = pDoc->LunarVelocityX;
	double dVy = pDoc->LunarVelocityY;

	// acceleration of earth's gravity on the moon is
	// calculated using Newton's equation
	const double dA = pDoc->AccelerationOfGravity;

	// starting acceleration of gravity
	double dAx = pDoc->LunarGravityX;
	double dAy = pDoc->LunarGravityY;

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

	double dOldAngle = DistanceVector.Degrees;

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
		const double dNewVx = dVx + dAx * dSt;

		// the new Y velocity in the Y direction is the original Y velocity
		// plus Y acceleration multiplied by the time increment
		const double dNewVy = dVy + dAy * dSt;

		// the new X position is the original X position plus the X velocity
		// multiplied by the time increment
		const double dNewX = dX + dVx * dSt;

		// the new Y position is the original Y position plus the Y velocity
		// multiplied by the time increment
		const double dNewY = dY + dVy * dSt;

		// test the current angle of the gravity vector if doing
		// 30 degree steps
		const bool bThirtyDegreeSteps = ThirtyDegreeSteps;

		// do not begin testing for an hour so we are not stopped on the
		// initial reading
		if ( bThirtyDegreeSteps && dTime > 3600 )
		{
			pDoc->MoonX = dNewX;
			pDoc->MoonY = dNewY;

			DistanceVector.FirstPoint = MoonCenter;
			const double dAngle = DistanceVector.Degrees;

			const double dDelta = dOldAngle - dAngle;
			dOldAngle = dAngle;
			const double dMod = fmod( dAngle, 30.0 );

			const double dAngleError = AngleError;
			const bool bThirty = 
				CHelper::NearlyEqual( dMod, 0.0, dAngleError );
			if ( bThirty )
			{
				KillTimer( 1 );
				Running = false;
				bDone = true;
			}
		}

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

			// if the delta is positive, we have reached the beginning
			// of the orbit
			if ( dDelta > 0 )
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
	pDoc->LunarVelocityX = dVx; // X velocity
	pDoc->LunarVelocityY = dVy; // Y velocity
	pDoc->LunarGravityX = dAx; // X gravity
	pDoc->LunarGravityY = dAy; // Y gravity
	pDoc->RunningTime = dTime;

	// update the distance vector with the moon's new position
	DistanceVector.FirstPoint = MoonCenter;
	
	// X and y components of the moon's distance to the earth
	DistanceX = DistanceVector.VectorX;
	DistanceX.Description = _T( "Sx" );
	DistanceY = DistanceVector.VectorY;
	DistanceY.Description = _T( "Sy" );

	// update the gravity vector with the moon's new position
	GravityVector.FirstPoint = MoonCenter;
	GravityVector.SecondPoint = EarthCenter;

	// set the vector's length to two inches while keeping the angle the same
	const double dGravityLength = InchesToLogical( 2.0 );
	GravityVector.Length = dGravityLength;

	// X and y components of gravity
	GravityX = GravityVector.VectorX;
	GravityX.Description = _T( "Ax" );
	GravityY = GravityVector.VectorY;
	GravityY.Description = _T( "Ay" );

	// copy the velocity vector location from the acceleration vector
	VelocityVector.FirstPoint = GravityVector.FirstPoint;
	VelocityVector.SecondPoint = GravityVector.SecondPoint;

	// set the vector's length to one inch while keeping the angle the
	// same as the acceleration vector
	const double dVelocityLength = InchesToLogical( 1.0 );
	VelocityVector.Length = dVelocityLength;

	// the velocity is 90 degrees out of phase with the acceleration
	// (rotation around the earth is counter-clockwise when looking down 
	// toward the north pole of the earth)
	VelocityVector.RotateAroundFirstPoint( -90 );
	VelocityX = VelocityVector.VectorX;
	VelocityX.Description = _T( "Vx" );
	VelocityY = VelocityVector.VectorY;
	VelocityY.Description = _T( "Vy" );

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
	return CBaseView::OnEraseBkgnd( pDC );

} // OnEraseBkgnd

/////////////////////////////////////////////////////////////////////////////
void CLunarOrbitView::OnVScroll( UINT nSBCode, UINT nPos, CScrollBar* pScrollBar )
{
	CLunarOrbitDoc* pDoc = Document;
	ASSERT_VALID( pDoc );

	CBaseView::OnVScroll( nSBCode, nPos, pScrollBar );

} // OnVScroll

/////////////////////////////////////////////////////////////////////////////
void CLunarOrbitView::OnTimer( UINT_PTR nIDEvent )
{
	// update the moon's position and velocity
	UpdateMoonPosition();

	// redraw the view
	Invalidate();

	CBaseView::OnTimer( nIDEvent );
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
	const bool value = SingleOrbit;
	if ( value )
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
	const bool value = SingleOrbit;
	pCmdUI->SetCheck( value == true );
}

/////////////////////////////////////////////////////////////////////////////
void CLunarOrbitView::OnEdit30DegSteps()
{
	const bool value = ThirtyDegreeSteps;
	if ( value )
	{
		ThirtyDegreeSteps = false;
	}
	else // false
	{
		ThirtyDegreeSteps = true;
	}
}

/////////////////////////////////////////////////////////////////////////////
void CLunarOrbitView::OnUpdateEdit30DegSteps( CCmdUI *pCmdUI )
{
	const bool value = ThirtyDegreeSteps;
	pCmdUI->SetCheck( value == true );
}

/////////////////////////////////////////////////////////////////////////////
