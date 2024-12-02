#pragma once
#include "afxwin.h"


// CPropPage3-Dialogfeld

class CPropPage3 : public CPropertyPage
{
	DECLARE_DYNAMIC(CPropPage3)

public:
	CPropPage3();
	virtual ~CPropPage3();

// Dialogfelddaten
	enum { IDD = IDD_PROPPAGE3 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung

	DECLARE_MESSAGE_MAP()
public:
	CComboBox cButtonBox[7];
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	CComboBox cLevelButton[2];
	afx_msg void OnBnClickedAppChoose();
	CString sAppPath;
	CComboBox cButtonApp;
};
