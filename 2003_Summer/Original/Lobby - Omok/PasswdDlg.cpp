// PasswdDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Lobby.h"
#include "PasswdDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPasswdDlg dialog


CPasswdDlg::CPasswdDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPasswdDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPasswdDlg)
	m_strPasswd = _T("");
	//}}AFX_DATA_INIT
}


void CPasswdDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPasswdDlg)
	DDX_Control(pDX, IDC_OK, m_ctrlOk);
	DDX_Text(pDX, IDC_PASSWD, m_strPasswd);
	DDV_MaxChars(pDX, m_strPasswd, 8);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPasswdDlg, CDialog)
	//{{AFX_MSG_MAP(CPasswdDlg)
	ON_BN_CLICKED(IDC_OK, OnOk)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPasswdDlg message handlers

BOOL CPasswdDlg::PreTranslateMessage(MSG* pMsg) 
{
	if(pMsg->message == WM_KEYDOWN)
	{
		if(pMsg->wParam == VK_RETURN)
		{
			UpdateData();
			CDialog::OnOK();
		}

		else if(pMsg->wParam == VK_ESCAPE)
		{
			CDialog::OnCancel();
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}

void CPasswdDlg::OnOk() 
{
	UpdateData();
	CDialog::OnOK();
}
