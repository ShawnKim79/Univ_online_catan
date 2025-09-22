#pragma once

#include <windows.h>

class __declspec(dllexport) ORIGINWINDOW{

public:
	HWND m_hWnd;                                                                  

	virtual void Destroy()=0;
	virtual void subclass() =0;
	virtual HWND Create(HWND parent,HINSTANCE hInst)=0;		
};