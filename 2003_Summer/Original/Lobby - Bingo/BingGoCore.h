
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
	POINT fiveline[5];										//x Ω√¿€ ¿Œµ¶Ω∫ y ¡æ∑· ¿Œµ¶Ω∫
	
	BOOL EndGame;
	DWORD line_cnt;
	BYTE Board[BG_B_HEIGHT][BG_B_WIDTH];				// y ,x
	BYTE Ch_Board[BG_B_HEIGHT][BG_B_WIDTH];				// y ,x

private:
	BOOL IsExist(BYTE cnt);
	BOOL IsPushed(int x,int y);
};
