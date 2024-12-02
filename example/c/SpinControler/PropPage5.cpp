#include "stdafx.h"
#include "SpinControler.h"
#include "PropPage5.h"



IMPLEMENT_DYNAMIC(CPropPage5, CPropertyPage)

CPropPage5::CPropPage5()
	: CPropertyPage(CPropPage5::IDD)
	, iSelect(0)
{

}

CPropPage5::~CPropPage5()
{
}

void CPropPage5::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REPORTLIST, cRepList);
}


BEGIN_MESSAGE_MAP(CPropPage5, CPropertyPage)
	ON_WM_CTLCOLOR()
	ON_LBN_SELCHANGE(IDC_REPORTLIST, &CPropPage5::OnLbnSelchangeReportlist)
END_MESSAGE_MAP()



HBRUSH CPropPage5::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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

void CPropPage5::OnLbnSelchangeReportlist()
{
	iSelect = cRepList.GetCurSel();

	UpdateData(FALSE);
}

void CPropPage5::ClearList(void)
{
	for(int i=0; i<=cRepList.GetCount(); i++)
		cRepList.DeleteString(i);
}
