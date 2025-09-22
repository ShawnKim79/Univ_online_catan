// UserInfoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Lobby.h"
#include "UserInfoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CUserInfoDlg dialog


CUserInfoDlg::CUserInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CUserInfoDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CUserInfoDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CUserInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CUserInfoDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CUserInfoDlg, CDialog)
	//{{AFX_MSG_MAP(CUserInfoDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUserInfoDlg message handlers

void CUserInfoDlg::SetUserInfo(USERINFO userinfo)
{
	m_Userinfo = userinfo;
}

BOOL CUserInfoDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CString str;
	
	str.Format("   %s", (LPCTSTR)m_Userinfo.id);
	GetDlgItem(IDC_USERID)->SetWindowText(str);

	str.Format("   %s", (LPCTSTR)m_Userinfo.alias);
	GetDlgItem(IDC_USERNICK)->SetWindowText(str);
	
	str.Format("   %d", m_Userinfo.score);
	GetDlgItem(IDC_USERSCORE)->SetWindowText(str);
	
	str.Format("   %dÀü %d½Â %d¹« %dÆÐ", m_Userinfo.playcount, m_Userinfo.victorycount, m_Userinfo.drawcount, m_Userinfo.losscount);
	GetDlgItem(IDC_STATISTICS)->SetWindowText(str);

	float victoryrate = 0.0f;

	if(m_Userinfo.playcount) victoryrate = ((float)m_Userinfo.victorycount) / ((float)m_Userinfo.playcount) * 100.0;
	
	str.Format("   %3.1f %%", victoryrate);
	
	GetDlgItem(IDC_RATIO)->SetWindowText(str);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
