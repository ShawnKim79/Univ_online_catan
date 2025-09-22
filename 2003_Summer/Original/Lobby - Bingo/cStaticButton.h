#include "ORIGINWINDOW.h"

class __declspec(dllexport) cStaticButton : public ORIGINWINDOW
{
public:
	void Destroy();
	void subclass();
	HWND Create(HWND parent,HINSTANCE hInst);		
};