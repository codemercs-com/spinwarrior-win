#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CPropPage4-Dialogfeld

class CPropPage4 : public CPropertyPage
{
	DECLARE_DYNAMIC(CPropPage4)

public:
	CPropPage4();
	virtual ~CPropPage4();

// Dialogfelddaten
	enum { IDD = IDD_PROPPAGE4 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung

	DECLARE_MESSAGE_MAP()
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	CSliderCtrl cSpinSpeed;
	afx_msg void OnNMCustomdrawSpinspeed(NMHDR *pNMHDR, LRESULT *pResult);
};
