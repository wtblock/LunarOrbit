/////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2022 by W. T. Block, All Rights Reserved
/////////////////////////////////////////////////////////////////////////////
#pragma once
#include <cmath>

/////////////////////////////////////////////////////////////////////////////
// a class to perform linear algebra
class CLinear
{
	// private data
private:
	double m_dX1; // x co-ordinate of point 1
	double m_dY1; // y co-ordinate of point 1
	double m_dX2; // x co-ordinate of point 2
	double m_dY2; // y co-ordinate of point 2

	// increment amount to move up the DC one pixel (1 or -1)
	int m_nUpIncrement;

	// logical pixels per inch
	int m_nMap;

	// public properties
public:
	// PI
	static double GetPI()
	{
		return 3.1415926535897932384626433832795;
	}
	// PI
	__declspec( property( get = GetPI ) )
		double PI;

	// logical pixels per inch
	int GetMap()
	{
		return m_nMap;
	}
	void SetMap( int value )
	{
		m_nMap = value;
	}
	// logical pixels per inch
	__declspec( property( get = GetMap, put = SetMap ) )
		int Map;

	// since device contexts can be set such that increasing 
	// Y values will go up or down, this attribute sets the 
	// increment amount required to move up the device by one 
	// pixel. When this value is correctly set, positive 
	// angles increase in the counter clockwise direction.
	void SetUpIncrement( int nValue = -1 )
	{
		m_nUpIncrement = nValue;
	}
	// since device contexts can be set such that increasing 
	// Y values will go up or down, this attribute sets the 
	// increment amount required to move up the device by one 
	// pixel. When this value is correctly set, positive 
	// angles increase in the counter clockwise direction.
	int GetUpIncrement() const
	{
		return m_nUpIncrement;
	}
	// up increment property
	__declspec( property( get = GetUpIncrement, put = SetUpIncrement ) )
		int UpIncrement;

	// get the first x co-ordinate
	inline double GetX1() const
	{
		return m_dX1;
	}
	// set the first x co-ordinate
	inline void SetX1( double dX1 )
	{
		m_dX1 = dX1;
	}
	// first x co-ordinate property
	__declspec( property( get = GetX1, put = SetX1 ) ) double X1;

	// get the second x co-ordinate
	inline double GetX2() const
	{
		return m_dX2;
	}
	// set the second x co-ordinate
	inline void SetX2( double dX2 )
	{
		m_dX2 = dX2;
	}
	// second x co-ordinate property
	__declspec( property( get = GetX2, put = SetX2 ) ) double X2;

	// get the first y co-ordinate
	inline double GetY1() const
	{
		return m_dY1;
	}
	// set the first y co-ordinate
	inline void SetY1( double dY1 )
	{
		m_dY1 = dY1;
	}
	// first y co-ordinate property
	__declspec( property( get = GetY1, put = SetY1 ) ) double Y1;

	// get the second y co-ordinate
	inline double GetY2() const
	{
		return m_dY2;
	}
	// set the second y co-ordinate
	inline void SetY2( double dY2 )
	{
		m_dY2 = dY2;
	}
	// second y co-ordinate property
	__declspec( property( get = GetY2, put = SetY2 ) ) double Y2;

	// first point
	CPoint GetFirstPoint()
	{
		CPoint value( round( X1 ), round( Y1 ));
		return value;
	}
	// first point
	void SetFirstPoint( CPoint value )
	{
		X1 = value.x;
		Y1 = value.y;
	}
	// first point
	__declspec( property( get = GetFirstPoint, put = SetFirstPoint ) ) CPoint FirstPoint;

	// second point
	CPoint GetSecondPoint()
	{
		CPoint value( round( X2 ), round( Y2 ));
		return value;
	}
	// second point
	void SetSecondPoint( CPoint value )
	{
		X2 = value.x;
		Y2 = value.y;
	}
	// second point
	__declspec( property( get = GetSecondPoint, put = SetSecondPoint ) ) CPoint SecondPoint;

	// delta X
	inline double GetRun()
	{
		return m_dX2 - m_dX1;
	}
	// delta X -- calculates x2 based on x1 and the run
	inline void SetRun( double dRun )
	{
		m_dX2 = m_dX1 + dRun;
	}
	// run property
	__declspec( property( get = GetRun, put = SetRun ) ) double Run;

	// delta Y
	inline double GetRise()
	{
		return m_dY2 - m_dY1;
	}
	// delta Y -- calculates y2 based on y1 and the rise
	inline void SetRise( double dRise )
	{
		m_dY2 = m_dY1 + dRise;
	}
	// rise property
	__declspec( property( get = GetRise, put = SetRise ) ) double Rise;

	// return the angle in radians from horizontal
	double GetAngleInRadians();
	// set angle in radians
	void SetAngleInRadians( double dRotation )
	{
		const double dDegrees = ConvertRadiansToDegrees( dRotation );
		Degrees = dDegrees;
	}
	// radian angle property
	__declspec( property( get = GetAngleInRadians, put = SetAngleInRadians ) )
		double Radians;

	// return the angle in degrees from horizontal
	double GetAngleInDegrees()
	{
		const double dRadians = Radians;
		const double dDegrees = 180 * dRadians / PI;
		return dDegrees;
	}
	// set the angle in degrees
	void SetAngleInDegrees( double dAngle )
	{
		const double dCurrent = Degrees;
		const double dDelta = dAngle - dCurrent;
		RotateAroundFirstPoint( dDelta );
	}
	// degrees angle property
	__declspec( property( get = GetAngleInDegrees, put = SetAngleInDegrees ) )
		double Degrees;

	// the direction of the X component where -1 is getting smaller and
	// +1 is getting larger
	int GetDirectionX()
	{
		int value = -1;
		if ( X2 > X1 )
		{
			value = 1;
		}
		return value;
	}
	// the direction of the X component where -1 is getting smaller and
	// +1 is getting larger
	__declspec( property( get = GetDirectionX ) )
		int DirectionX;

	// the direction of the Y component where -1 is getting smaller and
	// +1 is getting larger
	int GetDirectionY()
	{
		int value = -1;
		if ( Y2 > Y1 )
		{
			value = 1;
		}
		return value;
	}
	// the direction of the Y component where -1 is getting smaller and
	// +1 is getting larger
	__declspec( property( get = GetDirectionY ) )
		int DirectionY;

	// absolute length of this line segment
	double GetLength()
	{
		double dRun = Run;
		double dRise = Rise;
		if ( dRun == 0 )
		{
			return fabs( dRise );
		}
		else if ( dRise == 0 )
		{
			return fabs( dRun );
		}
		else // Pythagorean theorem (square root of the sum of the squares)
		{
			return sqrt( dRun * dRun + dRise * dRise );
		}
	}
	// absolute length of this line segment
	void SetLength( double dLength )
	{
		double dAngle = GetAngleInDegrees();
		RotateAroundFirstPoint( -dAngle ); // horizontal
		X2 = X1 + dLength;
		RotateAroundFirstPoint( dAngle );
	}
	// absolute length of this line segment
	__declspec( property( get = GetLength, put = SetLength ) )
		double Length;

	// magnitude of this line segment
	double GetMagnitude()
	{
		return Length;
	}
	// magnitude of this line segment (change second point)
	void SetMagnitude( double dMagnitude )
	{
		Length = dMagnitude;
	}
	// magnitude of line segment property
	__declspec( property( get = GetLength, put = SetLength ) )
		double Magnitude;

	// public static methods
public:
	// convert degrees to radians
	inline static double ConvertDegreesToRadians( double dDegrees )
	{
		const double dRadians = dDegrees * GetPI() / 180;
		return dRadians;
	}

	// convert radians to degrees
	inline static double ConvertRadiansToDegrees( double dRadians )
	{
		const double dDegrees = 180 * dRadians / GetPI();
		return dDegrees;
	}

	// Convert a mathematical angle in degrees into a compass angle 
	// where compass angles are defined as up being zero degrees 
	// with positive increments rotating in a clockwise direction.
	inline static double ConvertToCompassAngle( double dValue )
	{
		dValue = fmod( 360.0 - dValue + 90, 360.0 );
		return dValue;
	}

	// Convert a compass angle in degrees into a mathematical angle 
	// where mathematical angles are defined as right being zero 
	// degrees with positive increments rotating 
	// in a counter-clockwise direction.
	inline static double ConvertToMathematicalAngle( double dValue )
	{
		dValue = fmod( 360.0 - ( dValue - 90 ), 360.0 );
		return dValue;
	}

	// normalize angle from 0 to 360 degrees 
	inline static double NormalizeAngle( double dValue )
	{
		dValue = fmod( dValue, 360 );
		if ( dValue < 0 )
			dValue += 360;
		return dValue;
	}

	// round a double to an integer 
	inline static int round( double a )
	{
		return int( a < 0 ? ceil( a - 0.5 ) : floor( a + 0.5 ) );
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


	// public methods
public:
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

	// set both points
	inline void SetPoints( CPoint pt1, CPoint pt2 )
	{
		FirstPoint = pt1;
		SecondPoint = pt2;
	}

	// copies a line to another line
	CLinear operator=( const CLinear& ln )
	{
		X1 = ln.X1;
		X2 = ln.X2;
		Y1 = ln.Y1;
		Y2 = ln.Y2;
		UpIncrement = ln.UpIncrement;
		return *this;
	}

	// move the line by given delta
	inline void Translate( double dX, double dY )
	{
		m_dX1 += dX;
		m_dX2 += dX;
		m_dY1 += dY;
		m_dY2 += dY;
	}

	// what is the slope of this line
	// returns false if the line is vertical
	bool GetSlope( double& dM )
	{
		const double dRun = Run;
		if ( dRun != 0 )
		{
			const double dRise = Rise;
			dM = dRise / dRun;
			return true;
		}
		return false;
	}
	
	// where does this line intercept the y axis
	// returns false if it is a vertical line
	bool GetYIntercept( double& dB )
	{
		double dM; // slope
		if ( GetSlope( dM ) )
		{
			dB = Y1 - dM * X1;
			return true;
		}
		return false;
	}
	
	// given an X value, calculate the Y value
	// return false for vertical line
	bool GetY( double dX, double& dY )
	{
		double dM;
		if ( GetSlope( dM ) )
		{
			double dB;
			GetYIntercept( dB );
			dY = dM * dX + dB;
		}
		else // vertical line, return false
		{
			return false;
		}
		return true;
	}
	
	// given an X value, return the Y value
	// NOTE: use this version if you know the line is not vertical
	double GetY( double dX )
	{
		double dY;
		if ( GetY( dX, dY ) )
		{
			return dY;
		}
		ASSERT( m_dX1 != m_dX2 ); // not vertical
		return m_dY1;
	}
	
	// given a Y value, calculate the X value
	// return false for horizontal line
	bool GetX( double dY, double& dX )
	{
		double dM;
		if ( GetSlope( dM ) )
		{
			if ( dM == 0 ) // horizontal line
			{
				return false;
			}
			else
			{
				double dB;
				GetYIntercept( dB );
				dX = ( dY - dB ) / dM;
			}
		}
		else // vertical line, return either X value
		{
			dX = m_dX1;
		}
		return true;
	}
	
	// given a Y value, return the X value
	// NOTE: use this version when you know line is not horizontal
	double GetX( double dY )
	{
		double dX;
		if ( GetX( dY, dX ) )
		{
			return dX;
		}
		ASSERT( m_dY1 != m_dY2 ); // not horizontal
		return m_dX1;
	}
	
	// given x and y co-ordinates, return a line perpendicular to this line
	CLinear GetPerpendicularLine( double dX1, double dY1 );
	
	// given a point, return a line perpendicular to this line
	CLinear GetPerpendicularLine( CPoint pt )
	{
		return GetPerpendicularLine( pt.x, pt.y );
	}

	// returns the smallest rectangle that can contain this line segment
	CRect GetBoundingRect()
	{
		CRect rectOut;
		rectOut.left = round( m_dX1 );
		rectOut.top = round( m_dY1 );
		rectOut.right = round( m_dX2 );
		rectOut.bottom = round( m_dY2 );
		rectOut.NormalizeRect();
		// adjust for vertical lines
		if ( rectOut.Width() == 0 ) rectOut.InflateRect( 1, 0 );
		// adjust for horizontal lines
		if ( rectOut.Height() == 0 ) rectOut.InflateRect( 0, 1 );
		return rectOut;
	}

	// returns a rectangle that represents the overlapping area of the
	// bounding rectangles of two line segments
	CRect GetOverLappingRect( CLinear& refLine )
	{
		CRect rect1( GetBoundingRect() );
		CRect rect2( refLine.GetBoundingRect() );
		return ( rect1 & rect2 );
	}

	// while any two lines (except parallel lines) will intercept, this
	// routine  tests to see if they can intercept within the given segments
	// (if the bounding rectangles of the line segments do not overlap, then
	// the segments cannot intercept)
	bool SegmentCanIntercept( CLinear& refLine )
	{
		const double dX1 = refLine.X1;
		const double dX2 = refLine.X2;
		const double dY1 = refLine.Y1;
		const double dY2 = refLine.Y2;
		return
		(
			(
				( dX1 < m_dX1 && m_dX1 < dX2 ) &&
				( dY1 < m_dY1 && m_dY1 < dY2 )
			) 
			||
			(
				( dX1 < m_dX2 && m_dX2 < dX2 ) &&
				( dY1 < m_dY2 && m_dY2 < dY2 )
			)
		);
	}
	
	// given a line, return the x and y co-ordinates where it intercepts
	// this line and returns false if they do not intercept
	bool GetLineIntercept( CLinear& refLine, double& dX, double& dY );
	
	// given a line, return the point where it intercepts this line
	// returns false if they do not intercept
	bool GetLineIntercept( CLinear& refLine, CPoint& pt )
	{
		double dX, dY;
		if ( GetLineIntercept( refLine, dX, dY ) )
		{
			pt.x = round( dX );
			pt.y = round( dY );
			return true;
		}
		return false;
	}
	
	// given a line segment, return the point where it intercepts this line
	// segment and returns false if they do not intercept within the segments
	bool GetSegmentIntercept
	( 
		CLinear& refLine, // input line to test
		CPoint& pt, // returned intercept point
		CSize sizeSlop = CSize( 0, 0 ) // optional slop in pixels
	)
	{
		double dX, dY;
		if ( GetLineIntercept( refLine, dX, dY ) )
		{
			pt.x = round( dX );
			pt.y = round( dY );
			CRect rect( GetOverLappingRect( refLine ) );

			// factor in the slop
			rect.InflateRect( sizeSlop );
			if ( pt.x < rect.left || pt.x > rect.right )
			{
				return false;
			}
			if ( pt.y < rect.top || pt.y > rect.bottom )
			{
				return false;
			}
			return true;
		}
		return false;
	}

	// given x and y co-ordinates, return the distance to this line.  This
	// version also returns the co-ordinates on the line the point was 
	// closest to.
	double GetDistanceToPoint
	( 
		double dX1, double dY1, double& dX2, double& dY2
	)
	{
		CLinear lnPerp = GetPerpendicularLine( dX1, dY1 );
		dX2 = lnPerp.X2;
		dY2 = lnPerp.Y2;
		return lnPerp.Length;
	}

	// given x and y co-ordinates, return the distance to this line
	double GetDistanceToPoint( double dX1, double dY1 )
	{
		CLinear lnPerp = GetPerpendicularLine( dX1, dY1 );
		return lnPerp.Length;
	}

	// given a point, return the distance to this line.  This version
	// also returns the co-ordinates on the line the point was closest to.
	double GetDistanceToPoint( CPoint pt, double& dX2, double& dY2 )
	{
		return GetDistanceToPoint( pt.x, pt.y, dX2, dY2 );
	}

	// given a point, return the distance to this line
	double GetDistanceToPoint( CPoint pt )
	{
		return GetDistanceToPoint( pt.x, pt.y );
	}

	// Second point will be relocated by rotating it by dRotation degrees
	// with the first point being the center of the rotation
	void RotateAroundFirstPoint( double dRotation );
	
	// Rotate the segment around a given co-ordinate and angle in degrees
	void RotateAroundPoint( double dX, double dY, double dRotation );
	
	// Rotate the segment around a given point and angle in degrees
	void RotateAroundPoint( CPoint pt, double dRotation );

	// constructor / destructor
public:
	// construct an empty line
	CLinear()
	{
		m_dX1 = m_dY1 = m_dX2 = m_dY2 = 0; 

		// default to Y coordinate where Y gets smaller going up
		UpIncrement = -1;

		// logical pixels per inch
		Map = 1000;
	}

	// construct a line from two sets of co-ordinates
	CLinear( double dX1, double dY1, double dX2, double dY2 )
	{
		X1 = dX1;
		Y1 = dY1;
		X2 = dX2;
		Y2 = dY2;

		// default to Y coordinate where Y gets smaller going up
		UpIncrement = -1;

		// logical pixels per inch
		Map = 1000;
	}

	// construct a line from two points
	CLinear( CPoint pt1, CPoint pt2 )
	{
		SetPoints( pt1, pt2 );

		// default to Y coordinate where Y gets smaller going up
		UpIncrement = -1;

		// logical pixels per inch
		Map = 1000;
	} 

	// destructor
	virtual ~CLinear()
	{
	}
};
