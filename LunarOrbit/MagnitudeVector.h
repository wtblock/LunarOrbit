/////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2022 by W. T. Block, All Rights Reserved
/////////////////////////////////////////////////////////////////////////////
#pragma once
#include "Linear.h"

/////////////////////////////////////////////////////////////////////////////
// a class representing a magnitude and direction where the base CLinear
// class controls the direction
class CMagnitudeVector : public CLinear
{
	// public definitions
public:

// protected data
protected:
	// magnitude of the vector
	double m_dRealMagnitude;

	// units of magnitude
	CString m_csEngineeringUnits;

	// description of the value
	CString m_csDescription;

	// length of arrowhead in inches
	double m_dArrowheadLength;

	// angle in degrees of the arrowhead with respect to vector
	// (rotated around second point of vector)
	double m_dArrowheadAngle;

	// draw an arc of the angle
	bool m_bDrawArc;

	// thickness of the vector in inches
	double m_dThickness;

	// text height in inches
	double m_dTextHeight;

	// color of the vector
	COLORREF m_rgbColor;

	// number of decimal places for magnitude text output
	int m_nDecimals;

	// numerical format character for magnitude text: 
	// "i": integer 
	// "e": exponential
	// "f": floating point
	// "g": automatically picks between "f" and "e"
	CString m_csNumericalFormat;

// public properties
public:
	// value associated with the vector
	inline double GetRealMagnitude()
	{
		return m_dRealMagnitude;
	}
	// value associated with the vector
	inline void SetRealMagnitude( double value )
	{
		m_dRealMagnitude = value;
	}
	// value associated with the vector
	__declspec( property( get = GetRealMagnitude, put = SetRealMagnitude  ) )
		double RealMagnitude;

	// units of magnitude
	inline CString GetEngineeringUnits()
	{
		return m_csEngineeringUnits;
	}
	// units of magnitude
	inline void SetEngineeringUnits( CString value )
	{
		m_csEngineeringUnits = value;
	}
	// units of magnitude
	__declspec( property( get = GetEngineeringUnits, put = SetEngineeringUnits  ) )
		CString EngineeringUnits;

	// description of the value
	inline CString GetDescription()
	{
		return m_csDescription;
	}
	// description of the value
	inline void SetDescription( CString value )
	{
		m_csDescription = value;
	}
	// description of the value
	__declspec( property( get = GetDescription, put = SetDescription  ) )
		CString Description;

	// length of arrowhead in inches
	inline double GetArrowheadLength()
	{
		return m_dArrowheadLength;
	}
	// length of arrowhead in inches
	inline void SetArrowheadLength( double value )
	{
		m_dArrowheadLength = value;
	}
	// length of arrowhead in inches
	__declspec( property( get = GetArrowheadLength, put = SetArrowheadLength  ) )
		double ArrowheadLength;

	// angle in degrees of the arrowhead with respect to vector
	// (rotated around second point of vector)
	inline double GetArrowheadAngle()
	{
		return m_dArrowheadAngle;
	}
	// angle in degrees of the arrowhead with respect to vector
	// (rotated around second point of vector)
	inline void SetArrowheadAngle( double value )
	{
		m_dArrowheadAngle = value;
	}
	// angle in degrees of the arrowhead with respect to vector
	// (rotated around second point of vector)
	__declspec( property( get = GetArrowheadAngle, put = SetArrowheadAngle ) )
		double ArrowheadAngle;

	// draw an arc of the angle
	inline bool GetDrawArc()
	{
		return m_bDrawArc;
	}
	// draw an arc of the angle
	inline void SetDrawArc( bool value )
	{
		m_bDrawArc = value;
	}
	// draw an arc of the angle
	__declspec( property( get = GetDrawArc, put = SetDrawArc  ) )
		bool DrawArc;

	// thickness of the vector in inches
	inline double GetThickness()
	{
		return m_dThickness;
	}
	// thickness of the vector in inches
	inline void SetThickness( double value )
	{
		m_dThickness = value;
	}
	// thickness of the vector in inches
	__declspec( property( get = GetThickness, put = SetThickness  ) )
		double Thickness;

	// text height in inches
	inline double GetTextHeight()
	{
		return m_dTextHeight;
	}
	// text height in inches
	inline void SetTextHeight( double value )
	{
		m_dTextHeight = value;
	}
	// text height in inches
	__declspec( property( get = GetTextHeight, put = SetTextHeight  ) )
		double TextHeight;

	// color of the vector
	inline COLORREF GetColor()
	{
		return m_rgbColor;
	}
	// color of the vector
	inline void SetColor( COLORREF value )
	{
		m_rgbColor = value;
	}
	// color of the vector
	__declspec( property( get = GetColor, put = SetColor  ) )
		COLORREF Color;

	// number of decimal places for magnitude text output
	inline int GetDecimals()
	{
		return m_nDecimals;
	}
	// number of decimal places for magnitude text output
	inline void SetDecimals( int value )
	{
		m_nDecimals = value;
	}
	// number of decimal places for magnitude text output
	__declspec( property( get = GetDecimals, put = SetDecimals  ) )
		int Decimals;

	// numerical format character for magnitude text: 
	// "i": integer 
	// "e": exponential
	// "f": floating point
	// "g": automatically picks between "f" and "e"
	inline CString GetNumericalFormat()
	{
		return m_csNumericalFormat;
	}
	// numerical format character for magnitude text: 
	// "i": integer 
	// "e": exponential
	// "f": floating point
	// "g": automatically picks between "f" and "e"
	inline void SetNumericalFormat( CString value )
	{
		m_csNumericalFormat = value;
	}
	// numerical format character for magnitude text: 
	// "i": integer 
	// "e": exponential
	// "f": floating point
	// "g": automatically picks between "f" and "e"
	__declspec( property( get = GetNumericalFormat, put = SetNumericalFormat ) )
		CString NumericalFormat;

	// get the X vector
	CMagnitudeVector GetVectorX()
	{
		// copy this vector to the return value
		CMagnitudeVector value = *this;

		// default to not drawing the arc
		value.DrawArc = false;

		// the X vector is horizontal so the Y values are the same
		value.Y2 = Y1;

		// the absolute length of the current vector
		const double dLength = Length;

		// check for zero length
		if ( !NearlyEqual( dLength, 0.0 ) )
		{
			// the direction of X
			const int nDir = DirectionX;

			// length of the X vector
			const double dLengthX = nDir * value.Length;

			// the new real magnitude is the proportion of the lengths
			value.RealMagnitude = RealMagnitude * dLengthX / dLength;
		}

		return value;
	}
	// get the X vector
	__declspec( property( get = GetVectorX ) ) CMagnitudeVector VectorX;

	// get the Y vector
	CMagnitudeVector GetVectorY()
	{
		// copy this vector to the return value
		CMagnitudeVector value = *this;

		// default to not drawing the arc
		value.DrawArc = false;

		// the Y vector is vertical so the X values are the same
		value.X2 = X1;

		// the absolute length of the current vector
		const double dLength = Length;

		// check for a zero length
		if ( !NearlyEqual( dLength, 0.0 ) )
		{
			// the direction of Y
			const int nDir = DirectionY;

			// length of the Y vector
			const double dLengthY = nDir * value.Length;

			// the new real magnitude is the proportion of the lengths
			value.RealMagnitude = RealMagnitude * dLengthY / dLength;
		}

		return value;
	}
	// get the Y vector
	__declspec( property( get = GetVectorY ) ) CMagnitudeVector VectorY;

	// get the label text
	CString GetLabel()
	{
		// description in front of the magnitude value
		const CString csDesc = Description;

		// number of decimal places for the magnitude
		const int nDeci = Decimals;

		// numerical format characters
		const CString csForm = NumericalFormat;

		// the engineering units of the magnitude
		const CString csUnits = EngineeringUnits;

		// the magnitude value
		const double dMag = RealMagnitude;

		// build a format string from description, numerical format, units, 
		// and decimal places
		CString csFormat;
		csFormat.Format
		(
			_T( "%s=%%0.%d%s %s" ),
			csDesc, nDeci, csForm, csUnits
		);

		// format the text label using above format string and the 
		// real magnitude
		CString value;
		value.Format( csFormat, dMag );

		return value;
	}
	// get the label text
	__declspec( property( get = GetLabel ) )
		CString Label;

// protected methods
protected:
	// returns a line representing one side of the arrowhead
	// drawn at a positive or negative angle offset from the vector's
	// angle depending on the parameter value given
	CLinear CreateArrowhead( bool bPositiveAngle );

// public methods
public:
	// copies an input vector to this vector
	CMagnitudeVector operator=( CMagnitudeVector& in )
	{
		X1 = in.X1;
		X2 = in.X2;
		Y1 = in.Y1;
		Y2 = in.Y2;
		UpIncrement = in.UpIncrement;
		Map = in.Map;
		RealMagnitude = in.RealMagnitude;
		EngineeringUnits = in.EngineeringUnits;
		Description = in.Description;
		ArrowheadLength = in.ArrowheadLength;
		ArrowheadAngle = in.ArrowheadAngle;
		DrawArc = in.DrawArc;
		Thickness = in.Thickness;
		TextHeight = in.TextHeight;
		Color = in.Color;
		Decimals = in.Decimals;
		NumericalFormat = in.NumericalFormat;

		return *this;
	}

	// draw the vector on given device context
	void DrawVector( CDC* pDC );

	// draw the magnitude on given device context
	void DrawMagnitude( CDC* pDC );

	// draw on given device context
	void Draw( CDC* pDC );

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
	);
	
// protected overrides
protected:

// public overrides
public:

// public constructor
public:
	// constructor
	CMagnitudeVector()
	{
		ArrowheadAngle = 15; // 15 degree offset from the vector angle
		ArrowheadLength = 0.1; // length of arrowhead in inches
		RealMagnitude = 0; // default magnitude of the vector
		Color = RGB( 255, 0, 0 ); // red
		Thickness = 0.02; // inches
		TextHeight = 0.18; // inches
		Decimals = 3; // three decimal places of precision
		NumericalFormat = _T( "e" ); // exponential floating point format
		DrawArc = true;
	}
	// destructor
	virtual ~CMagnitudeVector()
	{

	}
};

