#if !defined(AFX_MIRLISTCTRL_H__9EDE1EC9_55B7_48A4_9451_D9FAB6F6724B__INCLUDED_)
#define AFX_MIRLISTCTRL_H__9EDE1EC9_55B7_48A4_9451_D9FAB6F6724B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MirListCtrl.h : header file
//

// #include "MirHeaderCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CMirHeaderCtrl window

class CMirHeaderCtrl : public CHeaderCtrl
{
// Construction
public:
	CMirHeaderCtrl();
	virtual ~CMirHeaderCtrl();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMirHeaderCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetColors(const COLORREF colorBack, const COLORREF colorText);

protected:
    COLORREF m_colorText, m_colorBack, m_colorBorder;

	// Generated message map functions
protected:
	//{{AFX_MSG(CMirHeaderCtrl)
	afx_msg void OnCustomDraw(NMHDR *pNMHDR, LRESULT *pResult);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////
// CMirListCtrl window

class CMirListCtrl : public CListCtrl
{
// Construction
public:
	CMirListCtrl();
	virtual ~CMirListCtrl();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMirListCtrl)
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetToolTips(BOOL bEnable = TRUE);
	int     OnToolHitTest(CPoint point, TOOLINFO *pTI) const;
	void    SetHeaderColors(const COLORREF colorBack, const COLORREF colorText);
	void    SetHighlightColors(const COLORREF colorBack, const COLORREF colorText);
	void    SetBackColors(const COLORREF colorOdd, const COLORREF colorEven);
	void    SetTextColors(const COLORREF colorOdd, const COLORREF colorEven);
    inline  void SetColumnCount(int nCount) { m_nColumnCount = nCount; }

protected:
    int      m_nColumnCount;
    LOGFONT  m_LogFont;
    COLORREF m_colorTextOdd, m_colorTextEven, m_colorBackOdd, m_colorBackEven;
    COLORREF m_colorTextHighlight, m_colorBackHighlight;
    CMirHeaderCtrl m_HeaderCtrl;

	// Generated message map functions
protected:
	int GetRowFromPoint(CPoint &point, CRect *rcCell, int *nCol) const;
	void    DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);

	//{{AFX_MSG(CMirListCtrl)
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	//}}AFX_MSG
    afx_msg BOOL OnToolTipText(UINT id, NMHDR *pNMHDR, LRESULT *pResult);

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MIRLISTCTRL_H__9EDE1EC9_55B7_48A4_9451_D9FAB6F6724B__INCLUDED_)
