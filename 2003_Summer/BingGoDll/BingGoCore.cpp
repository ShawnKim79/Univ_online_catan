#include "BingGoCore.h"
#include <stdlib.h>
#include <time.h>


BingGoCore::BingGoCore()
{
	memset(fiveline,0,sizeof(fiveline));
}

BingGoCore::~BingGoCore()
{

}

void BingGoCore::ResetBoard()
{
	memset(fiveline,0,sizeof(fiveline));
	memset(Board,BG_B_EMPTY,sizeof(Board));
	memset(Ch_Board,BG_B_NORMAL,sizeof(Ch_Board));
}

int BingGoCore::IsBinGo()
{
	int cnt = 0;
	BOOL empty_width = FALSE;
	BOOL empty_height = FALSE;

	for(int i = 0; i < BG_B_HEIGHT;i++)
	{
		for(int j = 0 ; j < BG_B_WIDTH;j++)
		{
			// 가로 검사
		/*	if(Ch_Board[i][j] != BG_B_PUSHED)
				empty_width = TRUE;
			// 세로 검사
			if(Ch_Board[j][i] != BG_B_PUSHED)
				empty_height = TRUE;*/
			if(Ch_Board[i][j] == BG_B_NORMAL)
				empty_width = TRUE;
			// 세로 검사
			if(Ch_Board[j][i] == BG_B_NORMAL)
				empty_height = TRUE;
			
		}

		if(empty_width == FALSE)
		{
			if(i >= BG_B_HEIGHT)
				i = BG_B_HEIGHT-1;

			if(j >= BG_B_WIDTH)
				j = BG_B_WIDTH-1;

			fiveline[cnt].x = i*BG_B_WIDTH;
			fiveline[cnt].y = i*BG_B_WIDTH+j;
			cnt++;
		}

		if(empty_height == FALSE)
		{
			if(i >= BG_B_HEIGHT)
				i = BG_B_HEIGHT-1;

			if(j >= BG_B_WIDTH)
				j = BG_B_WIDTH-1;

			fiveline[cnt].x = i;
			fiveline[cnt].y = i+BG_B_WIDTH*(BG_B_HEIGHT-1);
			cnt++;
		}

		empty_height = empty_width = FALSE;
	}

	// 크로스 검사
	BOOL empty_left=FALSE;
	BOOL empty_right=FALSE;
	
	for(i = 0 ; i < BG_B_HEIGHT;i++)
	{	
/*		if(Ch_Board[i][i] != BG_B_PUSHED)
			empty_left = TRUE;
		BG_B_NORMAL
		if(Ch_Board[i][BG_B_HEIGHT-1-i] != BG_B_PUSHED)
			empty_right = TRUE;*/

		if(Ch_Board[i][i] == BG_B_NORMAL)
			empty_left = TRUE;
		
		if(Ch_Board[i][BG_B_HEIGHT-1-i] == BG_B_NORMAL)
			empty_right = TRUE;
	}

	if(empty_left == FALSE)
	{
		fiveline[cnt].x = 0;
		fiveline[cnt].y = 24;
		cnt++;
	}

	if(empty_right == FALSE)
	{
		fiveline[cnt].x = 4;
		fiveline[cnt].y = 20;
		cnt++;
	}

	if(cnt >= BG_D_COUNT)
	{
		EndGame = TRUE;
		return TRUE;
	}

	return FALSE;
}

void BingGoCore::SetBoard()
{
	srand((unsigned int)time(NULL));
	
	char cnt;
	for(int i = 0; i < 24;i++)
	{
		cnt = rand()%25;

		if(!IsExist(cnt))
		{
			Board[cnt/BG_B_HEIGHT][cnt%BG_B_WIDTH] = i+1;
		}
		else
		{
			i--;
		}
	}
}

BOOL BingGoCore::IsExist(BYTE cnt)
{
	if(Board[cnt/BG_B_HEIGHT][cnt%BG_B_WIDTH] == BG_B_EMPTY)
		return FALSE;

	return TRUE;
}

BOOL BingGoCore::IsPushed(int x,int y)
{
	char Char = Ch_Board[y][x];
	if(Char == BG_B_PUSHED || Char == BG_B_MYPUSH)
		return TRUE;

	return FALSE;
}

BOOL BingGoCore::SetPush(int x,int y)
{
	if(!IsPushed(x,y))
	{
		Ch_Board[y][x] = BG_B_PUSHED;
		return TRUE;
	}

	return FALSE;
}

void BingGoCore::SetBoardByIndex(int index)
{
	for(int i = 0 ; i < BOARDWIDTH;i++)
	{
		for(int j = 0 ; j < BOARDHEIGHT;j++)
		{
			if(Board[i][j] == index)
			{
				Ch_Board[i][j] = BG_B_PUSHED;
				return ;
			}
		}
	}
}
