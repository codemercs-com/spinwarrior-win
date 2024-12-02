#pragma once


// CPropPage2-Dialogfeld

class CPropPage2 : public CPropertyPage
{
	DECLARE_DYNAMIC(CPropPage2)

public:
	CPropPage2();
	virtual ~CPropPage2();

// Dialogfelddaten
	enum { IDD = IDD_PROPPAGE2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung

	DECLARE_MESSAGE_MAP()
public:
	CComboBox cSpinBox[12];
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
