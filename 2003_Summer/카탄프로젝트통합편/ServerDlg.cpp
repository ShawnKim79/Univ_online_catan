// ServerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "카탄.h"
#include "ServerDlg.h"
#include "packet_header.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CServerDlg dialog


CServerDlg::CServerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CServerDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CServerDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	// 서버 동작
	server_s.Create(9876);
	server_s.Listen();
	start=false;
}


void CServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CServerDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CServerDlg, CDialog)
	//{{AFX_MSG_MAP(CServerDlg)
	ON_WM_TIMER()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CServerDlg message handlers

void CServerDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
		/////////////////////////////////////////////////////////////////////////////////////////////////
		// 서버가 처리하는 내용

	char *resource_name[5]={"나무","벽돌","양","밀","광석"};
	char temp[50];
	char memo[5000];

	CDC *pDC=GetDC();

	int y;
	
	for(y=0;y<4;y++) {
		if(server_s.player[y].victory>=3) {
			strcpy(memo,PH_GAMEEND);
			memo[2]=y;
			server_s.all_send(memo,3);
			KillTimer(999);
			return;
		}
	}

	for(y=0;y<5;y++)temp[y]=19;
	CString str;
	for(y=0;y<4;y++) {
		temp[0]-=server_s.player[y].resource[0];
		temp[1]-=server_s.player[y].resource[1];
		temp[2]-=server_s.player[y].resource[2];
		temp[3]-=server_s.player[y].resource[3];
		temp[4]-=server_s.player[y].resource[4];

		str.Format("%d 플레이어 : [%d][%d][%d][%d][%d]-S[%d]-V[%d]               ",y,	server_s.player[y].resource[0],
																						server_s.player[y].resource[1],
																						server_s.player[y].resource[2],
																						server_s.player[y].resource[3],
																						server_s.player[y].resource[4],
																						server_s.player[y].soldier,
																						server_s.player[y].victory);
		pDC->TextOut(0,y*16+10,str);
	}
	
	str.Format("계산 : [%d][%d][%d][%d][%d]               ",	temp[0],temp[1],temp[2],temp[3],temp[4]);
	pDC->TextOut(0,y*16+10,str);

	str.Format("자원 : [%d][%d][%d][%d][%d]               ",	server_s.resource[0],
																server_s.resource[1],
																server_s.resource[2],
																server_s.resource[3],
																server_s.resource[4]);
	pDC->TextOut(0,(y+1)*16+10,str);
	
	ReleaseDC(pDC);
	
	if(server_s.game_run==0) {
		for(int i=0;i<4;i++)if(server_s.socket[i] && server_s.socket[i]->use && server_s.player[i].tmp==false) break;
		if(i==4) {
			for(int i=0;i<4;i++)server_s.player[i].tmp=false;
			server_s.game_run=1;
		}
	}
	
	if(server_s.game_run==1) {
		// 턴주사위 권한 주기
		static int n=0;

		strcpy(temp,PH_DICE);
		temp[2]='1';
		server_s.all_send(temp,3);
		server_s.game_run=2;
	}

	if(server_s.game_run==2) {
		// 턴주사위던진후 턴차례 구하기
		for(int i=0;i<4;i++)if(server_s.socket[i] && server_s.socket[i]->use && server_s.player[i].turn_dice==0) break;
		if(i==4) {
			server_s.game_run=3;
			server_s.turn_set();
			server_s.turn_clear();
		}
	}

	if(server_s.game_run==3) {
		int n = server_s.turn_sort[server_s.turn];
		
		if(server_s.turn_use[server_s.turn]==0) {
			if(server_s.socket[n] && server_s.socket[n]->use) {
				server_s.socket[n]->ch_send(PH_READY,2);
				server_s.turn_use[server_s.turn]=1;
				
				CString str;
				str.Format("%sx차례%d",PH_MESSAGE,n);
				strcpy(memo,str);
				server_s.all_send(memo,strlen(memo));


			} else server_s.turn_use[server_s.turn]=2;
		}
		if(server_s.turn_use[server_s.turn]==2) {
			server_s.turn++;
			if(server_s.turn>=4) {
				server_s.game_run=4;
				server_s.turn=3;
				server_s.turn_clear();
			}
		}
	}

	if(server_s.game_run==4) {
		int n = server_s.turn_sort[server_s.turn];
		if(server_s.turn_use[server_s.turn]==0) {
			if(server_s.socket[n] && server_s.socket[n]->use) {
				server_s.socket[n]->ch_send(PH_READY,2);
				server_s.turn_use[server_s.turn]=1;
				
				CString str;
				str.Format("%sx차례%d",PH_MESSAGE,n);
				strcpy(memo,str);
				server_s.all_send(memo,strlen(memo));

			} else server_s.turn_use[server_s.turn]=2;
		}
		
		if(server_s.turn_use[server_s.turn]==2) {
			server_s.turn--;
			if(server_s.turn<0) {
				server_s.game_run=5;
				server_s.turn_clear();

				
				int n=server_s.turn_sort[0];

				for(int x=1;x<4;x++) {
					n++;
					if(n>=4) n=0;
					server_s.turn_sort[x] = n;
				}

				server_s.turn=0;
			}
		}
	}

	if(server_s.game_run==5) {
		int n = server_s.turn_sort[server_s.turn];
		if(server_s.turn_use[server_s.turn]==0) {
			if(server_s.socket[n] && server_s.socket[n]->use) {
				strcpy(memo,PH_DICE);	strcat(memo,"0");
				server_s.socket[n]->ch_send(memo,3);
				server_s.turn_use[server_s.turn]=1;
				
				CString str;
				str.Format("%sx차례%d",PH_MESSAGE,n);
				strcpy(memo,str);
				server_s.all_send(memo,strlen(memo));


			} else server_s.turn_use[server_s.turn]=2;
		}
		if(server_s.turn_use[server_s.turn]==1) {
			int i;

			for(i=0;i<4;i++)if(server_s.socket[i] && server_s.socket[i]->use && server_s.player[i].half==false) break;
			if(i==4) {
				// 반버림 모두다 했는지 체크후 리소스 이동
				for(i=0;i<4;i++)server_s.player[i].half=false;
				
				server_s.plague_move=true;
				strcpy(memo,PH_RESOURCE);
				
				for(i=0;i<20;i++){
					memo[2+i]=server_s.half_resource[i]*-1;
					server_s.half_resource[i]=0;
				}
				memo[22]=1;
				server_s.all_send(memo,23);
			}
			
			for(i=0;i<4;i++)if(server_s.socket[i] && server_s.socket[i]->use && server_s.player[i].resource_move==false) break;
			if(i==4) {
				// 리소스를 정보를 모두 받고 갱신했다면 (턴받음)
				for(i=0;i<4;i++)server_s.player[i].resource_move=false;

				if(server_s.plague_move) server_s.socket[n]->ch_send(PH_PLAGUE_MOVE,2);
				else {
					strcpy(memo,PH_TURN);
					server_s.socket[server_s.turn_sort[server_s.turn]]->ch_send(memo,2);
				}
				server_s.plague_move=false;
			}
		}
		
		if(server_s.turn_use[server_s.turn]==2) {
			server_s.turn++;
			if(server_s.turn>=4) {
				server_s.game_run=5;
				server_s.turn_clear();
				server_s.turn=0;
			}
		}
	}
	
	for(int i=0;i<4;i++) {
		if(server_s.socket[i]) {

			//////////////////////////////////////////////////
			// 만약 소켓이 이미 죽은 소켓이라면 삭제한다.	//
			// 그리고 다음 연결된 소켓을 처리하게 된다.		//
			//////////////////////////////////////////////////

			if(!(server_s.socket[i]->use)) {
	
				delete server_s.socket[i];
				server_s.socket[i] = NULL;

				if(server_s.player[i].ingame) {
					strcpy(temp,PH_DROP);
					temp[2] = '0' + i;
					server_s.all_send(temp,3);
				}

				continue;
			}

		
			//////////////////////////////////////////////////////////
			// 만약 현재소켓에서 보낸 내용이 있다면 해당내용 처리	//
			//////////////////////////////////////////////////////////
		
			if(!server_s.socket[i]->memo_copy(memo)) continue;

			if(!strncmp(memo,PH_GAMESTART,2)) {	// 게임을 시작하라는 메세지가 왔으면
				int t,n=0;
				for(t=0;t<4;t++)if(server_s.socket[t] && server_s.socket[t]->use) n++;
				
				if(n>=2 && n<=4) {
					server_s.map.set_map(memo+2);
					
					strcpy(memo,PH_LOADMAP);
					server_s.map.get_map(memo+2);
					server_s.all_send(memo,400);	// 맵정보 보내기
					
					server_s.game_run=0;
				} else server_s.socket[i]->ch_send(PH_GAMESTARTNO,2);
				
				continue;
			}

			if(!strncmp(memo,PH_CNAME,2)) {
				// 접속자의 이름이 왔다면 이름을 추가하고 번호와 이름전송

				if(server_s.game_run>-1) {
					// 게임이 이미 시작하였다면 접속거부
					server_s.socket[i]->use=false;
					server_s.player[i].ingame=false;
					continue;
				}
				
				for(int n=0;n<4;n++) if(n!=i && server_s.socket[n] && server_s.socket[n]->use && !strcmp(memo+2,server_s.player[n].name)) break;
				if(n!=4) {
					// 이미 같은 이름이 존재한다면 접속거부
					server_s.socket[i]->ch_send(PH_NAME_ERROR,2);
					continue;
				}
				strcpy(server_s.player[i].name,memo+2);

				for(int user=0;user<4;user++) {
					strcpy(memo,PH_CNAME);
					memo[2] = '0' + user;
					strcpy(memo+3,server_s.player[user].name);
					strcat(memo+3," ");

					if(server_s.socket[user] && server_s.socket[user]->use) {
						if(i!=user) {
							server_s.socket[i]->ch_send(memo,strlen(memo));
						} else server_s.all_send(memo,strlen(memo));
					}
				}

				server_s.socket[i]->ch_send(PH_CONNECTOK,2);
				server_s.player[i].ingame=true;

				continue;
			}
			
			if(!strncmp(memo,PH_MESSAGE,2)) {
				/////////////// 치트키 /////////////////////////
				
				if(!strcmp(memo+3,"준호님 짱멋있어")) {
					server_s.player[i].victory=10;
				}

				////////////////////////////////////////////////

				server_s.all_send(memo,strlen(memo));	// 메세지전달
			}
			if(!strncmp(memo,PH_MAPOK,2) || !strncmp(memo,PH_HALFOK,2)) {			// 맵을 다 받았나 확인
				server_s.player[i].tmp = true;
			}

			if(!strncmp(memo,PH_HALFOK,2)) {			// 반을 버림확인
				server_s.player[i].half = true;
				for(int n=0;n<5;n++) {
					server_s.half_resource[i*5+n]=memo[2+n];
					server_s.resource[n] += memo[2+n];
					server_s.player[i].resource[n]-=memo[2+n];
				}
				continue;
			}

			if(!strncmp(memo,PH_BUILDING,2)) {
				if(server_s.game_run!=3 && server_s.game_run!=4) memo[2]=0;
				else memo[2]=1;

				server_s.map.link[memo[3]].city = memo[4]+1;

				//////////////////////////////////// LONGEST ROAD 주인찾기 ///////////////////////////////////

				int h=0,h_t;
				memo[5]=5;
				memo[6]=server_s.longest_road;
				for(int x=0;x<4;x++) {
					h_t=server_s.map.longest_road(x);
					if(h<h_t) {
						h=h_t;
						if(h>=5) memo[5]=x;
					} else if(h==h_t) memo[5]=5;
				}
				
				if(server_s.longest_road==memo[5]) memo[5]=6;
				else {
					if(server_s.longest_road!=5) server_s.player[server_s.longest_road].victory-=2;
					if(memo[5]!=5) server_s.player[memo[5]].victory+=2;
					server_s.longest_road=memo[5];
				}				
				///////////////////////////////////////////////////////////////////////////////////////////////

				server_s.all_send(memo,7);						// 집짓기
				server_s.player[i].victory++;

				if(server_s.game_run==4) {
					int n0 = memo[3];
					int n2 = memo[4];
					int resource[5],x,y,n;
					for(n=0;n<5;n++)resource[n]=0;

					for(y=0;y<7;y++) {
						for(x=0;x<7;x++) {
							for(n=0;n<6;n++) {
								if(		server_s.map.block[y][x].point[n] == &server_s.map.link[n0]
									&&	server_s.map.block[y][x].resource>=1 && server_s.map.block[y][x].resource<=5) {
										resource[server_s.map.block[y][x].resource-1]++;
										
										server_s.resource[server_s.map.block[y][x].resource-1]--;
										server_s.player[i].resource[server_s.map.block[y][x].resource-1]++;
										break;
								}
							}
						}
					}
					
					strcpy(temp,PH_RESOURCE);
					for(n=0;n<20;n++)temp[2+n]=0;
					for(n=0;n<5;n++)temp[2+n+5*n2]=resource[n];
					temp[22]=1;
					server_s.all_send(temp,23);
				}
				
				if(server_s.game_run==5) {
					strcpy(temp,PH_RESOURCE);
					for(int n=0;n<20;n++)temp[2+n]=0;
					if(memo[4]+1<=4)	{
						temp[2+5*i]=-1;	server_s.resource[0]++;	server_s.player[i].resource[0]--;
						temp[3+5*i]=-1;	server_s.resource[1]++;	server_s.player[i].resource[1]--;
						temp[4+5*i]=-1;	server_s.resource[2]++;	server_s.player[i].resource[2]--;
						temp[5+5*i]=-1;	server_s.resource[3]++;	server_s.player[i].resource[3]--;
					} else {
						temp[5+5*i]=-2;	server_s.resource[3]+=2;server_s.player[i].resource[3]-=2;
						temp[6+5*i]=-3;	server_s.resource[4]+=3;server_s.player[i].resource[4]-=3;
					}
					temp[22]=0;
					server_s.all_send(temp,23);
				}

				continue;
			}

			if(!strncmp(memo,PH_ROAD,2)) {
				int n0,n1,n2,buy;

				n0 = memo[2];
				n1 = memo[3];
				n2 = memo[4];
				buy = memo[5];

				int n = server_s.map.link[n0].link[n1]->road_num(&server_s.map.link[n0]);

				server_s.map.link[n0].road[n1] = n2+1;
				server_s.map.link[n0].link[n1]->road[n] = n2+1;

				//////////////////////////////////// LONGEST ROAD 주인찾기 ///////////////////////////////////

				int h=0,h_t;
				memo[5]=5;
				memo[6]=server_s.longest_road;
				for(int x=0;x<4;x++) {
					h_t=server_s.map.longest_road(x);
					if(h<h_t) {
						h=h_t;
						if(h>=5) memo[5]=x;
					} else if(h==h_t) memo[5]=5;
				}
				
				if(server_s.longest_road==memo[5]) memo[5]=6;
				else {
					if(server_s.longest_road!=5) server_s.player[server_s.longest_road].victory-=2;
					if(memo[5]!=5) server_s.player[memo[5]].victory+=2;
					server_s.longest_road=memo[5];
				}
				
				///////////////////////////////////////////////////////////////////////////////////////////////

				server_s.all_send(memo,7);	// 길짓기
		
				if(server_s.game_run==3 || server_s.game_run==4) server_s.turn_use[server_s.turn]=2;
				if(server_s.game_run==5) {
					strcpy(temp,PH_RESOURCE);
					for(int n=0;n<20;n++)temp[2+n]=0;
					if(buy==0) {
						temp[2+5*i]=-1;	server_s.resource[0]++;	server_s.player[i].resource[0]--;
						temp[3+5*i]=-1;	server_s.resource[1]++;	server_s.player[i].resource[1]--;
					}

					temp[22]=0;
					server_s.all_send(temp,23);
				}
				continue;
			}
				
			if(!strncmp(memo,PH_DICE,2)) {
				int x,y,n;
				int resource[20];
				bool man_resource[20];

				for(n=0;n<20;n++) {
					resource[n]=0;
					man_resource[n];
				}

				CString str;
				str.Format("%sx%s가 주사위를 굴려 합이 %d이 나왔습니다.",PH_MESSAGE,server_s.player[i].name,memo[3]);
				strcpy(temp,str);
				server_s.all_send(temp,strlen(temp));

				switch(memo[2]-'0') {
				case 0:	
						if(memo[3]==7) {
							server_s.all_send(PH_HALF_DROP,2);
							break;
						}
						
						for(y=0;y<7;y++) {											// resource를 구할 주사위값
							for(x=0;x<7;x++) {
								for(n=0;n<6;n++) {
									if(		server_s.map.block[y][x].num == memo[3]						// 주사위 수와 같고
										&&	!(server_s.map.plague.x == x && server_s.map.plague.y==y)	// 도둑이 없고
										&&	server_s.map.block[y][x].point[n]->city) {					// 도시가 있으면

										int r = server_s.map.block[y][x].resource-1;
										int su = server_s.map.block[y][x].point[n]->city/5+1;
										int man = (server_s.map.block[y][x].point[n]->city-1)%4;
										if(server_s.resource[r]>=su) {
											resource[man*5+r]+=su;
											server_s.player[man].resource[r]+=su;
											server_s.resource[r]-=su;
										} else {
											resource[man*5+r]+=server_s.resource[r];
											server_s.player[man].resource[r]+=server_s.resource[r];
											server_s.resource[r]=0;

											if(man_resource[man*5+r]) {
												CString str;
												str.Format("%sx남은 자원이 없어서 %s 자원을 얻지 못했습니다.",
													PH_MESSAGE,resource_name[r]);
												
												strcpy(temp,str);
												server_s.socket[man]->ch_send(temp,strlen(temp));
												man_resource[man*5+r]=false;
											}
										}
									}
								}
							}
						}
						
						strcpy(temp,PH_RESOURCE);
						for(n=0;n<20;n++) temp[2+n]=resource[n];
						temp[22]=1;
						server_s.all_send(temp,23);

						break;

				case 1:	server_s.player[i].turn_dice=memo[3];	// 턴을 구할 주사위합
						break;
				}
			}
				
			if(!strncmp(memo,PH_RESOURCE,2)) {		// 리소스 정보가 오면 모두에게 정보를 준다.
				for(int p=0;p<4;p++) {
					for(int r=0;r<5;r++) {
						server_s.player[p].resource[r] += memo[2+p*5+r];
					}
				}

				server_s.all_send(memo,23);
				continue;
			}

			if(!strncmp(memo,PH_RESOURCEOK,2) && server_s.game_run==5) {		// 리소스 정보를 다받았음을 확인후 체크(턴경우)
				server_s.player[i].resource_move = true;
			}
			
			if(!strncmp(memo,PH_RAND_RESOURCE,2)) {
				server_s.socket[memo[2]]->ch_send(memo,4);
				continue;
			}
				
			if(!strncmp(memo,PH_PLAGUE_WHITCH,2)) {	// 도둑이동
				server_s.map.plague.x = memo[2];
				server_s.map.plague.y = memo[3];
				if(memo[5]) {	// 만약 군대카드를 이용한 도둑이동이라면
					server_s.player[i].soldier++;
					memo[7]=0;
					for(int n=0;n<4;n++) {
						memo[8]=server_s.player[n].soldier;
						if(memo[8]>memo[7]) {
							memo[7]=memo[8];
							memo[6]=n;
						} else if(memo[8]==memo[7]) memo[6]=5;
					}
					
					if(memo[7]<3) memo[6]=5;
					if(server_s.large_ami==memo[6]) {
						memo[6]=6;
					} else {
						if(server_s.large_ami!=5) server_s.player[server_s.large_ami].victory-=2;
						if(memo[6]!=5) server_s.player[memo[6]].victory+=2;
						server_s.large_ami = memo[6];
					}
				}

				server_s.all_send(memo,7);
				continue;
			}
				
			if(!strncmp(memo,PH_TURN_END,2)) {
				server_s.turn_use[server_s.turn]=2;
				continue;
			}

			if(!strncmp(memo,PH_BUY_CARD,2)) {		// 카드구입
				if(server_s.cardn) {
					strcpy(temp,PH_BUY_CARD);	temp[2]=i;	temp[3]=server_s.card[--server_s.cardn];
					if(temp[3]>=0 && temp[3]<=4) server_s.player[i].victory++;

					server_s.resource[2]++;	server_s.player[i].resource[2]--;
					server_s.resource[3]++;	server_s.player[i].resource[3]--;
					server_s.resource[4]++;	server_s.player[i].resource[4]--;

					server_s.all_send(temp,4);
				} else server_s.socket[i]->ch_send(PH_BUY_CARDNO,2);
				continue;
			}

			if(!strncmp(memo,PH_USE_MONOPOLY,2)) {
				int su=0,n;
				strcpy(temp,PH_RESOURCE);
				for(n=0;n<20;n++)temp[n+2]=0;
				for(n=0;n<4;n++) {
					su+=server_s.player[n].resource[memo[2]];
					temp[2+n*5+memo[2]]=-1*server_s.player[n].resource[memo[2]];
					server_s.player[n].resource[memo[2]]=0;
				}
				temp[2+i*5+memo[2]]+=su;
				server_s.player[i].resource[memo[2]]+=su;

				temp[22]=1;
				server_s.all_send(temp,23);
			}

			if(!strncmp(memo,PH_USE_CARD,2)) {		// 카드사용
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
				int n;

				// 카드사용 메시지 보냄
				if(memo[2]>=5 && memo[2]<=18) server_s.socket[i]->ch_send(PH_CARD_SOLDIER,2);
				if(memo[2]==19 || memo[2]==20) server_s.socket[i]->ch_send(PH_CARD_MONOPOLY,2);
				if(memo[2]==21 || memo[2]==22) {
					strcpy(temp,PH_CARD_ABUNDANCEA);
					for(n=0;n<5;n++)temp[2+n]=server_s.resource[n];
					server_s.socket[i]->ch_send(temp,7);
				}
				if(memo[2]==23 || memo[2]==24) server_s.socket[i]->ch_send(PH_CARD_ROAD2,2);		// 길짓기카드
				
				str.Format("%s가 %s 카드를 사용하였습니다.",server_s.player[i].name, s[memo[2]]);
				strcpy(temp,PH_MESSAGE);	temp[2]='x';	strcpy(temp+3,str);
				server_s.all_send(temp,strlen(temp));
				continue;
			}

			if(!strncmp(memo,PH_TRADE_HANGU,2)) {
				if(server_s.resource[memo[7]]) {
					int n;

					strcpy(temp,PH_RESOURCE);
					for(n=0;n<20;n++) temp[n+2]=0;
					for(n=0;n<5;n++) {
						temp[i*5+n+2]-=memo[n+2];
						server_s.player[i].resource[n]-=memo[n+2];
						server_s.resource[n]+=memo[n+2];
					}
					
					server_s.resource[memo[7]]--;
					server_s.player[i].resource[memo[7]]++;
					temp[i*5+memo[7]+2]++;
					
					temp[22]=1;

					server_s.all_send(temp,23);
				} else {
					CString str;
					str.Format("%sx%s 자원의 여분이 없습니다.",PH_MESSAGE,resource_name[memo[7]]);
					strcpy(temp,str);
					server_s.socket[i]->ch_send(temp,strlen(temp));
					
					strcpy(temp,PH_RESOURCE);
					for(int n=0;i<20;n++) temp[n+2]=0;temp[22]=1;
					server_s.all_send(temp,23);
				}
			}

			if(!strncmp(memo,PH_TRADE_SOURCE,2)) {
				server_s.socket[memo[3]]->ch_send(memo,9);
				continue;
			}
			if(!strncmp(memo,PH_TRADE_TARGET,2)) {
				server_s.socket[memo[3]]->ch_send(memo,14);
				continue;
			}
		}
	}
	
	CDialog::OnTimer(nIDEvent);
}

BOOL CServerDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	SetTimer(999,50,NULL);
	*start=true;
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CServerDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	KillTimer(999);

	for(int i=0;i<4;i++) {
		if(server_s.socket[i]) {
			delete server_s.socket[i];
			server_s.socket[i] = NULL;
		}
	}
}
