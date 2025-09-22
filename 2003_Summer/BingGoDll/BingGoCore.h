
#include "BGCore.h"
#include<windows.h>

class __declspec(dllexport) BingGoCore 
{
public:
	BingGoCore();
	~BingGoCore();
	
	BOOL IsBinGo();
	void SetBoard();
	void ResetBoard();
	BOOL SetPush(int x,int y);
	void SetBoardByIndex(int index);
	POINT fiveline[5];										//x 시작 인덱스 y 종료 인덱스
	
	BOOL EndGame;
	DWORD line_cnt;
	BYTE Board[BG_B_HEIGHT][BG_B_WIDTH];				// y ,x 숫자들이 배열 되어 있는 보드
	BYTE Ch_Board[BG_B_HEIGHT][BG_B_WIDTH];				// y ,x 눌렸는지가 판별 되어 들어 있는 보드

private:
	BOOL IsExist(BYTE cnt);
	BOOL IsPushed(int x,int y);
};
