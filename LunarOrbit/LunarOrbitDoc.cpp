
/////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2022 by W. T. Block, All Rights Reserved
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "LunarOrbit.h"
#include "LunarOrbitDoc.h"
#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNCREATE(CLunarOrbitDoc, CDocument)

/////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CLunarOrbitDoc, CDocument)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
CLunarOrbitDoc::CLunarOrbitDoc()
{
	AngleInDegrees = 180;

	LunarVelocity = 1022; // meters per second
	LunarVelocityX = 0; // meters per second

	// a positive Y velocity increases the Y distance to the earth
	// which is counterintuitive at first glance because the 
	// absolute y coordinate is getting smaller.
	LunarVelocityY = 1022; // meters per second

	MoonDistance = 382500000; // radius of lunar orbit
	MoonInches = 4.0; // radius of the lunar orbit on screen in inches

	// distance to the earth's y axis in meters where a positive value
	// indicates the earth is to the right of the moon and a negative
	// value indicates the earth is to the left of the moon
	MoonX = -MoonDistance;
	// distance to the earth's x axis in meters where a
	// positive value indicates the earth is below the moon and a
	// negative value indicates the earth is above the moon
	MoonY = 0;

	MassOfTheEarth = 5.983e24; // kg

	// absolute value of acceleration of gravity on the moon from the earth
	const double dA = AccelerationOfGravity;

	// if the gravity is negative, the moon is being pulled left
	// if the gravity is positive, the moon is being pulled right
	LunarGravityX = -dA; // meters per second squared
	// if the gravity is negative, the moon is being pulled up
	// if the gravity is positive, the moon is being pulled down
	LunarGravityY = 0; // meters per second squared

	SampleTime = 1; // seconds
	const double dSamplesPerDay = SamplesPerDay;
	RunningTime = 0; // seconds
	const double dLunarPeriod = LunarPeriod;

	// the distance vector starts at the moon's center and ends at
	// the earth's center
	DistanceVector.FirstPoint = MoonCenter;
	DistanceVector.SecondPoint = EarthCenter;

	// the distance magnitude is the absolute distance to the earth
	DistanceVector.RealMagnitude = fabs( MoonDistance );

	// engineering units of distance
	DistanceVector.EngineeringUnits = _T( "m" );

	// set the vector's description to "S" distance to earth
	DistanceVector.Description = _T( "S" );

	// change the color to purple
	DistanceVector.Color = RGB( 255, 0, 255 );

	// change the numerical format to "f" for floating point
	DistanceVector.NumericalFormat = _T( "f" );

	// change the number of decimal places to zero
	DistanceVector.Decimals = 0;

	// X and y components of the moon's distance to the earth
	DistanceX = DistanceVector.VectorX;
	DistanceX.Description = _T( "Sx" );
	DistanceY = DistanceVector.VectorY;
	DistanceY.Description = _T( "Sy" );

	// the gravity vector starts at the moon's center and ends at
	// the earth's center
	GravityVector.FirstPoint = MoonCenter;
	GravityVector.SecondPoint = EarthCenter;

	// set the gravity vector's length to two inches or half the
	// radius of the orbit
	const double dGravityLength = InchesToLogical( 2.0 );
	GravityVector.Length = dGravityLength;

	GravityVector.RealMagnitude = dA;
	GravityVector.EngineeringUnits = _T( "m/s²" );
	GravityVector.Description = _T( "Ag" );

	// X and y components of the earth's gravitational pull on the moon
	GravityX = GravityVector.VectorX;
	GravityX.Description = _T( "Ax" );
	GravityY = GravityVector.VectorY;
	GravityY.Description = _T( "Ay" );

	// copy the velocity vector from the acceleration vector
	VelocityVector = GravityVector;

	// the velocity vector is the absolute value of the velocity
	VelocityVector.RealMagnitude = fabs( LunarVelocity );

	// set the vector's length to one inch
	const double dVelocityLength = InchesToLogical( 1.0 );
	VelocityVector.Length = dVelocityLength;

	// set the vector's description to "Vg" velocity due to gravity
	VelocityVector.Description = _T( "Vg" );

	// the velocity is 90 degrees out of phase with the acceleration
	// (rotation around the earth is counter-clockwise when looking down 
	// toward the north pole of the earth). Since the initial condition
	// of the moon is on the X axis to the right of the earth, the angle
	// of the acceleration vector is 180 degrees and rotating the
	// velocity vector -90 degrees will point the vector up in the 
	// direction of the moon's initial velocity.
	VelocityVector.RotateAroundFirstPoint( -90 );

	// change the engineering units to meters per second
	VelocityVector.EngineeringUnits = _T( "m/s" );

	// change the color to blue
	VelocityVector.Color = RGB( 0, 0, 255 );

	// change the numerical format to "f" for floating point
	VelocityVector.NumericalFormat = _T( "f" );

	// change the number of decimal places to zero
	VelocityVector.Decimals = 0;

	// x and y components of velocity
	VelocityX = VelocityVector.VectorX;
	VelocityX.Description = _T( "Vx" );
	VelocityY = VelocityVector.VectorY;
	VelocityY.Description = _T( "Vy" );

}

/////////////////////////////////////////////////////////////////////////////
CLunarOrbitDoc::~CLunarOrbitDoc()
{
}

/////////////////////////////////////////////////////////////////////////////
BOOL CLunarOrbitDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
void CLunarOrbitDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
#ifdef _DEBUG
void CLunarOrbitDoc::AssertValid() const
{
	CDocument::AssertValid();
}

/////////////////////////////////////////////////////////////////////////////
void CLunarOrbitDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
