#if !defined(AFX_SERVERSOCKET_H__8B253B4E_A13D_489F_9A94_F424167499B4__INCLUDED_)
#define AFX_SERVERSOCKET_H__8B253B4E_A13D_489F_9A94_F424167499B4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ServerSocket.h : header file
//



/////////////////////////////////////////////////////////////////////////////
// CServerSocket command target

class CServerSocket : public CAsyncSocket
{
// Attributes
public:

// Operations
public:
	FILE *fp;
	CServerSocket();
	virtual ~CServerSocket();

	char memo[5000];
	int memo_len;
	
	int memo_n;
	int memo_len_n;

// Overrides
public:
	int memo_copy(char *copymemo);
	void ch_send(char *data, int len);
	
	bool use;
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CServerSocket)
	public:
	virtual void OnReceive(int nErrorCode);
	virtual void OnClose(int nErrorCode);
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CServerSocket)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

// Implementation
protected:
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SERVERSOCKET_H__8B253B4E_A13D_489F_9A94_F424167499B4__INCLUDED_)
