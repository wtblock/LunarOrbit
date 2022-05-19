
/////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2022 by W. T. Block, All Rights Reserved
/////////////////////////////////////////////////////////////////////////////

#pragma once

/////////////////////////////////////////////////////////////////////////////
class CBaseDoc : public CDocument
{
protected: // create from serialization only
	CBaseDoc();
	DECLARE_DYNCREATE(CBaseDoc)

	// properties
public:
	// logical pixels per inch
	static int GetMap()
	{
		return 1000;
	}
	// logical pixels per inch
	__declspec( property( get = GetMap ) )
		int Map;

	// number of pages in the document
	virtual UINT GetPages()
	{
		return 1;
	}
	// number of pages in the document
	__declspec( property( get = GetPages ) )
		UINT Pages;

	// height of document in inches
	virtual double GetHeight()
	{
		return 8.5;
	}
	// height of document in inches
	__declspec( property( get = GetHeight ) )
		double Height;

	// width of document in inches
	virtual double GetWidth()
	{
		return 11.0;
	}
	// width of document in inches
	__declspec( property( get = GetWidth ) )
		double Width;

	// margin of document in inches
	virtual double GetMargin()
	{
		return 0.25;
	}
	// margin of document in inches
	__declspec( property( get = GetMargin ) )
		double Margin;

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

	// Operations
public:

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// Implementation
public:
	virtual ~CBaseDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};
