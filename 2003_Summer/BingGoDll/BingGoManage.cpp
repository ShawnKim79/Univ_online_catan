#include "BingGoManage.h"
#include "Dibinfo.h"
#include "resource.h"
// #include "cConsole.h"
#include <mmsystem.h>


#pragma comment(lib,"Winmm")
#pragma warning(disable:4244)

BingGoManage* BingGoManage::This = NULL;

BingGoManage::BingGoManage()
{
	window[0] = (ORIGINWINDOW *) new cTalkField();
	window[1] = (ORIGINWINDOW *) new cChatView();
	window[2] = (ORIGINWINDOW *) new cNoticeview();
	
	m_msg = new char[RECVMAXSIZE];				// 65536
	
	m_msglen = 0;	
	m_curtime = LIMITETIME;
	
	This = this;
	EndApp = FALSE;
	m_bServer = FALSE;
	m_Played = FALSE;
	time = 0;
	lastpushed.x = -1;
	lastpushed.y = -1;
	m_Reserved = FALSE;
	
	StartButtonDown = FALSE;
	EButtonDown = FALSE;
	DrawGameDown = FALSE;
	GiveUpDown = FALSE;
	m_Pause = FALSE;
	m_giveup = FALSE;
	
	m_bDrawStartText = FALSE;
	m_bDrawWinText = FALSE;
	m_bDrawLostText = FALSE;
	m_bDrawSelected = FALSE;
	m_bDrawDrawText = FALSE;
	m_wait_hWnd = NULL;
		
	ZeroMemory(&m_myinfo,sizeof(USERINFOEX));
	ZeroMemory(&m_competitorinfo,sizeof(USERINFOEX));
	ZeroMemory(&m_justice,sizeof(USERINFOEX));

	m_myinfo.selfcolor = RGB(246,156,0);
	m_competitorinfo.selfcolor = RGB(81,134,222);

	char nick[] = "JUSTICE";
	m_justice.the_length_of_nickname = 7;
	m_justice.selfcolor = RGB(255,0,0);

	memcpy(m_justice.alias,nick,m_justice.the_length_of_nickname);
}

BingGoManage::~BingGoManage()
{
	for(int i = 0 ; i < WINDOWCOUNT;i++)
		window[i]->Destroy();
	
	CloseHandle(m_hWait);
	delete m_msg;
}


DWORD BingGoManage::MsgLoop(LPVOID This)
{
	BingGoManage* pThis = (BingGoManage*)This;
	
	MSG msg;
	BOOL done = 1;
	
	pThis->m_hWnd = CreateDialog(pThis->hInst,MAKEINTRESOURCE(IDD_DIALOG1),NULL,DialogProc);
	HWND hWnd = pThis->m_hWnd;
	pThis->sk.SetHandle(hWnd);
	
	SetEvent(pThis->m_hfinish);
	pThis->SetWindowsPosition();
	ShowWindow(pThis->m_hWnd,SW_SHOW);
	
	HACCEL hAccel = LoadAccelerators(pThis->hInst,MAKEINTRESOURCE(IDR_ACCELERATOR1));
	HANDLE ren = CreateThread(NULL,0,RenderLoop,0,0,0);
	
	
	while(GetMessage(&msg,NULL,0,0))
	{
	/*		if(PeekMessage(&msg,NULL,0,0,PM_REMOVE))
	{
	if(msg.message == WM_QUIT)
		break;*/
		
		if(!TranslateAccelerator(hWnd,hAccel,&msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		
		//}else
		//				pThis->OnIdle();
		
		//		pThis->OnRun();
	}
	
	pThis->OnDestroy();
	
	BOOL RE = CloseHandle(ren);
	return CloseHandle(pThis->m_hThreadLoop);
}

HWND BingGoManage::CreateBingGo()
{
	m_hWnd = NULL;
	m_hfinish = CreateEvent(NULL,FALSE,FALSE,"Finish game!!");
	font = CreateFont(13,0,0,0,0,0,0,0,DEFAULT_CHARSET,0,0,0,0,"굴림");
	tfont = CreateFont(20,0,0,0,FW_BOLD,0,0,0,DEFAULT_CHARSET,0,0,0,0,"굴림");
	hpen = CreatePen(PS_SOLID,PENWIDTH,RGB(255,0,0));
	
	m_hWait = CreateEvent(NULL,TRUE,FALSE,"Wait Event");
	
	m_hThreadLoop = CreateThread(NULL,0,MsgLoop,this,0,0);
	WaitForSingleObject(m_hfinish,INFINITE);
	CloseHandle(m_hfinish);
	
	return m_hWnd;
}

void BingGoManage::SetNotice(char *buff,BYTE* charformat)
{	
	HWND notice = window[2]->m_hWnd;
	
	LockWindowUpdate(notice);
	SendMessage(notice,EM_SETSEL,-1,-1);
	GETTEXTLENGTHEX gtl;
	gtl.flags = CP_ACP;
	gtl.codepage = GTL_NUMCHARS;
	int startx = SendMessage(notice,EM_GETTEXTLENGTHEX,(LONG)&gtl,0);
	SendMessage(notice,EM_REPLACESEL,TRUE,(LONG)buff);
	
	if(charformat != NULL)
	{
		int endx = SendMessage(notice,EM_GETTEXTLENGTHEX,(LONG)&gtl,0);
		SendMessage(notice,EM_SETSEL,startx-1,endx);
		SendMessage(notice,EM_SETCHARFORMAT,SCF_SELECTION,(LONG)charformat);		
	}
	
	LockWindowUpdate(NULL);
}

void BingGoManage::SetMyInfo(USERINFO& user)
{
	CopyMemory(&m_myinfo,&user,sizeof(USERINFO));	
	m_myinfo.the_length_of_nickname = strlen((char *)m_myinfo.alias);
	m_myinfo.the_length_of_id = strlen((char *)m_myinfo.id);
	/////////point	
	
	char buff[64] = {0,};
	
	wsprintf(buff,"%d",m_myinfo.score);
	m_myinfo.the_length_of_point = strlen(buff);
	
	if(m_myinfo.strpoint != NULL)
		delete []m_myinfo.strpoint;
	
	m_myinfo.strpoint = new char[m_myinfo.the_length_of_point];
	memcpy(m_myinfo.strpoint,buff,m_myinfo.the_length_of_point);
	
	//////////// info	
	wsprintf(buff,"%d/%d/%d",m_myinfo.victorycount,m_myinfo.drawcount,m_myinfo.losscount);
	m_myinfo.the_length_of_info = strlen(buff);
	
	if(m_myinfo.strinfo != NULL)
		delete []m_myinfo.strinfo;
	
	m_myinfo.strinfo = new char[m_myinfo.the_length_of_info];
	memcpy(m_myinfo.strinfo,buff,m_myinfo.the_length_of_info);

	m_myinfo.selfcolor = RGB(246,156,0);
}

void BingGoManage::SetcompetitorInfo(USERINFO& cuser)
{
	CopyMemory(&m_competitorinfo,&cuser,sizeof(USERINFO));
	m_competitorinfo.the_length_of_nickname = strlen((char *)m_competitorinfo.alias);
	m_competitorinfo.the_length_of_id = strlen((char *)m_competitorinfo.id);
	
	char buff[32] = {0,};
	wsprintf(buff,"%d",m_competitorinfo.score);
	m_competitorinfo.the_length_of_point = strlen(buff);
	
	if(m_competitorinfo.strpoint != NULL)
		delete []m_competitorinfo.strpoint;
	
	m_competitorinfo.strpoint = new char[m_competitorinfo.the_length_of_point];
	memcpy(m_competitorinfo.strpoint,buff,m_competitorinfo.the_length_of_point);
	
	wsprintf(buff,"%d/%d/%d",m_competitorinfo.victorycount,m_competitorinfo.drawcount,m_competitorinfo.losscount);
	m_competitorinfo.the_length_of_info = strlen(buff);
	
	if(m_competitorinfo.strinfo != NULL)
		delete []m_competitorinfo.strinfo;
	
	m_competitorinfo.strinfo = new char[m_competitorinfo.the_length_of_info];
	memcpy(m_competitorinfo.strinfo,buff,m_competitorinfo.the_length_of_info);

	m_competitorinfo.selfcolor = RGB(81,134,222);
}

void BingGoManage::SendMsg()
{
	
	if(m_bServer == TRUE && sk.m_bconnected == TRUE)
	{
		sk.sendData(sk.Clientsocket,m_msg,m_msglen);
	}
	else
	{
		sk.sendData(sk.m_socket,m_msg,m_msglen);
	}
}

// Window Controler
void BingGoManage::OnRun()
{
	static DWORD waittime = 0;
	HDC hdc = GetDC(m_hWnd);
	
	if(m_Played)
	{
		if(m_bDrawSelected)
		{
			if(waittime == 0)
			{
				sndPlaySound("./sound/put.wav",SND_ASYNC);
				SetBoard();
				waittime = timeGetTime();
			}
			
			if(timeGetTime() - waittime > 30)
			{
				if(bg.IsBinGo())
				{
//					m_bDrawWinText = TRUE;
					MsgProcess(GAMESTATUS,0,'V');
				}
				else
				{
					MsgProcess(GAMESTATUS,0,'N');
				//	MsgProcess(ENDTURN,0,0);
				}
				
				m_bDrawSelected = FALSE;
				waittime = 0;
			}
		}
		DrawBoard();	
		DrawLine();
		DrawTime(hdc);
		DrawClock(hdc);
	}
	else
	{
		if(m_bDrawStartText == TRUE)
		{
			DrawStartText(waittime);
		}
		else
		{
			SetBoard();
			DrawStartButton();
		}		
	}
	
	DrawWinnLost(waittime);
	DrawInfo(hdc);
	BitBlt(hdc,BOARD_POS_X,BOARD_POS_Y,BOARDWIDTH,BOARDHEIGHT,BackDC,0,0,SRCCOPY);
	DrawSButton(hdc);
	DrawEbutton(hdc);
	ReleaseDC(m_hWnd,hdc);
}

void BingGoManage::OnIdle()
{
	
}

BOOL BingGoManage::DialogProc(HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam)
{
	switch(msg)
	{
		//highest properties
	case WM_SOCKETMSG:
		return This->MsgSelect(wParam,lParam);
	case WM_LBUTTONDOWN:
		if(!This->m_Pause)
		{
			This->OnLButtonDown(wParam,lParam);
		}
		break;
	case WM_LBUTTONUP:
		if(!This->m_Pause)
		{
			This->OnLButtonUp(wParam,lParam);
		}
		break;
	case WM_COMMAND:
		return This->OnCommand(HIWORD(wParam),LOWORD(wParam),lParam);
	case WM_CTLCOLOREDIT:
		return (BOOL)This->OnCtlColorEdit((HWND)lParam,(HDC)wParam);
	case WM_ERASEBKGND:
		This->OnEraseBkGnd((HDC)wParam);
		return TRUE;
	case WM_SHOWSTRING:
		This->MsgProcess(CHATSTRING,wParam,lParam);
		break;	
		// Lowest Properties
	case WM_PAINT:
		break;
	case WM_SHOWWINDOW:
		if(wParam  == TRUE)
		{
			SetWindowText(This->window[1]->m_hWnd,"");
			SetWindowText(This->window[2]->m_hWnd,"");
			This->DrawChatText((LONG)"입장 하셨습니다 짝짝짝~",23,This->m_justice);
			SetForegroundWindow(hWnd);
			SetEvent(This->m_hWait);
		}
		else
		{
			if(wParam == FALSE)
			{
				ResetEvent(This->m_hWait);
				This->CloseConnect();
			}
		}
		break;
	case WM_INITDIALOG:
		break;
	case WM_CLOSE:
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	
	return 0;
}

void BingGoManage::SetWindowsPosition()
{
	int captionsize = GetSystemMetrics(SM_CYCAPTION);
	int yborder = GetSystemMetrics(SM_CYFIXEDFRAME);
	int xborder = GetSystemMetrics(SM_CXFIXEDFRAME);
	
	// resizing
	for(int i = 0 ; i < WINDOWCOUNT; i++)
	{
		window[i]->Create(m_hWnd,hInst);
		window[i]->subclass();
	}
	MoveWindow(m_hWnd,0,0,CLIENTWIDTH+xborder+xborder,CLIENTHEIGHT+captionsize+yborder+yborder,FALSE);
	
	MoveWindow(window[0]->m_hWnd,STARTX,EDITSTARTY,EDITWIDTH,EDITHEIGHT,FALSE);
	
	MoveWindow(window[1]->m_hWnd,STARTX,CHATVIEWSTARTY,CHATVIEWWIDTH,CHATVIEWHEIGHT,FALSE);
	
	MoveWindow(window[2]->m_hWnd,NOTICEPOSX,NOTICEPOSY,NOTICEWIDTH,NOTICEHEIGHT,FALSE);
	
	CDib dib;
	
	HDC hdc = GetDC(m_hWnd);
	dib.GetBitmapDC("./Image/Bingo.bmp",hdc,bkdc);
	dib.GetBitmapDC("./Image/digitnormal.bmp",hdc,bddc);
	dib.GetBitmapDC("./Image/digitselected.bmp",hdc,bdsdc);
	dib.GetBitmapDC("./Image/sbutton.bmp",hdc,sbuttondc);
	dib.GetBitmapDC("./Image/ebutton.bmp",hdc,ebuttondc);
	dib.GetBitmapDC("./Image/checker.bmp",hdc,checkerdc);
	dib.GetBitmapDC("./Image/draw.bmp",hdc,drawTextdc);
	
	HDC Tempdc;
	dib.GetBitmapDC("./Image/Buttons.bmp",hdc,Tempdc);
	//splite Image
	DWORD DestPosx = 0;
	DWORD TempDCHeight = STARTHEIGHT+STARTHEIGHT;
	
	startButtondc = CreateCompatibleDC(hdc);
	HBITMAP bitmap = CreateCompatibleBitmap(hdc,STARTWIDTH+STARTWIDTH,TempDCHeight);
	SelectObject(startButtondc,bitmap);
	BitBlt(startButtondc,0,0,STARTWIDTH+STARTWIDTH,TempDCHeight,Tempdc,DestPosx,0,SRCCOPY);
	DeleteObject(bitmap);
	
	DestPosx =  STARTWIDTH+STARTWIDTH;
	
	StartTextdc = CreateCompatibleDC(hdc);
	bitmap = CreateCompatibleBitmap(hdc,STARTTEXTWIDTH,TempDCHeight);
	SelectObject(StartTextdc,bitmap);
	BitBlt(StartTextdc,0,0,STARTTEXTWIDTH,TempDCHeight,Tempdc,DestPosx,0,SRCCOPY);
	DeleteObject(bitmap);
	
	DestPosx =  STARTWIDTH+STARTWIDTH+STARTTEXTWIDTH;
	
	WinTextdc = CreateCompatibleDC(hdc);
	bitmap = CreateCompatibleBitmap(hdc,WINANDLOSTWIDTH,TempDCHeight);
	SelectObject(WinTextdc,bitmap);
	BitBlt(WinTextdc,0,0,WINANDLOSTWIDTH,TempDCHeight,Tempdc,DestPosx,0,SRCCOPY);
	DeleteObject(bitmap);
	
	DestPosx =  STARTWIDTH+STARTWIDTH+STARTTEXTWIDTH+WINANDLOSTWIDTH;
	
	LostTextdc = CreateCompatibleDC(hdc);
	bitmap = CreateCompatibleBitmap(hdc,WINANDLOSTWIDTH,TempDCHeight);
	SelectObject(LostTextdc,bitmap);
	BitBlt(LostTextdc,0,0,WINANDLOSTWIDTH,TempDCHeight,Tempdc,DestPosx,0,SRCCOPY);
	DeleteObject(bitmap);
	
	DeleteDC(Tempdc);
	// 임시 버퍼
	BackDC = CreateCompatibleDC(hdc);
	bitmap = CreateCompatibleBitmap(hdc,BOARDWIDTH,BOARDHEIGHT);
	SelectObject(BackDC,bitmap);
	DeleteObject(bitmap);

	ReleaseDC(m_hWnd,hdc);	
}

void BingGoManage::MsgProcess(LONG msg,LONG wParam,LONG lParam)
{
	int* tmsg = (int *)m_msg;
	
	switch(msg)
	{
	case CHATSTRING:
		{
			*tmsg++ = m_msglen = 4+4+4+lParam;
			*tmsg++ = CHATSTRING;
			*tmsg++ = lParam;
			CopyMemory(tmsg,(char *)wParam,lParam);
			
			SendMsg();
			
			DrawChatText(wParam,lParam,m_myinfo);
		}
		break;
	case SELECTNUMBER:
		*tmsg++ = SELECTNUMBERSIZE;
		*tmsg++ = SELECTNUMBER;
		*tmsg++ = wParam;
		
		m_msglen = SELECTNUMBERSIZE;
		SendMsg();
		break;
	case GAMESTATUS:
		*tmsg++ = GAMESTATUSSIZE;
		*tmsg++ = GAMESTATUS;
		*tmsg++ = lParam;		// 'N','V'

		m_msglen = GAMESTATUSSIZE;
		SendMsg();
		break;
	case GAMERESULT:
		{
			
			*tmsg++ = GAMERESULTSIZE;
			*tmsg++ = GAMERESULT;
		//	cConsole::GetInstance()->Write("MsgProcess GAMRESULT %c \n",(char)lParam);
			
			char index=(char)lParam;
			BOOL victory = bg.IsBinGo();
			switch(index)
			{
			case 'N':
				if(victory)
				{
					// 승리 코드
					m_bDrawWinText = TRUE;
					*tmsg++ = (int)'V';
				}
				else
				{
					// 게임 진행 필요 없음..
					*tmsg++ = (int)'N';
					m_MyTurn = TRUE;
					m_curtime = LIMITETIME;
				//	cConsole::GetInstance()->Write("Normal End turn\n");
				}
				break;
			case 'V':
				if(victory)
				{
					// 비긴(draw) 코드
					m_bDrawDrawText = TRUE;
					*tmsg++ = (int)'D';
				}
				else
				{
					// 패배 코드 필요 없음
					m_bDrawLostText = TRUE;
					*tmsg++ = (int)'L';
				}
				break;
			default:
				*tmsg++ = 0;
			}
		//	cConsole::GetInstance()->Write("after GAMRESULT %c \n",(char)*(tmsg-1));
			m_msglen = GAMERESULTSIZE;
			SendMsg();
		}
		break;
		
	case STARTGAME:
		//packing
		*tmsg++ = STARTGAMESIZE;
		*tmsg++ = STARTGAME;
		
		m_msglen = STARTGAMESIZE;
		SendMsg();
		GameStart();
		break;
		
	case ENDTURN:
		{
			*tmsg++ = ENDTURNSIZE;
			*tmsg++ = ENDTURN;
			
			m_msglen = ENDTURNSIZE;
			
			SendMsg();
			
			m_MyTurn = FALSE;
			m_curtime = LIMITETIME;

		//	cConsole::GetInstance()->Write("Endturn\n");
		}
		break;

	case DRAWGAMEREQUEST:
		*tmsg++ = DRAWGAMESIZE;
		*tmsg++ = DRAWGAMEREQUEST; 
		m_msglen = DRAWGAMESIZE;
			
		switch(wParam)
		{
			case DRAWGAME_MSG_REQUEST:
				m_Pause = TRUE;
				*tmsg++ = DRAWGAME_MSG_REQUEST;
				DrawChatText((LONG)"비기기를 신청 하셨습니다. \r\n 응답을 기다리고 있습니다.",54,m_myinfo);
				SendMsg();
				break;
				
			case DRAWGAME_MSG_ACCEPT:
				*tmsg++ = DRAWGAME_MSG_ACCEPT;
				DrawChatText((LONG)"비기기를 신청을 허락하셨습니다.",31,m_myinfo);
				SendMsg();
				m_competitorinfo.drawcount++;
				m_myinfo.drawcount++;
				SendMessage(called_hWnd,WM_GAMERESULT,DRAWGAME,!m_Reserved);
		//		cConsole::GetInstance()->Write("SendMessage sever:%s %s %d \n","WM_GAMERESULT","DRAWGAME",!m_Reserved);
				// 신청 응답에 대한 사용자의 처리.
				m_Pause = FALSE; // 입력 막기.
				EndGame();
				break;
				
			case DRAWGAME_MSG_REFUSE:
				*tmsg++ = DRAWGAME_MSG_REFUSE;
				DrawChatText((LONG)"비기기를 신청을 거절하셨습니다.",31,m_myinfo);
				SendMsg();
				m_Pause = FALSE;
				break;
			}
			break;
	case GIVEUPNOTICE:
		*tmsg++ = GIVEUPSIZE;
		*tmsg++ = GIVEUPNOTICE;
		m_msglen = ENDGAMESIZE;
		SendMsg();
		
	/*	if(m_Reserved == TRUE)
		{
			ShowWindow(m_hWnd,SW_HIDE);
			m_Reserved = FALSE;
		}
		else
		{*/
			m_giveup = TRUE;
			m_bDrawLostText = TRUE;
			
	//	}
		break;

	case ENDGAME:
		{
//			*tmsg++ = ENDGAMESIZE;
//			*tmsg++ = ENDGAME;
//			m_msglen = ENDGAMESIZE;

			m_MyTurn = FALSE;
//			SendMsg();

			if(m_Reserved == TRUE)
			{
			//	m_bDrawWinText = TRUE;
				
			//	ShowWindow(m_hWnd,SW_HIDE);
			//	m_Reserved = FALSE;
				SendMessage(called_hWnd,WM_GAMERESULT,VICTORYGAME,OUTGAME);
			//	cConsole::GetInstance()->Write("SendMessage sever:%s %s %d \n","WM_GAMERESULT","VICTORYGAME",OUTGAME);
			
			}
		}
		break;
	}
}

void BingGoManage::MsgExcute()
{
	int* imsg = (int *)m_msg;
	for(int size = 0 ; size < m_msglen;)
	{
		int msgsize = *(int *)imsg++;
		int msg = *(int *)imsg++;
		
		switch(msg)
		{
		case CHATSTRING:
			{ 
				int buffsize = *(int *)imsg++;
				DrawChatText((LONG)imsg,buffsize,m_competitorinfo);
			}
			break;
			
		case STARTGAME:
			{
				GameStart();
			}
			break;
			
		case SELECTNUMBER:
			{
				int index = *(int *)imsg++;
				
				sndPlaySound("./sound/put.wav",SND_ASYNC);
				bg.SetBoardByIndex(index);
			}
			break;
		case GAMERESULT:
			{
				char index = (char)*imsg++;
			//	cConsole::GetInstance()->Write("MsgExecute %c\n",index);
				switch(index)
				{
				case 'V':
					m_bDrawLostText = TRUE;
				//	cConsole::GetInstance()->Write("e Lost game \n");
					break;
				case 'N':
					m_MyTurn = FALSE;
					m_curtime = LIMITETIME;
					break;
				case 'L':
					m_bDrawWinText = TRUE;
					break;
				case 'D':
					m_bDrawDrawText = TRUE;
					break;
				}
			}
			break;

		case GAMESTATUS:
			MsgProcess(GAMERESULT,0,*imsg++);
			break;

			case DRAWGAMEREQUEST:
			{
				int msg = *(int *)imsg++;
				switch(msg)
				{
				case DRAWGAME_MSG_REQUEST:
					m_Pause = TRUE;
					if(MessageBox(NULL,"비기기 신청이 들어 왔습니다 허락 하시겠습니까?","비기기 신청",MB_YESNO) == IDYES)
					{
						MsgProcess(DRAWGAMEREQUEST,DRAWGAME_MSG_ACCEPT,0);
					}
					else
					{
						MsgProcess(DRAWGAMEREQUEST,DRAWGAME_MSG_REFUSE,0);
					}
					break;
				case DRAWGAME_MSG_ACCEPT:
					DrawChatText((LONG)"비기기 신청을 수락하셨습니다.",29,m_competitorinfo);
					
					m_myinfo.drawcount++;
					m_competitorinfo.drawcount++;
					
					SendMessage(called_hWnd,WM_GAMERESULT,DRAWGAME,!m_Reserved);
				//	cConsole::GetInstance()->Write("SendMessage sever:%s %s %d \n","WM_GAMERESULT","DRAWGAME",!m_Reserved);
					m_Pause = FALSE;
					EndGame();
					break;

				case DRAWGAME_MSG_REFUSE:
					DrawChatText((LONG)"비기기 신청을 거절하셨습니다.",29,m_competitorinfo);
					m_Pause = FALSE;
					break;
				}
			}
			
			break;
		case GIVEUPNOTICE:
			{
			/*	if(m_Reserved == TRUE)
				{
					ShowWindow(m_hWnd,SW_HIDE);
					m_Reserved = FALSE;
				}
				else
				{*/
					m_bDrawWinText = TRUE;
			//	}
			}
			break;
			
		case ENDTURN:
			{
			//	cConsole::GetInstance()->Write("MsgExcute Endturn\n");
				m_MyTurn = TRUE;
				m_curtime = LIMITETIME;
				break;
			}
			
		case ENDGAME:

			m_bDrawLostText = TRUE;
			m_MyTurn = TRUE;

			if(m_Reserved == TRUE)
			{
				m_Reserved = FALSE;
			}
			
			break;
		}
		size += msgsize;
	}
	
	
}

void BingGoManage::DrawChatText(LONG wParam,LONG lParam,USERINFOEX& user) 
{
	
	HWND chWnd = window[1]->m_hWnd;
	LockWindowUpdate(chWnd);	
	int textbuffsize = 2+lParam + 1 + user.the_length_of_nickname+2;
	
	char* textbuff = new char[textbuffsize];
	ZeroMemory(textbuff,textbuffsize);
	
	char* tempbuff = textbuff;
	
	CopyMemory(tempbuff,user.alias,user.the_length_of_nickname);
	tempbuff += user.the_length_of_nickname;
	*tempbuff++ = '>';
	*tempbuff++ = 0x20;
	
	CopyMemory(tempbuff,(char *)wParam,lParam);
	tempbuff += lParam;
	
	memcpy(tempbuff,"\r\n",2);
	tempbuff +=2;
	
	SendMessage(chWnd,EM_SETSEL,-1,-1);

	GETTEXTLENGTHEX gtl;
	gtl.flags = CP_ACP;
	gtl.codepage = GTL_NUMCHARS;

	int startx = SendMessage(chWnd,EM_GETTEXTLENGTHEX,(LONG)&gtl,0);
	SendMessage(chWnd,EM_REPLACESEL,TRUE,(LONG)textbuff);

	int endx = SendMessage(chWnd,EM_GETTEXTLENGTHEX,(LONG)&gtl,0);

	CHARFORMAT charformat;

	ZeroMemory(&charformat,sizeof(CHARFORMAT));
	charformat.cbSize = sizeof(CHARFORMAT);
	charformat.crTextColor = user.selfcolor;
	charformat.dwMask = CFM_COLOR;
	
	SendMessage(chWnd,EM_SETSEL,startx-1,endx);
	SendMessage(chWnd,EM_SETCHARFORMAT,SCF_SELECTION,(LONG)&charformat);		

	SendMessage(chWnd,EM_LINESCROLL,0,1);
	LockWindowUpdate(NULL);
	
	delete textbuff;	
}

// Message Handler

BOOL BingGoManage::OnCommand(WORD nmsg,WORD id,LONG handle)
{
	switch(id)
	{
	case IDM_SERVERSELECT:
		m_bServer = TRUE;
		m_MyTurn = TRUE;
		CloseConnect();
	//	SetWindowText(m_hWnd,"Server");
		sk.readySocket();
		sk.serverAsyncSelect();
		sk.listenSocket();

		if(handle == WM_GAMREQUEST)
		{
			DrawWait();
		}
		break;
	case IDM_CLIENTSELECT:
		m_MyTurn = FALSE;
		m_bServer = FALSE;
		CloseConnect();
	//	SetWindowText(m_hWnd,"Client");
		sk.readySocket();
		sk.clientAsyncSelect();
		sk.connectSocket((char *)m_competitorinfo.ip);
		break;
	case IDM_STARTGAME:
		MsgProcess(STARTGAME,0,0);
		break;
	}
	return FALSE;
}

void BingGoManage::EndGame()
{
	EraseUserInfo();
	bg.ResetBoard();
	UpdateInfo();

	m_Played = FALSE;
	m_Pause = FALSE;
	SendMessage(called_hWnd,WM_GAMESTATUS,0,WM_ENDGAME);
//	cConsole::GetInstance()->Write("SendMessage sever:%s %s %d \n","WM_GAMESTATUS","L:WM_ENDGAME",!m_Reserved);
}

void BingGoManage::OutGame()
{
	for(int i = 0 ; i < WINDOWCOUNT ; i++)
		window[i]->Destroy();
}

void BingGoManage::OnLButtonDown(LONG wParam,LONG lParam)
{
	RECT rt;
	POINT pos;
	WORD bx = 0,by = 0;
	// Select Mark;
	SetCapture(m_hWnd);
	GetCursorPos(&pos);
	ScreenToClient(m_hWnd,&pos);
	SetRect(&rt,STARTX,STARTY,STARTX+BOARDWIDTH,STARTY+BOARDHEIGHT);
	
	if(m_Played== TRUE && PtInRect(&rt,pos))
	{
		ClientToBoard(pos.x,pos.y,bx,by);
		MouseDownPos = pos;		
		return ;
	}
	
	// START BUTTON
	if(m_Played == FALSE && sk.m_bconnected == TRUE)
	{
		SetRect(&rt,START_POS_X,START_POS_Y,START_POS_X+STARTWIDTH,START_POS_Y+STARTHEIGHT);
		
		if(PtInRect(&rt,pos))
		{
			StartButtonDown = TRUE;	
		}
	}
	
	// EXIT BUTTON	
	SetRect(&rt,EBUTTON_POS_X,EBUTTON_POS_Y,EBUTTON_POS_X+EBUTTONWIDTH,EBUTTON_POS_Y+EBUTTONHEIGHT);
	
	if(PtInRect(&rt,pos))
	{
		EButtonDown = TRUE;
	}
	
	// 게임이 시작되기 전에는 항상 disable 되어야 한다.
	if(m_Played == TRUE)
	{
		// DRAW BUTTON
		SetRect(&rt,DRAWBUTTON_POS_X,DRAWBUTTON_POS_Y,DRAWBUTTON_POS_X+SBUTTONWIDTH,DRAWBUTTON_POS_Y+SBUTTONHEIGHT);
		
		if(PtInRect(&rt,pos))
		{
			DrawGameDown = TRUE;
		}
		
		// Give UP BUTTON
		SetRect(&rt,GIVEUP_POS_X,GIVEUP_POS_Y,GIVEUP_POS_X+SBUTTONWIDTH,GIVEUP_POS_Y+SBUTTONHEIGHT);
		
		if(PtInRect(&rt,pos))
		{
			GiveUpDown = TRUE;
		}
	}
}

void BingGoManage::OnLButtonUp(LONG wParam,LONG lParam)
{
	RECT rt;
	POINT pos;
	
	// Select Mark;
	GetCursorPos(&pos);
	ScreenToClient(m_hWnd,&pos);
	
	if(m_Played)
	{
		if(m_MyTurn)
		{
			WORD bx = 0,by = 0;
			SetRect(&rt,STARTX,STARTY,STARTX+BOARDWIDTH,STARTY+BOARDHEIGHT);
			
			if(PtInRect(&rt,pos))
			{
				ClientToBoard(pos.x,pos.y,bx,by);
				if(MouseDownPos.x == pos.x && MouseDownPos.y == MouseDownPos.y)
				{
					// 게임 프로세서 시작 
					if(bg.SetPush(bx,by))
					{
						lastpushed.x = bx;
						lastpushed.y = by;
						bg.Ch_Board[by][bx] = BG_B_MYPUSH;
						LONG wParam = bg.Board[by][bx];
						
						MsgProcess(SELECTNUMBER,wParam,0);
						m_bDrawSelected = TRUE;
					}	
					else
					{
						// 이미 눌렸다는 표시를 해줌.
					}
				}
			}
		}
		
		// 	Draw button		
		SetRect(&rt,DRAWBUTTON_POS_X,DRAWBUTTON_POS_Y,DRAWBUTTON_POS_X+SBUTTONWIDTH,DRAWBUTTON_POS_Y+SBUTTONHEIGHT);
		if(PtInRect(&rt,pos))
		{
			// Draw를 요청 하는 코드
			MsgProcess(DRAWGAMEREQUEST,0,0);
		}
		
		//	Give up button
		SetRect(&rt,GIVEUP_POS_X,GIVEUP_POS_Y,GIVEUP_POS_X+SBUTTONWIDTH,GIVEUP_POS_Y+SBUTTONHEIGHT);
		if(PtInRect(&rt,pos))
		{
			//기권을 요청 하는 코드
			MsgProcess(GIVEUPNOTICE,0,0);
		}
		
		
	}
	// EXIT BUTTON
	SetRect(&rt,EBUTTON_POS_X,EBUTTON_POS_Y,EBUTTON_POS_X+EBUTTONWIDTH,EBUTTON_POS_Y+EBUTTONHEIGHT);
	
	if(PtInRect(&rt,pos))
	{
		if(m_Played == TRUE)
		{
			m_Reserved = !m_Reserved;

			if(m_Reserved == TRUE)
			{
				MsgProcess(CHATSTRING,(long)(char *)"나가기 예약입니다.",18);
			}
			else
			{
				MsgProcess(CHATSTRING,(long)(char *)"예약을 취소 하셨습니다.",23);	
			}
		}
		else
		{
			SendMessage(called_hWnd,WM_GAMERESULT,CORRECTEND,0); //lParam 사용 하지 않음
		//	cConsole::GetInstance()->Write("SendMessage sever:%s %s %d \n","WM_GAMERESULT","CORRECTEND",0);
			EndGame();
			ShowWindow(m_hWnd,SW_HIDE);
		}		
	}
	
	// START BUTTON
	if(m_Played == FALSE && sk.m_bconnected == TRUE)
	{
		SetRect(&rt,START_POS_X,START_POS_Y,START_POS_X+STARTWIDTH,START_POS_Y+STARTHEIGHT);
		if(PtInRect(&rt,pos) && StartButtonDown == TRUE)
		{
			SendMessage(m_hWnd,WM_COMMAND,IDM_STARTGAME,0);
		}
	}
	
	EButtonDown = FALSE;
	StartButtonDown = FALSE;
	DrawGameDown = FALSE;
	GiveUpDown = FALSE;
	
	ReleaseCapture();
}

BOOL BingGoManage::OnEraseBkGnd(HDC hdc)
{
	BitBlt(hdc,0,0,CLIENTWIDTH,CLIENTHEIGHT,bkdc,0,0,SRCCOPY);
	return FALSE;
}

HBRUSH BingGoManage::OnCtlColorEdit(HWND hWnd,HDC hdc)
{
	static BOOL setbk = FALSE;
	
	if(window[0]->m_hWnd == hWnd)
	{
		if(!setbk)
		{
			SetBkColor(hdc,RGB(233,210,122));
			
		}
		return ((cTalkField *)window[0])->hbrush;
	}
	return FALSE;
}

void BingGoManage::DrawBoard()
{
	int Value;
	int ch_Value;
	
	for(int i = 0 ; i < 5;i++)
	{
		for(int j = 0 ; j < 5;j++)
		{
			Value = bg.Board[i][j];
			
			BitBlt(BackDC,j*BOARDLENGTH,i*BOARDLENGTH,BOARDLENGTH,BOARDLENGTH,bddc,(Value/5)*BOARDLENGTH,(Value%5)*BOARDLENGTH,SRCCOPY);
			
			ch_Value = bg.Ch_Board[i][j];
			
			if(ch_Value != BG_B_NORMAL)
			{
				BitBlt(BackDC,j*BOARDLENGTH,i*BOARDLENGTH,BOARDLENGTH,BOARDLENGTH,bdsdc,(Value/5)*BOARDLENGTH,(Value%5)*BOARDLENGTH,SRCCOPY);

				if(lastpushed.x == j && lastpushed.y == i)
				{
					BitBlt(BackDC,j*BOARDLENGTH,i*BOARDLENGTH,BOARDLENGTH,BOARDLENGTH,checkerdc,BOARDLENGTH,BOARDLENGTH,SRCAND);
					BitBlt(BackDC,j*BOARDLENGTH,i*BOARDLENGTH,BOARDLENGTH,BOARDLENGTH,checkerdc,0,BOARDLENGTH,SRCPAINT);
				}
				else if(ch_Value == BG_B_MYPUSH)
				{
					BitBlt(BackDC,j*BOARDLENGTH,i*BOARDLENGTH,BOARDLENGTH,BOARDLENGTH,checkerdc,BOARDLENGTH,0,SRCAND);
					BitBlt(BackDC,j*BOARDLENGTH,i*BOARDLENGTH,BOARDLENGTH,BOARDLENGTH,checkerdc,0,0,SRCPAINT);
				}
			}
		}
	}
}

LONG BingGoManage::MsgSelect(WPARAM wParam,LPARAM lParam)
{
	switch(WSAGETSELECTEVENT(lParam))
	{
	case FD_CLOSE:
		{
			m_bServer = FALSE;
			CloseConnect();
			EndGame();
			SetWindowText(m_hWnd,"disconnected");
		}
		break;
		
	case FD_READ:
		{
			SOCKET recvsocket = (SOCKET)wParam;
			sk.receiveData(recvsocket,m_msg,m_msglen);
			MsgExcute();
		}
		break;
		
	case FD_ACCEPT:
		{
			if(sk.acceptSocket())
			{
				sndPlaySound("./sound/connect.wav",SND_ASYNC);
				if(m_wait_hWnd != NULL)
				{
					DestroyWindow(m_wait_hWnd);
					m_wait_hWnd = NULL;
				}
			}
		}
		break;
		
	case FD_CONNECT:
		{
			if(sk.m_bconnected == TRUE)
			{
				sndPlaySound("./sound/Entrance.wav",SND_ASYNC);
			}
		}
		break;
	}
	return 0;
}

void BingGoManage::CloseConnect()
{
//	cConsole::GetInstance()->Write("1. Close Connect %d\n",sk.m_bconnected);
	if(sk.m_bconnected)
	{
		if(m_bServer == TRUE)
		{
			sk.endSocket(sk.Clientsocket);
		}
		
		sk.endSocket();
	}
	else
	{
		sk.endSocket();
	}
		
//	cConsole::GetInstance()->Write("2. Close Connect %d\n",sk.m_bconnected);
}

void BingGoManage::GameStart()
{
	m_Pause = TRUE;
	SendMessage(called_hWnd,WM_GAMESTATUS,0,WM_STARTGAME);
//	cConsole::GetInstance()->Write("time : %d",m_curtime);
//	cConsole::GetInstance()->Write("SendMessage sever:%s %s %d \n","WM_GAMESTATUS","L:WM_STARTGAME",0);
	sndPlaySound("./sound/start.wav",SND_ASYNC);
	m_bDrawStartText = TRUE;
	m_Reserved = FALSE;
}

void BingGoManage::OnDestroy()
{
	DeleteObject(font);
	DeleteObject(hpen);
	DeleteObject(tfont);
	
	DeleteDC(drawTextdc);
	DeleteDC(checkerdc);
	DeleteDC(startButtondc);
	DeleteDC(WinTextdc);
	DeleteDC(StartTextdc);
	DeleteDC(BackDC);
	DeleteDC(bkdc);
	DeleteDC(bddc);
	DeleteDC(bdsdc);
	
}	

void BingGoManage::ClientToBoard(WORD x,WORD y,WORD& bx,WORD& by)
{
	bx = (x - STARTX)/BOARDLENGTH;
	by = (y - STARTY)/BOARDLENGTH;
}

DWORD BingGoManage::RenderLoop(LPVOID pThis)
{
	HANDLE htime  = CreateEvent(NULL,TRUE,TRUE,"Timer");
	MMRESULT mmrt = timeSetEvent(20,1,(LPTIMECALLBACK)htime,0,TIME_PERIODIC|TIME_CALLBACK_EVENT_SET);
	
	while(1)
	{
		WaitForSingleObject(This->m_hWait,INFINITE);
		
		if(WAIT_OBJECT_0 == WaitForSingleObject(htime,0))
		{
			This->OnRun();
			ResetEvent(htime);
		}
	}
	
	timeKillEvent(mmrt);
	CloseHandle(htime);
	
	return FALSE;
}

void BingGoManage::DrawInfo(HDC hdc)
{
	
	SetBkMode(hdc,TRANSPARENT);
	This->oldfont = (HFONT)SelectObject(hdc,font);
	
	TextOut(hdc,NICKNAMEX,MYNICKNAME_Y,(char *)m_myinfo.alias,m_myinfo.the_length_of_nickname);
	TextOut(hdc,FILEDBOX_X,MYID_Y,(char *)m_myinfo.id,m_myinfo.the_length_of_id);
	TextOut(hdc,FILEDBOX_X,MYPOINT_Y,m_myinfo.strpoint,m_myinfo.the_length_of_point);
	TextOut(hdc,FILEDBOX_X,MYINFO_Y,m_myinfo.strinfo,m_myinfo.the_length_of_info);
	
	if(sk.m_bconnected == TRUE)
	{
		TextOut(hdc,NICKNAMEX,COMNICKNAME_Y,(char *)This->m_competitorinfo.alias,This->m_competitorinfo.the_length_of_nickname);
		TextOut(hdc,FILEDBOX_X,COMID_Y,(char *)This->m_competitorinfo.id,This->m_competitorinfo.the_length_of_id);
		TextOut(hdc,FILEDBOX_X,COMPOINT_Y,This->m_competitorinfo.strpoint,This->m_competitorinfo.the_length_of_point);
		TextOut(hdc,FILEDBOX_X,COMINFO_Y,This->m_competitorinfo.strinfo,This->m_competitorinfo.the_length_of_info);
	}
	
	SelectObject(hdc,oldfont);
}

void BingGoManage::DrawStartButton()
{
	if(sk.m_bconnected == TRUE)
	{
		if(StartButtonDown == FALSE)
		{
			BitBlt(BackDC,START_POS_X,START_POS_Y,STARTWIDTH,STARTHEIGHT,startButtondc,0,STARTHEIGHT,SRCAND);
			BitBlt(BackDC,START_POS_X,START_POS_Y,STARTWIDTH,STARTHEIGHT,startButtondc,0,0,SRCPAINT);
		}
		else
		{
			BitBlt(BackDC,START_POS_X,START_POS_Y,STARTWIDTH,STARTHEIGHT,startButtondc,STARTWIDTH,STARTHEIGHT,SRCAND);
			BitBlt(BackDC,START_POS_X,START_POS_Y,STARTWIDTH,STARTHEIGHT,startButtondc,STARTWIDTH,0,SRCPAINT);
		}
	}
	
}

void BingGoManage::UpdateInfo()
{
	char buff[32]={0,};
	
	wsprintf(buff,"%d",m_competitorinfo.score);
	m_competitorinfo.the_length_of_point = strlen(buff);
	
	if(m_competitorinfo.strpoint != NULL)
		delete []m_competitorinfo.strpoint;
	
	m_competitorinfo.strpoint = new char[m_competitorinfo.the_length_of_point];
	memcpy(m_competitorinfo.strpoint,buff,m_competitorinfo.the_length_of_point);
	
	
	wsprintf(buff,"%d/%d/%d",m_competitorinfo.victorycount,m_competitorinfo.drawcount,m_competitorinfo.losscount);
	m_competitorinfo.the_length_of_info = strlen(buff);
	
	if(m_competitorinfo.strinfo != NULL)
		delete []m_competitorinfo.strinfo;
	
	m_competitorinfo.strinfo = new char[m_competitorinfo.the_length_of_info];
	memcpy(m_competitorinfo.strinfo,buff,m_competitorinfo.the_length_of_info);
	//////////////////////////// score
	wsprintf(buff,"%d",m_myinfo.score);
	m_myinfo.the_length_of_point = strlen(buff);
	
	if(m_myinfo.strpoint != NULL)
		delete []m_myinfo.strpoint;
	
	m_myinfo.strpoint = new char[m_myinfo.the_length_of_point];
	memcpy(m_myinfo.strpoint,buff,m_myinfo.the_length_of_point);
	
	
	wsprintf(buff,"%d/%d/%d",m_myinfo.victorycount,m_myinfo.drawcount,m_myinfo.losscount);
	m_myinfo.the_length_of_info = strlen(buff);
	
	if(m_myinfo.strinfo != NULL)
		delete []m_myinfo.strinfo;
	
	m_myinfo.strinfo = new char[m_myinfo.the_length_of_info];
	memcpy(m_myinfo.strinfo,buff,m_myinfo.the_length_of_info);
}

void BingGoManage::DrawStartText(DWORD& waittime)
{
	if(waittime == 0)
	{
		bg.ResetBoard();
		bg.SetBoard();
		DrawBoard();	
		waittime = timeGetTime();
	}
	
	if(timeGetTime() - waittime  > 1000)
	{
		m_Played = TRUE;
		time = 0;

		lastpushed.x = -1;
		lastpushed.y = -1;

		m_bDrawStartText = FALSE;
		m_Pause = FALSE;
		
		// 대기 타이머 초기화
		
		waittime = 0;
	//항상 10 초 부터 한다.
		m_curtime = LIMITETIME;
	}
	else
	{
		BitBlt(BackDC,STARTTEXT_POS_X,STARTTEXT_POS_Y,STARTTEXTWIDTH,STARTTEXTHEIGHT,StartTextdc,0,STARTHEIGHT,SRCAND);
		BitBlt(BackDC,STARTTEXT_POS_X,STARTTEXT_POS_Y,STARTTEXTWIDTH,STARTTEXTHEIGHT,StartTextdc,0,0,SRCPAINT);
	}
	
}

inline void BingGoManage::DrawWinnLost(DWORD& waittime)
{
	if(m_bDrawWinText == TRUE)
	{
		if(waittime == 0)
		{
			sndPlaySound("./sound/lostgame.wav",SND_ASYNC);
			m_myinfo.victorycount++;
			m_competitorinfo.losscount++;

			if(m_giveup == TRUE)
			{
				MsgProcess(ENDGAME,0,0);
				m_giveup = FALSE;
			}

			waittime = timeGetTime();
		}
		
		if(timeGetTime() - waittime > SHOWTIME)
		{

			SendMessage(called_hWnd,WM_GAMERESULT,VICTORYGAME,!m_Reserved);  // WM_GAMERESULT
		//	cConsole::GetInstance()->Write("SendMessage sever:%s %s %d \n","WM_GAMERESULT","VICTORYGAME",!m_Reserved);
			
			if(m_Reserved == TRUE)
			{
					ShowWindow(m_hWnd,SW_HIDE);
					m_Reserved = FALSE;
			}

			m_bDrawWinText = FALSE;
			waittime = 0;
			EndGame();
		}
		else
		{
			BitBlt(BackDC,WINANDLOST_POS_X,WINANDLOST_POS_Y,WINANDLOSTWIDTH,WINANDLOSTHEIGHT,WinTextdc,0,STARTHEIGHT,SRCAND);
			BitBlt(BackDC,WINANDLOST_POS_X,WINANDLOST_POS_Y,WINANDLOSTWIDTH,WINANDLOSTHEIGHT,WinTextdc,0,0,SRCPAINT);
		}
	}
	
	if(m_bDrawLostText == TRUE)
	{
		if(waittime == 0)
		{
			
			sndPlaySound("./sound/wingame.wav",SND_ASYNC);
			m_myinfo.losscount++;
			m_competitorinfo.victorycount++;

			waittime = timeGetTime();
		}
		
		if(timeGetTime() - waittime > SHOWTIME)
		{
			SendMessage(called_hWnd,WM_GAMERESULT,LOSEGAME,!m_Reserved);  // WM_GAMERESULT
		//	cConsole::GetInstance()->Write("SendMessage sever:%s %s %d \n","WM_GAMERESULT","LOSEGAME",!m_Reserved);
			
			if(m_Reserved == TRUE)
			{
				ShowWindow(m_hWnd,SW_HIDE);
				m_Reserved = FALSE;
			}

			m_bDrawLostText = FALSE;
			waittime = 0;
			EndGame();
		}
		else
		{
			BitBlt(BackDC,WINANDLOST_POS_X,WINANDLOST_POS_Y,WINANDLOSTWIDTH,WINANDLOSTHEIGHT,LostTextdc,0,STARTHEIGHT,SRCAND);
			BitBlt(BackDC,WINANDLOST_POS_X,WINANDLOST_POS_Y,WINANDLOSTWIDTH,WINANDLOSTHEIGHT,LostTextdc,0,0,SRCPAINT);
		}	
	}

	if(m_bDrawDrawText == TRUE)
	{
		if(waittime == 0)
		{
			m_myinfo.drawcount++;
			m_competitorinfo.drawcount++;

			waittime = timeGetTime();
		}

		if(timeGetTime() - waittime > SHOWTIME)
		{
			SendMessage(called_hWnd,WM_GAMERESULT,DRAWGAME,!m_Reserved);
			if(m_Reserved == TRUE)
			{
				ShowWindow(m_hWnd,SW_HIDE);
				m_Reserved = FALSE;
			}

			m_bDrawDrawText = FALSE;
			waittime = 0;
			EndGame();
		}
		else
		{
			BitBlt(BackDC,WINANDLOST_POS_X,WINANDLOST_POS_Y,WINANDLOSTWIDTH,WINANDLOSTHEIGHT,drawTextdc,0,WINANDLOSTHEIGHT,SRCAND);
			BitBlt(BackDC,WINANDLOST_POS_X,WINANDLOST_POS_Y,WINANDLOSTWIDTH,WINANDLOSTHEIGHT,drawTextdc,0,0,SRCPAINT);
			// bitblt
		}
	}
}

inline void BingGoManage::SetBoard()
{
	BitBlt(BackDC,0,0,BOARDWIDTH,BOARDHEIGHT,bddc,0,0,SRCCOPY);
}

inline void BingGoManage::EraseUserInfo()
{
	RECT rt;
	SetRect(&rt,400,10,550,300);
	InvalidateRect(m_hWnd,&rt,TRUE);
}

void BingGoManage::DrawLine()
{
	int startx=0,starty=0;
	int endx=0,endy=0;
	
	int index_x,index_y;
	oldpen = (HPEN)SelectObject(BackDC,hpen);
	
	for(int i = 0 ; i < 5;i++)
	{
		index_x = bg.fiveline[i].x;
		index_y = bg.fiveline[i].y;
		
		if(index_x != 0 || index_y != 0)
		{
			endy = endx = starty = startx= BOARDLENGTH/2;
			
			startx += bg.fiveline[i].x%5*BOARDLENGTH;
			starty += bg.fiveline[i].x/5*BOARDLENGTH;
			
			endx += bg.fiveline[i].y%5*BOARDLENGTH;
			endy += bg.fiveline[i].y/5*BOARDLENGTH;
			
			
			MoveToEx(BackDC,startx,starty,NULL);
			LineTo(BackDC,endx,endy);
			
		}
	}
	SelectObject(BackDC,oldpen);
}

void BingGoManage::DrawEbutton(HDC hdc)
{
	RECT rt;
	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(m_hWnd,&pt);
	SetRect(&rt,EBUTTON_POS_X,EBUTTON_POS_Y,EBUTTON_POS_X+EBUTTONWIDTH,EBUTTON_POS_Y+EBUTTONHEIGHT);
	
	if(m_Reserved == TRUE)
	{
		BitBlt(hdc,EBUTTON_POS_X,EBUTTON_POS_Y,EBUTTONWIDTH,EBUTTONHEIGHT,ebuttondc,EBUTTONWIDTH+EBUTTONWIDTH,0,SRCCOPY);
	}
	else
	{
		if(PtInRect(&rt,pt))
		{
			if(EButtonDown == FALSE)
			{
				BitBlt(hdc,EBUTTON_POS_X,EBUTTON_POS_Y,EBUTTONWIDTH,EBUTTONHEIGHT,ebuttondc,EBUTTONWIDTH,0,SRCCOPY);
			}
		}
		else
		{
			if(EButtonDown == FALSE)
			{
				BitBlt(hdc,EBUTTON_POS_X,EBUTTON_POS_Y,EBUTTONWIDTH,EBUTTONHEIGHT,ebuttondc,0,0,SRCCOPY);
			}
		}
		
	}
	
}

void BingGoManage::DrawSButton(HDC hdc)
{
	if(m_Played == TRUE)
	{
		RECT rt;
		POINT pos;
		GetCursorPos(&pos);
		ScreenToClient(m_hWnd,&pos);
		
		SetRect(&rt,DRAWBUTTON_POS_X,DRAWBUTTON_POS_Y,DRAWBUTTON_POS_X+SBUTTONWIDTH,DRAWBUTTON_POS_Y+SBUTTONHEIGHT);
		
		if(PtInRect(&rt,pos))
		{
			if(DrawGameDown == TRUE)
			{
				BitBlt(hdc,DRAWBUTTON_POS_X,DRAWBUTTON_POS_Y,SBUTTONWIDTH,SBUTTONHEIGHT,sbuttondc,0,SBUTTONHEIGHT+SBUTTONHEIGHT,SRCCOPY);	
				
				
			}
			else
			{
				BitBlt(hdc,DRAWBUTTON_POS_X,DRAWBUTTON_POS_Y,SBUTTONWIDTH,SBUTTONHEIGHT,sbuttondc,0,SBUTTONHEIGHT,SRCCOPY);	
			}
		}
		else
		{
			if(DrawGameDown == TRUE)
			{
				BitBlt(hdc,DRAWBUTTON_POS_X,DRAWBUTTON_POS_Y,SBUTTONWIDTH,SBUTTONHEIGHT,sbuttondc,0,SBUTTONHEIGHT+SBUTTONHEIGHT,SRCCOPY);	
			}
			else
			{
				BitBlt(hdc,DRAWBUTTON_POS_X,DRAWBUTTON_POS_Y,SBUTTONWIDTH,SBUTTONHEIGHT,sbuttondc,0,0,SRCCOPY);	
			}
		}
		
		// Give UP BUTTON
		SetRect(&rt,GIVEUP_POS_X,GIVEUP_POS_Y,GIVEUP_POS_X+SBUTTONWIDTH,GIVEUP_POS_Y+SBUTTONHEIGHT);
		
		if(PtInRect(&rt,pos))
		{
			if(GiveUpDown == TRUE)
			{
				BitBlt(hdc,GIVEUP_POS_X,GIVEUP_POS_Y,SBUTTONWIDTH,SBUTTONHEIGHT,sbuttondc,SBUTTONWIDTH,SBUTTONHEIGHT+SBUTTONHEIGHT,SRCCOPY);	
			}
			else
			{
				BitBlt(hdc,GIVEUP_POS_X,GIVEUP_POS_Y,SBUTTONWIDTH,SBUTTONHEIGHT,sbuttondc,SBUTTONWIDTH,SBUTTONHEIGHT,SRCCOPY);	
			}
		}
		else
		{
			if(GiveUpDown == TRUE)
			{
				BitBlt(hdc,GIVEUP_POS_X,GIVEUP_POS_Y,SBUTTONWIDTH,SBUTTONHEIGHT,sbuttondc,SBUTTONWIDTH,SBUTTONHEIGHT+SBUTTONHEIGHT,SRCCOPY);	
			}
			else
			{
				BitBlt(hdc,GIVEUP_POS_X,GIVEUP_POS_Y,SBUTTONWIDTH,SBUTTONHEIGHT,sbuttondc,SBUTTONWIDTH,0,SRCCOPY);
			}
		}
		
	}
	else
	{
		// disalble
		BitBlt(hdc,DRAWBUTTON_POS_X,DRAWBUTTON_POS_Y,SBUTTONWIDTH,SBUTTONHEIGHT,sbuttondc,0,SBUTTONHEIGHT*3,SRCCOPY);
		BitBlt(hdc,GIVEUP_POS_X,GIVEUP_POS_Y,SBUTTONWIDTH,SBUTTONHEIGHT,sbuttondc,SBUTTONWIDTH,SBUTTONHEIGHT*3,SRCCOPY);
	}
	
}

void BingGoManage::DrawTime(HDC hdc)
{
	static DWORD curtime = 0;
	RECT rt;
	SetBkMode(hdc,TRANSPARENT);

	if(m_curtime < 0)
	{
		if(m_MyTurn == TRUE)
			MsgProcess(ENDTURN,0,0);
		return ;
	}

	if(m_curtime == LIMITETIME)
	{
		SetRect(&rt,TIMETEXT_POS_X,!This->m_MyTurn ? MYTIMETEXT_POS_Y : COMTIMETEXT_POS_Y,TIMETEXT_POS_X+TIMETEXTWIDTH,(!This->m_MyTurn ? MYTIMETEXT_POS_Y : COMTIMETEXT_POS_Y)+TIMETEXTHEIGHT);			
		InvalidateRect(m_hWnd,&rt,TRUE);
	}
	
	if(timeGetTime() - curtime > 1000)
	{

		// 각각의 초기화를 위해 MsgProcess와 Msgexecute 및 버튼이 눌릴때 각각 초기화를 시켜 준다.
		
		char text[4];
		wsprintf(text,"%d\0",m_curtime);
	
		SetRect(&rt,TIMETEXT_POS_X,This->m_MyTurn ? MYTIMETEXT_POS_Y : COMTIMETEXT_POS_Y,TIMETEXT_POS_X+TIMETEXTWIDTH,(This->m_MyTurn ? MYTIMETEXT_POS_Y : COMTIMETEXT_POS_Y)+TIMETEXTHEIGHT);
		InvalidateRect(m_hWnd,&rt,TRUE);
		oldfont = (HFONT)SelectObject(hdc,tfont);
		DrawText(hdc,text,2,&rt,DT_VCENTER|DT_SINGLELINE|DT_CENTER );
		SelectObject(hdc,oldfont);
		m_curtime--;	

		if(time == 3)
		{
			time = 0;
		}
		else
		{
			time++;
		}

		curtime = timeGetTime();
	}
}

void BingGoManage::DrawClock(HDC hdc)
{

	int x,y;
	RECT rt;

	x = CLOCK_POS_X;
	
	if(m_MyTurn == TRUE)
	{
		y = MYCLOCK_POS_Y;
		
	}
	else
	{
		y = COMCLOCK_POS_Y;
	}
	
	SetRect(&rt,x,MYCLOCK_POS_Y,x+CLOCKWIDTH,MYCLOCK_POS_Y+CLOCKHEIGHT);
	InvalidateRect(m_hWnd,&rt,TRUE);

	SetRect(&rt,x,COMCLOCK_POS_Y,x+CLOCKWIDTH,COMCLOCK_POS_Y+CLOCKHEIGHT);
	InvalidateRect(m_hWnd,&rt,TRUE);

	BitBlt(hdc,x,y,CLOCKWIDTH,CLOCKHEIGHT,checkerdc,0+CLOCKWIDTH*time,140,SRCCOPY);	
}

void BingGoManage::DrawWait()
{
	m_wait_hWnd = CreateDialog(hInst,MAKEINTRESOURCE(IDD_DIALOG2),m_hWnd,WaitDlg);
	ShowWindow(m_wait_hWnd,SW_SHOW);
}

BOOL BingGoManage::WaitDlg(HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam)
{
	static int time = 0;
	PAINTSTRUCT ps;
	HDC hdc;
	RECT rt;
	static char buff[30] = {0,};
	static int buffsize = 0;
	switch(msg)
	{
	case WM_INITDIALOG:
		hdc = GetDC(hWnd);
		SetBkMode(hdc,TRANSPARENT);
		GetClientRect(hWnd,&rt);
		time = 15;
		wsprintf(buff,"%s %d","응답을 기다립니다.",time);
		buffsize = strlen(buff);
		DrawText(hdc,buff,buffsize,&rt,DT_CENTER|DT_VCENTER);
		ReleaseDC(hWnd,hdc);
		SetTimer(hWnd,300,1000,NULL);
		break;
	case WM_COMMAND:
		{
			WORD id  = LOWORD(wParam);
			if(id == ID_CANCEL_DLG)
			{
					SendMessage(This->called_hWnd,WM_GAMERESULT,CORRECTEND,OUTGAME);
					This->EndGame();
					ShowWindow(This->m_hWnd,SW_HIDE);
					DestroyWindow(hWnd);
					This->m_wait_hWnd = NULL;
			}
		}
		break;
	case WM_TIMER:
		time--;
		if(time < 0)
		{
			KillTimer(hWnd,300);
			SendMessage(This->called_hWnd,WM_GAMERESULT,CORRECTEND,OUTGAME);
			This->EndGame();
			ShowWindow(This->m_hWnd,SW_HIDE);
			DestroyWindow(hWnd);
			This->m_wait_hWnd = NULL;
		}
		wsprintf(buff,"%s %d","응답을 기다립니다.",time);
		buffsize = strlen(buff);
		InvalidateRect(hWnd,NULL,TRUE);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd,&ps);
		SetBkMode(hdc,TRANSPARENT);
		GetClientRect(hWnd,&rt);
		DrawText(hdc,buff,buffsize,&rt,DT_CENTER|DT_VCENTER);
		EndPaint(hWnd,&ps);
		break;
	}
	return 0;
}