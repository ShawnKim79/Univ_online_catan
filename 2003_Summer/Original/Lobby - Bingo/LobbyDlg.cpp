// LobbyDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Lobby.h"
#include "LobbyDlg.h"
#include "process.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define REFRESH_INTERVAL	1000

int m_nCurRecv = ANY;

extern "C" __declspec(dllimport) BingGoManage *getGameObject();
//unsigned __stdcall threadSocket(LPVOID arg);

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLobbyDlg dialog

CLobbyDlg::CLobbyDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLobbyDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLobbyDlg)
	m_pChannelDlg = NULL;
	m_pGameRoomDlg = NULL;
	m_pProgressDlg = NULL;
	m_pChatDlg = NULL;
	m_nCurPage = PAGE_CHANNEL;
	m_strID = _T("");
	m_strIP = _T("");
	m_nCurRecv = ANY;
	m_pReadBuff = NULL;
	m_pBingGo = NULL;
	m_hBingGoWnd = NULL;
	m_bOwner = FALSE;
	m_bReadyGame = FALSE;
	m_bExitRoom = FALSE;
	m_bOfferGame = FALSE;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	//memset(m_pReadBuff, 1, sizeof(m_pReadBuff));
	m_RecvInfo.nRead = 0;
}

void CLobbyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLobbyDlg)
	DDX_Control(pDX, IDC_MINIMIZE, m_ctrlMinimize);
	DDX_Control(pDX, IDC_CLOSE, m_ctrlClose);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CLobbyDlg, CDialog)
	//{{AFX_MSG_MAP(CLobbyDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_NCHITTEST()
	ON_BN_CLICKED(IDC_CLOSE, OnClose)
	ON_BN_CLICKED(IDC_MINIMIZE, OnMinimize)
	ON_WM_MOVE()
	ON_WM_TIMER()
	ON_WM_SETCURSOR()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_SOCKETMESSAGE, OnSocketMessage)
	ON_MESSAGE(WM_GAMERESULT, OnGameResult)
	ON_MESSAGE(WM_GAMESTATE, OnGameState)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLobbyDlg message handlers

BOOL CLobbyDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	SetWindowText("HGR Bingo...");
	ModifyStyle(0, WS_SYSMENU,0);

	m_ctrlMinimize.SetImages(IDB_MININOR, IDB_MINICLK, IDB_MINIOVER, IDB_MININOR);
	m_ctrlClose.SetImages(IDB_CLOSENOR, IDB_CLOSECLK, IDB_CLOSEOVER, IDB_CLOSENOR);

	// 채널 선택 및 게임 로비 페이지 생성
	m_pChannelDlg = new CChannelDlg(this);
	m_pGameRoomDlg = new CGameRoomDlg(this);
	m_pProgressDlg = new CProgressDlg();
	
	m_pChannelDlg->Create(IDD_CHANNEL_DIALOG, this);
	m_pGameRoomDlg->Create(IDD_GAMEROOM_DIALOG, this);
	m_pProgressDlg->Create(IDD_PROGRESS_DIALOG, this);

	if(!m_ClientSocket.InitSock())
	{
		AfxMessageBox("소켓 초기화 실패");
		RemovePages();
		PostQuitMessage(0);
		return FALSE;
	}

	ConnectToChannelServer();

	m_pChannelDlg->ShowWindow(SW_SHOW); // 처음에는 채널 선택 페이지 보임

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CLobbyDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CLobbyDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CLobbyDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}


UINT CLobbyDlg::OnNcHitTest(CPoint point)
{
	CRect rcCaption;
	CWnd *pWnd = GetDlgItem(IDC_CAPTION);

	UINT nHitTest = CDialog::OnNcHitTest(point);

	pWnd->GetClientRect(&rcCaption);
	pWnd->ClientToScreen(&rcCaption);
	ScreenToClient(&rcCaption);

	ScreenToClient(&point);

	if(rcCaption.PtInRect(point)) return HTCAPTION;
	return nHitTest;
}




void CLobbyDlg::OnClose() 
{
	Exit();
}




void CLobbyDlg::OnMinimize() 
{
	WINDOWPLACEMENT wp;

	GetWindowPlacement(&wp);
	wp.showCmd = SW_MINIMIZE;
	SetWindowPlacement(&wp);
}




void CLobbyDlg::OnMove(int x, int y) 
{
	CDialog::OnMove(x, y);

	CRect rect;

	GetClientRect(&rect);

	if(m_ctrlMinimize.GetSafeHwnd()) m_ctrlMinimize.SetWindowPos(NULL, rect.Width() - 38, 5, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
	if(m_ctrlClose.GetSafeHwnd()) m_ctrlClose.SetWindowPos(NULL, rect.Width() - 20, 5, 0, 0, SWP_NOZORDER | SWP_NOSIZE);

	CWnd *pWnd = GetDlgItem(IDC_PAGEHOLDER);

	CRect rcHolder;
	
	if(IsWindow(pWnd->GetSafeHwnd()))
	{
		pWnd->GetClientRect(&rcHolder);
		pWnd->ClientToScreen(&rcHolder);
		ScreenToClient(&rcHolder);
		
		switch(m_nCurPage)
		{
		case PAGE_CHANNEL :
			
			if(m_pChannelDlg != NULL) 
			{
				m_pChannelDlg->SetWindowPos(NULL, rcHolder.left, rcHolder.top, rcHolder.Width(), rcHolder.Height(), SWP_NOZORDER);
			}
			
			break;
			
		case PAGE_GAMEROOM :
			
			if(m_pGameRoomDlg != NULL) 
			{
				m_pGameRoomDlg->SetWindowPos(NULL, rcHolder.left, rcHolder.top, rcHolder.Width(), rcHolder.Height(), SWP_NOZORDER);
			}
			
			break;
		}
	}
}




void CLobbyDlg::RemovePages()
{
	m_pChannelDlg->DestroyWindow();
	m_pGameRoomDlg->DestroyWindow();
	m_pProgressDlg->DestroyWindow();
	
	delete m_pChannelDlg;
	delete m_pGameRoomDlg;
	delete m_pProgressDlg;
	
	m_pChannelDlg = NULL;
	m_pGameRoomDlg = NULL;
	m_pProgressDlg = NULL;
}




BOOL CLobbyDlg::PreTranslateMessage(MSG* pMsg) 
{
	if(pMsg->message == WM_KEYDOWN)
	{
		switch(pMsg->wParam)
		{
			case VK_RETURN : 

				if(m_nCurPage == PAGE_CHANNEL) m_pChannelDlg->EnterSelectedChannel();
				else m_pGameRoomDlg->PreTranslateMessage(pMsg);

				return TRUE;
				
			case VK_ESCAPE : return TRUE;
		}
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}


BOOL CLobbyDlg::ConnectToChannelServer()
{
	m_ClientSocket.Disconnect();
	
	if(!m_ClientSocket.Connect("218.53.191.241", 11001))
	{
		AfxMessageBox("채널 서버 접속 실패!!\n잠시 후 다시 시도해 주세요.");
		PostQuitMessage(0);

		return FALSE;
	}

	WSAAsyncSelect(m_ClientSocket.m_Socket, GetSafeHwnd(), WM_SOCKETMESSAGE, FD_READ | FD_CLOSE);

	return TRUE;
}


void CLobbyDlg::OnSocketMessage(WPARAM wParam, LPARAM lParam)
{
	SOCKET socket = (SOCKET)wParam;

	CMessage Recv;

	static LPBYTE readbuff = NULL;

	static bReadAllPacket = FALSE;

	static int nRead1 = 0;
	static int nRead2 = 0;

	CString str;

	static COMMON_PACKET cmnpck;

	DWORD dwError;
	DWORD ByteToRead;
	
	if(WSAGETSELECTEVENT(lParam) == FD_READ)
	{
		ioctlsocket(socket, FIONREAD, &ByteToRead);

		if(!ByteToRead) return;

		if(bReadAllPacket == FALSE)
		{
			memset(&cmnpck, 0, sizeof(cmnpck));

			nRead2 = recv(socket, (char *)&cmnpck, sizeof(cmnpck), 0);
			
			if(nRead2 < (int)sizeof(cmnpck))
			{
				if(nRead2 < 0)
				{
					nRead2 = 0;
					nRead1 = recv(socket, (char *)&cmnpck, sizeof(cmnpck), 0);
					
					if(nRead1 > 0)
						nRead2 += nRead1;
				}
				
				else
				{
					nRead1 = recv(socket, (char *)&cmnpck + nRead2, sizeof(cmnpck) - nRead2, 0);
					
					if(nRead1 > 0)
						nRead2 += nRead1;
				}
				
				if(nRead1 < 0) return;
			}
			
			if(!cmnpck.header) return;
			
			bReadAllPacket = TRUE;
			
			CString str;
			str.Format("ByteToRead = %d, cmnpck.header = %d, cmnpck.length = %d\n", ByteToRead, cmnpck.header, cmnpck.length);
			m_Console.writeConsol((LPSTR)(LPCTSTR)str);
			
			readbuff = new BYTE[cmnpck.length];
			memset(readbuff, 0, sizeof(readbuff)); 
			memcpy(readbuff, &cmnpck, sizeof(cmnpck));
		}

		if(nRead2 < 1) return;

		while(nRead2 < cmnpck.length)
		{
			Sleep(10);
			
			nRead1 = recv(socket, (char *)readbuff + nRead2, cmnpck.length - nRead2, 0);
			
			if(nRead1 < 0)
			{
				nRead1 = recv(socket, (char *)readbuff + nRead2, cmnpck.length - nRead2, 0);
				
				if(nRead1 > 0) nRead2 += nRead1;
			}
			
			else nRead2 += nRead1;
			
			dwError = WSAGetLastError();
			
			if(dwError == WSAESHUTDOWN || dwError == WSAECONNRESET)
				return;
			
			else if(dwError == WSAEWOULDBLOCK)
			{
				//DWORD ByteToRead;
				ioctlsocket(socket, FIONREAD, &ByteToRead);
				if(!ByteToRead) return;
			}
			
			CString str;
			str.Format("nRead2(%d) < cmnpck.length(%d)\n", nRead2, cmnpck.length);

			m_Console.writeConsol((LPSTR)(LPCTSTR)str);
		}
		
		m_RecvInfo.socket = socket;
		
		Recv.Clear();
		Recv.SetData(readbuff, 0, nRead2);
		
		if(nRead2) delete [] readbuff;
		
		nRead1 = 0;
		nRead2 = 0;

		bReadAllPacket = FALSE;

		ProcessMessage(&Recv);
	}
	
	else if(WSAGETSELECTEVENT(lParam) == FD_CLOSE)
	{
		closesocket(m_ClientSocket.m_Socket);
	}
}



void CLobbyDlg::ProcessMessage(CMessage *pMsg)
{
	COMMON_PACKET cmnpck;

	pMsg->GetData(&cmnpck, 0, sizeof(cmnpck));

	switch(cmnpck.header) // 패킷 해더 검색
	{

		/////////////////////////////////////////////////
		// Channel Server 패킷 처리
		case CSM_CONNECTED :
			RequestChInfo();
			m_Console.writeConsol("RequestInfo()\n");
			break;

		case CSM_CHINFO :
			UpdateChannelInfo(pMsg);
			m_Console.writeConsol("UpdateChannelInfo()\n");
			break;

		case CSM_DESTSVRINFO :
			ConnectToGameServer(pMsg);
			m_Console.writeConsol("ConnectToGameServer()\n");
			break;


		/////////////////////////////////////////////////
		// Game Server 패킷 처리

		case GSM_FAILURE :
			ProcessErrorMessage(pMsg);
			m_Console.writeConsol("ProcessErrorMessage()\n");
			break;

		case GSM_CONNECTED :
			RequestLogin();
			m_Console.writeConsol("RequestLogin()\n"); 
			break;

		case GSM_CLIENTINFO :
			GetMyData(pMsg);
			m_Console.writeConsol("GetMyData()\n");
			break;

		case GSM_ALLUSERSINFO :
			m_nCurRecv = ALLUSERS;
			RecvAllUsersInfo(pMsg);
			m_Console.writeConsol("RecvAllUsersInfo()\n");
			break;

		case GSM_ALLROOMSINFO :
			m_nCurRecv = ALLROOMS;
			RecvAllRoomsInfo(pMsg);
			m_Console.writeConsol("RecvAllRoomsInfo()\n");
			break;

		case GSM_ROOMUPDATED :
			UpdateRoomInfo(pMsg);
			m_Console.writeConsol("UpdateRoomInfo()\n");
			break;

		case GSM_CLIENTUPDATED :
			UpdateClientInfo(pMsg);
			m_Console.writeConsol("UpdateClientInfo()\n");
			break;

		case GSM_ALLUPDATED :
			UpdateAllInfo(pMsg);
			m_Console.writeConsol("UpdateAllInfo()\n");
			break;

		case GSM_CHATMESSAGE :
			UpdateChatMessage(pMsg);
			m_Console.writeConsol("UpdateChatMessage()\n");
			break;

		case GSM_COMPETITINFO :
			GetCompetitorData(pMsg);
			m_Console.writeConsol("GetCompetitorData()\n");
			break;

		case GSM_OWNTHISROOM :
			StartGame(TRUE);
			m_Console.writeConsol("\n\n\n*** StartGame(TRUE) ***\n");
			break;

		case GSM_JOINTHISROOM :
			StartGame(FALSE);
			m_Console.writeConsol("\n\n\n*** StartGame(FALSE) ***\n");
			break;

		case GSM_STARTNEWCHAT :
			StartNewChat();
			break;

		case GSM_REJECTNEWCHAT :
			NewChatRejected();
			break;

		case GSM_REQNEWGAME :
			DetermineToPlay(pMsg);
			break;
			
		case GSM_REJECTNEWGAME :
			::ShowWindow(m_hBingGoWnd, SW_HIDE);
			AfxMessageBox("상대방이 게임요청을 거절했습니다.");
			break;
	}
}

void CLobbyDlg::RequestChInfo()
{ 
	CMessage Send;
	COMMON_PACKET cmnpck;

	cmnpck.header = CM_REQCHINFO;
	cmnpck.length = sizeof(COMMON_PACKET);
	Send.SetData(&cmnpck, 0, sizeof(cmnpck));
	m_ClientSocket.SendMsg((char *)Send.GetDataPtr(), Send.GetLength());
}


void CLobbyDlg::RequestGetoutRoom()
{
	CMessage Send;
	COMMON_PACKET cmnpck;

	cmnpck.header = CM_REQGETOUTROOM;
	cmnpck.length = sizeof(COMMON_PACKET);
	Send.SetData(&cmnpck, 0, sizeof(cmnpck));

	int nResult = m_ClientSocket.SendMsg((char *)Send.GetDataPtr(), Send.GetLength());

	CString str;
	str.Format("RequestGetoutRoom()%d\n", nResult);

	m_Console.writeConsol((LPSTR)(LPCTSTR)str);
}

void CLobbyDlg::UpdateChannelInfo(CMessage *pMsg)
{
	int nLen = 0;

	COMMON_PACKET cmnpck;

	nLen = pMsg->GetData(&cmnpck, nLen, sizeof(COMMON_PACKET));

	m_pChannelDlg->ClearChannelInfo();

	while(nLen < cmnpck.length)
	{
		CHANNEL channel;

		nLen += pMsg->GetData(&channel, nLen, sizeof(channel));
		m_pChannelDlg->AddChannelInfo(channel);
	}
}


void CLobbyDlg::EnterChannel(unsigned short channelno)
{
	CMessage Send;

	COMMON_PACKET cmnpck;

	cmnpck.header = CM_ENTCHANNEL;
	cmnpck.length = sizeof(cmnpck) + sizeof(channelno);

	int nLen = Send.SetData(&cmnpck, 0, sizeof(cmnpck));
	Send.SetData(&channelno, nLen, sizeof(channelno));

	m_ClientSocket.SendMsg((char *)Send.GetDataPtr(), Send.GetLength());
}



void CLobbyDlg::ConnectToGameServer(CMessage *pMsg)
{
	SERVERINFO si;

	pMsg->GetData(&si, 0, sizeof(SERVERINFO));

	m_nChannelNo = si.rooms;

	CWSASocket TempSocket;

	if(!TempSocket.Connect((char *)si.ip, si.port))
	{
		MessageBox("게임 서버 접속에 실패했습니다.!!\n잠시 후 다시 시도해 주세요.", "에러", MB_OK);
	}

	else
	{
		m_ClientSocket.Disconnect(); // 채널 서버와의 접속 끊음
		m_ClientSocket.m_Socket = TempSocket.m_Socket; // 게임 서버와 접속한 소켓을 할당
		WSAAsyncSelect(m_ClientSocket.m_Socket, GetSafeHwnd(), WM_SOCKETMESSAGE, FD_READ | FD_CLOSE); // 이벤트 셀렉트
	}
}

void CLobbyDlg::RequestLogin()
{
	CMessage Send;

	COMMON_PACKET cmnpck;

	int nLen = sizeof(cmnpck);

	char userid[32] = {0, };
	char userip[32] = {0, };

	strcpy(userid, (LPCTSTR)m_strID);

	m_strIP = GetMyIP();

	strcpy(userip, (LPCTSTR)m_strIP);

	nLen = Send.SetData(userid, nLen, sizeof(userid));
	nLen = Send.SetData(&m_nChannelNo, nLen, sizeof(m_nChannelNo));
	nLen = Send.SetData(userip, nLen, sizeof(userip));

	cmnpck.header = CM_REQLOGIN;
	cmnpck.length = nLen;

	Send.SetData(&cmnpck, 0, sizeof(cmnpck));

	m_ClientSocket.SendMsg((char *)Send.GetDataPtr(), Send.GetLength());

	WSAAsyncSelect(m_ClientSocket.m_Socket, GetSafeHwnd(), WM_SOCKETMESSAGE, FD_READ | FD_CLOSE);
}

void CLobbyDlg::GetMyData(CMessage *pMsg)
{
	int nLen = sizeof(COMMON_PACKET);

	pMsg->GetData(&m_MyInfo, nLen, sizeof(USERINFO));

	SwitchToGamePage();

	m_pGameRoomDlg->SetMyInfo(m_MyInfo);

	RequestAllUsers();
}

void CLobbyDlg::GetCompetitorData(CMessage *pMsg)
{
	int nLen = sizeof(COMMON_PACKET);

	pMsg->GetData(&m_CompetInfo, nLen, sizeof(USERINFO));

	if(m_bOwner)
	{
		m_pBingGo->SetcompetitorInfo(m_CompetInfo);

		CString str;
		str.Format("ip : %s, id : %s\n", (LPSTR)m_CompetInfo.ip, (LPSTR)m_CompetInfo.id);

		m_Console.writeConsol((LPSTR)(LPCTSTR)str);
	}
}

void CLobbyDlg::RequestAllUsers(BOOL bSplash)
{
	KillTimer(0);

	if(bSplash)
	{
		m_pProgressDlg->CenterWindow();
		m_pProgressDlg->ShowWindow(SW_SHOW);
		m_pProgressDlg->SetStatusText("현재 접속자 확인 중입니다...");
	}

	CMessage Send;
	COMMON_PACKET cmnpck;

	cmnpck.header = CM_REQALLUSERS;
	cmnpck.length = sizeof(cmnpck);

	Send.SetData(&cmnpck, 0, sizeof(cmnpck));

	m_ClientSocket.SendMsg((char *)Send.GetDataPtr(), Send.GetLength());
}

void CLobbyDlg::RequestAllRooms()
{
	m_pProgressDlg->SetStatusText("방 정보를 확인 중입니다...");

	CMessage Send;
	COMMON_PACKET cmnpck;

	cmnpck.header = CM_REQALLROOMS;
	cmnpck.length = sizeof(cmnpck) + sizeof(m_MyInfo.channelno);

	Send.SetData(&cmnpck, 0, sizeof(cmnpck));
	Send.SetData(&m_MyInfo.channelno, sizeof(cmnpck), sizeof(m_MyInfo.channelno)); // 채널번호 같이 보냄

	m_ClientSocket.SendMsg((char *)Send.GetDataPtr(), Send.GetLength());
}


void CLobbyDlg::SwitchToGamePage()
{
	CRect rect;

	m_pChannelDlg->ShowWindow(SW_HIDE);

	SetWindowPos(NULL, 0, 0, 525, 510, SWP_NOZORDER | SWP_NOMOVE);

	CenterWindow();

	GetClientRect(&rect);

	CWnd *pWnd = GetDlgItem(IDC_CAPTION);
	pWnd->SetWindowPos(NULL, 0, 0, 525, 27, SWP_NOZORDER | SWP_NOMOVE);

	pWnd = GetDlgItem(IDC_PAGEHOLDER);
	pWnd->SetWindowPos(NULL, 0, 0, 525, 490, SWP_NOZORDER | SWP_NOMOVE);

	m_ctrlMinimize.SetWindowPos(NULL, rect.Width() - 38, 5, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
	m_ctrlClose.SetWindowPos(NULL, rect.Width() - 20, 5, 0, 0, SWP_NOZORDER | SWP_NOSIZE);

	m_ctrlMinimize.Invalidate();
	m_ctrlClose.Invalidate();

	pWnd->GetClientRect(&rect);
	pWnd->ClientToScreen(&rect);
	ScreenToClient(&rect);

	m_pGameRoomDlg->ShowWindow(SW_SHOW);
	m_pGameRoomDlg->SetWindowPos(NULL, rect.left, rect.top, rect.Width(), rect.Height(), SWP_NOZORDER);

	m_nCurPage = PAGE_GAMEROOM;
}


void CLobbyDlg::SwitchToChannelPage()
{
	CRect rect;

	m_pGameRoomDlg->ShowWindow(SW_HIDE);
	m_pChannelDlg->ShowWindow(SW_SHOW);

	SetWindowPos(NULL, 0, 0, 245, 402, SWP_NOZORDER | SWP_NOMOVE);

	CenterWindow();

	GetClientRect(&rect);

	m_ctrlMinimize.SetWindowPos(NULL, rect.Width() - 38, 5, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
	m_ctrlClose.SetWindowPos(NULL, rect.Width() - 20, 5, 0, 0, SWP_NOZORDER | SWP_NOSIZE);

	CWnd *pWnd = GetDlgItem(IDC_CAPTION);
	pWnd->SetWindowPos(NULL, 0, 0, 245, 27, SWP_NOZORDER | SWP_NOMOVE);

	pWnd = GetDlgItem(IDC_PAGEHOLDER);
	pWnd->SetWindowPos(NULL, 0, 0, 245, 490, SWP_NOZORDER | SWP_NOMOVE);

	m_nCurPage = PAGE_CHANNEL;
}


void CLobbyDlg::ProcessErrorMessage(CMessage *pMsg)
{
	int nLen = sizeof(COMMON_PACKET);
	FAILURE failure;

	pMsg->GetData(&failure, nLen, sizeof(failure));

	switch(failure)
	{
		case F_ALREADYLOGIN :
			AfxMessageBox("로그인에 실패했습니다.\n다른 곳에서 이미 로그인 중입니다.");

			///////////////////////////////////////////////////
			// 채널 서버에 다시 접속

			ConnectToChannelServer();
			break;

		case F_DBERROR :
			AfxMessageBox("서버 내부 DB 오류입니다.");
			break;

		case F_SERVERERROR :
			AfxMessageBox("서버 오류입니다.");
			break;

		case F_CHANNELFULL :
			AfxMessageBox("채널에 입장할 수 없습니다.");
			
			///////////////////////////////////////////////////
			// 채널 서버에 다시 접속

			ConnectToChannelServer();

			RequestChInfo();
			break;

		case F_ROOMOCCUPIED :
			AfxMessageBox("방에 입장할 수 없습니다.");
			m_MyInfo.roomno = -1;
			m_bOfferGame = FALSE;
			break;

		case F_JOINROOM :
			AfxMessageBox("방에 입장할 수 없습니다.\n빈 방 입니다.");
			break;

	}
}

void CLobbyDlg::RecvAllUsersInfo(CMessage *pMsg)
{
	m_pGameRoomDlg->m_ctrlUsersList.DeleteAllItems();
	
	int nOffset = sizeof(COMMON_PACKET); // 맨 앞의 헤더 정보는 건너뜀
	COMMON_PACKET cmnpck;
	
	memcpy(&cmnpck, pMsg->GetDataPtr(), sizeof(cmnpck));
	
	int nTotalUsers = (cmnpck.length - sizeof(cmnpck)) / sizeof(USERINFO);
	
	USERINFO user;
	
	for(int i = 0 ; i < nTotalUsers ; i++)
	{
		memcpy(&user, (char *)pMsg->GetDataPtr() + nOffset, sizeof(USERINFO));
		m_pGameRoomDlg->AddUser(i, user);
		
		m_UserManager.AddUser(user);

		nOffset += sizeof(USERINFO);
	}

	RequestAllRooms();
}


void CLobbyDlg::RecvAllRoomsInfo(CMessage *pMsg)
{
	m_pGameRoomDlg->m_ctrlRoomList.DeleteAllItems();
	
	int nOffset = sizeof(COMMON_PACKET); // 맨 앞의 헤더 정보는 건너뜀
	COMMON_PACKET cmnpck;
	
	memcpy(&cmnpck, pMsg->GetDataPtr(), sizeof(cmnpck));
	
	int nTotalUsers = (cmnpck.length - sizeof(cmnpck)) / sizeof(ROOMINFO);
	
	ROOMINFO room;
	
	for(int i = 0 ; i < nTotalUsers ; i++)
	{
		memcpy(&room, (char *)pMsg->GetDataPtr() + nOffset, sizeof(ROOMINFO));
		m_pGameRoomDlg->AddRoom(room);
		
		m_RoomManager.AddRoom(room);

		nOffset += sizeof(ROOMINFO);
	}

	m_pProgressDlg->ShowWindow(SW_HIDE);
}

CString CLobbyDlg::GetMyIP()
{
	CString strIP = _T("");
	char hostname[255];
	PHOSTENT hostentry;
	IN_ADDR addr;
	
	if(gethostname(hostname, sizeof(hostname)) == 0)
	{
		if(hostentry = gethostbyname(hostname))
		{
			int nLen = strlen(hostentry->h_addr); // 전체 길이를 구함
			nLen -= strlen(hostname); // 전체 문자열에서 호스트이름만큼 길이를 뺌
			nLen /= 4; // 4로 나누어 아이피 갯수 구함
			
			for(int i = 0 ; i < nLen ; i++)
			{
				memcpy(&addr, &hostentry->h_addr[i*4], 4);
				strIP = inet_ntoa(addr);
				
				if(strIP.Left(3) == "192" || strIP.Left(3) == "169" || strIP.Left(3) == "10") // 사설 IP임
					continue;
			}
		}
	}

	return strIP;
}


void CLobbyDlg::UpdateRoomInfo(CMessage *pMsg)
{
	ROOMINFO roominfo;

	pMsg->GetData(&roominfo, sizeof(COMMON_PACKET), sizeof(roominfo));

	m_RoomManager.UpdateRoom(roominfo);
	m_pGameRoomDlg->UpdateRoomList(roominfo);
}

void CLobbyDlg::UpdateClientInfo(CMessage *pMsg)
{
	USERINFO userinfo;

	pMsg->GetData(&userinfo, sizeof(COMMON_PACKET), sizeof(userinfo));

	m_UserManager.UpdateUser(userinfo);
	m_pGameRoomDlg->UpdateClientList(userinfo);
}

void CLobbyDlg::UpdateAllInfo(CMessage *pMsg)
{
	USERINFO userinfo;
	ROOMINFO roominfo;

	pMsg->GetData(&userinfo, sizeof(COMMON_PACKET), sizeof(userinfo));
	pMsg->GetData(&roominfo, sizeof(COMMON_PACKET) + sizeof(userinfo), sizeof(roominfo));

	if(!stricmp((char *)userinfo.id, (char *)m_MyInfo.id) && m_bReadyGame && userinfo.roomno == -1)
	{
		m_bReadyGame = FALSE;
		::SendMessage(m_hBingGoWnd, WM_COMMAND, IDC_OUTPUT, 0);
	}

	m_UserManager.UpdateUser(userinfo);
	m_RoomManager.UpdateRoom(roominfo);

	m_pGameRoomDlg->UpdateClientList(userinfo);
	m_pGameRoomDlg->UpdateRoomList(roominfo);
}

void CLobbyDlg::UpdateChatMessage(CMessage *pMsg)
{
	char *buff = NULL;
	COMMON_PACKET cmnpck;

	pMsg->GetData(&cmnpck, 0, sizeof(cmnpck));
	buff = new char[cmnpck.length];
	memset(buff, 0, cmnpck.length);
	pMsg->GetData(buff, sizeof(COMMON_PACKET), pMsg->GetLength() - sizeof(COMMON_PACKET));
	CString strChatMsg = buff;
	delete [] buff;
	m_pGameRoomDlg->AddChatMessage(strChatMsg);
}

void CLobbyDlg::OnTimer(UINT nIDEvent) 
{
	if(nIDEvent == 0)
	{
		if(m_nCurRecv != ALLUSERS && m_nCurRecv != ALLROOMS)
			RequestAllUsers(FALSE);
	}

	CDialog::OnTimer(nIDEvent);
}

void CLobbyDlg::StartGame(BOOL bServer)
{
	if(m_bExitRoom)
	{
		m_bExitRoom = FALSE;
		return;
	}

	CString str;

	ShowWindow(SW_HIDE);
	
	CHARFORMAT cf = {sizeof(CHARFORMAT) };
	
	DWORD dwMask = 0xffffffff;
	DWORD dwEffect = 0;
	
	dwEffect = CFE_UNDERLINE;
	
	cf.dwMask = dwMask;
	cf.dwEffects = dwEffect;
	cf.yHeight = 170;
	cf.crTextColor = RGB(0, 0, 255);
	cf.bCharSet = DEFAULT_CHARSET;
	cf.bPitchAndFamily = FF_DONTCARE;
	
	strcpy(cf.szFaceName, "굴림체");

	if(m_pBingGo == NULL)
	{
		m_pBingGo = getGameObject(); // 오목 윈도우 객체 생성
		m_hBingGoWnd = m_pBingGo->CreateBingGo();
		m_pBingGo->SetParentHandle(GetSafeHwnd());
		m_Console.writeConsol("\n\n\n*** First ***\n");
	}
 
	m_Console.writeConsol("\n\n\n*** Second ***\n");
	::ShowWindow(m_hBingGoWnd , SW_SHOW);

	m_pBingGo->SetMyInfo(m_MyInfo);

	if(bServer)
	{
		m_bOwner = TRUE;

		::SendMessage(m_hBingGoWnd, WM_COMMAND, IDC_SERVER, m_bOfferGame);

		m_bOfferGame = FALSE;

		m_pBingGo->SetNotice("", (LPBYTE)&cf);
		str.Format("▷ %d 번방 입장\n", m_CompetInfo.roomno);
	}

	else
	{
		m_pBingGo->SetcompetitorInfo(m_CompetInfo);

		m_pBingGo->SetNotice("", (LPBYTE)&cf);
		str.Format("▷ %d 번방 입장\n", m_CompetInfo.roomno);

		m_pBingGo->SetNotice((LPSTR)(LPCTSTR)str, (LPBYTE)&cf);

		::SendMessage(m_hBingGoWnd, WM_COMMAND, IDC_CLIENT, 0);

/*
		LPROOMINFO lpri = m_RoomManager.GetRoom(m_MyInfo.roomno);

		CString strTitle;
		strTitle.Format("[%d 번방] %s", lpri->id+1, (LPSTR)lpri->title);

		::SetWindowText(m_hOmokWnd, strTitle);
*/
	}
}


LRESULT CLobbyDlg::OnGameResult(WPARAM wParam, LPARAM lParam)
{
	TRACE("OnGameResult\n");

	COMMON_PACKET cmnpck;
	CMessage Send;

	cmnpck.header = CM_UPDATEINFO;
	cmnpck.length = sizeof(cmnpck) + sizeof(USERINFO);

	Send.SetData(&cmnpck, 0, sizeof(cmnpck));

	LPUSERINFO lpui = NULL;
	int tempscore;

	switch(wParam)
	{
		case 0 : // drawn

			m_MyInfo.drawcount++;
			m_MyInfo.playcount++;

			m_CompetInfo.drawcount++;
			m_CompetInfo.playcount++;

			lpui = m_UserManager.GetUser((char *)m_MyInfo.id);

			lpui->drawcount++;
			lpui->playcount++;

			m_pGameRoomDlg->SetMyInfo(*lpui);

			lpui = m_UserManager.GetUser((char *)m_CompetInfo.id);

			lpui->drawcount++;
			lpui->playcount++;

			m_pBingGo->SetMyInfo(m_MyInfo);
			m_pBingGo->SetcompetitorInfo(m_CompetInfo);

			Send.SetData(&m_MyInfo, sizeof(cmnpck), sizeof(m_MyInfo));
			m_ClientSocket.SendMsg((char *)Send.GetDataPtr(), Send.GetLength());
			m_Console.writeConsol("wParam : 0\n");
			break;

		case 1 : // victory

			//점수 계산
			// 기본 점수 + 기본 점수 * 상대방의 승률

			
			m_MyInfo.score += (unsigned long)(m_CompetInfo.score * BASERATIO);
			m_CompetInfo.score -= (unsigned long)(m_CompetInfo.score * BASERATIO);

			m_MyInfo.victorycount++;
			m_MyInfo.playcount++;

			m_CompetInfo.losscount++;
			m_CompetInfo.playcount++;

			lpui = m_UserManager.GetUser((char *)m_MyInfo.id);

			lpui->score = m_MyInfo.score;
			lpui->victorycount++;
			lpui->playcount++;

			m_pGameRoomDlg->SetMyInfo(*lpui);

			lpui = m_UserManager.GetUser((char *)m_CompetInfo.id);

			lpui->score = m_CompetInfo.score;
			lpui->losscount++;
			lpui->playcount++;

			m_pBingGo->SetMyInfo(m_MyInfo);
			m_pBingGo->SetcompetitorInfo(m_CompetInfo);

			// 점수 결과 보냄

			Send.SetData(&m_MyInfo, sizeof(cmnpck), sizeof(m_MyInfo));
			m_ClientSocket.SendMsg((char *)Send.GetDataPtr(), Send.GetLength());
			m_Console.writeConsol("wParam : 1\n");
			break;

		case 2 : // loss

			//점수 계산
			// 기본 점수 + 기본 점수 * 상대방의 승률

			tempscore = m_MyInfo.score;
			tempscore -= (unsigned long)(m_MyInfo.score * BASERATIO);
			m_CompetInfo.score += (unsigned long)(m_MyInfo.score * BASERATIO);
			m_MyInfo.score = tempscore;

			m_MyInfo.losscount++;
			m_MyInfo.playcount++;

			m_CompetInfo.victorycount++;
			m_CompetInfo.playcount++;

			lpui = m_UserManager.GetUser((char *)m_MyInfo.id);

			lpui->score = m_MyInfo.score;
			lpui->losscount++;
			lpui->playcount++;

			m_pGameRoomDlg->SetMyInfo(*lpui);

			lpui = m_UserManager.GetUser((char *)m_CompetInfo.id);

			lpui->score = m_CompetInfo.score;
			lpui->victorycount++;
			lpui->playcount++;

			m_pBingGo->SetMyInfo(m_MyInfo);
			m_pBingGo->SetcompetitorInfo(m_CompetInfo);

			// 점수 결과 보냄

			Send.SetData(&m_MyInfo, sizeof(cmnpck), sizeof(m_MyInfo));
			m_ClientSocket.SendMsg((char *)Send.GetDataPtr(), Send.GetLength());
			m_Console.writeConsol("wParam : 2\n");
			break;

		case 4 : // 그냥 종료..

			// 방 나감
			// 그냥 종료
			RequestGetoutRoom();
			m_Console.writeConsol("\n\n\n*** EndGame(그냥 종료) ***\n");
			m_Console.writeConsol("wParam : 4\n");
			return 0;
	}

	// lParam이 0이면 종료
	if(lParam == 0) 
	{
		RequestGetoutRoom();
		m_Console.writeConsol("\n\n\n*** EndGame(게임 후 종료) ***");
		m_bExitRoom = TRUE;
	}

	return 0;
}

void CLobbyDlg::Exit()
{
	KillTimer(0);

//	while(m_nCurRecv == ALLUSERS || m_nCurRecv == ALLROOMS);

	if(MessageBox("종료하시겠습니까?", "종료", MB_YESNO) == IDYES)
	{
		RemovePages();
		PostQuitMessage(0);
	}
}

void CLobbyDlg::NewChat(LPUSERINFO lpuserinfo)
{
	if(lpuserinfo->roomno == -1)
	{	
		CMessage Send;
		COMMON_PACKET cmnpck;
		
		cmnpck.header = CM_REQNEWCHAT;
		cmnpck.length = sizeof(cmnpck) + sizeof(lpuserinfo->id);

		Send.SetData(&cmnpck, 0, sizeof(cmnpck));
		Send.SetData(lpuserinfo->id, sizeof(cmnpck), sizeof(lpuserinfo->id));

		m_ClientSocket.SendMsg((char *)Send.GetDataPtr(), Send.GetLength());

		if(m_pChatDlg == NULL)
		{
			m_pChatDlg = new CChatDlg();
			m_pChatDlg->Create(IDD_CHAT_DIALOG, this);
			m_pChatDlg->ShowWindow(SW_SHOW);
		}
	}

	else
	{
		AfxMessageBox("게임 중인 상대입니다.");
	}
}

void CLobbyDlg::StartNewChat()
{
	m_pChatDlg->StartChat();
}

void CLobbyDlg::NewChatRejected()
{
	m_pChatDlg->EndChat();
}

void CLobbyDlg::OfferGame(LPUSERINFO lpuserinfo)
{
	if(lpuserinfo->roomno == -1)
	{	
		CMessage Send;
		COMMON_PACKET cmnpck;
		
		cmnpck.header = CM_REQNEWGAME;
		cmnpck.length = sizeof(cmnpck) + sizeof(lpuserinfo->id);

		Send.SetData(&cmnpck, 0, sizeof(cmnpck));
		Send.SetData(lpuserinfo->id, sizeof(cmnpck), sizeof(lpuserinfo->id));

		m_bExitRoom = FALSE;

		m_ClientSocket.SendMsg((char *)Send.GetDataPtr(), Send.GetLength());

		m_bReadyGame = TRUE;
		m_bOfferGame = TRUE;
	}

	else
	{
		AfxMessageBox("게임 중인 상대입니다.");
	}
}

void CLobbyDlg::DetermineToPlay(CMessage *pMsg)
{
	char id[32] = {0, };
	short roomno;

	CMessage Send;
	COMMON_PACKET cmnpck;

	pMsg->GetData(id, sizeof(COMMON_PACKET), sizeof(id));
	pMsg->GetData(&roomno, sizeof(cmnpck) + sizeof(id), sizeof(roomno));

	CString strMsg;

	strMsg.Format("%s님이 게임요청을 하셨습니다.\n게임요청을 승락하시겠습니까?", id);

	if(MessageBox(strMsg, "게임 요청", MB_YESNO | MB_ICONEXCLAMATION) == IDYES)
	{
		m_MyInfo.roomno = roomno;
		
		cmnpck.header = CM_REQJOINROOM;
		cmnpck.length = sizeof(cmnpck) + sizeof(roomno);
		
		Send.SetData(&cmnpck, 0, sizeof(cmnpck));
		Send.SetData(&roomno, sizeof(cmnpck), sizeof(roomno));
		
		m_ClientSocket.SendMsg((char *)Send.GetDataPtr(), Send.GetLength());
	}

	else
	{
		cmnpck.header = CM_REJECTNEWGAME;
		cmnpck.length = sizeof(cmnpck) + sizeof(id);

		Send.SetData(&cmnpck, 0, sizeof(cmnpck));
		Send.SetData(id, sizeof(cmnpck), sizeof(id));

		m_ClientSocket.SendMsg((char *)Send.GetDataPtr(), Send.GetLength());
	}
}

void CLobbyDlg::ChangeChannel()
{
	m_ClientSocket.Disconnect();

	m_UserManager.ClearUser();
	m_RoomManager.ClearRoom();

	SwitchToChannelPage();
	ConnectToChannelServer();
	RequestChInfo();
}

BOOL CLobbyDlg::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	if(pWnd == &m_ctrlMinimize || pWnd == &m_ctrlClose)
	{
		SetCursor(AfxGetApp()->LoadCursor(IDC_HAND));
		return TRUE;
	}
	
	return CDialog::OnSetCursor(pWnd, nHitTest, message);
}


LRESULT CLobbyDlg::OnGameState(WPARAM wParam, LPARAM lParam)
{
	COMMON_PACKET cmnpck;
	CMessage Send;
	short gamestate = (short)lParam + 1;

	cmnpck.header = CM_GAMESTATE;
	cmnpck.length = sizeof(cmnpck) + sizeof(gamestate);

	Send.SetData(&cmnpck, 0, sizeof(cmnpck));
	Send.SetData(&gamestate, sizeof(cmnpck), sizeof(gamestate));

	m_ClientSocket.SendMsg((char *)Send.GetDataPtr(), Send.GetLength());

	return 0;
}