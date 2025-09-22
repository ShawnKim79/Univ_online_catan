
#include <windows.h>
#include "richedit.h"
#include "originwindow.h"

class  __declspec(dllexport) cChatView : public ORIGINWINDOW
{
public:
	HMODULE hmod;

	cChatView();
	~cChatView();

	void Destroy();
	void subclass();
	HWND Create(HWND parent,HINSTANCE hInst);

private:

	static WNDPROC originalreProc;
	static BOOL Richedit(HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam);
};