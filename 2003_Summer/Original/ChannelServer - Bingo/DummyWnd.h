#if !defined(AFX_DUMMYWND_H__C3F13791_BEDA_45AD_B0A0_95100125404B__INCLUDED_)
#define AFX_DUMMYWND_H__C3F13791_BEDA_45AD_B0A0_95100125404B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DummyWnd.h : header file
//

#include "WSASocket.h"
#include "TrayIcon.h"
#include "structs.h"
#include "ServerManager.h"
#include "Message.h"

#include <list>
#include <string>
using namespace std;

#define WM_TRAYNOTIFY	(WM_USER + 10)

/////////////////////////////////////////////////////////////////////////////
// CDummyWnd frame

class CDummyWnd : public CWnd
{
	DECLARE_DYNCREATE(CDummyWnd)
public:
	CDummyWnd();           // protected constructor used by dynamic creation

// Attributes
public:

// Operations
public:
	void RedirectClient(CMessage *pMsg);
	void SendChInfo(SOCKET socket);
	void ProcessMessageServer(CMessage *pMsg);
	void ProcessMessageClient(CMessage *pMsg);
	CWSASocket m_ClientSocket; // 클라이언트의 연결을 관리할 소켓
	CWSASocket m_ServerSocket; // 서버의 연결을 관리할 소켓
	CServerManager m_ServerManager;

protected:

	UINT m_nPortServer;
	UINT m_nPortClient;

	CTrayIcon m_TrayIcon;

	CBitmap Start;
	CBitmap Stop;
	CBitmap Conf;
	CBitmap Exit;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDummyWnd)
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CDummyWnd();

	// Generated message map functions
	//{{AFX_MSG(CDummyWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnStartserver();
	afx_msg void OnStopserver();
	afx_msg void OnConfig();
	afx_msg void OnExit();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	afx_msg LRESULT OnTrayNotify(WPARAM wParam, LPARAM lParam);
	afx_msg long OnSocketMsgGameServer(WPARAM wParam, LPARAM lParam);
	afx_msg long OnSocketMsgClient(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DUMMYWND_H__C3F13791_BEDA_45AD_B0A0_95100125404B__INCLUDED_)
