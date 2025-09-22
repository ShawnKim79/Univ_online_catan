// TrainDlg.cpp : implementation file
//

#include "stdafx.h"
#include "카탄.h"
#include "TrainDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTrainDlg dialog


CTrainDlg::CTrainDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTrainDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTrainDlg)
	//}}AFX_DATA_INIT
	int i;

	select=0;
	con=0;
	turn=0;
	for(i=0;i<6;i++)hangu[i]=false;
	for(i=0;i<5;i++)resource[i]=0;
	sn=0;
	tn=0;
	target_select=-1;
	trade_ok=false;
	playern=0;
}


void CTrainDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTrainDlg)
	DDX_Control(pDX, IDC_SELECT, m_select);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTrainDlg, CDialog)
	//{{AFX_MSG_MAP(CTrainDlg)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_SOURCE_BTN, OnSourceBtn)
	ON_BN_CLICKED(IDC_TARGET_BTN, OnTargetBtn)
	ON_BN_CLICKED(IDC_CANCEL_BTN, OnCancelBtn)
	ON_BN_CLICKED(IDC_NO_BTN, OnNoBtn)
	ON_CBN_SELCHANGE(IDC_SELECT, OnSelchangeSelect)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTrainDlg message handlers

void CTrainDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CBitmap bitmap[5],*oldbitmap;
	CDC memdc;
	
	int x,n;
	
	memdc.CreateCompatibleDC(&dc);
	
	bitmap[0].LoadBitmap(IDB_CARD1);
	bitmap[1].LoadBitmap(IDB_CARD2);
	bitmap[2].LoadBitmap(IDB_CARD3);
	bitmap[3].LoadBitmap(IDB_CARD4);
	bitmap[4].LoadBitmap(IDB_CARD5);

	oldbitmap = memdc.SelectObject(&bitmap[0]);

	for(n=0;n<5;n++) {
		memdc.SelectObject(&bitmap[n]);
		for(x=0;x<resource[n];x++) {
			dc.BitBlt(x*2+10+n*100,50,60,80,&memdc,0,0,SRCCOPY);
		}
	}

	for(x=0;x<sn;x++) {
		memdc.SelectObject(&bitmap[source[x]]);
		dc.BitBlt(x*20+20,200,60,80,&memdc,0,0,SRCCOPY);
	}
	
	if(con) {
		for(x=0;x<tn;x++) {
			memdc.SelectObject(&bitmap[target[x]]);
			dc.BitBlt(x*20+20,350,60,80,&memdc,0,0,SRCCOPY);
		}
	} else {
		for(x=0;x<tn;x++) {
			memdc.SelectObject(&bitmap[target[x]]);
			dc.BitBlt(x*65+20,350,60,80,&memdc,0,0,SRCCOPY);
			if(target_select==x) {
				dc.MoveTo(x*65+20,350);
				dc.LineTo(x*65+80,350);
				dc.LineTo(x*65+80,430);
				dc.LineTo(x*65+20,430);
				dc.LineTo(x*65+20,350);
			}
		}
	}

	memdc.SelectObject(oldbitmap);
	memdc.DeleteDC();
	
	for(int i=0;i<5;i++)bitmap[i].DeleteObject();
}

BOOL CTrainDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_select.AddString("항구와의 거래");
	for(int i=0;i<playern;i++) m_select.AddString(player_name[i]);
		
	m_select.SetCurSel(select);

	switch(con) {
	case 0:	GetDlgItem(IDC_CANCEL_BTN)->EnableWindow(FALSE);
			GetDlgItem(IDC_TARGET_BTN)->EnableWindow(FALSE);
			break;
	
	case 1:
	case 2:	GetDlgItem(IDC_SOURCE_BTN)->EnableWindow(FALSE);
			m_select.EnableWindow(FALSE);	
			break;
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTrainDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	int n;
	
	// 카드선택

	if(turn==0) {
		for(n=0;n<5;n++) {
			if(!resource[n]) continue;
			if(point.x>=10+n*100 && point.x<=70+resource[n]*2+n*100 && point.y>=50 && point.y<=130) {
				if(con==0 && sn<20) {
					source[sn++]=n;	// 소스에 집어넣는다.
					resource[n]--;
				
				}

				if(con==1 && tn<20) {
					target[tn++]=n; // 타겟에 집어넣는다.
					resource[n]--;
				}
				

				Invalidate(TRUE);
				goto Button_End;
			}
		}

		// 카드빼기
		if(con==0) {
			for(int x=sn-1;x>=0;x--) {
				if(point.x>=x*20+20 && point.x<=x*20+80 && point.y>=200 && point.y<=280) {
					resource[source[x]]++;
					for(int n=x;n<sn-1;n++)source[n]=source[n+1];
					sn--;
					Invalidate(TRUE);
					goto Button_End;
				}
			}
		}
		if(con==1) {
			for(int x=sn-1;x>=0;x--) {
				if(point.x>=x*20+20 && point.x<=x*20+80 && point.y>=350 && point.y<=430) {
					resource[target[x]]++;
					for(int n=x;n<tn-1;n++)target[n]=target[n+1];
					tn--;
					Invalidate(TRUE);
					goto Button_End;
				}
			}
		}
	}

	if(turn==1) {
		if(select==0) {
			for(int x=0;x<tn;x++) {
				if(point.x>=x*65+20 && point.x<=x*65+80 && point.y>=350 && point.y<=430) {
					target_select=x;
					GetDlgItem(IDC_TARGET_BTN)->EnableWindow(TRUE);
					Invalidate(TRUE);
					goto Button_End;
				}
			}
		}
	}

Button_End:
	CDialog::OnLButtonDown(nFlags, point);
}

void CTrainDlg::OnSourceBtn() 
{
	int i;
	if(turn==0) {
		if(select==0 && con==0) {
			int s=source[0];
			for(i=1;i<sn;i++)if(source[i]!=s) break;
			if(i!=sn) return;

			if(	sn==4 || (sn==3 && hangu[0]==true) || (sn==2 && hangu[s+1]==true)) {
				tn=0;
				for(tn=0;tn<5;tn++)target[tn]=tn;
			} else return;
		}
		if(select && con==0) {
			trade_ok=true;
			OnOK();
		}
	}
	turn=1;
	m_select.EnableWindow(FALSE);
	GetDlgItem(IDC_CANCEL_BTN)->EnableWindow(TRUE);
	GetDlgItem(IDC_SOURCE_BTN)->EnableWindow(FALSE);
	Invalidate(TRUE);
}

void CTrainDlg::OnTargetBtn() 
{
	// TODO: Add your control notification handler code here
	trade_ok=true;
	OnOK();
}

void CTrainDlg::OnCancelBtn() 
{
	if(con==0) {
		tn=0;
		GetDlgItem(IDC_SOURCE_BTN)->EnableWindow(TRUE);
		GetDlgItem(IDC_CANCEL_BTN)->EnableWindow(FALSE);
		GetDlgItem(IDC_TARGET_BTN)->EnableWindow(FALSE);
		m_select.EnableWindow(TRUE);
	} else OnOK();

	turn=0;
	Invalidate(TRUE);
}

void CTrainDlg::OnNoBtn() 
{
	OnOK();	
}

void CTrainDlg::OnSelchangeSelect() 
{
	select=m_select.GetCurSel();	
}
