// GameRoomDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Lobby.h"
#include "GameRoomDlg.h"
#include "LobbyDlg.h"
#include "NewRoomDlg.h"
#include "UserInfoDlg.h"
#include "PasswdDlg.h"
#include "afxmt.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CCriticalSection g_Critical;

/////////////////////////////////////////////////////////////////////////////
// CGameRoomDlg dialog


CGameRoomDlg::CGameRoomDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGameRoomDlg::IDD, pParent)
{
	m_pParent = (CLobbyDlg *)pParent;
	//{{AFX_DATA_INIT(CGameRoomDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CGameRoomDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGameRoomDlg)
	DDX_Control(pDX, IDC_REFRESH, m_ctrlRefresh);
	DDX_Control(pDX, IDC_EXIT, m_ctrlExit);
	DDX_Control(pDX, IDC_CHANGECHANNEL, m_ctrlChgChannel);
	DDX_Control(pDX, IDC_NEWCHAT, m_ctrlEnterRoom);
	DDX_Control(pDX, IDC_PERSONALINFO, m_ctrlPersonalInfo);
	DDX_Control(pDX, IDC_OFFERGAME, m_ctrlOfferGame);
	DDX_Control(pDX, IDC_ROOMLIST, m_ctrlRoomList);
	DDX_Control(pDX, IDC_USERLIST, m_ctrlUsersList);
	DDX_Control(pDX, IDC_VIEWCHAT, m_ctrlViewChat);
	DDX_Control(pDX, IDC_CHAT, m_ctrlChat);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGameRoomDlg, CDialog)
	//{{AFX_MSG_MAP(CGameRoomDlg)
	ON_NOTIFY(NM_DBLCLK, IDC_ROOMLIST, OnDblclkRoomlist)
	ON_BN_CLICKED(IDC_REFRESH, OnRefresh)
	ON_BN_CLICKED(IDC_PERSONALINFO, OnPersonalinfo)
	ON_NOTIFY(NM_DBLCLK, IDC_USERLIST, OnDblclkUserlist)
	ON_BN_CLICKED(IDC_EXIT, OnExit)
	ON_BN_CLICKED(IDC_OFFERGAME, OnOffergame)
	ON_BN_CLICKED(IDC_CHANGECHANNEL, OnChangechannel)
	ON_WM_CTLCOLOR()
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_ENTERROOM, EnterSelectedRoom)
	ON_WM_SETCURSOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGameRoomDlg message handlers

void CGameRoomDlg::InsertColumn()
{
	LV_COLUMN lvcolumn;
	lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
	lvcolumn.fmt  = LVCFMT_CENTER;

	for(int i = 0 ; i < 3 ; i++)
	{
		lvcolumn.iSubItem = i;

		switch(i)
		{
			lvcolumn.iImage = i;

			case 0:
				lvcolumn.cx = 67;
                lvcolumn.pszText = _T("ID");
				break;

			case 1:
                lvcolumn.cx = 50;
                lvcolumn.pszText = _T("점수");
				break;

			case 2:
                lvcolumn.cx = 50;
                lvcolumn.pszText = _T("위치");
				break;
		}

		m_ctrlUsersList.InsertColumn(i, &lvcolumn);
	}


	for(i = 0 ; i < 5 ; i++)
	{
		lvcolumn.iSubItem = i;

		switch(i)
		{
			lvcolumn.iImage = i;

			case 0 :
				lvcolumn.cx = 25;
                lvcolumn.pszText = _T("");
				break;

			case 1 :
                lvcolumn.cx = 25;
                lvcolumn.pszText = _T("#");
				break;

			case 2 :
                lvcolumn.cx = 163;
                lvcolumn.pszText = _T("방제목");
				break;

			case 3 :
                lvcolumn.cx = 60;
                lvcolumn.pszText = _T("게임상태");
				break;

			case 4 :
                lvcolumn.cx = 30;
                lvcolumn.pszText = _T("참가인원");
				break;
		}

		m_ctrlRoomList.InsertColumn(i, &lvcolumn);
	}
}


BOOL CGameRoomDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_ctrlRefresh.SetImages(IDB_REFRESHNOR, IDB_REFRESHCLK, IDB_REFRESHOVER, IDB_REFRESHNOR);
	m_ctrlOfferGame.SetImages(IDB_OFFERNOR, IDB_OFFERCLK, IDB_OFFEROVER, IDB_OFFERNOR);
	m_ctrlPersonalInfo.SetImages(IDB_USERINFONOR, IDB_USERINFOCLK, IDB_USERINFOOVER, IDB_USERINFONOR);
	m_ctrlEnterRoom.SetImages(IDB_ENTROOMNOR, IDB_ENTROOMCLK, IDB_ENTROOMOVER, IDB_ENTROOMNOR);
	m_ctrlChgChannel.SetImages(IDB_CHGCHNNOR, IDB_CHGCHNCLK, IDB_CHGCHNOVER, IDB_CHGCHNNOR);
	m_ctrlExit.SetImages(IDB_EXITNOR, IDB_EXITCLK, IDB_EXITOVER, IDB_EXITNOR);

	m_ImageList.Create(IDB_LOCK, 22, 1, RGB(255, 255, 255));

	m_ctrlRoomList.SetImageList(&m_ImageList, LVSIL_SMALL);

	m_ctrlUsersList.SetExtendedStyle(m_ctrlUsersList.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_FLATSB);
	m_ctrlRoomList.SetExtendedStyle(m_ctrlUsersList.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_FLATSB);
	InsertColumn();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}



void CGameRoomDlg::AddUser(int nIndex, USERINFO userinfo)
{
	LV_ITEM lvitem;

	char buff[255];

	lvitem.iItem = nIndex;

	wsprintf(buff, "%s", userinfo.id);

    lvitem.mask		= LVIF_TEXT;
	lvitem.iSubItem = 0;
    lvitem.pszText  = buff;

	m_ctrlUsersList.InsertItem(&lvitem);

	wsprintf(buff, "%d", userinfo.score);
	m_ctrlUsersList.SetItemText(lvitem.iItem, 1, buff);

	if(userinfo.roomno != -1) wsprintf(buff, "%d번방", userinfo.roomno+1);
	else wsprintf(buff, "대기실");

	m_ctrlUsersList.SetItemText(lvitem.iItem, 2, buff);
}


void CGameRoomDlg::AddRoom(ROOMINFO roominfo)
{
	LV_ITEM lvitem;

	char buff[512];

	lvitem.iItem = roominfo.id;

    lvitem.mask	= LVIF_IMAGE;
	lvitem.iSubItem = 0;

	lvitem.iImage = roominfo.locked;

	m_ctrlRoomList.InsertItem(&lvitem);

	wsprintf(buff, "%d", roominfo.id + 1);
	m_ctrlRoomList.SetItemText(lvitem.iItem, 1, buff);

	wsprintf(buff, "%s", roominfo.title);
	m_ctrlRoomList.SetItemText(lvitem.iItem, 2, buff);

	if(roominfo.status) wsprintf(buff, "게임중");
	else wsprintf(buff, "대기중");

	m_ctrlRoomList.SetItemText(lvitem.iItem, 3, buff);

	wsprintf(buff, "%d/%d", roominfo.present, roominfo.maximum);
	m_ctrlRoomList.SetItemText(lvitem.iItem, 4, buff);
}

void CGameRoomDlg::OnDblclkRoomlist(NMHDR* pNMHDR, LRESULT* pResult) 
{
	EnterSelectedRoom();

	*pResult = 0;
}

void CGameRoomDlg::EnterSelectedRoom()
{
	POSITION pos = m_ctrlRoomList.GetFirstSelectedItemPosition();
	
	if(m_pParent->m_MyInfo.roomno == -1) // 현재 대기실에 있을 경우
	{
		if(pos != NULL)
		{
			int nItem = m_ctrlRoomList.GetNextSelectedItem(pos);
			
			LPROOMINFO lpri = m_pParent->m_RoomManager.GetRoom(nItem);

			if(!lpri->present) // 빈 방임
			{
				if(m_pParent->m_strIP.GetLength() == 0)
				{
					AfxMessageBox("사설 IP를 사용하고 있기 때문에 방을 만들 수 없습니다.");
					return;
				}

				CMessage Send;
				COMMON_PACKET cmnpck;
				
				CNewRoomDlg dlg;
				
				CString strTitle;
				strTitle.Format("%d번 방 개설", lpri->id+1);
				
				dlg.m_strTitle = strTitle;
				
				if(dlg.DoModal() == IDOK)
				{
					ROOMINFO roominfo;
					
					m_pParent->m_MyInfo.roomno = lpri->id;

					memset(&roominfo, 0, sizeof(roominfo));
					roominfo.id = lpri->id;
					strcpy((char *)roominfo.title, dlg.m_strRoomTitle);
					
					if(dlg.m_bLocked)
					{
						roominfo.locked = 1;
						strcpy((char *)roominfo.passwd, dlg.m_strPasswd);
					}

					cmnpck.header = LM_REQGMN;
					cmnpck.length = sizeof(cmnpck) + sizeof(roominfo);
					
					Send.SetData(&cmnpck, 0, sizeof(cmnpck));
					Send.SetData(&roominfo, sizeof(cmnpck), sizeof(roominfo));
					
					m_pParent->m_bExitRoom = FALSE;
					m_pParent->m_ClientSocket.SendMsg((char *)Send.GetDataPtr(), Send.GetLength());
				}
				
				else return;
			}
			// 개설된 방일때...
			else if(lpri->present < lpri->maximum)
			{
				if(!stricmp((char *)lpri->title, "*** 예약 ***"))
				{
					AfxMessageBox("예약된 방에 입장할 수 없습니다.");
					return;
				}

				if(lpri->locked)
				{
					CPasswdDlg dlg;

					if(dlg.DoModal() == IDOK)
					{
						if(strcmp(dlg.m_strPasswd, (char *)lpri->passwd))
						{
							MessageBox("패스워드가 다릅니다.", "알림", MB_OK | MB_ICONEXCLAMATION);
							return;
						}
					}

					else return;
				}

				m_pParent->m_MyInfo.roomno = lpri->id;

				CMessage Send;
				COMMON_PACKET cmnpck;
				
				cmnpck.header = LM_REQJOIN;
				cmnpck.length = sizeof(cmnpck) + sizeof(lpri->id);
				
				Send.SetData(&cmnpck, 0, sizeof(cmnpck));
				Send.SetData(&lpri->id, sizeof(cmnpck), sizeof(lpri->id));
				
				m_pParent->m_bExitRoom = FALSE;
				m_pParent->m_ClientSocket.SendMsg((char *)Send.GetDataPtr(), Send.GetLength());
			}
			
			else MessageBox("선택하신 방에 입장하실 수 없습니다.", "확인", MB_OK | MB_ICONEXCLAMATION);
		}

		else MessageBox("입장하실 방을 선택해주세요.", "확인", MB_OK | MB_ICONEXCLAMATION);
	}

	TRACE1("%d\n", m_pParent->m_MyInfo.roomno);
}


void CGameRoomDlg::UpdateRoomList(ROOMINFO roominfo)
{
	m_ctrlRoomList.SetItemText(roominfo.id, 2, (LPCTSTR)roominfo.title);

	CString buff;

	buff.Format("%d/%d", roominfo.present, roominfo.maximum);
	m_ctrlRoomList.SetItemText(roominfo.id, 4, buff);

	if(roominfo.status)
		m_ctrlRoomList.SetItemText(roominfo.id, 3, "게임중");

	else m_ctrlRoomList.SetItemText(roominfo.id, 3, "대기중");
	
	LV_ITEM lvitem;
	
	memset(&lvitem, 0, sizeof(lvitem));
	
	lvitem.iItem = roominfo.id;
    lvitem.mask	= LVIF_IMAGE;
	lvitem.iSubItem = 0;
	lvitem.iImage = roominfo.locked;

	m_ctrlRoomList.SetItem(&lvitem);
}

void CGameRoomDlg::UpdateClientList(USERINFO userinfo)
{
	LVFINDINFO info;
	int nIndex;
	
	info.flags = LVFI_PARTIAL|LVFI_STRING;
	info.psz = (char *)userinfo.id;

	TRACE1("%d\n", userinfo.roomno);

	nIndex = m_ctrlUsersList.FindItem(&info);

	////////////////////////////////////////
	// 새 사용자
	if(userinfo.roomno == -1)
	{
		if(nIndex != -1) // 사용자가 있음, 방에서 퇴장하여 대기실로 나옴
		{
			CString str;

			str.Format("%d", userinfo.score);
			m_ctrlUsersList.SetItemText(nIndex, 1, str);
			m_ctrlUsersList.SetItemText(nIndex, 2, "대기실");

			g_Critical.Lock();

			LPUSERINFO lpuser = m_pParent->m_UserManager.GetUser((LPCTSTR)userinfo.id);
			
			if(lpuser != NULL)
			{	
				lpuser->roomno = -1;

				// 내 정보 갱신
				if(m_pParent->m_strID == lpuser->id) 
				{
					this->userinfo = userinfo;
					m_pParent->m_MyInfo.roomno = -1;
					m_pParent->ShowWindow(SW_SHOW);

					Invalidate();
				}
			}

			g_Critical.Unlock();
		}

		else // 새로 들어온 사용자
		{
			if(m_pParent->m_strID != userinfo.id)
			{
				int nIndex = m_ctrlUsersList.GetItemCount();
				AddUser(nIndex, userinfo);
				m_pParent->m_UserManager.AddUser(userinfo);
			}
		}
	}

	////////////////////////////////////////
	// 사용자 접속 종료
	else if(userinfo.roomno == -2)
	{
		m_pParent->m_UserManager.DeleteUser((LPCTSTR)userinfo.id);
		m_ctrlUsersList.DeleteItem(nIndex);
	}

	else
	{
		CString str;

		str.Format("%d", userinfo.score);	
		m_ctrlUsersList.SetItemText(nIndex, 1, str);

		str.Format("%d번방", userinfo.roomno + 1);
		m_ctrlUsersList.SetItemText(nIndex, 2, str);

		g_Critical.Lock();

		LPUSERINFO lpuser = m_pParent->m_UserManager.GetUser((LPCTSTR)userinfo.id);
		
		if(lpuser != NULL) lpuser->roomno = userinfo.roomno;
		
		g_Critical.Unlock();
	}
}

BOOL CGameRoomDlg::PreTranslateMessage(MSG* pMsg) 
{
	if(pMsg->message == WM_KEYDOWN)
	{
		if(GetFocus() == &m_ctrlViewChat) return TRUE;

		if(pMsg->wParam == VK_RETURN)
		{
			if(GetFocus() == &m_ctrlChat) SendChatMessage();
			else if(GetFocus() == &m_ctrlRoomList) EnterSelectedRoom();
			return TRUE;
		}

		else if(pMsg->wParam == VK_ESCAPE) return TRUE;
	}


	return CDialog::PreTranslateMessage(pMsg);
}

void CGameRoomDlg::SendChatMessage()
{
	COMMON_PACKET cmnpck;
	CMessage Send;
	CString strChatMsg;

	m_ctrlChat.GetWindowText(strChatMsg);

	strChatMsg = m_pParent->m_strID + " :: " + strChatMsg;

	cmnpck.header = LM_CHATMSG;
	cmnpck.length = sizeof(cmnpck) + strChatMsg.GetLength();

	Send.SetData(&cmnpck, 0, sizeof(cmnpck));
	Send.SetData(strChatMsg, sizeof(cmnpck), strChatMsg.GetLength());

	int nResult = m_pParent->m_ClientSocket.SendMsg((char *)Send.GetDataPtr(), Send.GetLength());

	while(nResult < 0)
	{
		Sleep(10);
		nResult = m_pParent->m_ClientSocket.SendMsg((char *)Send.GetDataPtr(), Send.GetLength());
	}

	m_ctrlChat.SetWindowText("");
}

void CGameRoomDlg::OnRefresh() 
{
	m_pParent->RequestAllUsers();
}

void CGameRoomDlg::AddChatMessage(CString strChatMsg)
{
	strChatMsg += "\r\n";

	m_ctrlViewChat.SetSel(-1, -1);
	m_ctrlViewChat.ReplaceSel(strChatMsg);

	m_ctrlViewChat.LineScroll(3);
}

void CGameRoomDlg::SetMyInfo(USERINFO userinfo)
{
	CGameRoomDlg::userinfo = userinfo;
	Invalidate();
}

void CGameRoomDlg::OnPersonalinfo() 
{
	POSITION pos = m_ctrlUsersList.GetFirstSelectedItemPosition();

	if(pos != NULL)
	{
		int nItem = m_ctrlUsersList.GetNextSelectedItem(pos);

		CString userid = m_ctrlUsersList.GetItemText(nItem, 0);

		LPUSERINFO lpuser = m_pParent->m_UserManager.GetUser(userid);

		CUserInfoDlg dlg;

		dlg.SetUserInfo(*lpuser);

		dlg.DoModal();
	}

	else
	{
		AfxMessageBox("사용자를 선택해 주세요.");
	}
}

void CGameRoomDlg::OnDblclkUserlist(NMHDR* pNMHDR, LRESULT* pResult) 
{
	OnPersonalinfo();
	
	*pResult = 0;
}

void CGameRoomDlg::OnExit()
{
	m_pParent->Exit();
}


void CGameRoomDlg::OnOffergame() 
{
	POSITION pos = m_ctrlUsersList.GetFirstSelectedItemPosition();

	if(pos != NULL)
	{
		int nItem = m_ctrlUsersList.GetNextSelectedItem(pos);

		CString userid = m_ctrlUsersList.GetItemText(nItem, 0);

		LPUSERINFO lpuser = m_pParent->m_UserManager.GetUser(userid);

		if(!stricmp((char *)lpuser->id, (char *)m_pParent->m_MyInfo.id))
		{
			AfxMessageBox("자기 자신을 초대할 수 없습니다.");
			return;
		}

		m_pParent->OfferGame(lpuser);
	}

	else
	{
		AfxMessageBox("채팅할 상대를 선택해 주세요.");
	}
}

void CGameRoomDlg::OnChangechannel() 
{
	m_pParent->ChangeChannel();
}

HBRUSH CGameRoomDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	if(nCtlColor == CTLCOLOR_DLG)
		hbr = (HBRUSH)GetStockObject(WHITE_BRUSH);

	else if(nCtlColor == CTLCOLOR_STATIC)
	{
		hbr = (HBRUSH)GetStockObject(WHITE_BRUSH);
		pDC->SetBkMode(TRANSPARENT);
	}
	
	// TODO: Return a different brush if the default is not desired
	return hbr;
}

void CGameRoomDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	
	CDC MemDC;
	CBitmap background;
	MemDC.CreateCompatibleDC(&dc);
	background.LoadBitmap(IDB_GAMEROOM);

	MemDC.SelectObject(&background);

	BITMAP bmp;
	background.GetBitmap(&bmp);

	dc.BitBlt(0, 0, bmp.bmWidth, bmp.bmHeight, &MemDC, 0, 0, SRCCOPY);

	CFont font;
	font.CreatePointFont(90, "굴림체");

	CFont *pOldFont = dc.SelectObject(&font);

	dc.SetBkMode(TRANSPARENT);

	CString str;
	str.Format("아이디 : %s", userinfo.id);
	dc.TextOut(353, 26, str);

	str.Format("점  수 : %d", userinfo.score);
	dc.TextOut(353, 46, str);

	str.Format("전  적 : %d/%d/%d/%d", userinfo.playcount, userinfo.victorycount, userinfo.drawcount, userinfo.losscount);
	dc.TextOut(353, 66, str);

	dc.SelectObject(pOldFont);

	font.DeleteObject();
	
	// Do not call CDialog::OnPaint() for painting messages
}

BOOL CGameRoomDlg::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	if(pWnd == &m_ctrlRefresh || pWnd == &m_ctrlEnterRoom 
		|| pWnd == &m_ctrlOfferGame || pWnd == &m_ctrlPersonalInfo 
		|| pWnd == & m_ctrlChgChannel || pWnd == &m_ctrlExit)
	{
		SetCursor(AfxGetApp()->LoadCursor(IDC_HAND));
		return TRUE;
	}

	return CDialog::OnSetCursor(pWnd, nHitTest, message);
}
