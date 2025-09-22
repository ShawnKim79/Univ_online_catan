#include <windows.h>

class __declspec(dllexport) omokcore
{
public:
	omokcore();
	~omokcore();

// variable
	BOOL endgame;
	char Board[15][15];
	char currentmarker;			// Black or White checker ?
	UINT ntotal;				// a current number of total checker

// function
	void resetBoard();
	void drawline(HDC hdc);
	void drawchecker(HDC hdc);
	void SetNoneDol(WORD x,WORD y);
	BOOL isInBoard(WORD x,WORD y);
	BOOL isFive(WORD x,WORD y);			// end game?
	BOOL setchecker(WORD x,WORD y);

	HPEN pen;

	void changeBoardcoordiate(WORD& x,WORD& y,WORD cx,WORD cy);
	
};