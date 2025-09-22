#if !defined(AFX_CLIENT_H__88E2416E_9764_441D_AD82_B421A1E673B1__INCLUDED_)
#define AFX_CLIENT_H__88E2416E_9764_441D_AD82_B421A1E673B1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Client.h : header file
//



/////////////////////////////////////////////////////////////////////////////
// CClient command target

class CClient : public CAsyncSocket
{
// Attributes
public:

// Operations
public:
	char memo[5000];
	int memo_len;
	
	int memo_n;
	int memo_len_n;

	bool use;			// 접속 성공
	int no;

	CClient();
	virtual ~CClient();

// Overrides
public:
	
	int memo_copy(char *copymemo);
	void ch_send(char *data, int len);
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CClient)
	public:
	virtual void OnReceive(int nErrorCode);
	virtual void OnClose(int nErrorCode);
	virtual void OnConnect(int nErrorCode);
	//}}AFX_VIRTUAL
	
	// Generated message map functions
	//{{AFX_MSG(CClient)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

// Implementation
protected:
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CLIENT_H__88E2416E_9764_441D_AD82_B421A1E673B1__INCLUDED_)
