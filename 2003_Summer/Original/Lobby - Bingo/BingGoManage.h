
#include "cSocket.h"
#include "userinfo.h"
#include "cChatView.h"
#include "cNoticeView.h"
#include "cChatInput.h"
#include "BingGoCore.h"


#define WINDOWCOUNT		3

class __declspec(dllexport) BingGoManage{

public:
	BingGoManage();
	~BingGoManage();

	HWND CreateBingGo();		// 이게임 생성 된다.
	
	void SetParentHandle(HWND parent_hWnd) { called_hWnd = parent_hWnd;}; // 부모 윈도우를 call_hWnd 에 저장 한다.

	void SetMyInfo(USERINFO& user);				
	void SetcompetitorInfo(USERINFO& cuser);	
	
	void SetNotice(char* buff,BYTE* charformat);					// 공지를 전달 한다.
	HWND GetHandle(){return m_hWnd;};									// 나의 핸들을 전달 한다.

	HINSTANCE hInst;	
	
	HANDLE m_hfinish;
	HANDLE m_hThreadLoop;
	HANDLE m_hWait;
	

private:
// object
	BingGoCore	bg;
	cSocket		sk;

	HWND m_hWnd;
	HWND called_hWnd; // parent hWnd

	ORIGINWINDOW* window[WINDOWCOUNT];

// Image
	HDC bkdc;
	HDC BackDC;
	HDC bddc;
	HDC bdsdc;
	HDC startButtondc;
	HDC StartTextdc;
	HDC WinTextdc;
	HDC LostTextdc;

	HDC ebuttondc;
	HDC sbuttondc;

// game Controler
	BOOL EndApp;
	BOOL m_MyTurn;
	BOOL m_Played;
	BOOL m_Reserved;

	BOOL StartButtonDown;
	BOOL EButtonDown;
	
	BOOL m_bDrawStartText;
	BOOL m_bDrawWinText;
	BOOL m_bDrawLostText;
	BOOL m_bDrawSelected;

	BOOL m_Win;

	POINT MouseDownPos;
	HFONT font,oldfont;
	HPEN	hpen,oldpen;

	char* m_msg;
	int m_msglen;

	inline void SetBoard();
	void GameStart();
	void MsgProcess(LONG msg,LONG wParam,LONG lParam);
	void MsgExcute();

	void DrawBoard();
	void DrawInfo(HDC hdc);
	void DrawStartButton();
	void DrawStartText(DWORD& waittime);
	void DrawLine();
	void DrawEbutton(HDC hdc);
	void DrawSButton(HDC hdc);

	inline void DrawWinnLost(DWORD& waittime);
	inline void EraseUserInfo();
	void UpdateInfo();

	void EndGame();
	void OutGame();

	void SendMsg();
	void CloseConnect();

// game info
	BOOL m_bServer;
	void ClientToBoard(WORD x,WORD y,WORD& bx,WORD& by);

	USERINFOEX		m_myinfo;
	USERINFOEX		m_competitorinfo;			// 상대방의 정보

// Message Handler
	BOOL OnEraseBkGnd(HDC hdc);
	void OnDestroy();
	void OnLButtonUp(LONG wParam,LONG lParam);
	void OnLButtonDown(LONG wParam,LONG lParam);
	
	BOOL OnCommand(WORD nmsg,WORD id,LONG handle);

	HBRUSH OnCtlColorEdit(HWND hWnd,HDC hdc);
	LONG MsgSelect(WPARAM wParam,LPARAM lParam);

// Window Controler
	void OnRun();
	void OnIdle();

	void SetWindowsPosition();

	static DWORD WINAPI MsgLoop(LPVOID This);
	static BOOL __stdcall DialogProc(HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam);
	static DWORD WINAPI RenderLoop(LPVOID pThis);
// option
	static BingGoManage* This;

// child window Controler
	void DrawChatText(LONG wParam,LONG lParam,USERINFOEX& user); // wParam :buffer pointer lParam: buffer size
	
};