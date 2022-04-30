
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
	AngleInDegrees = -90;
	Velocity = -1022; // meters per second
	VelocityX = 0; // meters per second
	VelocityY = -1022; // meters per second
	MetersToMoon = 382500000; // meters
	DistanceScale = 100000000;
	MoonX = MetersToMoon;
	MoonY = 0;
	StartX = MoonX;
	StartY = 0;
	SampleTime = 1; // seconds
	const double dSamplesPerDay = SamplesPerDay;
	RunningTime = 0; // seconds
	MassOfTheEarth = 5.983e24; // kg
	const double dLunarPeriod = LunarPeriod;
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
