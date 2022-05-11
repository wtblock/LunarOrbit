/////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2022 by W. T. Block, All Rights Reserved
/////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "Linear.h"

/////////////////////////////////////////////////////////////////////////////
// given x and y co-ordinates, return a line perpendicular to this line
CLinear CLinear::GetPerpendicularLine( double dX1, double dY1 )
{
	const double dRun = Run;
	const double dRise = Rise;
	double dX2, dY2;
	if ( dRun == 0 )
	{
		dX2 = X1;
		dY2 = dY1;
	}
	else if ( dRise == 0 )
	{
		dX2 = dX1;
		dY2 = Y1;
	}
	else
	{
		double dM1, dM2, dB1, dB2;
		GetSlope( dM1 );
		// slope of perpendicular line is negative inverse
		dM2 = -1 / dM1;
		GetYIntercept( dB1 );
		// calculate y intercept of new line
		dB2 = dY1 - dM2 * dX1;
		// calculate intercept of the two lines knowing 
		// slopes and y-intercepts
		dY2 = ( dB1 * dM2 - dB2 * dM1 ) / ( dM2 - dM1 );
		dX2 = ( dY2 - dB2 ) / dM2;
	}
	CLinear lnOut( dX1, dY1, dX2, dY2 );
	return lnOut;
} // GetPerpendicularLine

/////////////////////////////////////////////////////////////////////////////
// given a line, return the x and y co-ordinates where it intercepts
// this line and returns false if they do not intercept
bool CLinear::GetLineIntercept( CLinear& refLine, double& dX, double& dY )
{
	const double dRun1 = Run;
	const double dRun2 = refLine.Run;
	const double dRise1 = Rise;
	const double dRise2 = refLine.Rise;
	double dM1, dM2, dB1, dB2;

	if ( dRun1 == 0 ) // internal vertical line
	{
		dX = X1; // we know x from vertical line
		if ( dRun2 == 0 ) // external vertical line
		{
			return false; // two vertical lines
		}
		else if ( dRise2 == 0 ) // external horizontal line
		{
			dY = refLine.Y1;
		}
		else // external line normal
		{
			refLine.GetSlope( dM2 );
			refLine.GetYIntercept( dB2 );
			dY = dM2 * dX + dB2;
		}
	}
	else if ( dRise1 == 0 ) // internal horizontal line
	{
		dY = Y1; // we know y from horizontal line
		if ( dRun2 == 0 ) // external vertical line
		{
			dX = refLine.X1; // we know x from vertical line
		}
		else if ( dRise2 == 0 ) // external horizontal line
		{
			return false; // two horizontal lines
		}
		else // external line normal
		{
			refLine.GetSlope( dM2 );
			refLine.GetYIntercept( dB2 );
			dX = ( dY - dB2 ) / dM2;
		}
	}
	else // internal line normal
	{
		if ( dRun2 == 0 ) // external vertical line
		{
			GetSlope( dM1 );
			GetYIntercept( dB1 );
			dX = refLine.X1; // we know x from vertical line
			dY = dM1 * dX + dB1;
		}
		else if ( dRise2 == 0 ) // external horizontal line
		{
			GetSlope( dM1 );
			GetYIntercept( dB1 );
			dY = refLine.Y1; // we know y from horizontal line
			dX = ( dY - dB1 ) / dM1;
		}
		else // both lines are normal
		{
			GetSlope( dM1 );
			refLine.GetSlope( dM2 );
			if ( dM1 == dM2 ) // parallel lines
			{
				return false;
			}
			else // two normal lines
			{
				GetYIntercept( dB1 );
				refLine.GetYIntercept( dB2 );
				dY = ( dB1 * dM2 - dB2 * dM1 ) / ( dM2 - dM1 );
				dX = ( dY - dB2 ) / dM2;
			}
		}
	}
	return true;
} // GetLineIntercept

/////////////////////////////////////////////////////////////////////////////
// return the angle in radians from horizontal
double CLinear::GetAngleInRadians()
{
	const double dHyp = Length;
	if ( dHyp == 0 )
		return 0;
	const double dOpp = UpIncrement * Rise;
	const double dAdj = Run;

	double dAngle;
	if ( dAdj == 0 )
	{
		if ( dOpp > 0 )
		{
			dAngle = PI / 2;
		}
		else
		{
			dAngle = -PI / 2;
		}
	}
	else if ( dOpp == 0 )
	{
		if ( dAdj > 0 )
		{
			dAngle = 0;
		}
		else
		{
			dAngle = PI;
		}
	}
	else
	{
		const double dTan = dOpp / dAdj;
		dAngle = atan( dTan );

		if ( dAdj < 0 )
		{
			if ( dOpp < 0 )
			{
				dAngle -= PI;
			}
			else
			{
				dAngle += PI;
			}
		}
	}
	return dAngle;
} // GetAngleInRadians

/////////////////////////////////////////////////////////////////////////////
// Second point will be relocated by rotating it by dRotation degrees
// with the first point being the center of the rotation
void CLinear::RotateAroundFirstPoint( double dRotation )
{
	const double dHyp = Length;
	if ( dHyp == 0 ) // nothing to do
		return;
	const double dRotationRadians = ConvertDegreesToRadians( dRotation );
	const double dLineAngle = Degrees;
	const double dRadians = Radians + dRotationRadians;
	const double dSine = sin( dRadians );
	const double dCosine = cos( dRadians );
	const double dOpp = dHyp * dSine; // new rise
	const double dAdj = dHyp * dCosine; // new run
	X2 = X1 + dAdj;
	Y2 = Y1 + UpIncrement * dOpp;

} // RotateAroundFirstPoint

/////////////////////////////////////////////////////////////////////////////
// Rotate the segment around a given co-ordinate and angle in degrees
void CLinear::RotateAroundPoint( double dX, double dY, double dRotation )
{
	CLinear lineVector1( dX, dY, X1, Y1 );
	lineVector1.RotateAroundFirstPoint( dRotation );
	CLinear lineVector2( dX, dY, X2, Y2 );
	lineVector2.RotateAroundFirstPoint( dRotation );
	X1 = lineVector1.X2;
	Y1 = lineVector1.Y2;
	X2 = lineVector2.X2;
	Y2 = lineVector2.Y2;
} // RotateAroundPoint

/////////////////////////////////////////////////////////////////////////////
// Rotate the segment around a given point and angle in degrees
void CLinear::RotateAroundPoint( CPoint pt, double dRotation )
{
	const double dX = pt.x;
	const double dY = pt.y;

	RotateAroundPoint( dX, dY, dRotation );
} // RotateAroundPoint

/////////////////////////////////////////////////////////////////////////////
