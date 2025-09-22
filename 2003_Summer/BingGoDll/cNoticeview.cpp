#include "cNoticeview.h"
#include <richedit.h>

WNDPROC cNoticeview::originalProc = NULL;

cNoticeview::cNoticeview()
{

}

cNoticeview::~cNoticeview()
{

}

HWND cNoticeview::Create(HWND parent,HINSTANCE hInst)
{
	m_hWnd = CreateWindow(RICHEDIT_CLASS,NULL,WS_VSCROLL|WS_CHILD|WS_VISIBLE|ES_MULTILINE|ES_AUTOVSCROLL|WS_CLIPSIBLINGS|ES_READONLY,0,0,0,0,parent,NULL,hInst,NULL);
	
	return m_hWnd;
}

BOOL cNoticeview::NoticeProc(HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam)
{
	if(msg == WM_SETFOCUS)
	{
		return FALSE;
	}
	return CallWindowProc(originalProc,hWnd,msg,wParam,lParam);
}


void cNoticeview::subclass()
{
	originalProc = (WNDPROC)SetWindowLong(m_hWnd,GWL_WNDPROC,(LONG)NoticeProc);
}

void cNoticeview::Destroy()
{
	SetWindowLong(m_hWnd,GWL_WNDPROC,(LONG)originalProc);
	delete this;
}