#include "originwindow.h"

class  __declspec(dllexport) cNoticeview : public ORIGINWINDOW
{
public:
	cNoticeview();
	~cNoticeview();
public:
	void Destroy();		
	void subclass();														
	HWND Create(HWND parent,HINSTANCE hInst);		
private:
	static WNDPROC originalProc;
	static BOOL NoticeProc(HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam);

};