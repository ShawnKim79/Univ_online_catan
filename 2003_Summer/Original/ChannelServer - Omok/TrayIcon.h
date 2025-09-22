// TrayIcon.h: interface for the CTrayIcon class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TRAYICON_H__1CBE230F_F626_11D4_9A8B_0050BF123E4A__INCLUDED_)
#define AFX_TRAYICON_H__1CBE230F_F626_11D4_9A8B_0050BF123E4A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CTrayIcon  
{
public:
	void RemoveIcon();
	BOOL SetTooltipText(UINT nID);
	BOOL SetTooltipText(LPCTSTR lpszTip);
	BOOL SetIcon(UINT nIDResource);
	BOOL SetIcon(LPCTSTR lpszIconName);
	BOOL SetIcon(HICON hIcon);
	virtual BOOL Create(CWnd* pWnd, UINT uCallbackMessage, LPCTSTR szTooltip, HICON icon, UINT uID);
	CTrayIcon();
	virtual ~CTrayIcon();

protected:
	NOTIFYICONDATA m_tnd;
};

#endif // !defined(AFX_TRAYICON_H__1CBE230F_F626_11D4_9A8B_0050BF123E4A__INCLUDED_)
