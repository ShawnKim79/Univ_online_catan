// ServerStatusDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GameServer.h"
#include "ServerStatusDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CServerStatusDlg dialog


CServerStatusDlg::CServerStatusDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CServerStatusDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CServerStatusDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CServerStatusDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CServerStatusDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CServerStatusDlg, CDialog)
	//{{AFX_MSG_MAP(CServerStatusDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CServerStatusDlg message handlers
