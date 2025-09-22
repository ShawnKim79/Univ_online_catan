#include "cChatinput.h"

WNDPROC cTalkField::originalProc = NULL;


cTalkField::cTalkField()
{
	
}

cTalkField::~cTalkField()
{
	
}

BOOL cTalkField::EditProc(HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam)
{
	if(msg == WM_KEYUP && wParam == VK_RETURN)
	{
		int textsize = SendMessage(hWnd,WM_GETTEXTLENGTH,0,0);
		if(textsize > 0)
		{
			char* textbuff = new char[textsize+1];
			ZeroMemory(textbuff,textsize+1);
			SendMessage(hWnd,WM_GETTEXT,textsize+1,(LONG)textbuff);
			SendMessage(GetParent(hWnd),WM_SHOWSTRING,(WPARAM)textbuff,textsize);
			SetWindowText(hWnd,"");
			delete textbuff;
		}
	}
	
	return CallWindowProc(cTalkField::originalProc,hWnd,msg,wParam,lParam);
}

HWND cTalkField::Create(HWND parent,HINSTANCE hInst)
{
	m_hWnd = CreateWindow("EDIT",NULL,WS_CHILD|WS_VISIBLE|ES_MULTILINE|ES_AUTOHSCROLL|ES_WANTRETURN|WS_CLIPSIBLINGS,0,0,0,0,parent,NULL,hInst,NULL);
	hbrush = CreateSolidBrush(RGB(233,210,122));
	font = CreateFont(12,0,0,0,0,0,0,0,HANGEUL_CHARSET,0,0,0,0,"±¼¸²Ã¼");
	SendMessage(m_hWnd,WM_SETFONT,(LONG)font,TRUE);
	return m_hWnd;
}

void cTalkField::Destroy()
{	
	DeleteObject(hbrush);
	DeleteObject(font);
	SetWindowLong(m_hWnd,GWL_WNDPROC,(LONG)originalProc);
	delete this;
}

void cTalkField::subclass()
{
	originalProc = (WNDPROC)SetWindowLong(m_hWnd,GWL_WNDPROC,(LONG)EditProc);
}
