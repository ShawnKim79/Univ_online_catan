// CardUseDlg.cpp : implementation file
//

#include "stdafx.h"
#include "카탄.h"
#include "CardUseDlg.h"
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCardUseDlg dialog


CCardUseDlg::CCardUseDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCardUseDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCardUseDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	strcpy(card_inf,"1234566666666666666778899AB");
	
	select=-1;
	select_ok=false;
}


void CCardUseDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCardUseDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCardUseDlg, CDialog)
	//{{AFX_MSG_MAP(CCardUseDlg)
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_CANCEL, OnCancel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCardUseDlg message handlers

void CCardUseDlg::OnPaint() 
{
	CPaintDC dc(this);
	CDC *pDC=GetDC();

	CBitmap map;
	CDC MemDC,MemDC2;

	map.CreateCompatibleBitmap(pDC,900,450);
	
	MemDC.CreateCompatibleDC(pDC);
	MemDC2.CreateCompatibleDC(pDC);
	CBitmap *pTempBitmap = (CBitmap *) MemDC.SelectObject(&map);
	CBitmap *pTempBitmap2 = (CBitmap *) MemDC2.SelectObject(&card_map);

	MemDC.BitBlt(0,0,900,450,&MemDC2,0,0,SRCCOPY);

	if(select!=-1) {
		CPen pen,*oldpen;
		pen.CreatePen(PS_SOLID,2,RGB(255,255,255));
		oldpen=MemDC.SelectObject(&pen);

		MemDC.MoveTo((select%9)*100,(select/9)*150);
		MemDC.LineTo((select%9)*100,(select/9)*150+150);
		MemDC.LineTo((select%9)*100+100,(select/9)*150+150);
		MemDC.LineTo((select%9)*100+100,(select/9)*150);
		MemDC.LineTo((select%9)*100,(select/9)*150);
		MemDC.SelectObject(oldpen);
		pen.DeleteObject();
	}
	
	dc.BitBlt(25,30,900,450,&MemDC,0,0,SRCCOPY);

	MemDC2.SelectObject(pTempBitmap2);
	MemDC2.DeleteDC();
	
	MemDC.SelectObject(pTempBitmap);
	MemDC.DeleteDC();
	
	map.DeleteObject();
	ReleaseDC(pDC);
}

BOOL CCardUseDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// 카드세팅 //
	int i,j;

	for(i=0;i<27;i++) {
		for(j=0;j<cardn;j++)if(card[j]==i) break;
		if(j==cardn) card_inf[i]='0';
	}

	// MemDC 가상화면

	CDC *pDC=GetDC(),MemDC,MemDC2;
	CBitmap bitmap;
	char filename[50];
	int x=0,y=0;	

	MemDC.CreateCompatibleDC(pDC);
	MemDC2.CreateCompatibleDC(pDC);
	
	card_map.CreateCompatibleBitmap(pDC,900,450);
	CBitmap *pTempBitmap = (CBitmap *) MemDC.SelectObject(&card_map);
	CBitmap *pTempBitmap2;
	
	for(int n=0;n<27;n++) {
		strcpy(filename,"./images/card/dcard1.bmp");

		if(card_inf[n]!='0') filename[19] = card_inf[n];
		else filename[19]='x';

		HBITMAP hBitmap;

		hBitmap = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), (LPCTSTR)
		filename, IMAGE_BITMAP, 0, 0,LR_LOADFROMFILE);
		
		bitmap.Attach(hBitmap);
		
		pTempBitmap2 = MemDC2.SelectObject(&bitmap);
		MemDC.BitBlt(x*100,y*150,100,150,&MemDC2,0,0,SRCCOPY);
		MemDC2.SelectObject(pTempBitmap2);
		bitmap.DeleteObject();

		if(++x>8) {
			x=0;
			y++;
		}
	}

	MemDC.SelectObject(pTempBitmap);
	MemDC2.DeleteDC();
	MemDC.DeleteDC();
	ReleaseDC(pDC);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCardUseDlg::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default

	int x=0,y=0;

	for(int n=0;n<27;n++) {
		if(point.x>=x*100+26 && point.x<=x*100+124 && point.y>=y*150+31 && point.y<=y*150+179 && card_inf[n]!='0') {
			select=n;
			Invalidate(FALSE);
			break;
		}

		if(++x>8) {
			x=0;
			y++;
		}
	}
	

	CDialog::OnMouseMove(nFlags, point);
}

BOOL CCardUseDlg::PreTranslateMessage(MSG* pMsg) 
{
	if(pMsg->message == WM_KEYDOWN)	// 키보드를 눌렀을때 //
	{
		if(	pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE) return TRUE;
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}

void CCardUseDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	int x=0,y=0;
	for(int n=0;n<27;n++) {
		if(point.x>=x*100+26 && point.x<=x*100+124 && point.y>=y*150+31 && point.y<=y*150+179 && card_inf[n]!='0') {
			if(n<5 ||n>24) break;
			select=n;
			select_ok=true;
			OnOK();
		}

		if(++x>8) {
			x=0;
			y++;
		}
	}
	
	CDialog::OnLButtonDown(nFlags, point);
}

void CCardUseDlg::OnCancel() 
{
	OnOK();	
}
