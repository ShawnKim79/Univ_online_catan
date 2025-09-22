// DropDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Ä«Åº.h"
#include "DropDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDropDlg dialog


CDropDlg::CDropDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDropDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDropDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	for(int i=0;i<5;i++)target_resource[i]=0;	
	drop_w=0;
	strcpy(caption,"");
}


void CDropDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDropDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDropDlg, CDialog)
	//{{AFX_MSG_MAP(CDropDlg)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_DROP_BTN, OnDropBtn)
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDropDlg message handlers

void CDropDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CBitmap bgmap,bitmap[5],*oldbitmap;
	CDC memdc;
	
	int y;
	
	memdc.CreateCompatibleDC(&dc);
	
	bitmap[0].LoadBitmap(IDB_CARD1);
	bitmap[1].LoadBitmap(IDB_CARD2);
	bitmap[2].LoadBitmap(IDB_CARD3);
	bitmap[3].LoadBitmap(IDB_CARD4);
	bitmap[4].LoadBitmap(IDB_CARD5);

	oldbitmap = memdc.SelectObject(&bitmap[0]);

	dc.SetBkMode(TRANSPARENT);
	for(y=0;y<5;y++) {
		CString str;
		memdc.SelectObject(&bitmap[y]);
		dc.BitBlt(50,10+y*80,60,80,&memdc,0,0,SRCCOPY);
		str.Format("x %d ",resource[y]);
		dc.TextOut(120,40+y*80,str);
	}
	
	dc.TextOut(160,190,"¢º¢º¢º");
	
	for(y=0;y<5;y++) {
		CString str;
		memdc.SelectObject(&bitmap[y]);
		dc.BitBlt(220,14+y*80,60,80,&memdc,0,0,SRCCOPY);
		str.Format("x %d ",target_resource[y]);
		dc.TextOut(290,40+y*80,str);
	}

	dc.SetBkMode(OPAQUE);
	
	char str[100];
	itoa(drop_n-drop_w,str,10);
	GetDlgItem(IDC_CARD_N)->SetWindowText(str);

	memdc.SelectObject(oldbitmap);
	memdc.DeleteDC();
	
	for(int i=0;i<5;i++)bitmap[i].DeleteObject();
}

void CDropDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	int y;

	for(y=0;y<5;y++) {
		if(!resource[y]) continue;
		if(point.x>=50 && point.x<=110 && point.y>=10+y*84 && point.y<=90+y*84 && drop_n>drop_w) {
			target_resource[y]++;
			resource[y]--;
			drop_w++;
			
			Invalidate(TRUE);
			goto Button_End;
		}
	}

	for(y=0;y<5;y++) {
		if(!target_resource[y]) continue;
		if(point.x>=220 && point.x<=280 && point.y>=10+y*84 && point.y<=90+y*84 && 0<drop_w) {
			target_resource[y]--;
			resource[y]++;
			drop_w--;
			
			Invalidate(TRUE);
			goto Button_End;
		}
	}
	SendMessage(WM_NCLBUTTONDOWN,HTCAPTION,0);

Button_End:
	CDialog::OnLButtonDown(nFlags, point);
}

BOOL CDropDlg::PreTranslateMessage(MSG* pMsg) 
{
	
	if(pMsg->message == WM_KEYDOWN)	// Å°º¸µå¸¦ ´­·¶À»¶§ //
	{
		if(	pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE) return TRUE;
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}

void CDropDlg::OnDropBtn() 
{
	if(drop_n-drop_w==0) OnOK();
}

BOOL CDropDlg::OnEraseBkgnd(CDC* pDC) 
{
	static HBITMAP hBMP=NULL,oldBMP;
	static BITMAP  bmp;
	bool win_rect=false;
	HDC     hDCImage;
	CRect   rect;
	if(!hBMP) {
		hBMP = (HBITMAP)LoadImage(AfxGetInstanceHandle(), "./images/other/drop_bg.bmp"
										, IMAGE_BITMAP, 0, 0,LR_LOADFROMFILE);
		GetObject(hBMP, sizeof(bmp), &bmp);
	}

	hDCImage = CreateCompatibleDC(pDC->m_hDC);
	
	if( hDCImage != NULL )
	{   
		oldBMP = (HBITMAP)SelectObject(hDCImage, hBMP);
		::BitBlt(pDC->m_hDC, 0, 0, bmp.bmWidth, bmp.bmHeight, hDCImage, 0, 0, SRCCOPY);
		SelectObject(hDCImage, oldBMP);
		DeleteObject(hDCImage);
	}

	return false;	
	//return CDialog::OnEraseBkgnd(pDC);
}

void CDropDlg::set_rgn()
{
	CRgn rgn,rgn2;
	int x,y,x1,max_x,max_y;
	char bit[4];
	bool x2;

	FILE *fp=fopen("./images/other/drop_bg.bmp","rb");
	
	rgn.CreateRectRgn(0,0,0,0);
				
	fseek(fp,16,0);for(x=0;x<4;x++)fread(&bit[x>1 ? x%2 : 2+x%2],1,1,fp);	memcpy(&max_x,bit,4);
	fseek(fp,20,0);for(x=0;x<4;x++)fread(&bit[x>1 ? x%2 : 2+x%2],1,1,fp);	memcpy(&max_y,bit,4);

	fseek(fp,54,0);
		
	for(y=max_y-1;y>=0;y--) {
		int bit_n=max_x*3;

		x1=-1;
		x2=false;
		for(x=0;x<max_x;x++) {
			fread(bit,1,3,fp);

			if(RGB(bit[2],bit[1],bit[0])!=RGB(0,0,255)) {
				if(x1==-1) x1=x;
			} else if(x1!=-1) x2=true;
			
			if(x1!=-1 && (x2 || x==max_x-1)) {
				rgn2.CreateRectRgn(x1,y,x,y+1);
				rgn.CombineRgn(&rgn,&rgn2,RGN_OR);
				rgn2.DeleteObject();
				x1=-1;
				x2=false;
			}
		}
		if(bit_n%4) for(x=0;x<4-(bit_n%4);x++) fread(bit,1,1,fp);
	}
	fclose(fp);

	SetWindowRgn(rgn,TRUE);

	rgn.DeleteObject();

}

BOOL CDropDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	GetDlgItem(IDC_CAPTION_STATIC)->SetWindowText(caption);
	
	set_rgn();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}