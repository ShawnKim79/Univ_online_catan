// GameServerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GameServer.h"
#include "GameServerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGameServerDlg dialog

CGameServerDlg::CGameServerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGameServerDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CGameServerDlg)
	m_strGSAddr = _T("");
	m_nServerBehavior = 0;
	m_nChannel = 1;
	m_strCSAddr = _T("");
	m_nGSPortNo = 12000;
	m_strDSN = _T("");
	m_strDBID = _T("");
	m_strDBPW = _T("");
	m_nRoom = 1;
	m_nCSPortNo = 10000;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CGameServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGameServerDlg)
	DDX_Control(pDX, IDC_CANCEL, m_ctrlCancel);
	DDX_Control(pDX, IDC_OK, m_ctrlOK);
	DDX_Text(pDX, IDC_GSADDR, m_strGSAddr);
	DDX_Radio(pDX, IDC_ASOMOK, m_nServerBehavior);
	DDX_Text(pDX, IDC_CHANNEL, m_nChannel);
	DDV_MinMaxUInt(pDX, m_nChannel, 1, 10);
	DDX_Text(pDX, IDC_CSADDR, m_strCSAddr);
	DDX_Text(pDX, IDC_GSPORT, m_nGSPortNo);
	DDX_Text(pDX, IDC_DSN, m_strDSN);
	DDX_Text(pDX, IDC_DBID, m_strDBID);
	DDX_Text(pDX, IDC_DBPW, m_strDBPW);
	DDX_Text(pDX, IDC_ROOM, m_nRoom);
	DDV_MinMaxUInt(pDX, m_nRoom, 1, 500);
	DDX_Text(pDX, IDC_CSPORT, m_nCSPortNo);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CGameServerDlg, CDialog)
	//{{AFX_MSG_MAP(CGameServerDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_OK, OnOk)
	ON_BN_CLICKED(IDC_ASOMOK, OnChangeType)
	ON_BN_CLICKED(IDC_CANCEL, OnCancel)
	ON_BN_CLICKED(IDC_ASBINGO, OnChangeType)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGameServerDlg message handlers

BOOL CGameServerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	((CSpinButtonCtrl *)GetDlgItem(IDC_SPIN1))->SetRange(1, 10);
	((CSpinButtonCtrl *)GetDlgItem(IDC_SPIN2))->SetRange(1, 500);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CGameServerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CGameServerDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CGameServerDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

BOOL CGameServerDlg::LoadSettings()
{
	HKEY hKey;
	DWORD dwDisp;
	DWORD dwSize;
	BOOL bSucceeded = TRUE;

	RegCreateKeyEx(HKEY_CURRENT_USER, "SOFTWARE\\HGRGame\\GameServer\\Omok\\Setting", 0, NULL, 
		REG_OPTION_NON_VOLATILE, KEY_QUERY_VALUE, NULL, &hKey, &dwDisp);

	///////////////////////////////////////
	// Game Server 주소
	dwSize = 255;
	RegQueryValueEx(hKey, "Game Server Addr", 0, NULL, (LPBYTE)m_strGSAddr.GetBufferSetLength(255), &dwSize);

	m_strGSAddr.TrimLeft();

	if(!m_strGSAddr.GetLength())
	{
		bSucceeded = FALSE;
	}



	///////////////////////////////////////
	// Game Server 포트 번호

	dwSize = sizeof(UINT);
	RegQueryValueEx(hKey, "Game Server PortNo", 0, NULL, (LPBYTE)&m_nGSPortNo, &dwSize);

	if(m_nGSPortNo == 0)
	{
		bSucceeded = FALSE;
	}



	///////////////////////////////////////
	// Game Server 동작

	dwSize = sizeof(UINT);
	RegQueryValueEx(hKey, "Behavior", 0, NULL, (LPBYTE)&m_nServerBehavior, &dwSize);

	if(m_nServerBehavior == 0)
	{
		bSucceeded = FALSE;
	}

	m_nServerBehavior--;


	///////////////////////////////////////
	// Game Server 채널 갯수

	dwSize = sizeof(UINT);
	RegQueryValueEx(hKey, "Number of Channels", 0, NULL, (LPBYTE)&m_nChannel, &dwSize);

	if(m_nChannel == 0)
	{
		bSucceeded = FALSE;
	}



	///////////////////////////////////////
	// Game Server 방 갯수

	dwSize = sizeof(UINT);
	RegQueryValueEx(hKey, "Number of Rooms", 0, NULL, (LPBYTE)&m_nRoom, &dwSize);

	if(m_nRoom == 0)
	{
		bSucceeded = FALSE;
	}



	///////////////////////////////////////
	// Channel Server 채널 서버 주소

	dwSize = 255;
	RegQueryValueEx(hKey, "Channel Server Addr", 0, NULL, (LPBYTE)m_strCSAddr.GetBufferSetLength(255), &dwSize);

	m_strCSAddr.TrimLeft();

	if(!m_strCSAddr.GetLength())
	{
		bSucceeded = FALSE;
	}



	///////////////////////////////////////
	// Channel Server 포트 번호

	dwSize = sizeof(UINT);
	RegQueryValueEx(hKey, "Channel Server PortNo", 0, NULL, (LPBYTE)&m_nCSPortNo, &dwSize);

	if(m_nCSPortNo == 0)
	{
		bSucceeded = FALSE;
	}



	///////////////////////////////////////
	// DSN name

	dwSize = 255;
	RegQueryValueEx(hKey, "DSN Name", 0, NULL, (LPBYTE)m_strDSN.GetBufferSetLength(255), &dwSize);

	m_strDSN.TrimLeft();

	if(!m_strDSN.GetLength())
	{
		bSucceeded = FALSE;
	}



	///////////////////////////////////////
	// DB ID

	dwSize = 255;
	RegQueryValueEx(hKey, "DB User ID", 0, NULL, (LPBYTE)m_strDBID.GetBufferSetLength(255), &dwSize);

	m_strDBID.TrimLeft();

	if(!m_strDBID.GetLength())
	{
		bSucceeded = FALSE;
	}



	///////////////////////////////////////
	// DB ID

	dwSize = 255;
	RegQueryValueEx(hKey, "DB Passwd", 0, NULL, (LPBYTE)m_strDBPW.GetBufferSetLength(255), &dwSize);

	m_strDBPW.TrimLeft();

	if(!m_strDBPW.GetLength())
	{
		bSucceeded = FALSE;
	}

	return bSucceeded;
}

BOOL CGameServerDlg::SaveSettings()
{
	HKEY hKey;
	DWORD dwDisp;

	RegCreateKeyEx(HKEY_CURRENT_USER, "SOFTWARE\\HGRGame\\GameServer\\Omok\\Setting", 0, NULL, 
		REG_OPTION_NON_VOLATILE, KEY_SET_VALUE, NULL, &hKey, &dwDisp);

	m_nServerBehavior++;

	if(!m_strGSAddr.GetLength() || !m_strCSAddr.GetLength() || !m_strDSN.GetLength() || !m_strDBID.GetLength() || !m_strDBPW.GetLength() ||
		!m_nGSPortNo || !m_nCSPortNo || !m_nChannel || !m_nRoom || !m_nServerBehavior)
	{
		AfxMessageBox("서버설정을 정확히 입력해 주세요.");
		return FALSE;
	}

	RegSetValueEx(hKey, "Game Server Addr", 0, REG_SZ, (LPBYTE)(LPCTSTR)m_strGSAddr, m_strGSAddr.GetLength());
	RegSetValueEx(hKey, "Game Server PortNo", 0, REG_DWORD, (LPBYTE)&m_nGSPortNo, sizeof(m_nGSPortNo));
	RegSetValueEx(hKey, "Behavior", 0, REG_DWORD, (LPBYTE)&m_nServerBehavior, sizeof(m_nServerBehavior));
	RegSetValueEx(hKey, "Number of Channels", 0, REG_DWORD, (LPBYTE)&m_nChannel, sizeof(m_nChannel));
	RegSetValueEx(hKey, "Number of Rooms", 0, REG_DWORD, (LPBYTE)&m_nRoom, sizeof(m_nRoom));

	RegSetValueEx(hKey, "Channel Server Addr", 0, REG_SZ, (LPBYTE)(LPCTSTR)m_strCSAddr, m_strCSAddr.GetLength());
	RegSetValueEx(hKey, "Channel Server PortNo", 0, REG_DWORD, (LPBYTE)&m_nCSPortNo, sizeof(m_nCSPortNo));

	RegSetValueEx(hKey, "DSN Name", 0, REG_SZ, (LPBYTE)(LPCTSTR)m_strDSN, m_strDSN.GetLength());
	RegSetValueEx(hKey, "DB User ID", 0, REG_SZ, (LPBYTE)(LPCTSTR)m_strDBID, m_strDBID.GetLength());
	RegSetValueEx(hKey, "DB Passwd", 0, REG_SZ, (LPBYTE)(LPCTSTR)m_strDBPW, m_strDBPW.GetLength());

	return TRUE;
}

void CGameServerDlg::OnOk() 
{
	UpdateData();

	CDialog::OnOK();
}

void CGameServerDlg::OnCancel() 
{
	CDialog::OnCancel();
}

void CGameServerDlg::OnChangeType() 
{
	UpdateData();


}