// PropPage2.cpp: Implementierungsdatei
//

#include "stdafx.h"
#include "SpinControler.h"
#include "PropPage2.h"


// CPropPage2-Dialogfeld

IMPLEMENT_DYNAMIC(CPropPage2, CPropertyPage)

CPropPage2::CPropPage2()
	: CPropertyPage(CPropPage2::IDD)
{

}

CPropPage2::~CPropPage2()
{
}

void CPropPage2::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_COMBO13, cSpinBox[0]);
	DDX_Control(pDX, IDC_COMBO14, cSpinBox[1]);
	DDX_Control(pDX, IDC_COMBO15, cSpinBox[2]);
	DDX_Control(pDX, IDC_COMBO16, cSpinBox[3]);
	DDX_Control(pDX, IDC_COMBO17, cSpinBox[4]);
	DDX_Control(pDX, IDC_COMBO18, cSpinBox[5]);
	DDX_Control(pDX, IDC_COMBO19, cSpinBox[6]);
	DDX_Control(pDX, IDC_COMBO20, cSpinBox[7]);
	DDX_Control(pDX, IDC_COMBO21, cSpinBox[8]);
	DDX_Control(pDX, IDC_COMBO22, cSpinBox[9]);
	DDX_Control(pDX, IDC_COMBO23, cSpinBox[10]);
	DDX_Control(pDX, IDC_COMBO24, cSpinBox[11]);
}


BEGIN_MESSAGE_MAP(CPropPage2, CPropertyPage)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CPropPage2-Meldungshandler

HBRUSH CPropPage2::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	//HBRUSH hbr = CPropertyPage::OnCtlColor(pDC, pWnd, nCtlColor);

	HBRUSH hbr = CreateSolidBrush(RGB(255, 255, 255));

	if(pWnd->GetDlgCtrlID() == IDC_STATIC)
		pDC->SetBkMode(TRANSPARENT);

	if(pWnd->GetDlgCtrlID() == IDC_COMBO1)
	{
		pDC->SetBkMode(OPAQUE);
		pDC->SetBkColor(RGB(255,255,255));
	}
	return hbr;
}
