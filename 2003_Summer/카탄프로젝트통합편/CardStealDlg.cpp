// CardStealDlg.cpp : implementation file
//

#include "stdafx.h"
#include "카탄.h"
#include "CardStealDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCardStealDlg dialog


CCardStealDlg::CCardStealDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCardStealDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCardStealDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CCardStealDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCardStealDlg)
	DDX_Control(pDX, IDC_PLAYER, m_players);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCardStealDlg, CDialog)
	//{{AFX_MSG_MAP(CCardStealDlg)
	ON_WM_PAINT()
	ON_CBN_SELCHANGE(IDC_PLAYER, OnSelchangePlayer)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCardStealDlg message handlers

BOOL CCardStealDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	select = 0;
	btn_click=false;
	for(int i=0;i<players;i++)m_players.AddString(name[i]);

	m_players.SetCurSel(0);
	OnSelchangePlayer();

	
	///////////////////////////////////////////////
	// 화면을 다시 그려줌 (이상하게 잘안나와서..)
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCardStealDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CBitmap bitmap,*oldbitmap;
	CDC memdc;
	
	int x=0,y=0,n;
	
	memdc.CreateCompatibleDC(&dc);
	
	bitmap.LoadBitmap(IDB_CARDX);

	oldbitmap = memdc.SelectObject(&bitmap);

	for(n=0;n<cardn[select];n++) {
		dc.BitBlt(10+x*60,50+y*80,60,80,&memdc,0,0,SRCCOPY);
		x++;
		if(x>6) {
			x=0;
			y++;
		}
	}
	
	memdc.SelectObject(oldbitmap);
	memdc.DeleteDC();
}

void CCardStealDlg::OnSelchangePlayer() 
{
	CRect rect;
	select=m_players.GetCurSel();
	
	this->GetWindowRect(rect);

	rect.bottom = rect.top+((cardn[select]-1>0 ? cardn[select]-1 : 0)/7+1)*80+100;
	this->MoveWindow(rect,TRUE);

	Invalidate(TRUE);
}

void CCardStealDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	int x=0,y=0,n;

	for(n=0;n<cardn[select];n++) {
		if(point.x>=10+x*60	&& point.x<=70+x*60 && point.y>=50+y*80 && point.y<=130+y*80) {
			btn_click=TRUE;
			OnOK();
		}
		x++;
		if(x>6) {
			x=0;
			y++;
		}
	}
	
	CDialog::OnLButtonDown(nFlags, point);
}


BOOL CCardStealDlg::PreTranslateMessage(MSG* pMsg) 
{
	if(pMsg->message == WM_KEYDOWN)	// 키보드를 눌렀을때 //
	{
		if(	pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE) return TRUE;
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}
