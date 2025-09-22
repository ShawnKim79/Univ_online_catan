// ItemInputDlg.cpp : implementation file
//

#include "stdafx.h"
#include "List.h"
#include "ItemInputDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CItemInputDlg dialog


CItemInputDlg::CItemInputDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CItemInputDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CItemInputDlg)
	m_strName = _T("");
	m_strPhone = _T("");
	m_nAge = 0;
	m_nSex = 0;
	//}}AFX_DATA_INIT
}


void CItemInputDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CItemInputDlg)
	DDX_Text(pDX, IDC_EDIT1, m_strName);
	DDV_MaxChars(pDX, m_strName, 10);
	DDX_Text(pDX, IDC_EDIT2, m_strPhone);
	DDV_MaxChars(pDX, m_strPhone, 15);
	DDX_Text(pDX, IDC_EDIT3, m_nAge);
	DDV_MinMaxInt(pDX, m_nAge, 0, 120);
	DDX_Radio(pDX, IDC_RADIO2, m_nSex);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CItemInputDlg, CDialog)
	//{{AFX_MSG_MAP(CItemInputDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CItemInputDlg message handlers
