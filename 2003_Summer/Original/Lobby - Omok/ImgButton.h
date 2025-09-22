#if !defined(AFX_IMGBUTTON_H__88BAD70E_28CC_4AB9_9C59_36E9A9E965B8__INCLUDED_)
#define AFX_IMGBUTTON_H__88BAD70E_28CC_4AB9_9C59_36E9A9E965B8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ImgButton.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CImgButton window

enum { BS_NORMAL, BS_OVER, BS_CLICK, BS_DISABLED };

class CImgButton : public CButton
{
// Construction
public:
	CImgButton();

// Attributes
public:

	CToolTipCtrl m_ToolTip;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CImgButton)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetToolTip(CString strTip);
	void SetToolTip(UINT nID);
	void SetImages(UINT ResourceIDNormal = 0, UINT ResourceIDSel = 0, UINT ResourceIDOver = 0, UINT ResourceIDDisable = 0);
	virtual ~CImgButton();

	// Generated message map functions
protected:
	//{{AFX_MSG(CImgButton)
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnEnable(BOOL bEnable);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

private:
	UINT m_nImgNormal;
	UINT m_nImgClicked;
	UINT m_nImgOver;
	UINT m_nImgDisabled;

	UINT m_nBtnState;
	BOOL m_bClicked;

	COLORREF m_clrLeftTop;
	COLORREF m_clrRightBottom;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IMGBUTTON_H__88BAD70E_28CC_4AB9_9C59_36E9A9E965B8__INCLUDED_)
