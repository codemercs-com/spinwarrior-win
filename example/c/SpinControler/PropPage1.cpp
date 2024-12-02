// PropPage1.cpp: Implementierungsdatei
//

#include "stdafx.h"
#include "SpinControler.h"
#include "PropPage1.h"


// CPropPage1-Dialogfeld

IMPLEMENT_DYNAMIC(CPropPage1, CPropertyPage)

CPropPage1::CPropPage1()
	: CPropertyPage(CPropPage1::IDD)
{

}

CPropPage1::~CPropPage1()
{
}

void CPropPage1::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, cSpinBox[0]);
	DDX_Control(pDX, IDC_COMBO2, cSpinBox[1]);
	DDX_Control(pDX, IDC_COMBO3, cSpinBox[2]);
	DDX_Control(pDX, IDC_COMBO4, cSpinBox[3]);
	DDX_Control(pDX, IDC_COMBO5, cSpinBox[4]);
	DDX_Control(pDX, IDC_COMBO6, cSpinBox[5]);
	DDX_Control(pDX, IDC_COMBO7, cSpinBox[6]);
	DDX_Control(pDX, IDC_COMBO8, cSpinBox[7]);
	DDX_Control(pDX, IDC_COMBO9, cSpinBox[8]);
	DDX_Control(pDX, IDC_COMBO10, cSpinBox[9]);
	DDX_Control(pDX, IDC_COMBO11, cSpinBox[10]);
	DDX_Control(pDX, IDC_COMBO12, cSpinBox[11]);

	/*DDX_CBString(pDX, IDC_COMBO1, sSpinBox[0]);
	DDX_CBString(pDX, IDC_COMBO2, sSpinBox[1]);
	DDX_CBString(pDX, IDC_COMBO3, sSpinBox[2]);
	DDX_CBString(pDX, IDC_COMBO4, sSpinBox[3]);
	DDX_CBString(pDX, IDC_COMBO5, sSpinBox[4]);
	DDX_CBString(pDX, IDC_COMBO6, sSpinBox[5]);
	DDX_CBString(pDX, IDC_COMBO7, sSpinBox[6]);
	DDX_CBString(pDX, IDC_COMBO8, sSpinBox[7]);
	DDX_CBString(pDX, IDC_COMBO9, sSpinBox[8]);
	DDX_CBString(pDX, IDC_COMBO10, sSpinBox[9]);
	DDX_CBString(pDX, IDC_COMBO11, sSpinBox[10]);
	DDX_CBString(pDX, IDC_COMBO12, sSpinBox[11]);*/
	
}


BEGIN_MESSAGE_MAP(CPropPage1, CPropertyPage)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CPropPage1-Meldungshandler

HBRUSH CPropPage1::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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
