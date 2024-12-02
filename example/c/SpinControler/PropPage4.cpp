// PropPage4.cpp: Implementierungsdatei
//

#include "stdafx.h"
#include "SpinControler.h"
#include "PropPage4.h"


// CPropPage4-Dialogfeld

IMPLEMENT_DYNAMIC(CPropPage4, CPropertyPage)

CPropPage4::CPropPage4()
	: CPropertyPage(CPropPage4::IDD)
{

}

CPropPage4::~CPropPage4()
{
}

void CPropPage4::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SPINSPEED, cSpinSpeed);
}


BEGIN_MESSAGE_MAP(CPropPage4, CPropertyPage)
	ON_WM_CTLCOLOR()
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SPINSPEED, &CPropPage4::OnNMCustomdrawSpinspeed)
END_MESSAGE_MAP()


// CPropPage4-Meldungshandler

HBRUSH CPropPage4::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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

void CPropPage4::OnNMCustomdrawSpinspeed(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);

	CString temp;
	temp.Format(_T("%d"), cSpinSpeed.GetPos());

	GetDlgItem(IDC_STATIC_SPEED)->SetWindowText(temp);
	*pResult = 0;
}
