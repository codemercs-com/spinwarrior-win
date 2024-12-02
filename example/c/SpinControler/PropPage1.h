#pragma once
#include "afxwin.h"


// CPropPage1-Dialogfeld

class CPropPage1 : public CPropertyPage
{
	DECLARE_DYNAMIC(CPropPage1)

public:
	CPropPage1();
	virtual ~CPropPage1();

// Dialogfelddaten
	enum { IDD = IDD_PROPPAGE1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung

	DECLARE_MESSAGE_MAP()
public:
	CComboBox cSpinBox[12];
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//CString sSpinBox[12];
};
