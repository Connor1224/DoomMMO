// TestGame.h : main header file for the TESTGAME application
//

#if !defined(AFX_TESTGAME_H__E110ED62_0ABF_4862_928B_9490A0DF222E__INCLUDED_)
#define AFX_TESTGAME_H__E110ED62_0ABF_4862_928B_9490A0DF222E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CTestGameApp:
// See TestGame.cpp for the implementation of this class
//

class CTestGameApp : public CWinApp
{
public:
	CTestGameApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTestGameApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CTestGameApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TESTGAME_H__E110ED62_0ABF_4862_928B_9490A0DF222E__INCLUDED_)
