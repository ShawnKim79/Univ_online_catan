#pragma once 

#include "omok.h"
#include "omoksound.h"
#include "cChatView.h"
#include "cchatinput.h"
#include "cNoticeview.h"
#include "userinfo.h"
#include "cConsol.h"


//////////////////////////////////////////////////////////////////////////////////

#define WINDOWCOUNT		4

//////////////////////////////////////////////////////////////////////////////////

enum WINDOWPOINTER{
	TALKFIELDPOINTER = 0,
	CHATVIEWPOINTER,
	NOTICEPOINTER,
	SOCKETPOINTER,
};

//////////////////////////////////////////////////////////////////////////////////

class  __declspec(dllexport) cOmokManage{

public:
	cOmokManage();
	~cOmokManage();
	
	HWND createOmokWnd();		// call it!
	
	void SetParentHandle(HWND parent_hWnd) { call_hWnd = parent_hWnd;};

	void SetMyInfo(USERINFO& user);
	void SetcompetitorInfo(USERINFO& cuser);	
	
	void SetNotice(char* buff,BYTE* charformat);					// show notice
	HWND GetHandle(){return hWnd;};
	HINSTANCE hInst;			
	
private:
/////////////////////////////////////////////////////
	USERINFOEX		m_myinfo;
	USERINFOEX		m_competitorinfo;

	HWND hWnd;									// handle of main-window(parent)
	HWND call_hWnd;								

	char* m_msg;								// processing buff
	char time;
	char clockloop;
	int strtime;
	int m_msglen;								// buffer length
	int m_bserver;								// is server ?

	UINT m_checkernum;							// a number of checkers

	BOOL m_played;								// game play
	BOOL m_Myturn;								// this my turn
	BOOL m_Returndol;							
	BOOL m_Reserved;							// reserved out flag
	BOOL m_GameOut;
	BOOL m_ButtonDown;
	BOOL m_Pause;
	BOOL m_StartButtonRefresh;
	BOOL m_Won;									// 

	HFONT font,oldfont;
	HFONT tfont,toldfont;

	omokcore omcore;							// game core
	ORIGINWINDOW* window[WINDOWCOUNT];			// each window object
	cOmokSound sound;
	HDC btnhdc,backhdc,imagehdc;
	HBITMAP oldbit,oldbit1,oldbit2;
	POINT m_myPrevPos,m_comPrevPos;

/////////////////////////////////////////////////////////////
	static cOmokManage* This;					// in other to process a static function
	static BOOL DialogProc(HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam);	// main proc

	void SelectServer();
	void SelectClient();
	void InitDialog(HWND hWnd);					
	void SetWindowPos(HWND hWnd);				// picking up and resizing window
	LONG SelectMessage(WPARAM wParam,LPARAM lParam);	// process msg
	void Refreshdol(WORD x,WORD y);
	void SetChatText(WPARAM wParam,LPARAM lParam,USERINFOEX& user);
	void Sendomokmessage();
	void ResetGame();
	void GameStart();
	void RefreshButton();

	void EndGame(BOOL bflag);
	void DestroyGame();
	void OutGame();
	void MouseMove();
	void LButtonDown();
	void LButtonUp();
	void DrawDolFlag(HDC hdc);
	void DrawClock(HDC hdc);
	void UpdateInfo();
	void PrintStartText(HDC hdc);
	void PrintVictoryLost(BOOL victory);
	void CloseConnect();

	BOOL MsgExcute();		// excute buffer msg
	BOOL MsgProcess(LONG msg,LONG variable1,LONG variable2);	// process buffer msg
	HBRUSH SetChatBkColor(HWND hWnd,HDC hdc);

	void* getWindowpointer(WINDOWPOINTER windowindex){return window[windowindex];};	
};

