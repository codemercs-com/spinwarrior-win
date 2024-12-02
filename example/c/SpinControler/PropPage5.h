#pragma once
#include "afxwin.h"


// CPropPage5-Dialogfeld

class CPropPage5 : public CPropertyPage
{
	DECLARE_DYNAMIC(CPropPage5)

public:
	CPropPage5();
	virtual ~CPropPage5();

// Dialogfelddaten
	enum { IDD = IDD_PROPPAGE5 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung

	DECLARE_MESSAGE_MAP()
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	CListBox cRepList;
	afx_msg void OnLbnSelchangeReportlist();
	int iSelect;
	void ClearList(void);
};
