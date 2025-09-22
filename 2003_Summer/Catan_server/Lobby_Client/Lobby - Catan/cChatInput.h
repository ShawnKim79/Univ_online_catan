#include "originwindow.h"

#define WM_SHOWSTRING WM_USER+11

class __declspec(dllexport) cTalkField : public ORIGINWINDOW
{
public:
	cTalkField();
	~cTalkField();

	void Destroy();
	void subclass();
	HWND Create(HWND parent,HINSTANCE hInst);
	HFONT font;
	HBRUSH	hbrush;

private:
	static WNDPROC originalProc;
	static BOOL EditProc(HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam);

};