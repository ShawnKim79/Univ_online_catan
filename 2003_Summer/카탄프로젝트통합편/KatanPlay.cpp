// KatanPlay.cpp : implementation file
//

#include "stdafx.h"
#include "카탄.h"
#include "KatanPlay.h"

#include "dropdlg.h"
#include "cardstealdlg.h"
#include "CardUseDlg.h"
#include "packet_header.h"
#include "traindlg.h"

#include <math.h>
#include "BgSound.h"

#define WM_GAMEOPEN (WM_USER+40)
#define WM_GAMEOVER (WM_USER+20)

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define CON_ROAD		01
#define CON_SETTLE		02
#define CON_CITY		03
#define CON_PLAGUE		04
#define CON_DICE		99

/////////////////////////////////////////////////////////////////////////////
// CKatanPlay dialog


CKatanPlay::CKatanPlay(CWnd* pParent /*=NULL*/)
	: CDialog(CKatanPlay::IDD, pParent)
{
	//{{AFX_DATA_INIT(CKatanPlay)
	//}}AFX_DATA_INIT
	server_use=false;
	trade=true;
	connect_ok=false;
	wait_picture=0;
}


void CKatanPlay::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CKatanPlay)
	DDX_Control(pDX, IDC_CHAT_INPUT, m_chat_input);
	DDX_Control(pDX, IDC_PLAYER_INFORMATION, m_playerlist);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CKatanPlay, CDialog)
	//{{AFX_MSG_MAP(CKatanPlay)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_BN_CLICKED(IDC_ROAD_BTN, OnRoadBtn)
	ON_BN_CLICKED(IDC_SETT_BTN, OnSettBtn)
	ON_BN_CLICKED(IDC_CITY_BTN, OnCityBtn)
	ON_BN_CLICKED(IDC_CARD_BTN, OnCardBtn)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_CHAT_BTN, OnChatBtn)
	ON_BN_CLICKED(IDC_TURN_BTN, OnTurnBtn)
	ON_BN_CLICKED(IDC_CARD_USE_BTN, OnCardUseBtn)
	ON_BN_CLICKED(IDC_TRADE_BTN, OnTradeBtn)
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_SETFOCUS()
	ON_BN_CLICKED(IDC_QUIT_BTN, OnQuitBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKatanPlay message handlers

void CKatanPlay::OnPaint() 
{
	static bool wait_room=true;
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	
	// Do not call CDialog::OnPaint() for painting messages

	draw_map();
	
	char temp[500];
	
	itoa(player.resource[0],temp,10);	GetDlgItem(IDC_CARD1_SU)->SetWindowText(temp);
	itoa(player.resource[1],temp,10);	GetDlgItem(IDC_CARD2_SU)->SetWindowText(temp);
	itoa(player.resource[2],temp,10);	GetDlgItem(IDC_CARD3_SU)->SetWindowText(temp);
	itoa(player.resource[3],temp,10);	GetDlgItem(IDC_CARD4_SU)->SetWindowText(temp);
	itoa(player.resource[4],temp,10);	GetDlgItem(IDC_CARD5_SU)->SetWindowText(temp);

}

void CKatanPlay::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default

	static int EnableLink[] = {0,1,2,3,4,5,6,7,8,9,10,18,19,20,21,31,32,33,34,46,47,48,49,61,62,63,64,74,75,76,77,85,86,87,88,89,90,91,92,93,94,95,-1};

	int x,y;
	char message[800];
	
	CRect view;
	GetDlgItem(IDC_VIEWER)->GetWindowRect(&view);
	ScreenToClient(&view);

	int i;
	
	if(server && player.start==false && point.x>=view.left+200 && point.x<=view.left+550 &&
										point.y>=view.top+110 && point.y<=view.top+237) {
		////////////////////////////////////
		// 게임시작 을 클릭하였을때 이벤트
		//

		int n=0;

		strcpy(message,PH_GAMESTART);
		g_map.get_map(message+2);
		Clientx.ch_send(message,400);

		return;
	}

	if(player.con== CON_DICE &&point.x>=view.left+260 && point.x<=view.left+462 && point.y>=view.top+230 && point.y<=view.top+367) {
		//////////////////////////////////////////////////
		// 주사위 작업

		strcpy(message,PH_DICE);
		message[2] = '0' + player.tmp[2];
		message[3] = player.tmp[0]+player.tmp[1];
		
		Clientx.ch_send(message,4);

		player.con=0;

		draw_map();
		
		sndPlaySound("./wave/click.wav",SND_ASYNC);
		return;
	}
	
	switch(player.con) {
	case CON_ROAD:
		///////////////////////////////////////////////////
		// 길 확장을 클릭하였을때 작업
		//

		if(player.tmp[0] && player.tmp[1]!=-1) {
			/////////////////////////////////////////////////////////////////
			// 시작 포인트가 지정이 되어 있고 끝 포인트도 지정이 되어 있다면
			// 두지점에 길을 만든다. A->B , B->A
			
			strcpy(message,PH_ROAD);
			message[2] = player.tmp[0];
			message[3] = player.tmp[1];
			message[4] = player.p_n;
			message[5] = player.tmp[99];
			
			Clientx.ch_send(message,6);
			
			player.con=0;
			
			load_map();
			draw_map();

		} else {
			for(i=0;i<96;i++) {
				if(	point.x-view.left>=g_map.link[i].x && point.x-view.left<=g_map.link[i].x+10 &&
					point.y-view.top>=g_map.link[i].y && point.y-view.top<=g_map.link[i].y+10) {
						/////////////////////////////////////////////////////
						// 클릭못하는 바깥쪽 테두리는 클릭하지 못하게 하라
						int n=0;
						while(EnableLink[n]!=-1) {
							if(i==EnableLink[n]) break;
							n++;
						}

						if(EnableLink[n]==-1 ) {
							////////////////////////////////////////////
							// 다른사람의 건물이 있다면 클릭할수 없다.
							
							if(g_map.link[i].city !=0 && (g_map.link[i].city-1) %4 != player.p_n) continue;

							////////////////////////////////////////////////////////////
							// 내건물이 세워져 있거나 주변에 내 도로가 있다면 클릭가능

								
							for(int r=0;r<g_map.link[i].link_max;r++)
								if(g_map.link[i].road[r]==player.p_n+1) break;

							if(r!=g_map.link[i].link_max || (g_map.link[i].city-1)%4 == player.p_n+1) {
								player.tmp[0] = i;
								player.tmp[1] = -1;
								return;
							}
						}
				}
			}
		}
			
		break;
		
	case CON_SETTLE:
		//////////////////////////////////////////////////
		// 정착지 건설을 클릭하였을때 작업
		//
		
		for(i=0;i<96;i++) {
			if(	point.x-view.left>=g_map.link[i].x && point.x-view.left<=g_map.link[i].x+10 &&
				point.y-view.top>=g_map.link[i].y && point.y-view.top<=g_map.link[i].y+10) {
					bool use=true;
					int n;

					if(player.tmp[0]) {
						for(int j=0;j<g_map.link[i].link_max;j++)
							if(g_map.link[i].road[j]-1 == player.p_n) break;

						if(j==g_map.link[i].link_max) continue;	// 주변에 길이 없으면 취소 (아직 안만듬)
					}
						
					for(n=0;n<g_map.link[i].link_max;n++) if(g_map.link[i].link[n]->city) use=false;

					n=0;
					
					while(EnableLink[n]!=-1) {
						if(i==EnableLink[n]) break;
						n++;
					}


					if(!g_map.link[i].city && use && EnableLink[n]==-1) {
						strcpy(message,PH_BUILDING);
						message[2] = 0;
						message[3] = i;
						message[4] = player.p_n;
						
						Clientx.ch_send(message,5);
						
						player.con = 0;
						break;
					}
				}
		}
		break;
	case CON_CITY:
		//////////////////////////////////////////////////
		// 도시로개조를 클릭하였을때 작업
		//

		for(i=0;i<96;i++) {
			if(	point.x-view.left>=g_map.link[i].x && point.x-view.left<=g_map.link[i].x+10 &&
				point.y-view.top>=g_map.link[i].y && point.y-view.top<=g_map.link[i].y+10 && g_map.link[i].city == player.p_n+1) {
					
					strcpy(message,PH_BUILDING);
					message[2] = 0;
					message[3] = i;
					message[4] = player.p_n+4;
					Clientx.ch_send(message,5);

					player.con=0;
					break;
			}
		}
			
		break;
	case CON_PLAGUE: {

		int mx=0,my=0;

		for(y=0;y<7;y++) {
			int max;
			if(y<=3) max=4+y;
			else max=10-y;

			 for(x=0;x<max;x++) {
				if(	point.x>=view.left+g_map.block[y][x].point[0]->x && point.x<= view.left+g_map.block[y][x].point[5]->x &&
					point.y>=view.top+g_map.block[y][x].point[0]->y && point.y<=view.top+g_map.block[y][x].point[5]->y &&
					!(g_map.plague.x == x && g_map.plague.y == y) && // 원래 도둑위치에 놓으면 안되고
					g_map.block[y][x].resource>=1 && g_map.block[y][x].resource<=6)	{	// 사막~자원이 있는땅
						mx=x;
						my=y;
				}
			 }
		}
		
		if(mx==0 && my==0) break;
		
		int n;

		CCardStealDlg StealDlg;
		StealDlg.players = 0;
	
		for(n=0;n<6;n++) {
			if(g_map.block[my][mx].point[n]->city && (g_map.block[my][mx].point[n]->city-1) %4!=player.p_n) {
				for(int i=0;i<StealDlg.players;i++)	if(StealDlg.p_n[i] == (g_map.block[my][mx].point[n]->city-1)%4) break;
				if(i!=StealDlg.players) continue;										// 이미 등록된 녀석제외
				if(player.ps[g_map.block[my][mx].point[n]->city-1].cardn==0) continue;	// 카드가 없는녀석제외

				strcpy(StealDlg.name[StealDlg.players],player.ps[(g_map.block[my][mx].point[n]->city-1)%4].name);
				StealDlg.cardn[StealDlg.players] = player.ps[(g_map.block[my][mx].point[n]->city-1)%4].cardn;
				StealDlg.p_n[StealDlg.players] = (g_map.block[my][mx].point[n]->city-1)%4;
				StealDlg.players++;
			}
		}
					
		if(StealDlg.players) {
			StealDlg.DoModal();
			if(StealDlg.btn_click) {
				strcpy(message,PH_RAND_RESOURCE);
				message[2] = StealDlg.p_n[StealDlg.select];
				message[3] = player.p_n;
				Clientx.ch_send(message,4);
				player.turn=false;
			} else con_enable(true);
		} else con_enable(true);

		strcpy(message,PH_PLAGUE_WHITCH);
		message[2]=mx;
		message[3]=my;
		message[4]=player.p_n;
		message[5]=player.tmp[0];
		Clientx.ch_send(message,6);
		
		player.con=0;
		break;
		}
	}
	if(	point.x<view.left || point.x>view.right ||
		point.y<view.top  || point.y>view.bottom) SendMessage(WM_NCLBUTTONDOWN,HTCAPTION,0);
}

void CKatanPlay::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	static int EnableLink[] = {0,1,2,3,4,5,6,7,8,9,10,18,19,20,21,31,32,33,34,46,47,48,49,61,62,63,64,74,75,76,77,85,86,87,88,89,90,91,92,93,94,95,-1};

	int i;
	
	CRect view;
	GetDlgItem(IDC_VIEWER)->GetWindowRect(&view);
	ScreenToClient(&view);

	
	switch(player.con) {
	case CON_ROAD:
		///////////////////////////////////////////////////
		// 길 확장을 클릭하였을때 작업
		//

		if(player.tmp[0]) {
				
			int min_xy=50000;
				
			/////////////////////////////////////////////
			// 마우스와 가장 가까운 도착점을 찾습니다.

			player.tmp[1] = -1;

			for(i=0;i<g_map.link[player.tmp[0]].link_max;i++) {
				int x = point.x - (view.left+g_map.link[player.tmp[0]].link[i]->x+5);
				int y = point.y - (view.top+g_map.link[player.tmp[0]].link[i]->y+5);
				
				if((i==0 || min_xy>sqrt(x*x + y*y)) && !g_map.link[player.tmp[0]].road[i]) {
					/////////////////////////////////////////////////////
					// 클릭못하는 바깥쪽 테두리는 클릭하지 못하게 하라

					int n=0;

					while(EnableLink[n]!=-1) {
						if(&g_map.link[EnableLink[n]]==g_map.link[player.tmp[0]].link[i]) break;
						n++;
					}

					if(EnableLink[n]==-1) {
						min_xy = (int)sqrt(x*x+y*y);
						player.tmp[1] = i;
					}
				}
			}
			if(player.tmp[1] == -1) {
				player.tmp[0] = 0;
				player.tmp[1] = 0;
				break;
			}
			
			draw_map();
		} else {
			player.tmp[1]=-1;

			for(i=0;i<96;i++) {
				if(	point.x-view.left>=g_map.link[i].x && point.x-view.left<=g_map.link[i].x+10 &&
					point.y-view.top>=g_map.link[i].y && point.y-view.top<=g_map.link[i].y+10) {
					player.tmp[1] = i;
					break;
				}
			}
			
			int n=0;

			while(EnableLink[n]!=-1) {
				if(&g_map.link[EnableLink[n]]==&g_map.link[player.tmp[1]]) break;
				n++;
			}
			if(EnableLink[n]!=-1) player.tmp[1]= -1;

			draw_map();
		}
		break;
	case CON_SETTLE:
		player.tmp[1]=-1;

		for(i=0;i<96;i++) {
			if(	point.x-view.left>=g_map.link[i].x && point.x-view.left<=g_map.link[i].x+10 &&
				point.y-view.top>=g_map.link[i].y && point.y-view.top<=g_map.link[i].y+10) {
				player.tmp[1] = i;
				break;
			}
		}
		
		int n=0;

		while(EnableLink[n]!=-1) {
			if(&g_map.link[EnableLink[n]]==&g_map.link[player.tmp[1]]) break;
			n++;
		}
		if(EnableLink[n]!=-1) player.tmp[1]= -1;

		draw_map();
		break;
	}
	
	CDialog::OnMouseMove(nFlags, point);
}

void CKatanPlay::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnLButtonUp(nFlags, point);
}

void CKatanPlay::load_bone_map()
{
	/////////////////////////////////////////////////////
	// 변하지 않는 골격이 되는 맵에 정보를 만들어 냅니다.

	int x,y;
	CRect view;
	GetDlgItem(IDC_VIEWER)->GetWindowRect(&view);
	ScreenToClient(&view);

	CBitmap image;
	CDC *pDC=GetDC();
	CDC MemDC;
	
	// MemDC 가상화면
	
	MemDC.CreateCompatibleDC(pDC);

	// 가상 화면을 만듭니다.
	
	bone_image.DeleteObject();
	bone_image.CreateCompatibleBitmap(pDC,view.right-view.left,view.bottom-view.top);
	CBitmap *pTempBitmap = (CBitmap *) MemDC.SelectObject(&bone_image);

	MemDC.FillSolidRect(0,0,view.right-view.left,view.bottom-view.top,RGB(0,0,0));
	MemDC.SetBkColor(RGB(255,255,255));

	// 테두리를 만듭니다.
	
	load_bmp(&image,"./images/path.bmp");
	
	for(y=0;y<7;y++) {
		int max;
		if(y<=3) max=4+y;
		else max=10-y;

		for(x=0;x<max;x++) {
			if(y!=0 && y!=6 && x!=0 && x!=max-1) {
				if(y<=3) draw_bmp(&MemDC,&image,x*100-y*50+153,y*81+4,102,108,RGB(0,0,0));
					else draw_bmp(&MemDC,&image,x*100-(6-y)*50+153,y*81+4,102,108,RGB(0,0,0));
			}
		}
	}

	// 맵을 그립니다.
	
	for(y=0;y<7;y++) {
		int max;
		if(y<=3) max=4+y;
		else max=10-y;

		for(x=0;x<max;x++) {
			char filename[50]="./images/map1.bmp";
			
			if(g_map.block[y][x].resource>=1) {
				unsigned col=RGB(32,16,8);
				if(g_map.block[y][x].resource<=6) {
					filename[12]='0'+g_map.block[y][x].resource;
					load_bmp(&image,filename);

					if(y<=3) draw_bmp(&MemDC,&image,x*100-y*50+156,y*81+8,96,100,col);
						else draw_bmp(&MemDC,&image,x*100-(6-y)*50+156,y*81+8,96,100,col);
				} else {
					//////////////////////////////////////////
					// 맵이 항구라면 항구를 교환가능한 정보기록

					if(g_map.block[y][x].resource==7) strcpy(filename,"./images/map7.bmp");
					else {
						strcpy(filename,"./images/harbor06.bmp");
						filename[15]='0'+(g_map.block[y][x].resource-8);
						
						/////////////
						// 회전정보
						
						if(g_map.block[y][x].harbor[0] && g_map.block[y][x].harbor[3]) filename[16]='0';
						if(g_map.block[y][x].harbor[0] && g_map.block[y][x].harbor[1]) filename[16]='1';
						if(g_map.block[y][x].harbor[1] && g_map.block[y][x].harbor[2]) filename[16]='2';
						if(g_map.block[y][x].harbor[2] && g_map.block[y][x].harbor[5]) filename[16]='3';
						if(g_map.block[y][x].harbor[5] && g_map.block[y][x].harbor[4]) filename[16]='4';
						if(g_map.block[y][x].harbor[4] && g_map.block[y][x].harbor[3]) filename[16]='5';
					}

					load_bmp(&image,filename);

					if(y<=3) draw_bmp(&MemDC,&image,x*100-y*50+156,y*81+8,96,100,RGB(0,0,0));
						else draw_bmp(&MemDC,&image,x*100-(6-y)*50+156,y*81+8,96,100,RGB(0,0,0));

				}
				////////////////////////////////////
				// 숫자판을 깝니다.

				if(g_map.block[y][x].num!=0) {
					CRect rect;
					
					if(y<=3) {
						rect.left = x*100-y*50+186;
						rect.right = x*100-y*50+221;
					} else {
						rect.left = x*100-(6-y)*50+186;
						rect.right = x*100-(6-y)*50+221;
					}

					rect.top = y*81+43;
					rect.bottom = y*81+78;

					MemDC.Ellipse(rect);
					
					CString Num,Bit;

					Num.Format("%d",g_map.block[y][x].num);
				
					MemDC.SetBkColor(RGB(255,255,255));
					MemDC.DrawText(Num,Num.GetLength(),rect,DT_CENTER|DT_VCENTER|DT_SINGLELINE);
				}
			}
		}
	}
	
	////////////////////////////////////////////////////////////////

	load_bmp(&image,"./images/buildcost.bmp");
	draw_bmp(&MemDC,&image,580,10,150,101,RGB(0,0,255));
	

	MemDC.SelectObject(pTempBitmap);
	MemDC.DeleteDC();
	image.DeleteObject();
	ReleaseDC(pDC);
}

void CKatanPlay::load_bmp(CBitmap *bitmap, char *filename)
{
	HBITMAP hBitmap;

	hBitmap = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), (LPCTSTR)
	filename, IMAGE_BITMAP, 0, 0,LR_LOADFROMFILE);
		
	bitmap->DeleteObject();
	bitmap->Attach(hBitmap);
}

void CKatanPlay::draw_bmp(CDC *pDC, CBitmap *bitmap, int x, int y, int sizex, int sizey,int m_r)
{
	//////////////////////////////// 마스크맵 생성 ////////////////////////////////////
	
	CDC *p=GetDC(),MemDC1,MemDC2,MemDC3;
	CBitmap TempPicture,TempPicture2, *hOldBmp1,*hOldBmp2,*hOldBmp3;

	MemDC1.CreateCompatibleDC(p);
	MemDC2.CreateCompatibleDC(p);
	MemDC3.CreateCompatibleDC(p);
	
	TempPicture.CreateBitmap(sizex,sizey,1,1,NULL);
	TempPicture2.CreateCompatibleBitmap(p,sizex,sizey);
	
	hOldBmp1 = MemDC1.SelectObject(&TempPicture);
	hOldBmp2 = MemDC2.SelectObject(bitmap);
	hOldBmp3 = MemDC3.SelectObject(&TempPicture2);
	
	MemDC3.FillSolidRect(0,0,sizex,sizey,m_r);
	MemDC3.BitBlt(0,0,sizex,sizey,&MemDC2,0,0,SRCCOPY);
	MemDC1.BitBlt(0,0,sizex,sizey,&MemDC3,0,0,SRCCOPY);

	MemDC1.BitBlt(0,0,sizex,sizey,&MemDC1,0,0,DSTINVERT);

	MemDC2.BitBlt(0,0,sizex,sizey,&MemDC1,0,0,SRCAND);
	MemDC1.BitBlt(0,0,sizex,sizey,&MemDC1,0,0,DSTINVERT);

	////////////////////////////
	// 기본골격 맵을 붙입니다.

	pDC->BitBlt(x,y,sizex,sizey,&MemDC1,0,0,SRCAND);
	pDC->BitBlt(x,y,sizex,sizey,&MemDC2,0,0,SRCPAINT);

	MemDC3.SelectObject(hOldBmp3);
	MemDC3.DeleteDC();
	
	MemDC2.SelectObject(hOldBmp2);
	MemDC2.DeleteDC();
	
	MemDC1.SelectObject(hOldBmp1);
	MemDC1.DeleteDC();
	
	TempPicture2.DeleteObject();
	TempPicture.DeleteObject();
	
	ReleaseDC(p);
}

void CKatanPlay::load_map()
{
	int x,y;
	CBitmap image;

	//////////////////////////////
	// 맵 블록을 화면에 그립니다.
	
	CDC *pDC=GetDC(),MemDC;
	
	// MemDC 가상화면
	
	MemDC.CreateCompatibleDC(pDC);
	
	// 가상 화면을 만듭니다.
	
	view_image.DeleteObject();
	view_image.CreateCompatibleBitmap(pDC,1024,768);
	
	CBitmap *pTempBitmap = (CBitmap *) MemDC.SelectObject(&view_image);
	
	////////////////////////////
	// 기본골격 맵을 붙입니다.

	CDC BoneDC;
	BoneDC.CreateCompatibleDC(pDC);
	CBitmap *pTempBitmap2 = (CBitmap *) BoneDC.SelectObject(&bone_image);
	
	MemDC.BitBlt(0,0,1024,768,&BoneDC,0,0,SRCCOPY);

	BoneDC.SelectObject(pTempBitmap2);
	BoneDC.DeleteDC();
	
	// 길을 만듭니다.
	
	int roadn[]={0,1,2,5,4,3};
	
	for(y=0;y<7;y++) {
		int max;
		if(y<=3) max=4+y;
		else max=10-y;

		for(x=0;x<max;x++) {
			for(int p=0;p<6;p++)
				for(int r=0;r<g_map.block[y][x].point[roadn[p]]->link_max;r++) {
					int n = g_map.block[y][x].point[roadn[p]]->road[r];
										
					if(n && g_map.block[y][x].point[roadn[(p+1)%6]] == g_map.block[y][x].point[roadn[p]]->link[r]) {
						char filename[]="./images/road/road00.bmp";
						
						filename[18] = '0' + n;
						filename[19] = '0' + p;
						
						load_bmp(&image,filename);

						if(y<=3) draw_bmp(&MemDC,&image,x*100-y*50+153,y*81+4,102,108,RGB(0,0,0));
							else draw_bmp(&MemDC,&image,x*100-(6-y)*50+153,y*81+4,102,108,RGB(0,0,0));
						
					}
					
				}
		}
	}
	
	// 맵에 건물을 그립니다.
	for(x=0;x<96;x++) {
		if(g_map.link[x].city!=0) {
			char filename[100] = "";
			///////////////////////////////
			// 건물이 존재한다면
			
			if(g_map.link[x].city>=1 && g_map.link[x].city<=4) {
				///////////////////////////
				// 세틀러먼드 출력
				strcpy(filename,"./images/sett1.bmp");
				filename[13] = '0' + g_map.link[x].city;

				load_bmp(&image,filename);
				draw_bmp(&MemDC,&image,g_map.link[x].x-5,g_map.link[x].y-5,23,23,RGB(0,0,0));
			}

			if(g_map.link[x].city>=5 && g_map.link[x].city<=8) {
				///////////////////////////
				// 시티 출력
				strcpy(filename,"./images/city1.bmp");
				filename[13] = '0' + g_map.link[x].city-4;
				
				load_bmp(&image,filename);
				draw_bmp(&MemDC,&image,g_map.link[x].x-5,g_map.link[x].y-25,32,38,RGB(0,0,0));
			}
		}
	}
	// 도둑을 놓습니다.
	load_bmp(&image,"./images/robber.bmp");
	
	if(g_map.plague.y<=3) draw_bmp(&MemDC,&image,g_map.plague.x * 100-g_map.plague.y*50+193 ,g_map.plague.y*81+34,25,36,RGB(255,0,255));
		else draw_bmp(&MemDC,&image,g_map.plague.x * 100-(6-g_map.plague.y)*50+193 ,g_map.plague.y*81+34,25,36,RGB(255,0,255));
	
	MemDC.SelectObject(pTempBitmap);
	MemDC.DeleteDC();
	image.DeleteObject();
	ReleaseDC(pDC);
}

UINT BGSoundProc(LPVOID lParam)
{
	int track=0;
	CKatanPlay *game_dlg = (CKatanPlay *)lParam;
	CBgSound sound;
	
	sound.BS_Initializemci();
		
	while(game_dlg->bgindex) {
		if(track!=game_dlg->bgindex) {
			sound.BS_Close();
			track=game_dlg->bgindex;
			if(track==1) {
				sound.BS_Openfile("./wave/wait.mp3");
				sound.BS_Play();
			}

			if(track==3) {
				sound.BS_Openfile("./wave/bgsound.mp3");
				sound.BS_Play();
			}

		}

		unsigned int play=sound.BS_Getlength();
		Sleep(50);
		if(play==sound.BS_Getlength()) sound.BS_Play();
	}

	sound.BS_Close();

	return 0;
}

BOOL CKatanPlay::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	//////////////

	bgindex=1;
	bgThread = AfxBeginThread(BGSoundProc,this);
	
	load_bone_map();
	load_map();	

	CRect rect;
	

	////////////////////////////////////////////////////
	// 플레이어정보 컨트롤을 초기화합니다.
	
	m_playerlist.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	m_playerlist.GetClientRect(rect);
	m_playerlist.InsertColumn(0,"플레이어",LVCFMT_CENTER,100);
	m_playerlist.InsertColumn(1,"색상",LVCFMT_CENTER,60);
	m_playerlist.InsertColumn(2,"T",LVCFMT_CENTER,25);
	m_playerlist.InsertColumn(3,"군대보유",LVCFMT_CENTER,70);
	m_playerlist.InsertColumn(4,"자원수",LVCFMT_CENTER,rect.Width()-255);
	
	player_check();
	
	// 클라이언트 동작
	if(!server) {
		Clientx.Create();
		Clientx.Connect(ip_address,9876);
		if(WSAGetLastError()==WSAENOTSOCK) {
			MessageBox("서버가 작동중인지 확인하세요");
			OnOK();
		}
	}

	SetTimer(998,100,NULL);

	CFont *font=GetFont();
	
	CRect view;
	
	GetDlgItem(IDC_CHAT_RECT)->GetWindowRect(&view);	ScreenToClient(&view);
	view.left++;view.top++;view.right--;view.bottom--;
	viewer[0].Create( ES_AUTOVSCROLL | ES_MULTILINE | WS_VSCROLL | WS_VISIBLE | WS_CHILD,view,this,1001);
	viewer[0].SetFont(font);

	GetDlgItem(IDC_EVENT_RECT)->GetWindowRect(&view);	ScreenToClient(&view);
	view.left++;view.top++;view.right--;view.bottom--;
	viewer[1].Create( ES_AUTOVSCROLL | ES_MULTILINE | WS_VSCROLL | WS_VISIBLE | WS_CHILD,view,this,1001);
	viewer[1].SetFont(font);
		
	///////////////////////////////////////////////

	set_rgn();
	
	con_enable(false);
	
	/////////////////////////////////////////
	// 로비에 다이알로그 시작메세지를 보냄 //
	/////////////////////////////////////////

	::SetWindowText(this->GetSafeHwnd(),"CatanPlayForm");

	HWND hWnd = ::FindWindow(NULL,"HGR Catan...");	//"CLobbyDlg",NULL);
	::SendMessage(hWnd,WM_GAMEOPEN,0,0);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CKatanPlay::set_rgn()
{
	CRgn rgn,rgn2;
	int x,y,x1,max_x,max_y;
	char bit[4];
	bool x2;

	FILE *fp=fopen("./images/other/background.bmp","rb");
	
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

void CKatanPlay::draw_map()
{
	CRect view;
	GetDlgItem(IDC_VIEWER)->GetWindowRect(&view);
	ScreenToClient(&view);
	
	CDC *pDC = GetDlgItem(IDC_VIEWER)->GetDC();
	CDC MemDC1;
	CBitmap temp_data;
	
	MemDC1.CreateCompatibleDC(pDC);
	temp_data.CreateCompatibleBitmap(pDC,view.right-view.left-1,view.bottom-view.top-1);
	CBitmap *pTempBitmap1 = (CBitmap *) MemDC1.SelectObject(&temp_data);
		
	// 가상화면을 실제 복사를 합니다.
		
	if(player.start==false) {
		MemDC1.FillSolidRect(1,1,view.right-view.left-1,view.bottom-view.top-1,RGB(192,192,192));
		MemDC1.SetBkColor(RGB(255,255,255));

		CBitmap image;

		char filename[]="./images/other/wait0.bmp";
		filename[19]=((wait_picture/5)>=3 ? 5-(wait_picture/5) : (wait_picture/5))+'0';

		load_bmp(&image,filename);
		draw_bmp(&MemDC1,&image,420,180,308,407,RGB(0,0,255));
		
		load_bmp(&image,"./images/other/message.bmp");
		draw_bmp(&MemDC1,&image,200,110,350,137,RGB(255,0,0));
		image.DeleteObject();
		
		CFont newfont,*oldfont;
		LOGFONT x;
		oldfont=MemDC1.GetCurrentFont();
		GetObject(*oldfont,sizeof(LOGFONT),&x);
		x.lfWeight= FW_BOLD;
		
		newfont.CreateFontIndirect(&x);
		
		MemDC1.SelectObject(newfont);
		
		if(server) {
			MemDC1.TextOut(250,150,"당신은 현재 이게임의 방장입니다");
			MemDC1.TextOut(250,192,"게임을 시작하시려면 박스를 클릭");
		}	else {
			MemDC1.TextOut(240,150,"게임을 시작하기위해 대기중입니다");
			MemDC1.TextOut(240,192,"방장이 게임을 시작할때를 가다려요");
		}
		MemDC1.SetBkColor(RGB(192,192,192));
		MemDC1.TextOut(5,5,ip_address);
		MemDC1.SetBkColor(RGB(255,255,255));
		MemDC1.SelectObject(oldfont);
	} else {
		CDC MemDC2;
		// MemDC 가상화면
	
		MemDC2.CreateCompatibleDC(pDC);

		CBitmap *pTempBitmap2 = (CBitmap *) MemDC2.SelectObject(&view_image);
		MemDC1.BitBlt(0,0,view.right-view.left-1,view.bottom-view.top-1,&MemDC2,0,0,SRCCOPY);

		CPen pen,*oldpen;
		pen.CreatePen(PS_SOLID,3, RGB(52,255,122));
		oldpen = MemDC1.SelectObject(&pen);

		if(player.con==CON_ROAD && player.tmp[0]) {

			MemDC1.MoveTo(g_map.link[player.tmp[0]].x+5,g_map.link[player.tmp[0]].y+5);
			MemDC1.LineTo(g_map.link[player.tmp[0]].link[player.tmp[1]]->x+5,
						g_map.link[player.tmp[0]].link[player.tmp[1]]->y+5);
		} else if((player.con==CON_ROAD || player.con == CON_SETTLE) && player.tmp[1]!=-1) {
			MemDC1.Ellipse(	g_map.link[player.tmp[1]].x,g_map.link[player.tmp[1]].y,
							g_map.link[player.tmp[1]].x+10,g_map.link[player.tmp[1]].y+10);
		}

		MemDC1.SelectObject(oldpen);
		pen.DeleteObject();

		if(player.con == CON_DICE) {
			CBitmap img;
			
			load_bmp(&img,"./images/other/dice_window.bmp");
			draw_bmp(&MemDC1,&img,260,230,202,137,RGB(255,0,0));
		
			char filename[50]="./images/dice/frame1.bmp";
			filename[19] = '0' + player.tmp[0];
			
			//////////////////////////////////////
	
			load_bmp(&img,filename);
			draw_bmp(&MemDC1,&img,285,260,72,72,RGB(0,0,32));

			//////////////////////////////////////

			filename[19] = '0' + player.tmp[1];

			load_bmp(&img,filename);

			draw_bmp(&MemDC1,&img,357,260,72,72,RGB(0,0,32));
			img.DeleteObject();
		}

		if(player.con == CON_PLAGUE) {
			CBitmap img;
			
			load_bmp(&img,"./images/other/plague.bmp");
			draw_bmp(&MemDC1,&img,(view.right-view.left)-
				(wait_picture>20 ? (wait_picture>40 ? 60-wait_picture : 20)	: wait_picture)*10,230,200,402,RGB(0,0,255));
		
			img.DeleteObject();
		}

		MemDC2.SelectObject(pTempBitmap2);
		MemDC2.DeleteDC();
	}
	pDC->BitBlt(1,1,view.right-view.left-2,view.bottom-view.top-2,&MemDC1,0,0,SRCCOPY);
	MemDC1.SelectObject(pTempBitmap1);
	MemDC1.DeleteDC();
		
	temp_data.DeleteObject();

	ReleaseDC(pDC);
}

void CKatanPlay::OnRoadBtn() 
{
	static int EnableLink[] = {0,1,2,3,4,5,6,7,8,9,10,18,19,20,21,31,32,33,34,46,47,48,49,61,62,63,64,74,75,76,77,85,86,87,88,89,90,91,92,93,94,95,-1};
	// 길확장 버튼을 클릭하였을 때 작업입니다.

	
	for(int i=0;i<96;i++) {
		if(g_map.link[i].city !=0 && (g_map.link[i].city-1)%4 != player.p_n) continue;

		int n=0;
		while(EnableLink[n]!=-1) {
			if(i==EnableLink[n]) break;
			n++;
		}

		for(int r=0;r<g_map.link[i].link_max;r++)	if(g_map.link[i].road[r]==player.p_n+1) break;
		if(EnableLink[n]==-1 && (r!=g_map.link[i].link_max || (g_map.link[i].city-1)%4 == player.p_n)) break;
	}

	if(i==96) {
		MessageBox("길을 지을만한 위치가 없습니다.");
		player.tmp[99]=0;
		player.turn=true;
		return;
	}
	if(player.road>=15) {
		MessageBox("15개까지 길을 지을수 있습니다.");
		player.tmp[99]=0;
		player.turn=true;
		return;
	}

	player.turn=false;
	trade=false;

	player.tmp[0] = 0;	// 선택된 포인트(시작) : 생략하면 최근 지은 건물
	player.tmp[1] = 0;	// 선택된 포인트(끝)

	player.con = CON_ROAD;	

	Invalidate(FALSE);
	con_enable(FALSE);
}

void CKatanPlay::OnSettBtn() 
{
	// 세틀러먼트 짓기 버튼을 클릭하였을때 작업입니다.
	///////////////////////////////////////////////////////////////////////////////////////////

	static int EnableLink[] = {0,1,2,3,4,5,6,7,8,9,10,18,19,20,21,31,32,33,34,46,47,48,49,61,62,63,64,74,75,76,77,85,86,87,88,89,90,91,92,93,94,95,-1};

	// 세틀러먼트 짓기 수행가능 검사

	for(int i=0;i<96;i++) {
		bool use=true;
		int n;

		for(int j=0;j<g_map.link[i].link_max;j++)	if(g_map.link[i].road[j]-1 == player.p_n) break;
		if(j==g_map.link[i].link_max) continue;	// 주변에 길이 없으면 취소
		for(n=0;n<g_map.link[i].link_max;n++) if(g_map.link[i].link[n]->city) use=false;	// 주변에 건물이 있으면 취소
		
		n=0;	while(EnableLink[n]!=-1) {	if(i==EnableLink[n]) break;		n++;	}

		if(EnableLink[n]==-1 && use && g_map.link[i].city==0) break;
	}
	
	if(i==96) {
		MessageBox("건물을 지을만한 위치가 없습니다.");
		return;
	}
	if(player.settle>=5) {
		MessageBox("정착지는 5개까지 가능합니다.");
		return;
	}
	
	player.turn=false;
	trade=false;
	player.tmp[0]=1;	player.tmp[1]=-1;	player.con = CON_SETTLE;
	Invalidate(FALSE);
	con_enable(FALSE);
}

void CKatanPlay::OnCityBtn() 
{
	// 시티 버튼을 클릭하였을때 작업입니다.
	for(int settle=0;settle<96;settle++)
		if(g_map.link[settle].city==player.p_n+1) break;

	if(settle==96) {
		MessageBox("시티가 될수 있는 정착지가 없습니다.");
		return;
	}
	if(player.city>=4) {
		MessageBox("시티는 4개까지 가능합니다.");
		return;
	}

	player.turn=false;
	trade=false;
	player.con=CON_CITY;
	
	Invalidate(FALSE);
	con_enable(FALSE);
}


void CKatanPlay::OnTurnBtn() 
{
	player.turn=false;
	con_enable(FALSE);
	
	player.newcardn=0;
	player.carduse=false;
	trade=true;
	
	Clientx.ch_send(PH_TURN_END,2);
}

void CKatanPlay::call_dice()
{
	/////////////////////////////////////////
	// 주사위 번호를 생성하고 뿌려줍니다.

	player.con = CON_DICE;
	player.tmp[0] = randdice();
	player.tmp[1] = randdice();

	draw_map();
}


void CKatanPlay::OnCardBtn() 
{
	player.turn=false;
	trade=false;
	con_enable(false);
	Clientx.ch_send(PH_BUY_CARD,2);
}


void CKatanPlay::OnCardUseBtn() 
{
	player.turn=false;
	con_enable(false);
	
	CCardUseDlg CUDlg;

	int i,j;
	CUDlg.cardn=0;

	for(i=0;i<player.cardn;i++)	{
		for(j=0;j<player.newcardn;j++)
			if(player.card[i]==player.newcard[j]) break;

		if(j == player.newcardn) CUDlg.card[CUDlg.cardn++]=player.card[i];
	}

	CUDlg.DoModal();

	if(CUDlg.select_ok) {
		char memo[4];
		
		for(i=0;i<player.cardn;i++) {
			if(player.card[i]==CUDlg.select) {
				for(j=i;j<player.cardn-1;j++)	player.card[j]=player.card[j+1];		// 카드정보 없앰
				player.cardn--;
				break;
			}
		}
		player.carduse=true;

		strcpy(memo,PH_USE_CARD);	// 카드사용
		memo[2] = CUDlg.select;
		Clientx.ch_send(memo,3);
	} else {
		player.turn=true;
		con_enable(false);
	}
}

void CKatanPlay::OnTradeBtn() 
{
	player.turn=false;
	con_enable(false);

	CTrainDlg tradedlg;
	int i;
	
	for(i=0;i<4;i++) {
		if(player.ps[i].enable==true && player.p_n!=i && player.ps[i].cardn) {
			strcpy(tradedlg.player_name[tradedlg.playern],player.ps[i].name);
			tradedlg.player_pn[tradedlg.playern] = i;
			tradedlg.playern++;
		}
	}

	for(i=0;i<5;i++)tradedlg.resource[i]=player.resource[i];
	for(int x=0;x<7;x++) {
		for(int y=0;y<7;y++) {
			for(int p=0;p<6;p++) {
				if(	g_map.block[y][x].resource>=8 && g_map.block[y][x].resource<=13 &&
					g_map.block[y][x].harbor[p] && (g_map.block[y][x].point[p]->city-1)%4 == player.p_n)
						tradedlg.hangu[g_map.block[y][x].resource-8]=true;
			}
		}
	}

	tradedlg.DoModal();

	if(tradedlg.trade_ok) {
		char message[50];
		if(tradedlg.select==0) {
			int i;

			strcpy(message,PH_TRADE_HANGU);
			for(i=0;i<5;i++) message[2+i]=0;
			for(i=0;i<tradedlg.sn;i++)message[tradedlg.source[i]+2]++;
			message[7]=tradedlg.target_select;
			Clientx.ch_send(message,8);
		} else {
			int i;

			strcpy(message,PH_TRADE_SOURCE);
			message[2]=player.p_n;
			message[3]=tradedlg.player_pn[tradedlg.select-1];
			for(i=0;i<5;i++)message[4+i]=0;
			for(i=0;i<tradedlg.sn;i++)message[4+tradedlg.source[i]]++;
			Clientx.ch_send(message,9);
		}
	} else {
		player.turn=true;
		con_enable(true);
	}
}

void CKatanPlay::con_enable(bool e)
{
	if(player.tmp[99] && e) {
		player.tmp[99]--;
		if(player.tmp[99]) {
			OnRoadBtn();
			return;
		}
	}
	//////////////////////////////////
	// 작업으로 인해 버튼을 비활성화 시키거나 활성화 시킵니다.

	if(e	&& player.resource[0]>=1 && player.resource[1]>=1) {	// 길
			GetDlgItem(IDC_ROAD_BTN)->EnableWindow(true);
	} else	GetDlgItem(IDC_ROAD_BTN)->EnableWindow(false);
	
	if(e	&& player.resource[0]>=1 && player.resource[1]>=1
			&& player.resource[2]>=1 && player.resource[3]>=1) {	// 세틀러먼트
			GetDlgItem(IDC_SETT_BTN)->EnableWindow(true);
	} else	GetDlgItem(IDC_SETT_BTN)->EnableWindow(false);
	
	if(e	&& player.resource[3]>=2 && player.resource[4]>=3) {	// 시티
			GetDlgItem(IDC_CITY_BTN)->EnableWindow(true);
	} else	GetDlgItem(IDC_CITY_BTN)->EnableWindow(false);
	
	if(e	&& player.resource[2]>=1 && player.resource[3]>=1 && player.resource[4]>=1) {	// 카드
			GetDlgItem(IDC_CARD_BTN)->EnableWindow(true);
	} else	GetDlgItem(IDC_CARD_BTN)->EnableWindow(false);
	
	if(e	&& trade) {
			GetDlgItem(IDC_TRADE_BTN)->EnableWindow(true);
	} else	GetDlgItem(IDC_TRADE_BTN)->EnableWindow(false);
	
	if(e	&& player.cardn - player.newcardn && !player.carduse)	{		// 카드사용
			GetDlgItem(IDC_CARD_USE_BTN)->EnableWindow(true);
	} else	GetDlgItem(IDC_CARD_USE_BTN)->EnableWindow(false);
	GetDlgItem(IDC_TURN_BTN)->EnableWindow(e);
}

int CKatanPlay::player_add(CString name,bool pn)
{
	return 0;
}

void CKatanPlay::player_check()
{
	CString color[4]={"빨간색","파란색","주황색","흰색"};
	char temp[100];
	int n=0;

	m_playerlist.DeleteAllItems();

	for(int i=0;i<4;i++) {
		if(player.ps[i].enable) {
			m_playerlist.InsertItem(n,player.ps[i].name,0);
			m_playerlist.SetItemText(n,1,color[i]);
			
			if(player.who_turn==i) m_playerlist.SetItemText(n,2,"●");
			else m_playerlist.SetItemText(n,2,"○");

			itoa(player.ps[i].soldier,temp,10);
			m_playerlist.SetItemText(n,3,temp);
			
			itoa(player.ps[i].cardn,temp,10);
			m_playerlist.SetItemText(n,4,temp);
			n++;
		}
	}
}

void CKatanPlay::OnTimer(UINT nIDEvent) 
{
	char memo[5000],temp[5000];

	///////////////////////////////////////////
	// 기다림 그림 움직임(동적변환)
	
	if(!player.start) {
		wait_picture++;
		if(wait_picture>=30) wait_picture=0;
		draw_map();
	}
	
	if(player.start && player.con==CON_PLAGUE) {
		if(wait_picture<60) wait_picture++;
		draw_map();
	}

	///////////////////////////////////////////


	if(server && server_use) {
		Clientx.Create();
		Clientx.Connect(ip_address,9876);
		server_use=false;
	}

	if(!player.con && player.turn) {
		con_enable(TRUE);
	}
	
	if(nIDEvent==998) {
		/////////////////////////////////////////////////////////////////////////////////////////////////
		// 클라이언트가 처리하는 내용

		if(Clientx.use==false) {
			KillTimer(998);
			if(player.start) {
				HWND hWnd = ::FindWindow(NULL,"HGR Catan...");
				::SendMessage(hWnd,WM_GAMEOVER,4,0);

				MessageBox("한 사람이 게임을 빠져나감으로 게임은 종료됩니다.");
			} else {
				if(!strcmp(player.ps[player.p_n].name,"")) MessageBox("게임에 들어갈수 없습니다.");
				else MessageBox("방장이 방을 파괴하였습니다.");
			}

			OnOK();
		}
		
		while(Clientx.memo_copy(memo)) {
			if(!strncmp(memo,PH_CONNECT,2)) {	// 접속허가가 떨어졌다면 서버에 사용자정보(이름)을 보낸다.
				player.p_n=memo[2]-'0';
								
				strcpy(temp,PH_CNAME);
				strcat(temp,name);
				Clientx.ch_send(temp,strlen(temp));
								
				continue;
			}

			if(!strncmp(memo,PH_NAME_ERROR,2)) {
				KillTimer(998);
				MessageBox("이름이 중복되어 접속할수 없습니다.");
				OnOK();
			}

			if(!strncmp(memo,PH_GAMESTARTNO,2)) {
				MessageBox("게임인원은 2∼4명이어야 합니다.");
				continue;
			}
			
			if(!strncmp(memo,PH_CNAME,2)) {	// 사용자정보받기
				int n=memo[2]-'0';
				strcpy(player.ps[n].name,memo+3);
				player.ps[n].enable=true;

				if(connect_ok) {
					CString str;
					str.Format("%s 님이 접속하셨습니다.",player.ps[n].name);
					strcpy(temp,str);
					add_chat(5,temp);
				}

				player_check();
							
				continue;
			}
			if(!strncmp(memo,PH_CONNECTOK,2)) { // 접속완료
				connect_ok=true;
				continue;
			}
			
			if(!strncmp(memo,PH_MESSAGE,2)) {
				if(memo[2]=='x') add_chat(-1,memo+3);
				else add_chat(memo[2]-'0',memo+3);	// 메세지전달
				continue;
			}
			
			if(!strncmp(memo,PH_DROP,2)) {	// 접속끊기
				int n=memo[2]-'0';
				
				if(player.start) {
					KillTimer(998);
					HWND hWnd = ::FindWindow(NULL,"HGR Catan...");
					::SendMessage(hWnd,WM_GAMEOVER,4,0);
					
					MessageBox("한 사람이 게임을 빠져나감으로 게임은 종료됩니다.");
					OnOK();
				} else {
					CString str;
					str.Format("%s 님이 접속을 끊으셨습니다.",player.ps[n].name);
					strcpy(temp,str);
					add_chat(5,temp);
					player.ps[n].enable=false;
					player_check();
				}
			
				continue;
			}
			if(!strncmp(memo,PH_LOADMAP,2)) {	// 맵전송 (게임시작)
				bgindex=2;

				player.start=true;
				g_map.set_map(memo+2);
				
				strcpy(temp,PH_MAPOK);
				Clientx.ch_send(temp,2);
				
				sndPlaySound("./wave/start.wav",SND_SYNC);
				
				load_bone_map();
				load_map();
				draw_map();

				add_chat(-1,"게임을 시작합니다.");
				
				bgindex=3;

				continue;
			}
			if(!strncmp(memo,PH_DICE,2)) {		// 주사위
				int n=memo[2]-'0';

				player.tmp[2] = n;
				
				call_dice();
				sndPlaySound("./wave/dice.wav",SND_ASYNC);
				continue;
			}
			if(!strncmp(memo,PH_READY,2)) {		// 준비하기 / 세틀러먼드 짓기
				int n=memo[2]-'0';

				player.con = CON_SETTLE;
				player.tmp[0] = 0;
				player.tmp[1] = -1;
				
				draw_map();
				continue;
			}
			if(!strncmp(memo,PH_BUILDING,2)) {
				g_map.link[memo[3]].city = memo[4]+1;

				
				CString str;
				if(memo[4]>=4) str.Format("%s의 정착지가 시티로 번화하였습니다.",player.ps[memo[4]-4].name);
				else str.Format("%s가 집을 지었습니다.",player.ps[memo[4]].name);
				
				strcpy(temp,str);
				add_chat(-1,temp);
				
				if(memo[4]%4==player.p_n) {
					if(memo[4]>=4) {
						player.settle--;
						player.city++;
					} else player.settle++;
				}

				if(memo[4] == player.p_n && memo[2]) {
					player.tmp[0] = memo[3];
					player.tmp[1] = 0;
					player.con = CON_ROAD;
				}

				set_longest_road(memo[5],memo[6]);
				
				load_map();
				sndPlaySound("./wave/build.wav",SND_ASYNC);
				draw_map();
				continue;
			}
			if(!strncmp(memo,PH_ROAD,2)) {
				int n0,n1,n2;

				n0 = memo[2];
				n1 = memo[3];
				n2 = memo[4];
				
				if(n2==player.p_n) player.road++;

				int n = g_map.link[n0].link[n1]->road_num(&g_map.link[n0]);

				g_map.link[n0].road[n1] = n2+1;
				g_map.link[n0].link[n1]->road[n] = n2+1;

				CString str;
				str.Format("%s가 길을 지었습니다.",player.ps[n2].name);
				strcpy(temp,str);
				add_chat(-1,temp);

				set_longest_road(memo[5],memo[6]);
				
				load_map();
				sndPlaySound("./wave/build.wav",SND_ASYNC);
				draw_map();
				continue;
			}
			if(!strncmp(memo,PH_RESOURCE,2)) {		// 카드를 얻음
				CString s[5]={"나무","벽돌","양","밀","광석"};
				CString str;
				int p,n,l[4],count=0;

				for(p=0;p<4;p++) {
					for(n=0;n<5;n++) {
						if(p==player.p_n) player.resource[n]+=memo[p*5+n+2];
						player.ps[p].cardn+=memo[p*5+n+2];
					}
						
					if(memo[p*5+2] || memo[p*5+3] || memo[p*5+4] || memo[p*5+5] || memo[p*5+6]) {
						l[count]=p;
						count++;
					}
				}
				
				if(count!=0) add_chat(-1,"▼▼▼▼▼▼▼▼▼▼ 거래내역시작 ▼▼▼▼▼▼▼▼▼▼");
				
				if(count==2 &&	memo[l[0]*5+2] == -1*memo[l[1]*5+2] &&
								memo[l[0]*5+3] == -1*memo[l[1]*5+3] &&
								memo[l[0]*5+4] == -1*memo[l[1]*5+4] &&
								memo[l[0]*5+5] == -1*memo[l[1]*5+5] &&
								memo[l[0]*5+6] == -1*memo[l[1]*5+6]) {

					for(n=0;n<5;n++) {
						if(memo[22] && memo[l[0]*5+n+2]!=0) {

							str.Format("%s 카드 %d장을 %s가 %s에게 주었습니다.",	s[n],
																					abs(memo[l[0]*5+n+2]),
																					memo[l[0]*5+n+2]<0 ? player.ps[l[0]].name : player.ps[l[1]].name,
																					memo[l[0]*5+n+2]>0 ? player.ps[l[0]].name : player.ps[l[1]].name);
							strcpy(temp,str);
							add_chat(-1,temp);
						}
					}
				} else {
					for(p=0;p<4;p++) {
						for(n=0;n<5;n++) {
							if(memo[p*5+n+2]!=0) {
								if(memo[22]) {
									if(memo[p*5+n+2]>0)	str.Format("%s가 %s 카드를 %d장 얻었습니다.",player.ps[p].name,s[n],memo[p*5+n+2]);
									else str.Format("%s가 %s 카드를 %d장 버렸습니다.",	player.ps[p].name,s[n],memo[p*5+n+2]*-1);
									strcpy(temp,str);
									add_chat(-1,temp);
								}
							}
						}
					}
				}
				
				if(count!=0) add_chat(-1,"▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲");

				Clientx.ch_send(PH_RESOURCEOK,2);

				Invalidate(FALSE);
				player_check();
				
				continue;
			}
			if(!strncmp(memo,PH_BUY_CARD,2)) {		// 카드를 얻음
				CString s[25]={	"대성당(포인트+1)",
								"카탄대학(포인트+1)",
								"도서관(포인트+1)",
								"통치자의거처(포인트+1)",
								"사당(포인트+1)",
								"군대",
								"군대",
								"군대",
								"군대",
								"군대",
								"군대",
								"군대",
								"군대",
								"군대",
								"군대",
								"군대",
								"군대",
								"군대",
								"군대",
								"자원독점",
								"자원독점",
								"풍년",
								"풍년",
								"길공사",
								"길공사"
				};

				CString str;

				if(memo[2]==player.p_n) {
					player.card[player.cardn]=memo[3];
					player.cardn++;								// 카드얻음
					
					player.newcard[player.newcardn]=memo[3];
					player.newcardn++;
					
					player.resource[2]--;
					player.resource[3]--;
					player.resource[4]--;
					str.Format("◆ [%s] 카드를 구입했습니다. ◆",s[memo[3]]);
				} else str.Format("◆ %s가 카드를 구입했습니다. ◆",player.ps[memo[2]].name);

				player.ps[memo[2]].cardn-=3;
				
				strcpy(temp,str);
				add_chat(-1,temp);

				Clientx.ch_send(PH_RESOURCEOK,2);

				Invalidate(FALSE);
				player_check();
				
				continue;
			}

			if(!strncmp(memo,PH_BUY_CARDNO,2)) {
				add_chat(-1,"카드가 없어 구매할수 없습니다.");
				con_enable(true);
				continue;
			}

			if(!strncmp(memo,PH_CARD_ROAD2,2)) {
				player.tmp[99] = 2;		// 돈없이 건물을 한번 더지어라
				OnRoadBtn();
				continue;
			}
			if(!strncmp(memo,PH_CARD_ABUNDANCEA,2)) {
				// 풍년카드사용으로 얻을 카드선택

				int l;
				int resource[5];

				while(1) {
					CDropDlg dropdlg;
				
					strcpy(dropdlg.caption,"얻을카드(N):");
					
					for(l=0;l<5;l++) {
						dropdlg.resource[l]=memo[2+l];
						resource[l]=0;
					}

					dropdlg.drop_n=2;

					dropdlg.DoModal();

					if(dropdlg.drop_w==2) {
						for(l=0;l<5;l++) resource[l]=dropdlg.target_resource[l];
						break;
					}
				}
				
				Invalidate(FALSE);

				strcpy(temp,PH_RESOURCE);
				for(l=0;l<20;l++)temp[2+l]=0;
				for(l=0;l<5;l++)temp[player.p_n*5+l+2]=resource[l];
				
				temp[22]=1;
				
				Clientx.ch_send(temp,23);
				continue;
			}

			if(!strncmp(memo,PH_CARD_MONOPOLY,2)) {
				// 독점카드 사용
				
				strcpy(temp,PH_USE_MONOPOLY);
				while(1) {
					CDropDlg dropdlg;
					strcpy(dropdlg.caption,"독점할 카드(N):");
				
					for(int l=0;l<5;l++) dropdlg.resource[l]=1;
					dropdlg.drop_n=1;

					dropdlg.DoModal();

					if(dropdlg.drop_w==1) {
						for(l=0;l<5;l++)if(dropdlg.target_resource[l]) temp[2]=l;
						break;
					}
				}
				Clientx.ch_send(temp,3);
			}
			if(!strncmp(memo,PH_CARD_SOLDIER,2)) {
				// 군대카드 사용
				player.con=CON_PLAGUE;
				player.tmp[0]=1;
				wait_picture=0;
				sndPlaySound("./wave/plague_which.wav",SND_ASYNC);
				add_chat(-1,"도둑을 움직여주세요");
				continue;
			}

			if(!strncmp(memo,PH_RAND_RESOURCE,2)) {				// 카드 랜덤하여 하나 뺏김
				int n=0,r,target=memo[3],i;
				for(i=0;i<5;i++) n+=player.resource[i];

				r=rand()%n;
				n=0;

				while(1) {
					if(r-player.resource[n]>=0) {
						r-=player.resource[n];
						n++;
						if((n>4 || r==0 ) && player.resource[n]) break;
					} else break;
				}

				strcpy(temp,PH_RESOURCE);
				for(i=0;i<20;i++)temp[i+2]=0;
				temp[n+player.p_n*5+2]--;	temp[n+target*5+2]++;
				temp[22]=1;

				Clientx.ch_send(temp,23);
				continue;
			}
			
			if(!strncmp(memo,PH_TURN,2)) {
				player.turn=true;
				Invalidate(FALSE);
				continue;
			}
			
			if(!strncmp(memo,PH_PLAGUE_MOVE,2)) {	// 도둑설정

				player.tmp[0]=0;
				player.con=CON_PLAGUE;
				wait_picture=0;
				sndPlaySound("./wave/plague_which.wav",SND_ASYNC);
				add_chat(-1,"도둑을 움직여주세요");

				continue;
			}
			
			if(!strncmp(memo,PH_PLAGUE_WHITCH,2)) {	// 도둑이동
				int n;

				g_map.plague.x = memo[2];
				g_map.plague.y = memo[3];
				if(memo[5]) {
					player.ps[memo[4]].soldier++;
					if(memo[6]!=6) {
						if(memo[6]==5) {
							add_chat(-1,"대군대카드의 소유주가 없어졌습니다.");
							for(n=0;n<player.cardn;n++) if(player.card[n] == 25) break;
							if(n!=player.cardn) {
								for(;n<player.cardn-1;n++) player.card[n] = player.card[n+1];
								player.cardn--;
							}
						} else {
							CString str;
							if(memo[6]==player.p_n) {
								str = "대군대카드를 얻었습니다.";
								player.card[player.cardn++]=25;
							} else str.Format("대군대카드를 %s가 얻었습니다.",player.ps[memo[6]].name);

							strcpy(temp,str);
							add_chat(-1,temp);
						}
						
					}
				}

				add_chat(-1,"도둑이 이동하였습니다.");
				
				load_map();
				player_check();
				Invalidate(FALSE);
				sndPlaySound("./wave/plague.wav",SND_ASYNC);
				continue;
			}
			
			if(!strncmp(memo,PH_HALF_DROP,2)) {
				// 카드를 버린다. (7장이상이면)
				int n=0,l;
				int resource[5];
				for(l=0;l<5;l++) {n+=player.resource[l];resource[l]=0;}

				if(n>7) {
					
					while(1) {
						CDropDlg dropdlg;
						strcpy(dropdlg.caption,"반버릴카드(N):");
					
						for(l=0;l<5;l++)dropdlg.resource[l]=player.resource[l];
						dropdlg.drop_n=n/2;

						dropdlg.DoModal();

						if(dropdlg.drop_w==n/2) {
							for(l=0;l<5;l++) resource[l]+=dropdlg.target_resource[l];
							break;
						}
					}
					
					Invalidate(FALSE);
				}

				strcpy(temp,PH_HALFOK);	for(l=0;l<5;l++)temp[l+2]=resource[l];
				Clientx.ch_send(temp,7);
				
				continue;
			}
			if(!strncmp(memo,PH_TRADE_SOURCE,2)) {
				CTrainDlg tradedlg;
				int n,l;

				for(n=0;n<5;n++) for(l=0;l<memo[4+n];l++) tradedlg.source[tradedlg.sn++]=n;
				for(n=0;n<5;n++) tradedlg.resource[n]=player.resource[n];
				strcpy(tradedlg.player_name[tradedlg.playern++],player.ps[memo[2]].name);
				tradedlg.select=1;
				tradedlg.con=1;

				tradedlg.DoModal();

				strcpy(temp,PH_TRADE_TARGET);
				for(n=0;n<10;n++)temp[4+n]=0;

				if(tradedlg.trade_ok) {
					temp[2]=player.p_n;
					temp[3]=memo[2];

					for(n=0;n<tradedlg.sn;n++)temp[4+tradedlg.source[n]]++;
					for(n=0;n<tradedlg.tn;n++)temp[9+tradedlg.target[n]]++;
					
				} else {
					temp[2]=5;
					temp[3]=memo[2];
				}
				
				Clientx.ch_send(temp,14);
			}
			if(!strncmp(memo,PH_TRADE_TARGET,2)) {
				int resource[20];
				int n,l;

				for(n=0;n<20;n++)resource[n]=0;
				if(memo[2]!=5) {
					CTrainDlg tradedlg;

					for(n=0;n<5;n++) for(l=0;l<memo[4+n];l++) tradedlg.source[tradedlg.sn++]=n;
					for(n=0;n<5;n++) for(l=0;l<memo[9+n];l++) tradedlg.target[tradedlg.tn++]=n;
					strcpy(tradedlg.player_name[tradedlg.playern++],player.ps[memo[2]].name);
					tradedlg.select=1;
					tradedlg.con=2;

					tradedlg.DoModal();
					if(tradedlg.trade_ok) {
						for(n=0;n<5;n++) {
							resource[memo[3]*5+n]-=memo[4+n];
							resource[memo[2]*5+n]+=memo[4+n];
						}
						for(n=0;n<5;n++) {
							resource[memo[3]*5+n]+=memo[9+n];
							resource[memo[2]*5+n]-=memo[9+n];
						}
					}
				}

				strcpy(temp,PH_RESOURCE);
				for(n=0;n<20;n++)temp[2+n]=resource[n];
				temp[22]=1;
				Clientx.ch_send(temp,23);
			}

			if(!strncmp(memo,PH_GAMEEND,2)) {
				KillTimer(998);
				CString str;
				
				bgindex=4;

				if(memo[2]==player.p_n) {
					HWND hWnd = ::FindWindow(NULL,"HGR Catan...");
					::SendMessage(hWnd,WM_GAMEOVER,1,0);

					str.Format("당신승리로 게임이 끝났습니다.",player.ps[memo[3]].name);
					sndPlaySound("./wave/victory.wav",SND_ASYNC);
				} else {
					HWND hWnd = ::FindWindow(NULL,"HGR Catan...");
					::SendMessage(hWnd,WM_GAMEOVER,2,0);

					str.Format("%s 의 승리로 게임이 끝났습니다.",player.ps[memo[3]].name);
					sndPlaySound("./wave/lost.wav",SND_ASYNC);
				}

				MessageBox(str);
				OnOK();
				
				return;
			}
		}
	}
	
	CDialog::OnTimer(nIDEvent);
}

void CKatanPlay::OnChatBtn() 
{
	char message[500],temp[500];
	////////////////////////////////////////////////////////////
	// 대화 버튼을 클릭하였을때

	strcpy(message,PH_MESSAGE);

	message[2]='0'+player.p_n;
	m_chat_input.GetLine(0,temp,255);	strcpy(message+3,temp);

	if(strlen(message+3))	Clientx.ch_send(message,strlen(message));
		
	m_chat_input.SetSel(0, -1);
	m_chat_input.Clear();
}

void CKatanPlay::add_chat(int n, char *message)
{
	////////////////////////////////////////////////////////////////////////////////////////////
	// 채팅창에 내용을 추가합니다. (n은 사용자 고유번호이고 0일때에는 사용자번호가 생략됩니다.
	//
	////////////////////////////////////////////////////////////////////////////////////////////

	static chat_line=0, event_line=0;
	int nCnt;
	char enter_char[]={13,10,0};
	
	CString str,str2;
	
	if(n!=-1) {
		if(chat_line>100) {

			while(chat_line>50) {
				viewer[0].GetWindowText(str);
				int nx=str.Find('\n',0);
			
				str2 = (LPCTSTR)str+nx+1;
				viewer[0].SetWindowText(str2);

				chat_line--;
			}
		}
	
		if(n!=5) {
			nCnt = viewer[0].GetWindowTextLength();
			viewer[0].SetSel(nCnt, -1);
			viewer[0].ReplaceSel(player.ps[n].name);

			nCnt = viewer[0].GetWindowTextLength();
			viewer[0].SetSel(nCnt, -1);
			viewer[0].ReplaceSel(") ");
		}

		nCnt = viewer[0].GetWindowTextLength();
		viewer[0].SetSel(nCnt, -1);
		viewer[0].ReplaceSel(message);

		nCnt = viewer[0].GetWindowTextLength();
		viewer[0].SetSel(nCnt, -1);
		viewer[0].ReplaceSel(enter_char);
		chat_line++;

		sndPlaySound("./wave/chat.wav",SND_ASYNC);
	} else {
		if(!strncmp(message,"차례",2)) {
			player.who_turn=message[4]-'0';
			player_check();
			sndPlaySound("./wave/turnend.wav",SND_ASYNC);
			return;
		}
		if(event_line>100) {

			while(event_line>50) {
				viewer[1].GetWindowText(str);
				int nx=str.Find('\n',0);
			
				str2 = (LPCTSTR)str+nx+1;
				viewer[1].SetWindowText(str2);

				event_line--;
			}
		}
	
		nCnt = viewer[1].GetWindowTextLength();
		viewer[1].SetSel(nCnt, -1);
		viewer[1].ReplaceSel(message);

		nCnt = viewer[1].GetWindowTextLength();
		viewer[1].SetSel(nCnt, -1);
		viewer[1].ReplaceSel(enter_char);
		event_line++;
	}
}

BOOL CKatanPlay::PreTranslateMessage(MSG* pMsg) 
{
	if(pMsg->message == WM_KEYDOWN)	// 키보드를 눌렀을때 //
	{
		if	(pMsg->hwnd == viewer[0].GetSafeHwnd() ||
			pMsg->hwnd == viewer[1].GetSafeHwnd()) {
			return TRUE;
		}

		if(	pMsg->wParam == VK_RETURN) {
				/////////////////////////////////////////////////////////////
				// 만약 채팅창에서 엔터를 쳤다면 글입력 이벤트를 발생한다. //
				/////////////////////////////////////////////////////////////

			if(pMsg->hwnd == m_chat_input.GetSafeHwnd()) OnChatBtn();
			return TRUE;	// ENTER키 막기
		}

		if(pMsg->wParam == VK_ESCAPE) return TRUE;
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}



void CKatanPlay::set_longest_road(int new_road, int old_road)
{
	char temp[500];
	CString str;
	if(new_road!=6) {
		
		if(old_road==player.p_n) {
			for(int x=0;x<player.cardn;x++) if(player.card[x]==26) break;
			if(x!=player.cardn) for(;x<player.cardn-1;x++) player.card[x]=player.card[x+1];
			player.cardn--;
		}

		if(new_road==5) {
			str.Format("LONGEST ROAD 소유주가 없어졌습니다.");
		}
		else if(new_road==player.p_n) {
			player.card[player.cardn++]=26;
			str.Format("LONGEST ROAD를 얻었습니다.");
		} else str.Format("LONGEST ROAD를 %s가 얻었습니다.",player.ps[new_road].name);
		
		strcpy(temp,str);
		add_chat(-1,temp);
	}
}



void CKatanPlay::OnDestroy() 
{
	bgindex=0;
	WaitForSingleObject(bgThread->m_hThread,2000);
	
	CDialog::OnDestroy();
}



int CKatanPlay::randdice()
{
	// 주사위 확률 랜덤
	static int probability[60],n=0;

	if(!n) {
		bool rnd[60];
		int i;
		for(i=0;i<60;i++)rnd[i]=false;
		for(i=0;i<60;i++) {
			do {
				probability[i]=rand()%60;
			} while(rnd[probability[i]]);
			rnd[probability[i]]=true;
		}
	}
	
	int v = probability[n]/10 +1;
	if(++n>=60) n=0;
	
	return v;

}


BOOL CKatanPlay::OnEraseBkgnd(CDC* pDC) 
{
	static HBITMAP hBMP=NULL,oldBMP;
	static BITMAP  bmp;
	bool win_rect=false;
	HDC     hDCImage;
	CRect   rect;
	if(!hBMP) {
		hBMP = (HBITMAP)LoadImage(AfxGetInstanceHandle(), "./images/other/background.bmp"
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



void CKatanPlay::OnQuitBtn() 
{
	OnOK();	
}

