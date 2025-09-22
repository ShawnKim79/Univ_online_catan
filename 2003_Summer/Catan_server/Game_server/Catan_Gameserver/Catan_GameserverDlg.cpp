// Catan_GameserverDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Catan_Gameserver.h"
#include "Catan_GameserverDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCatan_GameserverDlg dialog

CCatan_GameserverDlg::CCatan_GameserverDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCatan_GameserverDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCatan_GameserverDlg)
	m_strDBNAME = _T("");
	m_strGameserverIP = _T("");
	m_nGameserverPORT = 0;
	m_strDBPASSWD = _T("");
	m_strDBID = _T("");
	m_strChannelserverIP = _T("");
	m_nChannelPORT = 0;
	m_nChannel = 0;
	m_nRoom = 0;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCatan_GameserverDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCatan_GameserverDlg)
	DDX_Text(pDX, IDC_DBNAME, m_strDBNAME);
	DDX_Text(pDX, IDC_GAMEIP, m_strGameserverIP);
	DDX_Text(pDX, IDC_GAMEPORT, m_nGameserverPORT);
	DDX_Text(pDX, IDC_DBPASSWORD, m_strDBPASSWD);
	DDX_Text(pDX, IDC_DBID, m_strDBID);
	DDX_Text(pDX, IDC_CHANNELIP, m_strChannelserverIP);
	DDX_Text(pDX, IDC_CHANNELPORT, m_nChannelPORT);
	DDX_Text(pDX, IDC_CHANNEL, m_nChannel);
	DDX_Text(pDX, IDC_ROOM, m_nRoom);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCatan_GameserverDlg, CDialog)
	//{{AFX_MSG_MAP(CCatan_GameserverDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCatan_GameserverDlg message handlers

BOOL CCatan_GameserverDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CCatan_GameserverDlg::OnPaint() 
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
HCURSOR CCatan_GameserverDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

BOOL CCatan_GameserverDlg::LoadSetting()
{
	HKEY hKey;
	DWORD dwDisp;
	DWORD dwSize;
	BOOL bSucceeded = TRUE;

	RegCreateKeyEx(HKEY_CURRENT_USER, "SOFTWARE\\HGRGame\\GameServer\\Bingo\\Setting", 0, NULL, 
		REG_OPTION_NON_VOLATILE, KEY_QUERY_VALUE, NULL, &hKey, &dwDisp);

	///////////////////////////////////////
	// Game Server 주소
	dwSize = 255;
	RegQueryValueEx(hKey, "Game Server Addr", 0, NULL, (LPBYTE)m_strGameserverIP.GetBufferSetLength(255), &dwSize);

	m_strGameserverIP.TrimLeft();

	if(!m_strGameserverIP.GetLength())
	{
		bSucceeded = FALSE;
	}



	///////////////////////////////////////
	// Game Server 포트 번호

	dwSize = sizeof(UINT);
	RegQueryValueEx(hKey, "Game Server PortNo", 0, NULL, (LPBYTE)&m_nGameserverPORT, &dwSize);

	if(m_nGameserverPORT == 0)
	{
		bSucceeded = FALSE;
	}



	///////////////////////////////////////
	// Game Server 동작 (원본에서는 오목 or 빙고를 선택한다...
/*
	dwSize = sizeof(UINT);
	RegQueryValueEx(hKey, "Behavior", 0, NULL, (LPBYTE)&m_nServerBehavior, &dwSize);
//  RegQueryValueEx(hKey, "Game Server Addr", 0, NULL, (LPBYTE)m_strGSAddr.GetBufferSetLength(255), &dwSize);
	if(m_nServerBehavior == 0)
	{
		bSucceeded = FALSE;
	}

	m_nServerBehavior--;
*/

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
	RegQueryValueEx(hKey, "Channel Server Addr", 0, NULL, (LPBYTE)m_strChannelserverIP.GetBufferSetLength(255), &dwSize);

	m_strChannelserverIP.TrimLeft();

	if(!m_strChannelserverIP.GetLength())
	{
		bSucceeded = FALSE;
	}



	///////////////////////////////////////
	// Channel Server 포트 번호

	dwSize = sizeof(UINT);
	RegQueryValueEx(hKey, "Channel Server PortNo", 0, NULL, (LPBYTE)&m_nChannelPORT, &dwSize);

	if(m_nChannelPORT == 0)
	{
		bSucceeded = FALSE;
	}



	///////////////////////////////////////
	// DSN name

	dwSize = 255;
	RegQueryValueEx(hKey, "DSN Name", 0, NULL, (LPBYTE)m_strDBNAME.GetBufferSetLength(255), &dwSize);

	m_strDBNAME.TrimLeft();

	if(!m_strDBNAME.GetLength())
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
	RegQueryValueEx(hKey, "DB Passwd", 0, NULL, (LPBYTE)m_strDBPASSWD.GetBufferSetLength(255), &dwSize);

	m_strDBPASSWD.TrimLeft();

	if(!m_strDBPASSWD.GetLength())
	{
		bSucceeded = FALSE;
	}

	return bSucceeded;
}

BOOL CCatan_GameserverDlg::SaveSetting()
{
	HKEY hKey;
	DWORD dwDisp;

	RegCreateKeyEx(HKEY_CURRENT_USER, "SOFTWARE\\HGRGame\\GameServer\\Bingo\\Setting", 0, NULL, 
		REG_OPTION_NON_VOLATILE, KEY_SET_VALUE, NULL, &hKey, &dwDisp);

//	m_nServerBehavior++;

	if(!m_strGameserverIP.GetLength() || !m_strChannelserverIP.GetLength() || !m_strDBNAME.GetLength() || !m_strDBID.GetLength() || !m_strDBPASSWD.GetLength() ||
		!m_nGameserverPORT || !m_nChannelPORT || !m_nChannel || !m_nRoom)
	{
		AfxMessageBox("서버설정을 정확히 입력해 주세요.");
		return FALSE;
	}

	RegSetValueEx(hKey, "Game Server Addr", 0, REG_SZ, (LPBYTE)(LPCTSTR)m_strGameserverIP, m_strGameserverIP.GetLength());
	RegSetValueEx(hKey, "Game Server PortNo", 0, REG_DWORD, (LPBYTE)&m_nGameserverPORT, sizeof(m_nGameserverPORT));
//	RegSetValueEx(hKey, "Behavior", 0, REG_DWORD, (LPBYTE)&m_nServerBehavior, sizeof(m_nServerBehavior));
	RegSetValueEx(hKey, "Number of Channels", 0, REG_DWORD, (LPBYTE)&m_nChannel, sizeof(m_nChannel));
	RegSetValueEx(hKey, "Number of Rooms", 0, REG_DWORD, (LPBYTE)&m_nRoom, sizeof(m_nRoom));

	RegSetValueEx(hKey, "Channel Server Addr", 0, REG_SZ, (LPBYTE)(LPCTSTR)m_strChannelserverIP, m_strChannelserverIP.GetLength());
	RegSetValueEx(hKey, "Channel Server PortNo", 0, REG_DWORD, (LPBYTE)&m_nChannelPORT, sizeof(m_nChannelPORT));

	RegSetValueEx(hKey, "DSN Name", 0, REG_SZ, (LPBYTE)(LPCTSTR)m_strDBNAME, m_strDBNAME.GetLength());
	RegSetValueEx(hKey, "DB User ID", 0, REG_SZ, (LPBYTE)(LPCTSTR)m_strDBID, m_strDBID.GetLength());
	RegSetValueEx(hKey, "DB Passwd", 0, REG_SZ, (LPBYTE)(LPCTSTR)m_strDBPASSWD, m_strDBPASSWD.GetLength());

	return TRUE;
}

void CCatan_GameserverDlg::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData();
	
	CDialog::OnOK();
}

void CCatan_GameserverDlg::OnCancel() 
{
	// TODO: Add extra cleanup here

	
	CDialog::OnCancel();
}
