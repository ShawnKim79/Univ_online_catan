// WaitRoomDlg.cpp : implementation file
//

#include "stdafx.h"
#include "카탄.h"
#include "WaitRoomDlg.h"
#include "math.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWaitRoomDlg dialog


CWaitRoomDlg::CWaitRoomDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CWaitRoomDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CWaitRoomDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	
	m_p.x=0;
	m_p.y=0;
	set=false;
}


void CWaitRoomDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWaitRoomDlg)
	DDX_Control(pDX, IDC_INPUT_EDIT, m_input_edit);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CWaitRoomDlg, CDialog)
	//{{AFX_MSG_MAP(CWaitRoomDlg)
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_EXIT_BTN, OnExitBtn)
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWaitRoomDlg message handlers

void CWaitRoomDlg::set_rgn()
{
	CRgn rgn,rgn2;
	int x,y,x1,max_x,max_y;
	char bit[4];
	bool x2;

	FILE *fp=fopen("./images/other/wait_room.bmp","rb");
	
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

			if(RGB(bit[2],bit[1],bit[0])!=RGB(255,0,0)) {
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

BOOL CWaitRoomDlg::OnEraseBkgnd(CDC* pDC) 
{
	static HBITMAP hBMP=NULL,oldBMP;
	static BITMAP  bmp;
	bool win_rect=false;
	HDC     hDCImage;
	CRect   rect;
	if(!hBMP) {
		hBMP = (HBITMAP)LoadImage(AfxGetInstanceHandle(), "./images/other/wait_room.bmp"
										, IMAGE_BITMAP, 0, 0,LR_LOADFROMFILE);
		GetObject(hBMP, sizeof(bmp), &bmp);
	}

	hDCImage = CreateCompatibleDC(pDC->m_hDC);

	CRect win_which;
	this->GetWindowRect(win_which);
	this->SetWindowPos(NULL, win_which.left,win_which.top, bmp.bmWidth, bmp.bmHeight, SWP_NOZORDER);
	
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


BOOL CWaitRoomDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CRect rect;
	((CListCtrl *)GetDlgItem(IDC_WAIT_LIST))->SetExtendedStyle(LVS_EX_FULLROWSELECT);
	((CListCtrl *)GetDlgItem(IDC_WAIT_LIST))->GetClientRect(rect);
	
	((CListCtrl *)GetDlgItem(IDC_WAIT_LIST))->InsertColumn(0,"방이름",LVCFMT_CENTER,70);
	((CListCtrl *)GetDlgItem(IDC_WAIT_LIST))->InsertColumn(1,"인원",LVCFMT_CENTER,rect.Width()-70);

	set_rgn();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CWaitRoomDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	SendMessage(WM_NCLBUTTONDOWN,HTCAPTION,0);
	CDialog::OnLButtonDown(nFlags, point);
}

void CWaitRoomDlg::OnExitBtn() 
{
	OnOK();
}

void CWaitRoomDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	EyeMove();
}

#define M_PI 3.141592

double CWaitRoomDlg::angle(int x1, int y1, int x2, int y2)
{
    double a,rad;
	if((x2-x1)!=0) {
		a= double(y2-y1) / (x2-x1);
		rad = 90-(atan(a) / M_PI*180);
		
		if(x2>=x1 && y2<=y1) rad=360+rad;
		else if(x2<=x1) rad=180+rad;

	} else {
		if(y2>=y1) rad=0;
		else if(y2<y1) rad=180;
	}

    return rad;
}

void CWaitRoomDlg::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	m_p=point;
	EyeMove();

	CDialog::OnMouseMove(nFlags, point);
}

#define abs(x) (x>0 ? x : -x)

void CWaitRoomDlg::EyeMove()
{
	CDC *pDC=GetDC();
	
	CBrush black_brush, *old_brush;
	black_brush.CreateSolidBrush(RGB(0,0,0));
	
	old_brush=pDC->SelectObject(&black_brush);
	pDC->Ellipse(40,80,60,100);
	pDC->SelectObject(old_brush);

	double rad=angle(50,90,m_p.x,m_p.y);
	
	int x = int(sin(rad * M_PI / 180.) * 5);
	int y = int(cos(rad * M_PI / 180.) * 5);

	if(sqrt((m_p.x-50)*(m_p.x-50)+(m_p.y-90)*(m_p.y-90))<5){
		x = m_p.x-50;
		y = m_p.y-90;
	}
	
	pDC->Ellipse(	x+50-5,y+90-5, x+50+5,y+90+5);
	
	ReleaseDC(pDC);
}

BOOL CWaitRoomDlg::PreTranslateMessage(MSG* pMsg) 
{
	if(pMsg->hwnd == m_input_edit.GetSafeHwnd() && pMsg->message == WM_KEYDOWN)	{
		CRect rect;
		m_input_edit.GetWindowRect(rect);
		m_p.y = rect.top;
		
		CString str;
		m_input_edit.GetWindowText(str);

		m_p.x = str.GetLength()*16;
		EyeMove();
	}
	return CDialog::PreTranslateMessage(pMsg);
}
