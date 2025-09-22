// block_inf.cpp: implementation of the Cblock_inf class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "카탄.h"
#include "block_inf.h"
#include <time.h>


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Cblock_inf::Cblock_inf()
{
	srand(time(NULL));
	normal_setting();
}

Cblock_inf::~Cblock_inf()
{

}

void Cblock_inf::normal_setting()
{
	char o_resource[]={ 4,3,4,4,3,1 };
	
	// 0 : 없음   1 : 나무 (4) 2 : 벽돌 (3) 3 : 양 (4)
	// 4 : 밀 (4) 5 : 광석 (3) 6 : 사막 (1) 7 : 바다
	// 8~13 : 항구

	int o_num[] = {0,0,1,2,2,2,2,0,2,2,2,2,1}; // 주사위합계 갯수

	int harbor[]={4,1,1,1,1,1};	// 항구지정

	int p_n[]={0,1,2,5,4,3};	// 꼭지점 연결순서
	
	int max,l_p=0;
	bool out_set = true , out_set2;

	int sp[]={ 0, 9,20,33,49,64,77};
	int dp[]={10,21,34,48,63,76,87},ux,dx;
	
	clear_setting();
	
	for(int y=0;y<7;y++) {
		if(y<=3) max=4+y;
			else max=10-y;

		out_set = !out_set;
		out_set2 = out_set;

		ux=sp[y];
		dx=dp[y];

		for(int x=0;x<max;x++) {
			block[y][x].point[0] = &link[ux+0];	link[ux+0].number = ux+0;
			block[y][x].point[1] = &link[ux+1];	link[ux+1].number = ux+1;
			block[y][x].point[2] = &link[ux+2];	link[ux+2].number = ux+2;

			block[y][x].point[3] = &link[dx+0];	link[dx+0].number = dx+0;
			block[y][x].point[4] = &link[dx+1];	link[dx+1].number = dx+1;
			block[y][x].point[5] = &link[dx+2];	link[dx+2].number = dx+2;

			for(int i=0;i<6;i++) {
				block[y][x].harbor[i]=0;

				int n;
				for(n=0;n<6;n++)if(p_n[n]==i) break;

				block[y][x].point[i]->add_point(block[y][x].point[p_n[(n+1)%6]]);
				block[y][x].point[i]->add_point(block[y][x].point[p_n[(n+5)%6]]);
			}
			if(y==0 || y==6 || x==0 || x==max-1) {
				if(!out_set2) block[y][x].resource = 7;	// 바다지정
				else {
					int rnd_value;
					while(!harbor[rnd_value=rand()%6]);

					block[y][x].resource = rnd_value+8;	// 항구지정
					harbor[rnd_value]--;

					////////////////////////////
					// 항구 포인터 지정

					int h_p[6],h_n;
					for(h_n=0;h_n<6;h_n++)h_p[h_n]=1;
					
					if(y==0) {	h_p[0]=0;	h_p[1]=0;	h_p[2]=0;	}
					if(y==6) {	h_p[3]=0;	h_p[4]=0;	h_p[5]=0;	}

					if(x==0) {
						if(y<=3) h_p[1]=0;
						if(y>=3) h_p[4]=0;
						h_p[0]=0;
						h_p[3]=0;
					}
					
					if(x==max-1) {
						if(y<=3) h_p[1]=0;
						if(y>=3) h_p[4]=0;
						h_p[2]=0;
						h_p[5]=0;
					}
					
					h_n=0;	for(int i=0;i<6;i++)if(h_p[i]) h_n++;
					while(h_n>2) {
						rnd_value=rand()%2;
						if(rnd_value) {
							for(int i=0;i<6;i++) {
								if(h_p[p_n[i]] && h_p[p_n[(i+5)%6]]==0) {
									h_p[p_n[i]]=0;
									h_n--;
									break;
								}
							}
						} else {
							for(int i=5;i>=0;i--) {
								if(h_p[p_n[i]] && h_p[p_n[(i+7)%6]]==0) {
									h_p[p_n[i]]=0;
									h_n--;
									break;
								}
							}
						}
					}

					for(h_n=0;h_n<6;h_n++)if(h_p[h_n]) block[y][x].harbor[h_n]=1;
				}
				out_set2 = !out_set2;
			} else {
				/////////////////////////////////////////////////////////
				// 자원 배정 o_resouce 에 있는 자원맵중 남아 있는 것을 //
				// 무작위로 하나를 꺼내와 넣음						   //
				/////////////////////////////////////////////////////////

				int rnd_value;
					
				while(o_resource[rnd_value=(rand()%6)]==0);
				block[y][x].resource=rnd_value+1;
				o_resource[rnd_value]--;
				
				if(rnd_value!=5) {
					// 사막이 아니라면 숫자판을 배정한다.
					while(o_num[rnd_value=(rand()%13)]==0);
					block[y][x].num=rnd_value;
					o_num[rnd_value]--;
				} else {
					plague.x = x;
					plague.y = y;
				}
			}

			ux+=2;
			dx+=2;
		}
	}
	
	mapxy_setting(100,24,105);
}

void Cblock_inf::clear_setting()
{
	for(int y=0;y<7;y++) {
		for(int x=0;x<7;x++) {
			block[y][x].resource=0;
			block[y][x].num=0;
			for(int i=0;i<6;i++) block[y][x].point[i]=NULL;
		}
	}
}

void Cblock_inf::mapxy_setting(int sx,int sy,int mid)
{
	//////////////////////////////////////////////////
	// sx 는 가로크기								//
	// sy 는 각진부분의 세로						//
	// mid는 세로									//
	//////////////////////////////////////////////////
	
	int i_x=sx*2,i_y=0,max;
		
	for(int y=0;y<7;y++) {
		if(y<=3) {
			max=4+y;
			i_x-=sx/2;
		} else {
			max=10-y;
			i_x+=sx/2;
		}
		if(y==0) i_y=0;
			else i_y+=mid-sy;

		for(int x=0;x<max;x++) {
			for(int p=0;p<6;p++) {
				switch(p) {
				case 0:	block[y][x].point[p]->x = i_x + sx*x;
						block[y][x].point[p]->y = i_y + sy;
						break;
				case 1:	block[y][x].point[p]->x = i_x + sx*x + sx/2;
						block[y][x].point[p]->y = i_y;
						break;
				case 2:	block[y][x].point[p]->x = i_x + sx*x + sx;
						block[y][x].point[p]->y = i_y + sy;
						break;
				case 3:	block[y][x].point[p]->x = i_x + sx*x;
						block[y][x].point[p]->y = i_y + (mid-sy);
						break;
				case 4:	block[y][x].point[p]->x = i_x + sx*x + sx/2;
						block[y][x].point[p]->y = i_y + mid;
						break;
				case 5:	block[y][x].point[p]->x = i_x + sx*x + sx;
						block[y][x].point[p]->y = i_y + (mid-sy);
						break;
				}
			}
		}
	}
}

void Cblock_inf::get_map(char *temp)
{
	int n=0;
	int x,y,z;
	
	for(y=0;y<7;y++)for(x=0;x<7;x++) temp[n++] = block[y][x].resource;
	for(y=0;y<7;y++)for(x=0;x<7;x++) temp[n++] = block[y][x].num;
	for(y=0;y<7;y++)for(x=0;x<7;x++)for(z=0;z<6;z++) temp[n++] = block[y][x].harbor[z];
}

void Cblock_inf::set_map(char *temp)
{
	int n=0;
	int x,y,z;
	
	for(y=0;y<7;y++) {
		for(x=0;x<7;x++) {
			block[y][x].resource = temp[n++];

			if(block[y][x].resource == 6) {
				plague.x = x;
				plague.y = y;
			}
		}
	}

	for(y=0;y<7;y++)for(x=0;x<7;x++) block[y][x].num = temp[n++];
	for(y=0;y<7;y++)for(x=0;x<7;x++)for(z=0;z<6;z++) block[y][x].harbor[z] = temp[n++];
}

void Cblock_inf::copy_map(Cblock_inf *source)
{
	int x,y,z;

	for(y=0;y<7;y++)for(x=0;x<7;x++) block[y][x].resource = source->block[y][x].resource;
	for(y=0;y<7;y++)for(x=0;x<7;x++) block[y][x].num = source->block[y][x].num;
	for(y=0;y<7;y++)for(x=0;x<7;x++)for(z=0;z<6;z++) block[y][x].harbor[z] = source->block[y][x].harbor[z];
}

// 가장 긴길을 찾는다

int Cblock_inf::longest_road(int target)
{
	int longest=0,temp;
	char map_road[300];
	int list[20];

	memset(map_road,0,300);
	memset(list,0,20);

	for(int i=0;i<96;i++) {
	//////////////////////////////////
	
		int high_n=0;
		int n=0;
		int road,start_road=0;

		link_type *lp = &link[i];

	loop_start:
		while(1) {
			if(high_n<n) high_n=n;
		
			for(road=start_road; road<lp->link_max; road++) {
				if(lp->road[road]-1 == target && map_road[lp->number*3+road]==0 && (n==0 || lp->city==0 || (lp->city-1)%4==target)) {
					for(int x=0;x< lp->link[road]->link_max; x++) {
						if(lp->link[road]->link[x] == lp && map_road[lp->link[road]->number*3+x]==0) {
							map_road[lp->number*3+road]=1;
							list[n]=lp->number*3+road;
				
							lp = lp->link[road];
							start_road=0;
							n++;
							goto loop_start;
						}
					}
				}
			}

			if(road==lp->link_max) {
				if(n) {
					start_road = list[n-1]%3+1;
					map_road[list[n-1]]=0;
					lp=&link[list[n-1]/3];
					n--;
				} else break;
			}
		}
		
		////////////////////////////////////
		temp=high_n;
	
		if(longest<temp) longest=temp;
	}
	
	return longest;
}