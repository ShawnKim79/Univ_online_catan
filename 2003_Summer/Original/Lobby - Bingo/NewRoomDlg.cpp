// NewRoomDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Lobby.h"
#include "NewRoomDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNewRoomDlg dialog


CNewRoomDlg::CNewRoomDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CNewRoomDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNewRoomDlg)
	m_bLocked = FALSE;
	m_strPasswd = _T("");
	m_strRoomTitle = _T("들어오세요.");
	//}}AFX_DATA_INIT
}


void CNewRoomDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNewRoomDlg)
	DDX_Control(pDX, IDC_CANCEL, m_ctrlCancel);
	DDX_Control(pDX, IDC_OK, m_ctrlOK);
	DDX_Check(pDX, IDC_LOCKED, m_bLocked);
	DDX_Text(pDX, IDC_PASSWD, m_strPasswd);
	DDV_MaxChars(pDX, m_strPasswd, 8);
	DDX_CBString(pDX, IDC_TITLECOMBO, m_strRoomTitle);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNewRoomDlg, CDialog)
	//{{AFX_MSG_MAP(CNewRoomDlg)
	ON_BN_CLICKED(IDC_OK, OnOk)
	ON_BN_CLICKED(IDC_LOCKED, OnLocked)
	ON_BN_CLICKED(IDC_CANCEL, OnCancel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNewRoomDlg message handlers

BOOL CNewRoomDlg::PreTranslateMessage(MSG* pMsg) 
{
	if(pMsg->message == WM_KEYDOWN)
	{
		if(pMsg->wParam == VK_RETURN) 
		{
			OnOK();
			return TRUE;
		}

		else if(pMsg->wParam == VK_ESCAPE)
		{
			OnCancel();
			return TRUE;
		}
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}

void CNewRoomDlg::OnOk() 
{
	UpdateData();

	if(m_bLocked)
	{
		if(!m_strPasswd.GetLength())
		{
			AfxMessageBox("패스워드를 입력해주세요.");
			return;
		}
	}

	CDialog::OnOK();
}

void CNewRoomDlg::OnCancel() 
{
	UpdateData();

	CDialog::OnCancel();
}

void CNewRoomDlg::OnLocked() 
{
	UpdateData();

	if(m_bLocked) GetDlgItem(IDC_PASSWD)->EnableWindow();
	else GetDlgItem(IDC_PASSWD)->EnableWindow(FALSE);
}

BOOL CNewRoomDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	SetWindowText(m_strTitle);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
