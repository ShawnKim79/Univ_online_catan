// Catan_DummyDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Catan_Dummy.h"
#include "Catan_DummyDlg.h"
#include "header.h"

#include <afxmt.h>

#include <stdarg.h>
#include <stdio.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CCriticalSection g_Critical; // ...?
/*
UINT nGAMESTART;
UINT nLIVECHECK;
UINT nINLIVE;
UINT nGAMEEND;
UINT nGAMESTATU;
*/
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
	ON_WM_SIZE()
	ON_WM_CLOSE()
//	ON_BN_CLICKED(IDC_SHOWALL, OnShowall)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCatan_DummyDlg dialog

CCatan_DummyDlg::CCatan_DummyDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCatan_DummyDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCatan_DummyDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCatan_DummyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCatan_DummyDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	DDX_Control(pDX, IDC_DUMMYEDIT, m_ctrlLog);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCatan_DummyDlg, CDialog)
	//{{AFX_MSG_MAP(CCatan_DummyDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(WM_GAMESTART, OnStartGame)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCatan_DummyDlg message handlers

BOOL CCatan_DummyDlg::OnInitDialog()
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
	
	// TODO: Add extra initialization here

	AddLogString("애플리케이션 스타트");

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CCatan_DummyDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CCatan_DummyDlg::OnPaint() 
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
HCURSOR CCatan_DummyDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}


////////////////////////////////////////////////////////////////////////////////////////

void CCatan_DummyDlg::AddLogString(const char *format, ...)
{
	// Log String에 문자열을 저장 기록하는 함수인것은 알겠는데...왜 크리티컬 섹션이 들어갈까??
	char szBuff[2048];
	va_list args;
	va_start(args, format);
	DWORD nBuf = _vsnprintf(szBuff, 2048, format, args);
	va_end(args);

	CTime Time = CTime::GetCurrentTime();

	CString str;

	str.Format("[%d-%02d-%02d %02d:%02d:%02d]",Time.GetYear(), Time.GetMonth(), Time.GetDay(), 
		Time.GetHour(), Time.GetMinute(), Time.GetSecond());

	str += szBuff;
	str += "\r\n";

	g_Critical.Lock();

	m_ctrlLog.SetSel(-1, -1);
	m_ctrlLog.ReplaceSel(str);

	g_Critical.Unlock();
}

void CCatan_DummyDlg::OnClose()
{
	ShowWindow(SW_HIDE);
}
/*
void CCatan_DummyDlg::OnShowall()
{

}
*/
void CCatan_DummyDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
	CWnd *pWnd = GetDlgItem(IDC_DUMMYEDIT);

	if(IsWindow(pWnd->GetSafeHwnd()))
	{
		pWnd->SetWindowPos(NULL, 0, 0, cx, cy-30, SWP_NOZORDER | SWP_NOMOVE);
	}
}

BOOL CCatan_DummyDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->message == WM_KEYDOWN)
	{
		if(pMsg->wParam == VK_ESCAPE)
		{
			ShowWindow(SW_HIDE);
			return TRUE;
		}
		else return TRUE;
	}
	return CDialog::PreTranslateMessage(pMsg);
}

void CCatan_DummyDlg::OnStartGame(WPARAM m_wParam, LPARAM m_lParam)
{
//	DestroyWindow();
	AfxMessageBox("메세지 수신");
    AddLogString("게임 시작 메세지 받음");

	AddLogString("%s", m_strNICK);
	AddLogString("%s", m_strIP);
	
}
