#if !defined(AFX_CHATDLG_H__76DEA5ED_5B49_469F_B9F8_F710763DCC70__INCLUDED_)
#define AFX_CHATDLG_H__76DEA5ED_5B49_469F_B9F8_F710763DCC70__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ChatDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CChatDlg dialog

#include "structs.h"
#include "Message.h"
#include "ImgButton.h"

class CChatDlg : public CDialog
{
// Construction
public:
	void EndChat();
	void StartChat();
	void AddChatMessage(CHATINFO chatinfo);
	void SendMsg();
	CChatDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CChatDlg)
	enum { IDD = IDD_CHAT_DIALOG };
	CImgButton	m_ctrlChgFont;
	CImgButton	m_ctrlOfferGame;
	CImgButton	m_ctrlExit;
	CEdit	m_ctrlChat;
	CRichEditCtrl	m_ctrlChatView;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChatDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CHARFORMAT m_cf;

	// Generated message map functions
	//{{AFX_MSG(CChatDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHATDLG_H__76DEA5ED_5B49_469F_B9F8_F710763DCC70__INCLUDED_)
