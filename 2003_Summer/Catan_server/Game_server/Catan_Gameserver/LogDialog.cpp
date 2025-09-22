// LogDialog.cpp : implementation file
//

#include "stdafx.h"
#include "Catan_Gameserver.h"
#include "LogDialog.h"
#include "afxmt.h"

#include <stdarg.h>
#include <stdio.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CCriticalSection g_Critical; // ...?

/////////////////////////////////////////////////////////////////////////////
// CLogDialog dialog


CLogDialog::CLogDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CLogDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLogDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CLogDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLogDialog)
	DDX_Control(pDX, IDC_LOGEDIT, m_ctrlLog);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLogDialog, CDialog)
	//{{AFX_MSG_MAP(CLogDialog)
	ON_WM_SIZE()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_SHOWALL, OnShowall)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLogDialog message handlers

void CLogDialog::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
	CWnd *pWnd = GetDlgItem(IDC_LOGEDIT);

	if(IsWindow(pWnd->GetSafeHwnd()))
	{
		pWnd->SetWindowPos(NULL, 0, 0, cx, cy-30, SWP_NOZORDER | SWP_NOMOVE);
	}
}

BOOL CLogDialog::PreTranslateMessage(MSG* pMsg) 
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

void CLogDialog::AddLogString(const char *format, ...)
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

	FILE *fp = fopen("c:\\gameserver.log", "a");
	fprintf(fp, "%s", str);
	fclose(fp);

//	g_Critical.Lock();

//	m_ctrlLog.SetSel(-1, -1);
//	m_ctrlLog.ReplaceSel(str);

//	g_Critical.Unlock();
}

void CLogDialog::OnClose()
{
	ShowWindow(SW_HIDE);
}

void CLogDialog::OnShowall()
{

}
