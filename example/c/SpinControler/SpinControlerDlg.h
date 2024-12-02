// SpinControlerDlg.h : Headerdatei
//

#pragma once
#include "spinkit.h"
#include "afxcmn.h"
#include "proppage1.h"
#include "proppage2.h"
#include "proppage3.h"
#include "proppage4.h"
#include "proppage5.h"



// CSpinControlerDlg-Dialogfeld
class CSpinControlerDlg : public CDialog
{
// Konstruktion
public:
	CSpinControlerDlg(CWnd* pParent = NULL);	// Standardkonstruktor

// Dialogfelddaten
	enum { IDD = IDD_SPINCONTROLER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV-Unterstützung


// Implementierung
protected:
	HICON m_hIcon;

	// Generierte Funktionen für die Meldungstabellen
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnWindowPosChanging(WINDOWPOS* lpwndpos);
	afx_msg LRESULT OnDeviceChange(WPARAM wParam, LPARAM lParam); //Devicechange (PlugIn - PlugOut)
	afx_msg LRESULT OnTrayNotification(WPARAM wParam, LPARAM lParam); //TrayIcon
	afx_msg void OnFileExit();
	afx_msg void OnFileSetup();
	afx_msg void OnTcnSelchangingTabMenu(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTcnSelchangeTabMenu(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnBnClickedExit();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedSave();
	afx_msg void OnBnClickedClose();

	void OnMini(void);
	void HidToKeyboard(BYTE Msg, BYTE Event);
	void CreateComboBox(void);
	void WriteInArray(CString path);
	void ReadConfig(CString path);
	void SaveConfig(CString path);
	void Initialize(void);
	void SearchSpins(void);

	CString GetPathString(void);
	CString GetHex(CString string);
	CString GetName(CString string);
	UCHAR ConvStrToByte(CString value);

	SPINKIT_HANDLE GetActiveHandle(CString serial, ULONG prodID);
	void ChooseTimer(ULONG pid);

	int iSpinSave[7];
	int iSpinCall[7];
	int iSpinCount[7];
	int iSpinButton[7];
	int ButtonCount[11];
	int iCount1;
	int iSelect;

	BOOL bSpinButton[7];

	SPINKIT_HANDLE spinHandle;
	SPINKIT_HANDLE spinHandles[SPINKIT_MAX_DEVICES];
	SPINKIT_HANDLE activeHandle;
	
	BOOL bVisible;
	CTabCtrl cTabMenu;
	UCHAR AllValues[48];

	CString sSerial;
	ULONG iProductID;

	CPropPage1 cPropPage1;
	CPropPage2 cPropPage2;
	CPropPage3 cPropPage3;
	CPropPage4 cPropPage4;
	CPropPage5 cPropPage5;

	CStringArray ArrayName;
	CStringArray ArrayHex;
};
