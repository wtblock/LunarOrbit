
/////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2022 by W. T. Block, All Rights Reserved
/////////////////////////////////////////////////////////////////////////////

#pragma once
#include "BaseDoc.h"
#include "MagnitudeVector.h"

/////////////////////////////////////////////////////////////////////////////
class CLunarOrbitDoc : public CBaseDoc
{
protected: // create from serialization only
	CLunarOrbitDoc();
	DECLARE_DYNCREATE(CLunarOrbitDoc)
	double m_dAngle; // angle of the moon in degrees
	double m_dLunarVelocity; // velocity in meters per second
	double m_dLunarVelocityX; // X velocity in meters per second
	double m_dLunarVelocityY; // Y velocity in meters per second
	double m_dMoonDistance; // distance to moon in meters
	double m_dMoonInches; // distance to the moon from earth on screen
	double m_dMoonScaling; // distance scale meters to inches on screen
	double m_dMoonX; // X coordinate of the moon in meters
	double m_dMoonY; // Y coordinate of the moon in meters
	double m_dSampleTime; // time in seconds between samples
	double m_dSamplesPerDay; // number of samples per day
	double m_dRunningTime; // number of seconds the application has run
	double m_dMassOfTheEarth; // mass of the earth in kilograms
	double m_dLunarPeriod; // period of the lunar orbit in seconds
	double m_dAccelerationOfGravity; // meters per second squared
	double m_dLunarGravityX; // X Vector of the acceleration of gravity
	double m_dLunarGravityY; // Y Vector of the acceleration of gravity

	// these are the vectors describing acceleration of the moon
	CMagnitudeVector m_GravityVector;
	CMagnitudeVector m_GravityX;
	CMagnitudeVector m_GravityY;

	// these are the vectors describing velocity of the moon
	CMagnitudeVector m_VelocityVector;
	CMagnitudeVector m_VelocityX;
	CMagnitudeVector m_VelocityY;

	// these are the vectors describing position of the moon
	CMagnitudeVector m_DistanceVector;
	CMagnitudeVector m_DistanceX;
	CMagnitudeVector m_DistanceY;

	// properties
public:
	// PI
	static double GetPI()
	{
		return CLinear::GetPI();
	}
	// PI
	__declspec( property( get = GetPI ) )
		double PI;

	// degrees from radians
	static double GetDegrees( double dRadians )
	{
		return 180.0 / GetPI() * dRadians;
	}
	// degrees from radians
	__declspec( property( get = GetDegrees ) )
		double Degrees[];

	// radians from degrees
	static double GetRadians( double dDegrees )
	{
		return GetPI() / 180.0 * dDegrees;
	}
	// radians from degrees
	__declspec( property( get = GetRadians ) )
		double Radians[];

	// logical pixels per inch
	static int GetMap()
	{
		return 1000;
	}
	// logical pixels per inch
	__declspec( property( get = GetMap ) )
		int Map;

	// number of pages in the document
	static UINT GetPages()
	{
		return 1;
	}
	// number of pages in the document
	__declspec( property( get = GetPages ) )
		UINT Pages;

	// height of document in inches
	static double GetHeight()
	{
		return 8.5;
	}
	// height of document in inches
	__declspec( property( get = GetHeight ) )
		double Height;

	// width of document in inches
	static double GetWidth()
	{
		return 11.0;
	}
	// width of document in inches
	__declspec( property( get = GetWidth ) )
		double Width;

	// margin of document in inches
	static double GetMargin()
	{
		return 0.25;
	}
	// margin of document in inches
	__declspec( property( get = GetMargin ) )
		double Margin;

	// acceleration of gravity vector
	inline CMagnitudeVector& GetGravityVector()
	{
		return m_GravityVector;
	}
	// acceleration of gravity vector
	__declspec( property( get = GetGravityVector  ) )
		CMagnitudeVector& GravityVector;

	// acceleration of gravity X vector
	inline CMagnitudeVector& GetGravityX()
	{
		return m_GravityX;
	}
	// acceleration of gravity X vector
	inline void SetGravityX( CMagnitudeVector& value )
	{
		m_GravityX = value;
	}
	// acceleration of gravity X vector
	__declspec( property( get = GetGravityX, put = SetGravityX ) )
		CMagnitudeVector& GravityX;

	// acceleration of gravity Y vector
	inline CMagnitudeVector& GetGravityY()
	{
		return m_GravityY;
	}
	// acceleration of gravity Y vector
	inline void SetGravityY( CMagnitudeVector& value )
	{
		m_GravityY = value;
	}
	// acceleration of gravity Y vector
	__declspec( property( get = GetGravityY, put = SetGravityY  ) )
		CMagnitudeVector& GravityY;

	// lunar velocity vector
	inline CMagnitudeVector& GetVelocityVector()
	{
		return m_VelocityVector;
	}
	// lunar velocity vector
	inline void SetVelocityVector( CMagnitudeVector& value )
	{
		m_VelocityVector = value;
	}
	// lunar velocity vector
	__declspec( property( get = GetVelocityVector, put = SetVelocityVector ) )
		CMagnitudeVector& VelocityVector;

	// lunar velocity X vector
	inline CMagnitudeVector& GetVelocityX()
	{
		return m_VelocityX;
	}
	// lunar velocity X vector
	inline void SetVelocityX( CMagnitudeVector& value )
	{
		m_VelocityX = value;
	}
	// lunar velocity X vector
	__declspec( property( get = GetVelocityX, put = SetVelocityX ) )
		CMagnitudeVector& VelocityX;

	// lunar velocity Y vector
	inline CMagnitudeVector& GetVelocityY()
	{
		return m_VelocityY;
	}
	// lunar velocity Y vector
	inline void SetVelocityY( CMagnitudeVector& value )
	{
		m_VelocityY = value;
	}
	// lunar velocity Y vector
	__declspec( property( get = GetVelocityY, put = SetVelocityY ) )
		CMagnitudeVector& VelocityY;

	// lunar distance vector
	inline CMagnitudeVector& GetDistanceVector()
	{
		return m_DistanceVector;
	}
	// lunar distance vector
	inline void SetDistanceVector( CMagnitudeVector& value )
	{
		m_DistanceVector = value;
	}
	// lunar distance vector
	__declspec( property( get = GetDistanceVector, put = SetDistanceVector ) )
		CMagnitudeVector& DistanceVector;

	// lunar distance X vector
	inline CMagnitudeVector& GetDistanceX()
	{
		return m_DistanceX;
	}
	// lunar distance X vector
	inline void SetDistanceX( CMagnitudeVector& value )
	{
		m_DistanceX = value;
	}
	// lunar distance X vector
	__declspec( property( get = GetDistanceX, put = SetDistanceX ) )
		CMagnitudeVector& DistanceX;

	// lunar distance Y vector
	inline CMagnitudeVector& GetDistanceY()
	{
		return m_DistanceY;
	}
	// lunar distance Y vector
	inline void SetDistanceY( CMagnitudeVector& value )
	{
		m_DistanceY = value;
	}
	// lunar distance Y vector
	__declspec( property( get = GetDistanceY, put = SetDistanceY ) )
		CMagnitudeVector& DistanceY;

	// angle in degrees of the moon
	double GetAngleInDegrees()
	{
		return m_dAngle;
	}
	// angle in degrees of the moon
	void SetAngleInDegrees( double value )
	{
		m_dAngle = value;
	}
	// angle in degrees of the moon
	__declspec( property( get = GetAngleInDegrees, put = SetAngleInDegrees ) )
		double AngleInDegrees;

	// velocity in meters per second
	double GetLunarVelocity()
	{
		return m_dLunarVelocity;
	}
	// velocity in meters per second
	void SetLunarVelocity( double value )
	{
		m_dLunarVelocity = value;
	}
	// velocity in meters per second
	__declspec( property( get = GetLunarVelocity, put = SetLunarVelocity ) )
		double LunarVelocity;

	// X velocity in meters per second
	double GetLunarVelocityX()
	{
		return m_dLunarVelocityX;
	}
	// X velocity in meters per second
	void SetLunarVelocityX( double value )
	{
		m_dLunarVelocityX = value;
	}
	// X velocity in meters per second
	__declspec( property( get = GetLunarVelocityX, put = SetLunarVelocityX ) )
		double LunarVelocityX;

	// Y velocity in meters per second
	double GetLunarVelocityY()
	{
		return m_dLunarVelocityY;
	}
	// Y velocity in meters per second
	void SetLunarVelocityY( double value )
	{
		m_dLunarVelocityY = value;
	}
	// Y velocity in meters per second
	__declspec( property( get = GetLunarVelocityY, put = SetLunarVelocityY ) )
		double LunarVelocityY;

	// absolute distance to moon in meters with respect to the earth
	// (the radius of the lunar orbit)
	double GetMoonDistance()
	{
		return m_dMoonDistance;
	}
	// absolute distance to moon in meters with respect to the earth
	// (the radius of the lunar orbit)
	void SetMoonDistance( double value )
	{
		m_dMoonDistance = value;
	}
	// absolute distance to moon in meters with respect to the earth
	// (the radius of the lunar orbit)
	__declspec( property( get = GetMoonDistance, put = SetMoonDistance ) )
		double MoonDistance;

	// radius of lunar orbit around the earth on the screen
	// in inches
	double GetMoonInches()
	{
		return m_dMoonInches;
	}
	// radius of lunar orbit around the earth on the screen
	// in inches
	void SetMoonInches( double value )
	{
		m_dMoonInches = value;
	}
	// radius of lunar orbit around the earth on the screen
	// in inches
	__declspec( property( get = GetMoonInches, put = SetMoonInches ) )
		double MoonInches;

	// lunar distance scale meters to inches on screen
	double GetMoonScaling()
	{
		const double dMoonDistance = MoonDistance;
		const double dMoonInches = MoonInches;
		const double value = dMoonDistance / dMoonInches;
		MoonScaling = value;
		return value;
	}
	// lunar distance scale meters to inches on screen
	void SetMoonScaling( double value )
	{
		m_dMoonScaling = value;
	}
	// lunar distance scale meters to inches on screen
	__declspec( property( get = GetMoonScaling, put = SetMoonScaling ) )
		double MoonScaling;

	// given a distance in meters, return the number of inches on the screen
	double GetMoonScreenInches( double meters )
	{
		const double scale = MoonScaling;
		const double value = meters / scale;
		return value;
	}
	// given a distance in meters, return the number of inches on the screen
	__declspec( property( get = GetMoonScreenInches ) )
		double MoonScreenInches[];

	// mass of the earth in kilograms
	double GetMassOfTheEarth()
	{
		return m_dMassOfTheEarth;
	}
	// mass of the earth in kilograms
	void SetMassOfTheEarth( double value )
	{
		m_dMassOfTheEarth = value;
	}
	// mass of the earth in kilograms
	__declspec( property( get = GetMassOfTheEarth, put = SetMassOfTheEarth ) )
		double MassOfTheEarth;

	// period of the lunar orbit in seconds
	double GetLunarPeriod()
	{
		const double dDays = 27.32;
		const double dSeconds = dDays * 86400;
		m_dLunarPeriod = dSeconds;
		return m_dLunarPeriod;
	}
	// period of the lunar orbit in seconds
	__declspec( property( get = GetLunarPeriod ) )
		double LunarPeriod;

	// absolute value of acceleration of gravity on the moon from the earth
	double GetAccelerationOfGravity()
	{
		// g = M/R²
		// g is the acceleration of gravity being calculated
		// G is the gravitational constant (6.657 x 10^-11 Nm2kg-2)
		// M is the mass of the earth in kg
		// R is radius (distance to the moon) in meters

		// universal gravitational constant
		const double dG = 6.657e-11;
		const double dM = MassOfTheEarth;
		const double dR = MoonDistance;

		// distance to the moon squared
		const double dR2 = dR * dR;

		// solve Newton's equation (g = GM/R²)
		m_dAccelerationOfGravity = dG * dM / dR2;

		return m_dAccelerationOfGravity;
	}
	// absolute value of acceleration of gravity on the moon from the earth
	__declspec( property( get = GetAccelerationOfGravity ) )
		double AccelerationOfGravity;

	// if the gravity is negative, the moon is being pulled left
	// if the gravity is positive, the moon is being pulled right
	double GetLunarGravityX()
	{
		return m_dLunarGravityX;
	}
	// if the gravity is negative, the moon is being pulled left
	// if the gravity is positive, the moon is being pulled right
	void SetLunarGravityX( double value )
	{
		m_dLunarGravityX = value;
	}
	// if the gravity is negative, the moon is being pulled left
	// if the gravity is positive, the moon is being pulled right
	__declspec( property( get = GetLunarGravityX, put = SetLunarGravityX ) )
		double LunarGravityX;

	// if the gravity is negative, the moon is being pulled up
	// if the gravity is positive, the moon is being pulled down
	double GetLunarGravityY()
	{
		return m_dLunarGravityY;
	}
	// if the gravity is negative, the moon is being pulled up
	// if the gravity is positive, the moon is being pulled down
	void SetLunarGravityY( double value )
	{
		m_dLunarGravityY = value;
	}
	// if the gravity is negative, the moon is being pulled up
	// if the gravity is positive, the moon is being pulled down
	__declspec( property( get = GetLunarGravityY, put = SetLunarGravityY ) )
		double LunarGravityY;

	// distance to the earth's y axis in meters where a positive value
	// indicates the earth is to the right of the moon and a negative
	// value indicates the earth is to the left of the moon
	double GetMoonX()
	{
		return m_dMoonX;
	}
	// distance to the earth's y axis in meters where a positive value
	// indicates the earth is to the right of the moon and a negative
	// value indicates the earth is to the left of the moon
	void SetMoonX( double value )
	{
		m_dMoonX = value;
	}
	// distance to the earth's y axis in meters where a positive value
	// indicates the earth is to the right of the moon and a negative
	// value indicates the earth is to the left of the moon
	__declspec( property( get = GetMoonX, put = SetMoonX ) )
		double MoonX;

	// distance to the earth's x axis in meters where a
	// positive value indicates the earth is below the moon and a
	// negative value indicates the earth is above the moon
	double GetMoonY()
	{
		return m_dMoonY;
	}
	// distance to the earth's x axis in meters where a
	// positive value indicates the earth is below the moon and a
	// negative value indicates the earth is above the moon
	void SetMoonY( double value )
	{
		m_dMoonY = value;
	}
	// distance to the earth's x axis in meters where a
	// positive value indicates the earth is below the moon and a
	// negative value indicates the earth is above the moon
	__declspec( property( get = GetMoonY, put = SetMoonY ) )
		double MoonY;

	// time in seconds between samples
	double GetSampleTime()
	{
		return m_dSampleTime;
	}
	// time in seconds between samples
	void SetSampleTime( double value )
	{
		m_dSampleTime = value;
	}
	// time in seconds between samples
	__declspec( property( get = GetSampleTime, put = SetSampleTime ) )
		double SampleTime;

	// samples per day
	double GetSamplesPerDay()
	{
		const double dSecondsPerDay = 86400.0;
		const double dSampleTime = SampleTime;
		const double value = dSecondsPerDay / dSampleTime;
		SamplesPerDay = value;
		return value;
	}
	// samples per day
	void SetSamplesPerDay( double value )
	{
		m_dSamplesPerDay = value;
	}
	// samples per day
	__declspec( property( get = GetSamplesPerDay, put = SetSamplesPerDay ) )
		double SamplesPerDay;

	// number of seconds the application has run
	double GetRunningTime()
	{
		return m_dRunningTime;
	}
	// number of seconds the application has run
	void SetRunningTime( double value )
	{
		m_dRunningTime = value;
	}
	// number of seconds the application has run
	__declspec( property( get = GetRunningTime, put = SetRunningTime ) )
		double RunningTime;

	// get a pointer to the view
	CView* GetView()
	{
		POSITION pos = GetFirstViewPosition();
		CView* value = 0;
		while ( value == 0 && pos )
		{
			value = GetNextView( pos );
		}

		return value;
	}
	// get a pointer to the view
	__declspec( property( get = GetView ) )
		CView* View;

	// convert logical co-ordinate value to inches
	double LogicalToInches( int nValue )
	{
		const int nMap = Map;
		return ( double( nValue ) / nMap );
	}

	// convert inches to logical co-ordinate value
	int InchesToLogical( double dValue )
	{
		const int nMap = Map;
		return int( dValue * nMap );
	}

	// point defining the earth's center in logical coordinates
	CPoint GetEarthCenter()
	{
		// get the center of the earth on the document
		const double dDocumentWidth = Width;
		const double dDocumentHeight = Height;
		const int nX = InchesToLogical( dDocumentWidth / 2 );
		const int nY = InchesToLogical( dDocumentHeight / 2 );

		// build the point
		CPoint value( nX, nY );
		return value;
	}
	// point defining the earth's center in logical coordinates
	__declspec( property( get = GetEarthCenter ) )
		CPoint EarthCenter;

	// rectangle defining the earth in logical coordinates
	CRect GetEarthRectangle()
	{
		// get the center of the earth on the document
		CPoint ptCenter = EarthCenter;

		// rectangle size (width and height) in inches
		const double dSize = 0.5;
		const int nSize = InchesToLogical( dSize );
		CSize size( nSize, nSize );

		// build the rectangle
		CRect value( ptCenter, size );

		// recenter the rectangle
		value.OffsetRect( -size.cx / 2, -size.cy / 2 );
		return value;
	}
	// rectangle defining the earth in logical coordinates
	__declspec( property( get = GetEarthRectangle ) )
		CRect EarthRectangle;

	// center of the moon relative to the earth where
	// a negative x indicates the earth is to the left of the moon
	// a negative y indicates the earth is above the moon
	CPoint GetMoonCenterRelativeToEarth()
	{
		// x and y coordinates of the moon relative to the earth
		// in meters
		const double dMetersX = MoonX;
		const double dMetersY = MoonY;

		// x and y coordinates of the moon relative to the earth
		// in screen inches
		const double dInchesX = MoonScreenInches[ dMetersX ];
		const double dInchesY = MoonScreenInches[ dMetersY ];

		// x and y coordinates of the moon relative to the earth
		// in logical pixels
		const int nX = InchesToLogical( dInchesX );
		const int nY = InchesToLogical( dInchesY );

		// the moon's relative coordinates
		CPoint value( nX, nY );

		return value;
	}
	// center of the moon relative to the earth where
	// a negative x indicates the earth is to the left of the moon
	// a negative y indicates the earth is above the moon
	__declspec( property( get = GetMoonCenterRelativeToEarth ) )
		CPoint MoonCenterRelativeToEarth;

	// point defining the moon's center in logical coordinates
	CPoint GetMoonCenter()
	{
		// build the point starting with the earth's center in logical coordinates
		CPoint value = EarthCenter;

		// get the moon's center relative to the earth
		CPoint moon = MoonCenterRelativeToEarth;

		// offset the earth center by the moon's relative coordinates and since
		// the coordinates are from the moon's perspective, the coordinates 
		// need to be negated in order to get the correct offset
		value.Offset( -moon.x, -moon.y );

		return value;
	}
	// point defining the moon's center in logical coordinates
	__declspec( property( get = GetMoonCenter ) )
		CPoint MoonCenter;

	// rectangle defining the moon
	CRect GetMoonRectangle()
	{
		// get the center of the earth on the document
		CPoint ptCenter = MoonCenter;

		// rectangle size (width and height) in inches
		const double dSize = 0.25;
		const int nSize = InchesToLogical( dSize );
		CSize size( nSize, nSize );

		// build the rectangle
		CRect value( ptCenter, size );

		// recenter the rectangle
		value.OffsetRect( -size.cx / 2, -size.cy / 2 );
		return value;
	}
	// rectangle defining the moon
	__declspec( property( get = GetMoonRectangle ) )
		CRect MoonRectangle;

	// Operations
public:

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// Implementation
public:
	virtual ~CLunarOrbitDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};
