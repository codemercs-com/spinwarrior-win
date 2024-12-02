// SpinControlerDlg.cpp : Implementierungsdatei
//

#include "stdafx.h"
#include "SpinControler.h"
#include "SpinControlerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define WM_NOTIFICATION (WM_APP+100)

#define START_PUSH 0
#define END_PUSH KEYEVENTF_KEYUP


// CSpinControlerDlg-Dialogfeld




CSpinControlerDlg::CSpinControlerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSpinControlerDlg::IDD, pParent)
	, iSelect(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSpinControlerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB_MENU, cTabMenu);
}

BEGIN_MESSAGE_MAP(CSpinControlerDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(WM_NOTIFICATION, &CSpinControlerDlg::OnTrayNotification)
	ON_MESSAGE(WM_DEVICECHANGE, &CSpinControlerDlg::OnDeviceChange)
	//}}AFX_MSG_MAP
	ON_WM_WINDOWPOSCHANGING()
	ON_COMMAND(ID_FILE_EXIT, &CSpinControlerDlg::OnFileExit)
	ON_COMMAND(ID_FILE_SETUP, &CSpinControlerDlg::OnFileSetup)
	ON_WM_WINDOWPOSCHANGED()
	ON_NOTIFY(TCN_SELCHANGING, IDC_TAB_MENU, &CSpinControlerDlg::OnTcnSelchangingTabMenu)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_MENU, &CSpinControlerDlg::OnTcnSelchangeTabMenu)
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_EXIT, &CSpinControlerDlg::OnBnClickedExit)
	ON_WM_CTLCOLOR()
	ON_WM_DRAWITEM()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_SAVE, &CSpinControlerDlg::OnBnClickedSave)
	ON_BN_CLICKED(IDC_CLOSE, &CSpinControlerDlg::OnBnClickedClose)
END_MESSAGE_MAP()


// CSpinControlerDlg-Meldungshandler

BOOL CSpinControlerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetIcon(m_hIcon, TRUE);
	SetIcon(m_hIcon, FALSE);

	memset(&ButtonCount, 0, 11);
	memset(&AllValues, 0, 48);
	memset(&iSpinButton, 0, 7);
	memset(&bSpinButton, 0, 7);
	memset(&iSpinSave, 0, 7);
	memset(&iSpinCall, 0, 7);
	memset(&ButtonCount, 0, 8);


	Initialize();
	WriteInArray(GetPathString());
	CreateComboBox();
	ReadConfig(GetPathString()); //_T("config.cfg")

	SearchSpins();


	activeHandle = GetActiveHandle(sSerial, iProductID);//spinHandles[0];

	ChooseTimer(iProductID);

	bVisible = FALSE;
	OnMini();

	

	UpdateData(FALSE);

	return TRUE;
}

void CSpinControlerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // Gerätekontext zum Zeichnen

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Symbol in Clientrechteck zentrieren
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Symbol zeichnen
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

HCURSOR CSpinControlerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

HBRUSH CSpinControlerDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	//HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	HBRUSH hbr = CreateSolidBrush(RGB(128, 184, 223));

	if(pWnd->GetDlgCtrlID() == IDC_STATIC)
		pDC->SetBkMode(TRANSPARENT);


	return hbr;
}
LRESULT CSpinControlerDlg::OnDeviceChange(WPARAM wParam, LPARAM lParam)
{
	SearchSpins();
	return LRESULT();
}

void CSpinControlerDlg::OnTcnSelchangingTabMenu(NMHDR *pNMHDR, LRESULT *pResult)
{
	iSelect = cTabMenu.GetCurSel();

    switch(iSelect)
    {
    case 0:
        cPropPage1.ShowWindow(SW_HIDE);
        break;
	case 1:
        cPropPage2.ShowWindow(SW_HIDE);
        break;
	case 2:
        cPropPage3.ShowWindow(SW_HIDE);
        break;
	case 3:
        cPropPage4.ShowWindow(SW_HIDE);
        break;
	case 4:
        cPropPage5.ShowWindow(SW_HIDE);
        break;
    default:
        break;
    }

	*pResult = 0;
}

void CSpinControlerDlg::OnTcnSelchangeTabMenu(NMHDR *pNMHDR, LRESULT *pResult)
{
	CRect rcItem;
	cTabMenu.GetItemRect(0, &rcItem);

	iSelect = cTabMenu.GetCurSel();

    switch(iSelect)
    {
    case 0:
		cPropPage1.SetWindowPos(&wndTop, rcItem.left+10, rcItem.bottom+15, 0, 0, SWP_NOSIZE | SWP_NOZORDER );
        cPropPage1.ShowWindow(SW_SHOW);
        break;
    case 1:
		cPropPage2.SetWindowPos(&wndTop, rcItem.left+10, rcItem.bottom+15, 0, 0, SWP_NOSIZE | SWP_NOZORDER );
        cPropPage2.ShowWindow(SW_SHOW);
        break;
    case 2:
		cPropPage3.SetWindowPos(&wndTop, rcItem.left+10, rcItem.bottom+15, 0, 0, SWP_NOSIZE | SWP_NOZORDER );
        cPropPage3.ShowWindow(SW_SHOW);
        break;
    case 3:
		cPropPage4.SetWindowPos(&wndTop, rcItem.left+10, rcItem.bottom+15, 0, 0, SWP_NOSIZE | SWP_NOZORDER );
        cPropPage4.ShowWindow(SW_SHOW);
        break;
    case 4:
		cPropPage5.SetWindowPos(&wndTop, rcItem.left+10, rcItem.bottom+15, 0, 0, SWP_NOSIZE | SWP_NOZORDER );
        cPropPage5.ShowWindow(SW_SHOW);
        break;
    default:
        break;
    }

	*pResult = 0;
}

int CSpinControlerDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	if(!cPropPage1.Create(IDD_PROPPAGE1, this))
		return -1;
	if(!cPropPage2.Create(IDD_PROPPAGE2, this))
		return -1;
	if(!cPropPage3.Create(IDD_PROPPAGE3, this))
		return -1;
	if(!cPropPage4.Create(IDD_PROPPAGE4, this))
		return -1;
	if(!cPropPage5.Create(IDD_PROPPAGE5, this))
		return -1;

	return 0;
}

/*Search for all connected SpinWarrior and ompare with saved values from config.cfg*/
SPINKIT_HANDLE CSpinControlerDlg::GetActiveHandle(CString serial, ULONG prodID)
{
	ULONG numDev, pID, i;
	WCHAR sn[9];
	SPINKIT_HANDLE handle = NULL;
	CString temp;

	if(spinHandle != NULL)
	{
		numDev = SpinKitGetNumDevs();

		for(i=0; i<numDev; i++)
		{
			pID = SpinKitGetProductId(spinHandles[i]);
			SpinKitGetSerialNumber(spinHandles[i], sn);
			temp.Format(_T("%s"), sn);

			if((pID == prodID) && (temp == serial))
			{
				handle = spinHandles[i];
				cPropPage5.cRepList.SetCurSel(i);
			//	MessageBox(_T("config"),0,MB_OK);
				break;
			}
		}

		if(handle == NULL)
		{
			handle = spinHandles[0];
			cPropPage5.cRepList.SetCurSel(0);
			//MessageBox(_T("ZERO"),0,MB_OK);
		}
	}
	else
		handle = NULL;

	return handle;
}

void CSpinControlerDlg::Initialize(void)
{
	iCount1 = 0;
	iSelect = 4;
	activeHandle = NULL;
	spinHandle = NULL;
	memset(&spinHandles, NULL, SPINKIT_MAX_DEVICES);

	cTabMenu.InsertItem(0, _T("SpinSet 1"));
	cTabMenu.InsertItem(1, _T("SpinSet 2"));
	cTabMenu.InsertItem(2, _T("Buttons"));
	cTabMenu.InsertItem(3, _T("Different"));
	cTabMenu.InsertItem(4, _T("Informaions"));

	cPropPage4.cSpinSpeed.SetPos(0);
	cPropPage4.cSpinSpeed.SetRange(1,10);

	UpdateData(FALSE);
}

/*Start timer with individual settings*/
void CSpinControlerDlg::ChooseTimer(ULONG pid)
{
	switch(pid)
	{
		case SPINKIT_PID24R4:
		case SPINKIT_PID24R6:
			SetTimer(IDC_TIMER1, 10, FALSE);
			break;
		case SPINKIT_PID24A3:
			SetTimer(IDC_TIMER1, 75, FALSE);
			break;
		default:
			break;
	}
}

/*For make the dialog invisible*/
void CSpinControlerDlg::OnWindowPosChanging(WINDOWPOS* lpwndpos)
{
	CDialog::OnWindowPosChanging(lpwndpos);
	
	if(!bVisible)
		lpwndpos->flags &= ~SWP_SHOWWINDOW;
}

/*Create a tray-icon*/
void CSpinControlerDlg::OnMini()
{
   NOTIFYICONDATA tnd;
   ZeroMemory(&tnd, sizeof (NOTIFYICONDATA));
   tnd.cbSize = sizeof (NOTIFYICONDATA);
   tnd.hWnd = this ->m_hWnd;
   tnd.uID = 1;
   tnd.uFlags = NIF_MESSAGE|NIF_ICON|NIF_TIP;
   tnd.uCallbackMessage = WM_NOTIFICATION;
   tnd.hIcon = (HICON) LoadImage(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDI_ICON2), IMAGE_ICON, 32, 32, LR_SHARED|LR_DEFAULTCOLOR);
   lstrcpyn(tnd.szTip, _T("SpinControl-Center"), sizeof(tnd.szTip));

   Shell_NotifyIcon(NIM_ADD, &tnd);
}

/*Klick-events for tray-icon*/
LRESULT CSpinControlerDlg::OnTrayNotification(WPARAM wp, LPARAM lp)
{
    switch (lp)
    {
	//case WM_RBUTTONUP:
 //       ShowWindow(SW_RESTORE);
 //       NOTIFYICONDATA iconData;
 //       iconData.cbSize = sizeof(NOTIFYICONDATA);
 //       iconData.hWnd = this->GetSafeHwnd();
 //       iconData.uID = 1;
 //       return Shell_NotifyIcon(NIM_DELETE, &iconData);

    case WM_LBUTTONUP:
		CMenu oMenu;
		if (oMenu.LoadMenu(IDR_MENU1))        
		{
			CMenu* pPopup = oMenu.GetSubMenu(0);
			ASSERT(pPopup != NULL);
			CPoint oPoint;

			GetCursorPos(&oPoint);
			SetForegroundWindow();
			pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON,	oPoint.x, oPoint.y, this);
		} 
		break;
	}

	return 0;
}

/*Shutdown programm*/
void CSpinControlerDlg::OnFileExit()
{
    NOTIFYICONDATA iconData;
    iconData.cbSize = sizeof(NOTIFYICONDATA);
    iconData.hWnd = this->GetSafeHwnd();
    iconData.uID = 1;
    Shell_NotifyIcon(NIM_DELETE, &iconData);

	SpinKitCloseDevice(spinHandle);

	bVisible = FALSE;

	OnOK();
}

void CSpinControlerDlg::OnFileSetup()
{
	KillTimer(IDC_TIMER1);

	CRect rcItem;
	cTabMenu.GetItemRect(0, &rcItem);
	
	cTabMenu.SetCurSel(iSelect); //Get last register

	UpdateData(FALSE);

	switch(iSelect)
    {
    case 0:
		cPropPage1.SetWindowPos(&wndTop, rcItem.left+10, rcItem.bottom+15, 0, 0, SWP_NOSIZE | SWP_NOZORDER );
        cPropPage1.ShowWindow(SW_SHOW);
        break;
    case 1:
		cPropPage2.SetWindowPos(&wndTop, rcItem.left+10, rcItem.bottom+15, 0, 0, SWP_NOSIZE | SWP_NOZORDER );
        cPropPage2.ShowWindow(SW_SHOW);
        break;
    case 2:
		cPropPage3.SetWindowPos(&wndTop, rcItem.left+10, rcItem.bottom+15, 0, 0, SWP_NOSIZE | SWP_NOZORDER );
        cPropPage3.ShowWindow(SW_SHOW);
        break;
    case 3:
		cPropPage4.SetWindowPos(&wndTop, rcItem.left+10, rcItem.bottom+15, 0, 0, SWP_NOSIZE | SWP_NOZORDER );
        cPropPage4.ShowWindow(SW_SHOW);
        break;
    case 4:
		cPropPage5.SetWindowPos(&wndTop, rcItem.left+10, rcItem.bottom+15, 0, 0, SWP_NOSIZE | SWP_NOZORDER );
        cPropPage5.ShowWindow(SW_SHOW);
        break;
    default:
        break;
    }

		

	bVisible = TRUE;
	ShowWindow(SW_SHOW);

	//GetDlgItem(IDC_STATIC_APP)->SetWindowTextW(_T("test"));
}


void CSpinControlerDlg::OnBnClickedExit()
{
	if(MessageBox(_T("Anwendung wirklich Beenden?"), _T("Beenden"), IDOK) == IDOK)
		OnFileExit();
}

/*Send keyboard-inputs - also you can use the spinkit-functions (discription found into PDF)*/
void CSpinControlerDlg::HidToKeyboard(BYTE Msg, BYTE Event)
{
	HWND hwnd = ::GetActiveWindow();

	DWORD idAttach   = GetCurrentThreadId();
	DWORD idAttachTo = GetWindowThreadProcessId(hwnd, NULL);

	AttachThreadInput(idAttach, idAttachTo, TRUE);

	INPUT ipSignal;
	ipSignal.type           = INPUT_KEYBOARD;
	ipSignal.ki.wVk         = Msg;
	ipSignal.ki.wScan       = 0;
	ipSignal.ki.dwFlags     = Event;
	ipSignal.ki.time        = 0;
	ipSignal.ki.dwExtraInfo = 0;
	SendInput(1, &ipSignal, sizeof(ipSignal));

	AttachThreadInput(idAttach, idAttachTo, FALSE);
}
/*Fill up the comboboxes with values from virtual_key_codes*/
void CSpinControlerDlg::CreateComboBox(void)
{
	int index = 0;
	CString temp;

	while(index < ArrayName.GetCount())
	{
		for(int i=0; i<12; i++)
		{
			cPropPage1.cSpinBox[i].InsertString(index, ArrayName.GetAt(index));
			cPropPage2.cSpinBox[i].InsertString(index, ArrayName.GetAt(index));
		}

		for(int j=0; j<7; j++)
			cPropPage3.cButtonBox[j].InsertString(index, ArrayName.GetAt(index));

		index++;
	}

	cPropPage3.cLevelButton[0].InsertString(0, _T("None"));
	cPropPage3.cLevelButton[1].InsertString(0, _T("None"));
	for(int j=1; j<8; j++)
	{
		temp.Format(_T("%d"), j);
		cPropPage3.cLevelButton[0].InsertString(j, temp);
		cPropPage3.cLevelButton[1].InsertString(j, temp);
	}

	UpdateData(FALSE);
}

/*Write values from virtual_key_codes into array*/
void CSpinControlerDlg::WriteInArray(CString path)
{
	CString Buffer, Wert, newPath, value;
	int pos;
	CStdioFile File;

	newPath.Format(_T("%s\\%s"), path, _T("virtual_key_codes.cfg"));

	if(File.Open(newPath, CFile::modeRead | CFile::typeBinary))
	{
		File.SeekToBegin();

		while(File.ReadString(Buffer))
		{
			pos = 0;
			Wert.Format(_T("%s"), Buffer);
			pos = Wert.Find(';');
			ArrayName.Add(Wert.Mid(0,pos));
			ArrayHex.Add(Wert.Mid(pos+3, 2));
		}
		File.Close();
	}
	else
	{
		MessageBox(_T("No key-table would be found.\r\nPlease check for 'virtual_key_codes.cfg' in application-path.\r\nThe program will be closed."), _T("Key-Table not found"), MB_OK);
		OnFileExit();
	}
}

/*Get program-path*/
CString CSpinControlerDlg::GetPathString(void)
{
	char save[1024];
	int pos1 = 0;
	CString path;

	GetModuleFileNameW(NULL, (LPWCH) save, 1023);
	path.Format(_T("%s"), save);

	pos1 = path.ReverseFind('\\');

	path.SetString(path.Mid(0, pos1));

	return path;
}

void CSpinControlerDlg::OnTimer(UINT_PTR nIDEvent)
{
	CString temp;
	SPINKIT_DATA SpinData;

	int pos_1[6] = {0,2,4,6,8,10};
	int pos_2[6] = {1,3,5,7,9,11};

	int pos_3[6] = {12,14,16,18,20,22};
	int pos_4[6] = {13,15,17,19,21,23};

	int button[8] = {24,25,26,27,28,29,30};

	if(spinHandle != NULL)
	{
		if(nIDEvent == 108)
		{
			if(SpinKitReadNonBlocking(activeHandle, &SpinData))
			{
				if(bVisible != TRUE) //Keine Tastenevents bei sichtbarem Fenster
				{
					/*Abfrage der 6 möglichen Drehgeber*/
					switch(SpinKitGetProductId(activeHandle)) //<--------- Aus datei lesen oder via "OK" laden !!!
					{
					case SPINKIT_PID24R4:
					case SPINKIT_PID24R6:
						for(int i=0; i<6; i++)
						{
							if ((SpinData.Spins[i] > 0) && (SpinData.Spins[i] < 10))
							{
								iCount1++;
								if(iCount1 == cPropPage4.cSpinSpeed.GetPos())
								{
									iCount1 = 0;

									if((SpinData.Buttons[cPropPage3.cLevelButton[0].GetCurSel()-1] == TRUE) || (SpinData.Buttons[cPropPage3.cLevelButton[1].GetCurSel()-1] == TRUE))
									{
										HidToKeyboard(AllValues[pos_3[i]], START_PUSH);
										HidToKeyboard(AllValues[pos_3[i]], END_PUSH);
									}
									else
									{
										HidToKeyboard(AllValues[pos_1[i]], START_PUSH);
										HidToKeyboard(AllValues[pos_1[i]], END_PUSH);
									}
								}
							}
							/*NEGATIVE*/
							if ((SpinData.Spins[i] < 0) && (SpinData.Spins[i] > -10))
							{
								iCount1++;

								if(iCount1 == cPropPage4.cSpinSpeed.GetPos())
								{
									iCount1 = 0;
									if((SpinData.Buttons[cPropPage3.cLevelButton[0].GetCurSel()-1] == TRUE) || (SpinData.Buttons[cPropPage3.cLevelButton[1].GetCurSel()-1] == TRUE))
									{
										HidToKeyboard(AllValues[pos_4[i]], START_PUSH);
										HidToKeyboard(AllValues[pos_4[i]], END_PUSH);
									}
									else
									{
										HidToKeyboard(AllValues[pos_2[i]], START_PUSH);
										HidToKeyboard(AllValues[pos_2[i]], END_PUSH);
									}
								}
							}
						}

						/*Aufruf der eingestellten Applikation auf dem entsprechenden Button*/
						if(SpinData.Buttons[cPropPage3.cButtonApp.GetCurSel()] == TRUE) //Button gedrückt
						{
							if(ButtonCount[10] < 1) //Button nur Ein Mal gedrückt (Entprellt)
							{
								ButtonCount[10]++; //Hochzählen für Zeitabgleich

								char appli[512];
								WideCharToMultiByte(CP_ACP, NULL, cPropPage3.sAppPath, -1, appli, 255, NULL, NULL);

								WinExec(appli, SW_SHOW);
							}
						}
						if(SpinData.Buttons[cPropPage3.cButtonApp.GetCurSel()] == FALSE) //Button leer
						{
							if(ButtonCount[10] != 0)
							{
								ButtonCount[10] = 0;
							}
						}




						/*Abfrage der 7 möglichen Buttons mit Ausführlimit = 1*/
						for(int i=0;i<8; i++)	
						{





							if(SpinData.Buttons[i] == TRUE) //Button gedrückt
							{
								if(ButtonCount[i] < 1) //Button nur Ein Mal gedrückt (Entprellt)
								{
									ButtonCount[i]++; //Hochzählen für Zeitabgleich
									HidToKeyboard(AllValues[button[i]], START_PUSH); //Tastendruck: "Taste Unten"
								}
							}
							if(SpinData.Buttons[i] == FALSE) //Button leer
							{
								if(ButtonCount[i] != 0)
								{
									ButtonCount[i] = 0;
									HidToKeyboard(AllValues[button[i]], END_PUSH); //Tastendruck: "Taste Oben"
								}
							}
						}
					break;

					case SPINKIT_PID24A3:
						for(int i=0; i<3; i++)
						{
							if(SpinData.Spins[i] != iSpinSave[i]) //Filtern auf "wirklich" neue Daten
							{
								if(iSpinSave[i] < SpinData.Spins[i])
									iSpinCall[i]++;
								if(iSpinSave[i] > SpinData.Spins[i])
									iSpinCall[i]--;

								iSpinSave[i] = SpinData.Spins[i]; //Wen neue Daten vorhanden, Setzen
	
								if((iSpinCall[i] > cPropPage4.cSpinSpeed.GetPos()) && (iSpinCall[i] < cPropPage4.cSpinSpeed.GetPos()+2 ))
								{
									iSpinCall[i] = 0;
									if((SpinData.Buttons[cPropPage3.cLevelButton[0].GetCurSel()-1] == TRUE) || (SpinData.Buttons[cPropPage3.cLevelButton[1].GetCurSel()-1] == TRUE))
									{
										HidToKeyboard(AllValues[pos_3[i]], START_PUSH);
										HidToKeyboard(AllValues[pos_3[i]], END_PUSH);
									}
									else
									{
										HidToKeyboard(AllValues[pos_1[i]], START_PUSH);
										HidToKeyboard(AllValues[pos_1[i]], END_PUSH);
									}
								}


								if((iSpinCall[i] < -cPropPage4.cSpinSpeed.GetPos()) && (iSpinCall[i] > -cPropPage4.cSpinSpeed.GetPos()-2 ))
								{
									iSpinCall[i] = 0;
									if((SpinData.Buttons[cPropPage3.cLevelButton[0].GetCurSel()-1] == TRUE) || (SpinData.Buttons[cPropPage3.cLevelButton[1].GetCurSel()-1] == TRUE))
									{
										HidToKeyboard(AllValues[pos_2[i]], START_PUSH);
										HidToKeyboard(AllValues[pos_2[i]], END_PUSH);
									}
									else
									{
										HidToKeyboard(AllValues[pos_2[i]], START_PUSH);
										HidToKeyboard(AllValues[pos_2[i]], END_PUSH);
									}
								}
							}
						}

						/*Aufruf der eingestellten Applikation auf dem entsprechenden Button*/
						if(SpinData.Buttons[cPropPage3.cButtonApp.GetCurSel()] == TRUE) //Button gedrückt
						{
							if(ButtonCount[10] < 1) //Button nur Ein Mal gedrückt (Entprellt)
							{
								ButtonCount[10]++; //Hochzählen für Zeitabgleich

								char appli[512];
								WideCharToMultiByte(CP_ACP, NULL, cPropPage3.sAppPath, -1, appli, 255, NULL, NULL);

								WinExec(appli, SW_SHOW);
							}
						}
						if(SpinData.Buttons[cPropPage3.cButtonApp.GetCurSel()] == FALSE) //Button leer
						{
							if(ButtonCount[10] != 0)
							{
								ButtonCount[10] = 0;
							}
						}

						/*Abfrage der 6 möglichen Buttons mit Ausführlimit = 1*/
						for(int i=0;i<7; i++)
						{
							if(SpinData.Buttons[i] == TRUE) //Button gedrückt
							{
								if(ButtonCount[i] == 0) //Button nur Ein Mal gedrückt (Entprellt)
								{
									ButtonCount[i]++; //Hochzählen für Zeitabgleich
									HidToKeyboard(AllValues[button[i]], START_PUSH); //Tastendruck: "Taste Unten"
								}
							}
							if(SpinData.Buttons[i] == FALSE) //Button leer
							{
								if(ButtonCount[i] != 0)
								{
									ButtonCount[i] = 0;
									HidToKeyboard(AllValues[button[i]], END_PUSH); //Tastendruck: "Taste Oben"
		
								}
							}
						}
						break;
					}
				}
			}
		}
	}

	CDialog::OnTimer(nIDEvent);
}

/*Convert string into BYTE-Value for SendInput*/
UCHAR CSpinControlerDlg::ConvStrToByte(CString value)
{
	int	ret, ret1, exec;
	ret = 0;
	ret1 = 0;
	exec = 0;
	char newvalue[2];

	for(int h=0; h<2; h++)
		newvalue[h] = (char) value.GetAt(h);

	switch(newvalue[1])
	{
	case '0':
		ret = 0;
		break;
	case '1':
		ret = 1;
		break;
	case '2':
		ret = 2;
		break;
	case '3':
		ret = 3;
		break;
	case '4':
		ret = 4;
		break;
	case '5':
		ret = 5;
		break;
	case '6':
		ret = 6;
		break;
	case '7':
		ret = 7;
		break;
	case '8':
		ret = 8;
		break;
	case '9':
		ret = 9;
		break;
	case 'A':
		ret = 10;
		break;
	case 'B':
		ret = 11;
		break;
	case 'C':
		ret = 12;
		break;
	case 'D':
		ret = 13;
		break;
	case 'E':
		ret = 14;
		break;
	case 'F':
		ret = 15;
		break;
	}

	switch(newvalue[0])
	{
	case '0':
		ret1 = 0;
		break;
	case '1':
		ret1 = 16;
		break;
	case '2':
		ret1 = 32;
		break;
	case '3':
		ret1 = 48;
		break;
	case '4':
		ret1 = 64;
		break;
	case '5':
		ret1 = 80;
		break;
	case '6':
		ret1 = 96;
		break;
	case '7':
		ret1 = 112;
		break;
	case '8':
		ret1 = 128;
		break;
	case '9':
		ret1 = 144;
		break;
	case 'A':
		ret1 = 160;
		break;
	case 'B':
		ret1 = 176;
		break;
	case 'C':
		ret1 = 192;
		break;
	case 'D':
		ret1 = 208;
		break;
	case 'E':
		ret1 = 224;
		break;
	case 'F':
		ret1 = 240;
		break;
	}

	exec = ret + ret1;

	return exec;
}

void CSpinControlerDlg::ReadConfig(CString path)
{
	CString Buffer, Wert, newPath;
	CString Combo[40];
	int index = 0;
	CStdioFile File;

	newPath.Format(_T("%s\\%s"), path, _T("config.cfg"));

	if(File.Open(newPath, CFile::modeRead | CFile::typeBinary))
	{
		File.SeekToBegin();

		while(File.ReadString(Buffer))
		{
			Wert.Format(_T("%s"), Buffer);
			AllValues[index] = ConvStrToByte(Wert); //Convert string into UCHAR and save for furhter use
			Combo[index].SetString(Wert); //Set string for setting selection in CComboCoxes
			index++;
		}

		File.Close();

		for(int i=0; i<12; i++)
			cPropPage1.cSpinBox[i].SelectString(0, GetName(Combo[i]));
			
		for(int i=0; i<12; i++)
			cPropPage2.cSpinBox[i].SelectString(0, GetName(Combo[i+12]));

		for(int j=0; j<7; j++)
			cPropPage3.cButtonBox[j].SelectString(0, GetName(Combo[j+24]));
	
		/*SpinSpeed*/
		cPropPage4.cSpinSpeed.SetPos(_ttoi(Combo[31]));

		/*Levelbutton 1*/
		cPropPage3.cLevelButton[0].SetCurSel(_ttoi(Combo[32]));

		/*Levelbutton 2*/
		cPropPage3.cLevelButton[1].SetCurSel(_ttoi(Combo[33]));

		/*Serlial*/
		sSerial.SetString(Combo[34]);

		/*ProductID*/
		iProductID = _ttoi(Combo[35]);

		/*App-Button*/
		cPropPage3.cButtonApp.SetCurSel(_ttoi(Combo[36]));

		/*App-Path*/
		cPropPage3.sAppPath.SetString(Combo[37]);
	}
	else
	{
		MessageBox(_T("No configuration-file would be found. Please check for config.cfg in application-path."), _T("No file found"), MB_OK);
		memset(&AllValues, 0, 48); //Set AllValues ZERO (selection 0, non keyboard-events)

		for(int i=0; i<12; i++)
			cPropPage1.cSpinBox[i].SetCurSel(0);

		for(int i=0; i<12; i++)
			cPropPage2.cSpinBox[i].SetCurSel(0);

		for(int j=0; j<7; j++)
			cPropPage3.cButtonBox[j].SetCurSel(0);

		cPropPage4.cSpinSpeed.SetPos(0);

				/*Levelbutton 1*/
		cPropPage3.cLevelButton[0].SetCurSel(0);

		/*Levelbutton 2*/
		cPropPage3.cLevelButton[1].SetCurSel(0);

		sSerial.Empty();

		/*ProductID*/
		iProductID = 0;
	}

	UpdateData(FALSE);
}

void CSpinControlerDlg::SaveConfig(CString path)
{
	UpdateData(TRUE);
	WCHAR sn[9];

	CString temp, save, newPath;
	int pos = cPropPage4.cSpinSpeed.GetPos();

	newPath.Format(_T("%s\\%s"), path, _T("config.cfg"));

	CFile cFile(newPath, CFile::modeCreate | CFile::modeWrite);


	/*SpinSet 1*/
	for(int i=0; i<12; i++)
	{
		if(cPropPage1.cSpinBox[i].GetLBTextLen(cPropPage1.cSpinBox[i].GetCurSel()) > 0)
		{
			cPropPage1.cSpinBox[i].GetLBText(cPropPage1.cSpinBox[i].GetCurSel(), temp);

			if(temp != _T(""))
				save.Format(_T("%s\n"), GetHex(temp));
			else
				save.SetString(_T("00\n"));
		}
		else
			save.SetString(_T("00\n"));


		cFile.Write(save, save.GetLength()*sizeof(TCHAR));
	}

	/*SpinSet 2*/
	for(int i=0; i<12; i++)
	{
		if(cPropPage2.cSpinBox[i].GetLBTextLen(cPropPage2.cSpinBox[i].GetCurSel()) > 0)
		{
			cPropPage2.cSpinBox[i].GetLBText(cPropPage2.cSpinBox[i].GetCurSel(), temp);

			if(temp != _T(""))
				save.Format(_T("%s\n"), GetHex(temp));
			else
				save.SetString(_T("00\n"));
		}
		else
			save.SetString(_T("00\n"));


		cFile.Write(save, save.GetLength()*sizeof(TCHAR));
	}

	/*Buttons*/
	for(int i=0; i<7; i++)
	{
		if(cPropPage3.cButtonBox[i].GetLBTextLen(cPropPage3.cButtonBox[i].GetCurSel()) > 0)
		{
			cPropPage3.cButtonBox[i].GetLBText(cPropPage3.cButtonBox[i].GetCurSel(), temp);

			if(temp != _T(""))
				save.Format(_T("%s\n"), GetHex(temp));
			else
				save.SetString(_T("00\n"));
		}
		else
			save.SetString(_T("00\n"));


		cFile.Write(save, save.GetLength()*sizeof(TCHAR));
	}

	/*SpinSpeed*/
	temp.Empty();
	temp.Format(_T("%d\n"), pos);
	cFile.Write((LPCTSTR) temp, temp.GetLength()*sizeof(TCHAR));

	/*2nd Level*/
	temp.Empty();
	temp.Format(_T("%d\n"), cPropPage3.cLevelButton[0].GetCurSel());
	cFile.Write((LPCTSTR) temp, temp.GetLength()*sizeof(TCHAR));

	temp.Empty();
	temp.Format(_T("%d\n"), cPropPage3.cLevelButton[1].GetCurSel());
	cFile.Write((LPCTSTR) temp, temp.GetLength()*sizeof(TCHAR));

	/*Serial*/
	temp.Empty();
	SpinKitGetSerialNumber(spinHandles[cPropPage5.iSelect], sn);
	temp.Format(_T("%s\n"), sn);
	cFile.Write((LPCTSTR) temp, temp.GetLength()*sizeof(TCHAR));

	/*ProductID*/
	temp.Empty();
	temp.Format(_T("%d\n"), SpinKitGetProductId(spinHandles[cPropPage5.iSelect]));//cPropPage4.cLevelButton);
	cFile.Write((LPCTSTR) temp, temp.GetLength()*sizeof(TCHAR));

	/*App-Button*/
	temp.Empty();
	temp.Format(_T("%d\n"), cPropPage3.cButtonApp.GetCurSel());
	cFile.Write((LPCTSTR) temp, temp.GetLength()*sizeof(TCHAR));

	/*App-Path*/
	temp.Empty();
	temp.Format(_T("%s\0"), cPropPage3.sAppPath.GetString());
	cFile.Write((LPCTSTR) temp, temp.GetLength()*sizeof(TCHAR));

	cFile.Close();
}
/*Extract string from Hex-Codes from virtual_key_codes*/
CString CSpinControlerDlg::GetName(CString string)
{
	int pos;
	int index = 0;
	CString substr;

	while(index < ArrayHex.GetCount())
	{
		pos = 0;
	//	pos = string.Find(ArrayHex.ElementAt(index));

		pos = string.Compare(ArrayHex.ElementAt(index));
		
		if(pos == 0)
		{
			substr.SetString(ArrayName.ElementAt(index));
			break;
		}

		//if(pos > -1)
		//{
		//	substr.SetString(ArrayName.ElementAt(index));
		//	break;
		//}
		index++;
	}
	return substr;
}

/*Extract Hex-Code from String from virtual_key_codes*/
CString CSpinControlerDlg::GetHex(CString string)
{
	int pos;
	int index = 0;
	CString substr;
	int count = string.GetLength();

	while(index < ArrayHex.GetCount())
	{
		pos = 0;
//		pos = string.Find(ArrayName.ElementAt(index));
		pos = string.Compare(ArrayName.ElementAt(index));
		

		if(pos == 0)
		{
			substr.SetString(ArrayHex.ElementAt(index));
			break;
		}

		//if(pos > -1)
		//{
		//	substr.SetString(ArrayHex.ElementAt(index));
		//	break;
		//}
		index++;
	}
	return substr;
}


void CSpinControlerDlg::OnBnClickedSave()
{
	SaveConfig(GetPathString());
}

void CSpinControlerDlg::OnBnClickedClose()
{
	CString temp;

	for(int i=0; i<12; i++)
	{
		temp.Empty();
		if(cPropPage1.cSpinBox[i].GetLBTextLen(cPropPage1.cSpinBox[i].GetCurSel()) > 0)
		{
			cPropPage1.cSpinBox[i].GetLBText(cPropPage1.cSpinBox[i].GetCurSel(), temp);
			AllValues[i] = ConvStrToByte(GetHex(temp));
		}
	}

	for(int i=0; i<12; i++)
	{
		temp.Empty();
		if(cPropPage2.cSpinBox[i].GetLBTextLen(cPropPage2.cSpinBox[i].GetCurSel()) > 0)
		{
			cPropPage2.cSpinBox[i].GetLBText(cPropPage2.cSpinBox[i].GetCurSel(), temp);
			AllValues[i+12] = ConvStrToByte(GetHex(temp));
		}
	}

	for(int i=0; i<7; i++)
	{
		temp.Empty();
		if(cPropPage3.cButtonBox[i].GetLBTextLen(cPropPage3.cButtonBox[i].GetCurSel()) > 0)
		{
			cPropPage3.cButtonBox[i].GetLBText(cPropPage3.cButtonBox[i].GetCurSel(), temp);
			AllValues[i+24] = ConvStrToByte(GetHex(temp));
		}
	}

	//if(cPropPage3.cLevelButton[0].GetLBTextLen(cPropPage3.cLevelButton[0].GetCurSel()) > 0)
	//{
	//	temp.Empty();
	//	cPropPage3.cLevelButton[0].GetLBText(cPropPage3.cLevelButton[0].GetCurSel(), temp);
	//	AllValues[32] = ConvStrToByte(GetHex(temp));
	//}

	//	
	//if(cPropPage3.cLevelButton[1].GetLBTextLen(cPropPage3.cLevelButton[1].GetCurSel()) > 0)
	//{
	//	temp.Empty();
	//	cPropPage3.cLevelButton[1].GetLBText(cPropPage3.cLevelButton[1].GetCurSel(), temp);
	//	AllValues[33] = ConvStrToByte(GetHex(temp));
	//}

	ChooseTimer(SpinKitGetProductId(spinHandles[cPropPage5.iSelect]));
	activeHandle = spinHandles[cPropPage5.iSelect];

	ShowWindow(SW_HIDE);
	bVisible = FALSE;

	UpdateData(FALSE);
}

/*Connect to all available SpinWarriors and insert into CListCtrl.*/
void CSpinControlerDlg::SearchSpins(void)
{
	ULONG NumDev, pid;
	WCHAR sn[9];
	CString string;

	cPropPage5.cRepList.ResetContent();

	if(spinHandle != NULL)
		SpinKitCloseDevice(spinHandle);
	
	spinHandle = SpinKitOpenDevice();

	if(spinHandle != NULL)
	{
		NumDev = SpinKitGetNumDevs();
		
		for(ULONG i=0; i<NumDev; i++)
		{
			memset(&sn, 0, sizeof(sn));
			spinHandles[i] = SpinKitGetDeviceHandle(i+1);

			SpinKitGetSerialNumber(spinHandles[i], sn);
			pid = SpinKitGetProductId(spinHandles[i]);

			switch(pid)
			{
			case SPINKIT_PID24R4:
				string.Format(_T("%s  -  SN:%s"), _T("SpinWarrior24R4"), sn);
				break;
			case SPINKIT_PID24R6:
				string.Format(_T("%s  -  SN:%s"), _T("SpinWarrior24R6"), sn);
				break;
			case SPINKIT_PID24A3:
				string.Format(_T("%s  -  SN:%s"), _T("SpinWarrior24A3"), sn);
				break;
			}
	
			cPropPage5.cRepList.InsertString(i, string);
			UpdateData(FALSE);
		}
	}
	//else
	//	MessageBox(_T("No SpinWarrior would be detected.\r\nPlease connect an SpinWarrior."),  _T("No Device"), MB_OK);

}
