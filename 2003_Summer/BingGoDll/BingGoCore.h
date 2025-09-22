
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
	POINT fiveline[5];										//x ���� �ε��� y ���� �ε���
	
	BOOL EndGame;
	DWORD line_cnt;
	BYTE Board[BG_B_HEIGHT][BG_B_WIDTH];				// y ,x ���ڵ��� �迭 �Ǿ� �ִ� ����
	BYTE Ch_Board[BG_B_HEIGHT][BG_B_WIDTH];				// y ,x ���ȴ����� �Ǻ� �Ǿ� ��� �ִ� ����

private:
	BOOL IsExist(BYTE cnt);
	BOOL IsPushed(int x,int y);
};
