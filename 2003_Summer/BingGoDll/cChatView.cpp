#include "cChatView.h"
#pragma warning(disable:4273)

WNDPROC cChatView::originalreProc = NULL;

cChatView::cChatView()
{
	hmod = LoadLibrary("Riched20.dll");
}

cChatView::~cChatView()
{
	FreeLibrary(hmod);
}

BOOL cChatView::Richedit(HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam)
{
	if(msg == WM_SETFOCUS)
	{
		return FALSE;
	}

	return CallWindowProc(originalreProc,hWnd,msg,wParam,lParam);
}

HWND cChatView::Create(HWND parent,HINSTANCE hInst)
{
	m_hWnd = CreateWindow(RICHEDIT_CLASS,NULL,WS_CHILD|WS_VISIBLE|ES_MULTILINE|ES_AUTOVSCROLL|ES_READONLY|WS_VSCROLL|WS_CLIPSIBLINGS,0,0,0,0,parent,NULL,hInst,NULL);
	CHARFORMAT cf={0,};
	cf.cbSize = sizeof(CHARFORMAT);
	SendMessage(m_hWnd,EM_GETCHARFORMAT,SCF_DEFAULT,(LONG)&cf);

	ZeroMemory(&cf,sizeof(CHARFORMAT));
	cf.cbSize = sizeof(CHARFORMAT);
	cf.dwMask = CFM_COLOR|CFM_FACE|CFM_SIZE;
	cf.yHeight = 180;
	cf.crTextColor = RGB(255,0,0);
	memcpy(cf.szFaceName,"Äð¸²Ã¼",6);

	SendMessage(m_hWnd,EM_SETCHARFORMAT,SCF_ALL,(LONG)&cf);
	SendMessage(m_hWnd,EM_SETBKGNDCOLOR,0,(LONG)RGB(204,224,125));
	return m_hWnd;
}

void cChatView::Destroy()
{
	SetWindowLong(m_hWnd,GWL_WNDPROC,(LONG)originalreProc);
	delete this;
}

void cChatView::subclass()
{
	originalreProc = (WNDPROC)SetWindowLong(m_hWnd,GWL_WNDPROC,(LONG)Richedit);	
}
