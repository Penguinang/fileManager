
// ManagerGUI.h : main header file for the ManagerGUI application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CManagerGUIApp:
// See ManagerGUI.cpp for the implementation of this class
//

class CManagerGUIApp : public CWinApp
{
public:
	CManagerGUIApp() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CManagerGUIApp theApp;
