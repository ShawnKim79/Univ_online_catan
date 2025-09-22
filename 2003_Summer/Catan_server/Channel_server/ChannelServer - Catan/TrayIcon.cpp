// TrayIcon.cpp: implementation of the CTrayIcon class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ChannelServer.h"
#include "TrayIcon.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTrayIcon::CTrayIcon()
{

}

CTrayIcon::~CTrayIcon()
{

}

BOOL CTrayIcon::Create(CWnd *pWnd, UINT uCallbackMessage, LPCTSTR szTooltip, HICON hIcon, UINT uID)
{
	m_tnd.cbSize = sizeof(m_tnd);
	m_tnd.hWnd = pWnd->GetSafeHwnd();
	m_tnd.hIcon = hIcon;
	m_tnd.uCallbackMessage = uCallbackMessage;
	m_tnd.uID = uID;
	m_tnd.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
	strcpy(m_tnd.szTip, szTooltip);

	return Shell_NotifyIcon(NIM_ADD, &m_tnd);
}

BOOL CTrayIcon::SetIcon(HICON hIcon)
{
	m_tnd.uFlags = NIF_ICON;
	m_tnd.hIcon = hIcon;

	return Shell_NotifyIcon(NIM_MODIFY, &m_tnd);
}

BOOL CTrayIcon::SetIcon(LPCTSTR lpszIconName)
{
	HICON hIcon = AfxGetApp()->LoadIcon(lpszIconName);

	return SetIcon(hIcon);
}

BOOL CTrayIcon::SetIcon(UINT nIDResource)
{
	HICON hIcon = AfxGetApp()->LoadIcon(nIDResource);

	return SetIcon(hIcon);
}

BOOL CTrayIcon::SetTooltipText(LPCTSTR lpszTip)
{
	m_tnd.uFlags = NIF_TIP;
	_tcscpy(m_tnd.szTip, lpszTip);

	return Shell_NotifyIcon(NIM_MODIFY, &m_tnd);
}

BOOL CTrayIcon::SetTooltipText(UINT nID)
{
	CString str;
	VERIFY(str.LoadString(nID));

	return SetTooltipText(str);
}

void CTrayIcon::RemoveIcon()
{
	m_tnd.uFlags = 0;
	Shell_NotifyIcon(NIM_DELETE, &m_tnd);
}
