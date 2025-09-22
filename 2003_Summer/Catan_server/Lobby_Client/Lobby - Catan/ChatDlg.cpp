// ChatDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Lobby.h"
#include "ChatDlg.h"
#include "LobbyDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CChatDlg dialog


CChatDlg::CChatDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CChatDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CChatDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CChatDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CChatDlg)
	DDX_Control(pDX, IDC_CHGFONT, m_ctrlChgFont);
	DDX_Control(pDX, IDC_OFFERGAME, m_ctrlOfferGame);
	DDX_Control(pDX, IDC_EXIT, m_ctrlExit);
	DDX_Control(pDX, IDC_LITTLECHAT, m_ctrlChat);
	DDX_Control(pDX, IDC_LITTLECHATVIEW, m_ctrlChatView);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CChatDlg, CDialog)
	//{{AFX_MSG_MAP(CChatDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChatDlg message handlers

BOOL CChatDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CChatDlg::PreTranslateMessage(MSG* pMsg) 
{
	if(pMsg->message == WM_KEYDOWN)
	{
		if(GetFocus() == &m_ctrlChatView) return TRUE;
		if(pMsg->wParam == VK_RETURN)
		{
			if(GetFocus() == &m_ctrlChat) 
			{
				SendMsg();
				return TRUE;
			}

			else if(GetFocus() == this) return TRUE;
		}
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}

void CChatDlg::SendMsg()
{
	CString strMsg;

	m_ctrlChat.GetWindowText(strMsg);

	if(strMsg.GetLength())
	{

	}
}

void CChatDlg::AddChatMessage(CHATINFO chatinfo)
{

}

void CChatDlg::StartChat()
{

}

void CChatDlg::EndChat()
{

}
