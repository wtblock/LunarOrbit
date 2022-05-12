
/////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2022 by W. T. Block, All Rights Reserved
/////////////////////////////////////////////////////////////////////////////

#pragma once
#include <vector>
#include <algorithm>

using namespace std;

/////////////////////////////////////////////////////////////////////////////
class CLunarOrbitView : public CScrollView
{
protected: // create from serialization only
	CLunarOrbitView();
	DECLARE_DYNCREATE(CLunarOrbitView)

	CSize m_sizeClient;	// physical size of client area
	double m_dTop; // top of view in inches

	int m_nPhysicalPageWidth;
	int m_nPhysicalPageHeight;
	int m_nLogicalPageWidth;
	int m_nLogicalPageHeight;
	bool m_bInitialUpdate;
	int m_nNumPages;
	bool m_bRunning;
	bool m_bSingleOrbit;
	bool m_bThirtyDegreeSteps;
	double m_dAngleError;
	vector<CPoint> m_OrbitPoints;

	// properties
public:
	// pointer to the document class
	CLunarOrbitDoc* GetDocument() const;
	// pointer to the document class
	__declspec( property( get = GetDocument ) )
		CLunarOrbitDoc* Document;

	// logical pixels per inch
	int GetMap()
	{
		CLunarOrbitDoc* pDoc = Document;
		const int value = pDoc->Map;
	}
	// logical pixels per inch
	__declspec( property( get = GetMap ) )
		int Map;

	// height of the document in inches
	double GetDocumentHeight()
	{
		CLunarOrbitDoc* pDoc = Document;
		const double value = pDoc->Height;
		return value;
	}
	// height of document in inches
	__declspec( property( get = GetDocumentHeight ) )
		double DocumentHeight;

	// width of the document in inches
	double GetDocumentWidth()
	{
		CLunarOrbitDoc* pDoc = Document;
		const double value = pDoc->Width;
		return value;
	}
	// width of the document in inches
	__declspec( property( get = GetDocumentWidth ) )
		double DocumentWidth;

	// margin of the document in inches
	double GetDocumentMargin()
	{
		CLunarOrbitDoc* pDoc = Document;
		const double value = pDoc->Margin;
		return value;
	}
	// margin of the document in inches
	__declspec( property( get = GetDocumentMargin ) )
		double DocumentMargin;

	// height of the document in logical pixels
	int GetLogicalDocumentHeight()
	{
		const int value = InchesToLogical( DocumentHeight );
		return value;
	}
	// height of the document in logical pixels
	__declspec( property( get = GetLogicalDocumentHeight ) )
		int LogicalDocumentHeight;

	// width of the document in logical pixels
	int GetLogicalDocumentWidth()
	{
		const int value = InchesToLogical( DocumentWidth );
		return value;
	}
	// width of the document in logical pixels
	__declspec( property( get = GetLogicalDocumentWidth ) )
		int LogicalDocumentWidth;

	// margin of the document in logical pixels
	int GetLogicalDocumentMargin()
	{
		const int value = InchesToLogical( DocumentMargin );
		return value;
	}
	// margin of the document in logical pixels
	__declspec( property( get = GetLogicalDocumentMargin ) )
		int LogicalDocumentMargin;

	// get height of the view in inches
	double GetVisibleHeight()
	{
		CClientDC dc( this );
		SetDrawDC( &dc );
		CSize sizeLogical = m_sizeClient;
		dc.DPtoLP( &sizeLogical );
		const double value = LogicalToInches( sizeLogical.cy );
		return value;
	}
	// get height of the view in inches
	__declspec( property( get = GetVisibleHeight ) )
		double VisibleHeight;

	// get line height in inches
	double GetLineHeight()
	{
		return 0.25;
	}
	// get line height in inches
	__declspec( property( get = GetLineHeight ) )
		double LineHeight;

	// get last viewable position in inches accounting for view height
	double GetLast()
	{
		double value = DocumentHeight - VisibleHeight;
		if ( value < 0 )
		{
			value = 0;
		}
		return value;
	}
	// get last viewable position in inches accounting for view height
	__declspec( property( get = GetLast ) )
		double Last;

	// get page height in inches
	double GetPageHeight()
	{
		const double value = VisibleHeight / 2;
		return value;
	}
	// get page height in inches
	__declspec( property( get = GetPageHeight ) )
		double PageHeight;

	// number of scrolling lines
	int GetScrollLines()
	{
		const double dLines = Last / LineHeight;
		int value = int( dLines );
		if ( !NearlyEqual( double( value ), dLines ) )
		{
			value++;
		}
		return value;
	}
	// number of scrolling lines
	__declspec( property( get = GetScrollLines ) )
		int ScrollLines;

	// top of view in inches
	double GetTopOfView()
	{
		return m_dTop;
	}
	// top of view in inches
	void SetTopOfView( double value )
	{
		m_dTop = value;
	}
	// top of view in inches
	__declspec( property( get = GetTopOfView, put = SetTopOfView ) )
		double TopOfView;

	// margin of error when comparing angles
	double GetAngleError()
	{
		return m_dAngleError;
	}
	// margin of error when comparing angles
	void SetAngleError( double value )
	{
		m_dAngleError = value;
	}
	// margin of error when comparing angles
	__declspec( property( get = GetAngleError, put = SetAngleError ) )
		double AngleError;

	// running?
	bool GetRunning()
	{
		return m_bRunning;
	}
	// running?
	void SetRunning( bool value )
	{
		m_bRunning = value;
	}
	// running?
	__declspec( property( get = GetRunning, put = SetRunning ) )
		bool Running;

	// stop the model after a single orbit
	bool GetSingleOrbit()
	{
		return m_bSingleOrbit;
	}
	// stop the model after a single orbit
	void SetSingleOrbit( bool value )
	{
		m_bSingleOrbit = value;
	}
	// stop the model after a single orbit
	__declspec( property( get = GetSingleOrbit, put = SetSingleOrbit ) )
		bool SingleOrbit;

	// stop the model every 30 degrees
	bool GetThirtyDegreeSteps()
	{
		return m_bThirtyDegreeSteps;
	}
	// stop the model every 30 degrees
	void SetThirtyDegreeSteps( bool value )
	{
		m_bThirtyDegreeSteps = value;
	}
	// stop the model every 30 degrees
	__declspec( property( get = GetThirtyDegreeSteps, put = SetThirtyDegreeSteps ) )
		bool ThirtyDegreeSteps;

	// get bottom of view in inches
	double GetBottomOfView()
	{
		return m_dTop + VisibleHeight;
	}
	// get bottom of view in inches
	__declspec( property( get = GetBottomOfView ) )
		double BottomOfView;

	// velocity in meters per second
	double GetVelocity()
	{
		CLunarOrbitDoc* pDoc = Document;
		const double value = pDoc->LunarVelocity;
		return value;
	}
	// velocity in meters per second
	void SetVelocity( double value )
	{
		Document->LunarVelocity = value;
	}
	// velocity in meters per second
	__declspec( property( get = GetVelocity, put = SetVelocity ) )
		double Velocity;

	// horizontal velocity in meters per second
	double GetHorizontalVelocity()
	{
		// earth moon angle in radians forming the triangle
		// on the display
		const double dRadians = AngleInRadians;

		// the velocity corresponds to the hypotenuse of the
		// triangle 
		const double dVelocity = Velocity;

		// the sine of the angle is used for the vector
		// opposite of the angle and since the velocity
		// is actually perpendicular to the hypotenuse
		// it is appropriate here to calculate the 
		// horizontal velocity 
		const double dSine = sin( dRadians );

		// the sine of the angle multiplied by the velocity (hypotenuse 
		// of the triangle) yields the horizontal velocity
		const double value = dVelocity * dSine;
		return value;
	}
	// horizontal velocity in meters per second
	__declspec( property( get = GetHorizontalVelocity ) )
		double HorizontalVelocity;

	// vertical velocity in meters per second
	double GetVerticalVelocity()
	{
		// earth moon angle in radians forming the triangle
		// on the display
		const double dRadians = AngleInRadians;

		// the velocity corresponds to the hypotenuse of the
		// triangle 
		const double dVelocity = Velocity;

		// the cosine of the angle is used for the vector
		// adjacent to the angle and since the velocity
		// is actually perpendicular to the hypotenuse
		// it is appropriate here to calculate the 
		// vertical velocity 
		const double dCosine = cos( dRadians );

		// the cosine of the angle multiplied by the velocity (hypotenuse 
		// of the triangle) yields the vertical velocity
		const double value = dVelocity * dCosine;
		return value;
	}
	// vertical velocity in meters per second
	__declspec( property( get = GetVerticalVelocity ) )
		double VerticalVelocity;

	// angle in radians of the moon
	double GetAngleInRadians()
	{
		// center of the moon on the screen
		CPoint ptMoon = MoonCenterRelativeToEarth;

		const int nX = ptMoon.x;
		const int nY = ptMoon.y;

		// length of the hypotenuse using the Pythagorean theorem
		// i.e. square root of the sum of the squares of the sides
		const double dH = sqrt( double( nX * nX + nY * nY ) );

		// sine of the angle is the ratio of the opposite side / hypotenuse 
		const double dSine = double( nY ) / dH;

		// angle in radians is the arc sine of the sine of the angle
		const double value = asin( dSine );

		return value;
	}
	// angle in radians of the moon
	__declspec( property( get = GetAngleInRadians ) )
		double AngleInRadians;

	// angle in degrees of the moon
	double GetAngleInDegrees()
	{
		// angle in radians
		const double dRadians = AngleInRadians;

		// angle in degrees 
		const double value = Degrees( dRadians );

		// record the angle
		AngleInDegrees = value;

		return value;
	}
	// angle in degrees of the moon
	void SetAngleInDegrees( double value )
	{
		CLunarOrbitDoc* pDoc = Document;
		pDoc->AngleInDegrees = value;
	}
	// angle in degrees of the moon
	__declspec( property( get = GetAngleInDegrees, put = SetAngleInDegrees ) )
		double AngleInDegrees;

	// text angle in degrees of the moon
	double GetTextAngleInDegrees()
	{
		// angle in radians
		const double dRadians = AngleInRadians;

		// angle in degrees 
		double value = Degrees( dRadians );

		CPoint ptMoon = MoonCenterRelativeToEarth;

		// if the moon is to the left of the earth,
		// convert the angle to one less than -90
		// degrees
		if ( ptMoon.x < 0 )
		{
			value = -90 - ( 90 + value );
		}

		return value;
	}
	// text angle in degrees of the moon
	__declspec( property( get = GetTextAngleInDegrees ) )
		double TextAngleInDegrees;

	// distance to moon in meters from earth
	double GetMoonDistance()
	{
		CLunarOrbitDoc* pDoc = Document;
		const double value = pDoc->MoonDistance;
		return value;
	}
	// distance to moon in meters from earth
	void SetMoonDistance( double value )
	{
		CLunarOrbitDoc* pDoc = Document;
		pDoc->MoonDistance = value;
	}
	// distance to moon in meters from earth
	__declspec( property( get = GetMoonDistance, put = SetMoonDistance ) )
		double MoonDistance;

	// time in seconds between samples
	double GetSampleTime()
	{
		CLunarOrbitDoc* pDoc = Document;
		const double value = pDoc->SampleTime;
		return value;
	}
	// time in seconds between samples
	void SetSampleTime( double value )
	{
		CLunarOrbitDoc* pDoc = Document;
		pDoc->SampleTime = value;
	}
	// time in seconds between samples
	__declspec( property( get = GetSampleTime, put = SetSampleTime ) )
		double SampleTime;

	// lunar distance scale meters to inches on screen
	double GetMoonScaling()
	{
		CLunarOrbitDoc* pDoc = Document;
		
		const double value = pDoc->MoonScaling;
		return value;
	}
	// lunar distance scale meters to inches on screen
	__declspec( property( get = GetMoonScaling, put = SetMoonScaling ) )
		double MoonScaling;

	// point defining the earth's center in logical coordinates
	CPoint GetEarthCenter()
	{
		CLunarOrbitDoc* pDoc = Document;
		CPoint value = pDoc->EarthCenter;
		return value;
	}
	// point defining the earth's center in logical coordinates
	__declspec( property( get = GetEarthCenter ) )
		CPoint EarthCenter;

	// rectangle defining the earth in logical coordinates
	CRect GetEarthRectangle()
	{
		CLunarOrbitDoc* pDoc = Document;
		CRect value = pDoc->EarthRectangle;
		return value;
	}
	// rectangle defining the earth in logical coordinates
	__declspec( property( get = GetEarthRectangle ) )
		CRect EarthRectangle;

	// center of the moon relative to the earth in logical coordinates
	CPoint GetMoonCenterRelativeToEarth()
	{
		CLunarOrbitDoc* pDoc = Document;
		CPoint value = pDoc->MoonCenterRelativeToEarth;
		return value;
	}
	// center of the moon relative to the earth
	__declspec( property( get = GetMoonCenterRelativeToEarth ) )
		CPoint MoonCenterRelativeToEarth;

	// point defining the moon's center
	CPoint GetMoonCenter()
	{
		CLunarOrbitDoc* pDoc = Document;
		CPoint value = pDoc->MoonCenter;
		return value;
	}
	// point defining the moon's center
	__declspec( property( get = GetMoonCenter ) )
		CPoint MoonCenter;

	// rectangle defining the moon in logical unit
	CRect GetMoonRectangle()
	{
		CLunarOrbitDoc* pDoc = Document;
		CRect value = pDoc->MoonRectangle;
		return value;
	}
	// rectangle defining the moon in logical unit
	__declspec( property( get = GetMoonRectangle ) )
		CRect MoonRectangle;

	// acceleration of gravity vector
	inline CMagnitudeVector& GetGravityVector()
	{
		return Document->GravityVector;
	}
	// acceleration of gravity vector
	__declspec( property( get = GetGravityVector ) )
		CMagnitudeVector& GravityVector;

	// acceleration of gravity X vector
	inline CMagnitudeVector& GetGravityX()
	{
		return Document->GravityX;
	}
	// acceleration of gravity X vector
	inline void SetGravityX( CMagnitudeVector& value )
	{
		Document->GravityX = value;
	}
	// acceleration of gravity X vector
	__declspec( property( get = GetGravityX, put = SetGravityX ) )
		CMagnitudeVector& GravityX;

	// acceleration of gravity Y vector
	inline CMagnitudeVector& GetGravityY()
	{
		return Document->GravityY;
	}
	// acceleration of gravity Y vector
	inline void SetGravityY( CMagnitudeVector& value )
	{
		Document->GravityY = value;
	}
	// acceleration of gravity Y vector
	__declspec( property( get = GetGravityY, put = SetGravityY ) )
		CMagnitudeVector& GravityY;

	// lunar velocity vector
	inline CMagnitudeVector& GetVelocityVector()
	{
		return Document->VelocityVector;
	}
	// lunar velocity vector
	inline void SetVelocityVector( CMagnitudeVector& value )
	{
		Document->VelocityVector = value;
	}
	// lunar velocity vector
	__declspec( property( get = GetVelocityVector, put = SetVelocityVector ) )
		CMagnitudeVector& VelocityVector;

	// lunar velocity X vector
	inline CMagnitudeVector& GetVelocityX()
	{
		return Document->VelocityX;
	}
	// lunar velocity X vector
	inline void SetVelocityX( CMagnitudeVector& value )
	{
		Document->VelocityX = value;
	}
	// lunar velocity X vector
	__declspec( property( get = GetVelocityX, put = SetVelocityX ) )
		CMagnitudeVector& VelocityX;

	// lunar velocity Y vector
	inline CMagnitudeVector& GetVelocityY()
	{
		return Document->VelocityY;
	}
	// lunar velocity Y vector
	inline void SetVelocityY( CMagnitudeVector& value )
	{
		Document->VelocityY= value;
	}
	// lunar velocity Y vector
	__declspec( property( get = GetVelocityY, put = SetVelocityY ) )
		CMagnitudeVector& VelocityY;

	// lunar distance vector
	inline CMagnitudeVector& GetDistanceVector()
	{
		return Document->DistanceVector;
	}
	// lunar distance vector
	inline void SetDistanceVector( CMagnitudeVector& value )
	{
		Document->DistanceVector = value;
	}
	// lunar distance vector
	__declspec( property( get = GetDistanceVector, put = SetDistanceVector ) )
		CMagnitudeVector& DistanceVector;

	// lunar distance X vector
	inline CMagnitudeVector& GetDistanceX()
	{
		return Document->DistanceX;
	}
	// lunar distance X vector
	inline void SetDistanceX( CMagnitudeVector& value )
	{
		Document->DistanceX = value;
	}
	// lunar distance X vector
	__declspec( property( get = GetDistanceX, put = SetDistanceX ) )
		CMagnitudeVector& DistanceX;

	// lunar distance Y vector
	inline CMagnitudeVector& GetDistanceY()
	{
		return Document->DistanceY;
	}
	// lunar distance Y vector
	inline void SetDistanceY( CMagnitudeVector& value )
	{
		Document->DistanceY = value;
	}
	// lunar distance Y vector
	__declspec( property( get = GetDistanceY, put = SetDistanceY ) )
		CMagnitudeVector& DistanceY;

	// public methods
public:
	// PI
	static inline const double PI()
	{
		return 3.1415926535897932384626433832795;
	}

	// degrees from radians
	template <class T> static inline T Degrees( T radians )
	{
		return (T)180 / (T)PI() * radians;
	}

	// radians from degrees
	template <class T> static inline T Radians( T degrees )
	{
		return (T)PI() / (T)180 * degrees;
	}

	// compare two reals and determine if they are nearly equal 
	// (within the given error range)
	template <class T> static inline bool NearlyEqual
	(
		T value1, T value2, T error = T( 0.0001 )
	)
	{
		const T diff = fabs( value1 - value2 );
		return diff < error;
	}

	// Round value to nearest multiple of nearest (ie. if nearest is 0.1
	// round to the nearest tenth or if nearest is 5, round to the nearest
	// multiple of five).  If second parameter is not supplied, value is 
	// rounded to the nearest whole number 
	template <class T> static inline T RoundToNearest
	(
		T value, T nearest = T( 1 )
	)
	{
		if ( nearest == 0 )
		{
			nearest = 1;
		}
		nearest = fabs( nearest ); // positive number
		value /= nearest;
		value = value < 0 ?
			ceil( value - T( 0.5 ) ) :
			floor( value + T( 0.5 ) );
		value *= nearest;
		return value;
	}

	// convert logical co-ordinate value to inches
	double LogicalToInches( int nValue )
	{
		CLunarOrbitDoc* pDoc = Document;
		const int nMap = pDoc->Map;
		return ( double( nValue ) / nMap );
	}

	// convert inches to logical co-ordinate value
	int InchesToLogical( double dValue )
	{
		CLunarOrbitDoc* pDoc = Document;
		const int nMap = pDoc->Map;
		return int( dValue * nMap );
	}

	// prepare the device context for printing
	void SetPrintDC
	(
		CDC* pDC,
		int& nPhysicalWidth, // in pixels
		int& nPhysicalHeight, // in pixels
		int& nLogicalWidth, // in inches * Map
		int& nLogicalHeight // in inches * Map
	);

	// prepare the device context for drawing and
	// return the logical width
	int SetDrawDC( CDC* pDC );

	// protected methods
protected:
	// add the current moon position to the historical points of the lunar orbit
	void AddOrbitalPoint();

	// update the position of the moon for a day
	void UpdateMoonPosition();

	// render the lunar orbit 
	void RenderLunarOrbit( CDC* pDC );

	// render the equations of motion
	void RenderEquations( CDC* pDC );

	// render the initial condition text
	void RenderInitialConditions( CDC* pDC );

	// render the distance text information
	void RenderDistanceText( CDC* pDC );

	// render the gravity text information
	void RenderGravityText( CDC* pDC );

	// render the velocity text information
	void RenderVelocityText( CDC* pDC );

	// render the grid labels
	void RenderGridLabels( CDC* pDC );

	// render the grid
	void RenderGrid( CDC* pDC );

	// render the scale labels in inches
	void RenderScale( CDC* pDC );

	// render the moon's shape
	void RenderMoon( CDC* pDC );

	// render the earth's shape
	void RenderEarth( CDC* pDC );

	// render distance vector
	void RenderDistance( CDC* pDC );

	// render acceleration vector
	void RenderAcceleration( CDC* pDC );

	// render velocity vector
	void RenderVelocity( CDC* pDC );

	// render the page or view
	void render
	(
		CDC* pDC, double dTopOfView, double dBottomOfView, int nLogicalWidth
	);

	// set the scroll bar position and range
	void SetupScrollBars()
	{
		const double dLast = Last;
		if ( NearlyEqual( dLast, 0.0 ) )
		{
			SetScrollPos( SB_VERT, 0, FALSE );
			SetScrollRange( SB_VERT, 0, 1, TRUE );
		}
		else
		{
			int nLines = ScrollLines;
			if ( nLines <= 1 )
			{
				nLines = 2;
			}
			const double dRatio = TopOfView / Last;
			const double dLine = double( nLines ) * dRatio;
			int nLine = int( dLine + 0.5 );
			if ( nLine >= nLines )
			{
				nLine = nLines - 1;
			}

			SetScrollPos( SB_VERT, nLine, FALSE );
			SetScrollRange( SB_VERT, 0, nLines - 1, TRUE );
		}
	}

	/////////////////////////////////////////////////////////////////////////////
	// generate font characteristics from given font enumeration, where
	// the enumeration is based on Atlas PDF definition
	static void BuildFont
	(
		CString csFace, // name of the font face
		bool bBold, // bold font if true
		bool bItalic, // italic font if true
		int nTextHeight, // text height in pixels
		bool bVertical, // vertical orientation
		CFont& font, // generated font
		double dAngle = 0, // angle in degrees
		BYTE nCharSet = ANSI_CHARSET, // current character set
		bool bFlipX = false, // flip horizontally
		bool bFlipY = false, // flip vertically
		short nUp = -1, // moving up is a negative value
		int nTextWidth = 0 // default width
	)
	{
		CMagnitudeVector::BuildFont
		( 
			csFace, bBold, bItalic, nTextHeight, bVertical, 
			font, dAngle, nCharSet, bFlipX, bFlipY, nUp, nTextWidth
		);
	}


// Overrides
public:
	virtual void OnPrepareDC( CDC* pDC, CPrintInfo* pInfo = NULL );
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnPrint( CDC* pDC, CPrintInfo* pInfo );
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
	afx_msg void OnSize( UINT nType, int cx, int cy );
	afx_msg void OnVScroll( UINT nSBCode, UINT nPos, CScrollBar* pScrollBar );
	afx_msg BOOL OnEraseBkgnd( CDC* pDC );
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTimer( UINT_PTR nIDEvent );
	afx_msg void OnEditPause();
	afx_msg void OnUpdateEditPause( CCmdUI *pCmdUI );
	afx_msg void OnEditRun();
	afx_msg void OnUpdateEditRun( CCmdUI *pCmdUI );
	afx_msg void OnEditSingleOrbit();
	afx_msg void OnUpdateEditSingleorbit( CCmdUI *pCmdUI );
	afx_msg void OnEdit30DegSteps();
	afx_msg void OnUpdateEdit30DegSteps( CCmdUI *pCmdUI );
};

#ifndef _DEBUG  // debug version in LunarOrbitView.cpp
inline CLunarOrbitDoc* CLunarOrbitView::GetDocument() const
   { return reinterpret_cast<CLunarOrbitDoc*>(m_pDocument); }
#endif

