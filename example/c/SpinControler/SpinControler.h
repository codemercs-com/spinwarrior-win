// SpinControler.h : Hauptheaderdatei für die PROJECT_NAME-Anwendung
//

#pragma once

#ifndef __AFXWIN_H__
	#error "\"stdafx.h\" vor dieser Datei für PCH einschließen"
#endif

#include "resource.h"		// Hauptsymbole


// CSpinControlerApp:
// Siehe SpinControler.cpp für die Implementierung dieser Klasse
//

class CSpinControlerApp : public CWinApp
{
public:
	CSpinControlerApp();

// Überschreibungen
	public:
	virtual BOOL InitInstance();

// Implementierung

	DECLARE_MESSAGE_MAP()
};

extern CSpinControlerApp theApp;