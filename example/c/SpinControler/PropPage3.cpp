// PropPage3.cpp: Implementierungsdatei
//

#include "stdafx.h"
#include "SpinControler.h"
#include "PropPage3.h"


// CPropPage3-Dialogfeld

IMPLEMENT_DYNAMIC(CPropPage3, CPropertyPage)

CPropPage3::CPropPage3()
	: CPropertyPage(CPropPage3::IDD)
	, sAppPath(_T(""))
{

}

CPropPage3::~CPropPage3()
{
}

void CPropPage3::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, cButtonBox[0]);
	DDX_Control(pDX, IDC_COMBO2, cButtonBox[1]);
	DDX_Control(pDX, IDC_COMBO3, cButtonBox[2]);
	DDX_Control(pDX, IDC_COMBO4, cButtonBox[3]);
	DDX_Control(pDX, IDC_COMBO5, cButtonBox[4]);
	DDX_Control(pDX, IDC_COMBO6, cButtonBox[5]);
	DDX_Control(pDX, IDC_COMBO7, cButtonBox[6]);

	DDX_Control(pDX, IDC_COMBO8, cLevelButton[0]);
	DDX_Control(pDX, IDC_COMBO9, cLevelButton[1]);
	DDX_Control(pDX, IDC_COMBO10, cButtonApp);
}


BEGIN_MESSAGE_MAP(CPropPage3, CPropertyPage)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_APP_CHOOSE, &CPropPage3::OnBnClickedAppChoose)
END_MESSAGE_MAP()


// CPropPage3-Meldungshandler

HBRUSH CPropPage3::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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
void CPropPage3::OnBnClickedAppChoose()
{
	CFileDialog cDialog(TRUE, 0, 0, 4|2, _T("Exec (*.exe) |*.exe||"), 0 ,0);
	//CString Wert;

	if(cDialog.DoModal() == IDOK)
	{
		sAppPath.Format(_T("%s"), cDialog.GetPathName());
		GetDlgItem(IDC_STATIC_APP)->SetWindowTextW(sAppPath);
				
		UpdateData(FALSE);
	}
}
