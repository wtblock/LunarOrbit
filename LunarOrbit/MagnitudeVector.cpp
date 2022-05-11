/////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2022 by W. T. Block, All Rights Reserved
/////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "MagnitudeVector.h"
#include <vector>

using namespace std;

/////////////////////////////////////////////////////////////////////////////
// returns a line representing one side of the arrowhead
// drawn at a positive or negative angle offset from the vector's
// angle depending on the parameter value given
CLinear CMagnitudeVector::CreateArrowhead( bool bPositiveAngle )
{
	// arrowhead length in inches
	const double dLength = ArrowheadLength;

	// arrowhead length in logical units
	const int nLength = InchesToLogical( dLength );

	// angle of the vector
	double dAngle = Degrees;

	// angle of the arrowhead with respect to the vector
	const double dArrowheadAngle = ArrowheadAngle;

	// create a horizontal arrowhead line
	CLinear value( X2, Y2, X2 - nLength, Y2 );

	// calculate the angle to rotate the arrowhead to
	dAngle += bPositiveAngle ? dArrowheadAngle : -dArrowheadAngle;

	// do the rotation around the first point of the arrowhead which
	// is actually the second point of the vector which is where the
	// arrowhead will be drawn
	value.RotateAroundFirstPoint( dAngle );

	return value;
} // CreateArrowhead

/////////////////////////////////////////////////////////////////////////////
// generate font characteristics from given properties
void CMagnitudeVector::BuildFont
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
// draw the vector on given device context
void CMagnitudeVector::DrawVector( CDC * pDC )
{
	CLinear lineArrowheadPos = CreateArrowhead( true );
	CLinear lineArrowheadNeg = CreateArrowhead( false );
	const int nThick = InchesToLogical( Thickness );

	// create the pen
	CPen pen;
	pen.CreatePen( PS_SOLID, nThick, Color );

	// select the pen into the device context
	CPen* pPenOld = pDC->SelectObject( &pen );

	// create the brush
	CBrush br;
	br.CreateSolidBrush( Color );

	// select the brush into the device context
	CBrush* pBrOld = pDC->SelectObject( &br );

	// get the vector points
	CPoint pt1 = FirstPoint;
	CPoint pt2 = SecondPoint;

	// draw the vector
	pDC->MoveTo( pt1 );
	pDC->LineTo( pt2 );

	// vector to contain the arrowhead polygon
	vector<CPoint> arrow;
	arrow.push_back( pt2 );
	arrow.push_back( lineArrowheadPos.SecondPoint );
	arrow.push_back( lineArrowheadNeg.SecondPoint );
	arrow.push_back( pt2 );

	// draw the arrowhead as a polygon
	pDC->Polygon( &arrow[ 0 ], (int)arrow.size());

	// restore the device context
	pDC->SelectObject( pBrOld );
	pDC->SelectObject( pPenOld );

} // DrawVector

/////////////////////////////////////////////////////////////////////////////
// draw the magnitude on given device context
void CMagnitudeVector::DrawMagnitude( CDC * pDC )
{
	// text height in logical units
	const int nTextHeight = InchesToLogical( TextHeight );

	// color of the text
	const COLORREF rgbOld = pDC->SetTextColor( Color );

	// text alignment centered on the text bottom
	const int nTA = pDC->SetTextAlign( TA_CENTER | TA_BOTTOM );

	// background of text is transparent
	const int nBM = pDC->SetBkMode( TRANSPARENT );

	// angle of the vector in degrees
	const double dAngle = Degrees;

	// create a font that is rotated the same as the vector
	CFont font;

	// note the last parameter is the font's equivalent of the vector's
	// UpIncrement and it is being set as positive one to prevent reversing
	// the CVector::UpIncrement property
	BuildFont
	(
		_T( "Arial" ), false, false, nTextHeight, false, font, dAngle,
		ANSI_CHARSET, false, false, 1
	);

	// select the font into the device context 
	CFont* pFontOld = pDC->SelectObject( &font );
	
	// first and second points of the vector
	CPoint pt1 = FirstPoint;
	CPoint pt2 = SecondPoint;

	// calculate the mid-point of the vector
	const int nX = pt1.x + ( pt2.x - pt1.x ) / 2;
	const int nY = pt1.y + ( pt2.y - pt1.y ) / 2;

	// draw the text label centered at the mid-point of the vector
	const CString csText = Description;
	pDC->TextOut( nX, nY, csText );

	// restore the device context to its original state
	pDC->SelectObject( pFontOld );
	pDC->SetBkMode( nBM );
	pDC->SetTextAlign( nTA );
	pDC->SetTextColor( rgbOld );

} // DrawMagnitude

/////////////////////////////////////////////////////////////////////////////
// draw on given device context
void CMagnitudeVector::Draw( CDC * pDC )
{
	DrawVector( pDC );
	DrawMagnitude( pDC );

} // Draw

/////////////////////////////////////////////////////////////////////////////
