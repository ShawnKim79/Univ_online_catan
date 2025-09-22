// ChannelDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Lobby.h"
#include "ChannelDlg.h"
#include "LobbyDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CChannelDlg dialog


CChannelDlg::CChannelDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CChannelDlg::IDD, pParent)
{
	m_pParent = (CLobbyDlg *)pParent;
	//{{AFX_DATA_INIT(CChannelDlg)
	m_nTotalConnection = 0;
	//m_strID = _T("aluqard");
	//}}AFX_DATA_INIT
}


void CChannelDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CChannelDlg)
	DDX_Control(pDX, IDC_EXIT, m_ctrlExit);
	DDX_Control(pDX, IDC_ENTERCHANNEL, m_ctrlEnterChannel);
	DDX_Control(pDX, IDC_REFRESH, m_ctrlRefresh);
	DDX_Control(pDX, IDC_CHANNELLIST, m_ctrlChannelList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CChannelDlg, CDialog)
	//{{AFX_MSG_MAP(CChannelDlg)
	ON_BN_CLICKED(IDC_REFRESH, OnRefresh)
	ON_NOTIFY(NM_DBLCLK, IDC_CHANNELLIST, OnDblclkChannellist)
	ON_BN_CLICKED(IDC_ENTERCHANNEL, OnEnterchannel)
	ON_BN_CLICKED(IDC_EXIT, OnExit)
	ON_WM_CTLCOLOR()
	ON_WM_PAINT()
	ON_WM_SETCURSOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChannelDlg message handlers

void CChannelDlg::InsertColumn()
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
				lvcolumn.cx = 59;
                lvcolumn.pszText = _T("채널 #");
				break;

			case 1:
                lvcolumn.cx = 78;
                lvcolumn.pszText = _T("접속자 수");
				break;

			case 2:
                lvcolumn.cx = 90;
                lvcolumn.pszText = _T("최대접속자 수");
				break;
		}

		m_ctrlChannelList.InsertColumn(i, &lvcolumn);
	}
}

BOOL CChannelDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_ctrlRefresh.SetImages(IDB_REFRESH2NOR, IDB_REFRESH2CLK, IDB_REFRESH2OVER, IDB_REFRESH2NOR);
	m_ctrlEnterChannel.SetImages(IDB_ENTCHNOR, IDB_ENTCHCLK, IDB_ENTCHOVER, IDB_ENTCHNOR);
	m_ctrlExit.SetImages(IDB_EXIT2NOR, IDB_EXIT2CLK, IDB_EXIT2OVER, IDB_EXIT2NOR);

    m_ctrlChannelList.SetExtendedStyle(m_ctrlChannelList.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_FLATSB);

	InsertColumn();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CChannelDlg::AddChannelInfo(CHANNEL channel)
{
	LV_ITEM lvitem;

	char buff[5];

	lvitem.iItem = channel.channelno;

	wsprintf(buff, "%d", channel.channelno+1);

    lvitem.mask		= LVIF_TEXT;
	lvitem.iSubItem = 0;
    lvitem.pszText  = buff;

	m_ctrlChannelList.InsertItem(&lvitem);

	wsprintf(buff, "%d", channel.present);
	m_ctrlChannelList.SetItemText(lvitem.iItem, 1, buff);

	wsprintf(buff, "%d", channel.maximum);
	m_ctrlChannelList.SetItemText(lvitem.iItem, 2, buff);

	channellist.push_back(channel);

	m_nTotalConnection += channel.present;
}

void CChannelDlg::ClearChannelInfo()
{
	channellist.clear();
	m_ctrlChannelList.DeleteAllItems();
}

void CChannelDlg::OnRefresh()
{
	m_nTotalConnection = 0;

	m_ctrlRefresh.EnableWindow(FALSE);

	m_pParent->RequestChInfo();

	m_ctrlRefresh.EnableWindow();
}

void CChannelDlg::OnEnterchannel() 
{
	EnterSelectedChannel();
}

void CChannelDlg::OnDblclkChannellist(NMHDR* pNMHDR, LRESULT* pResult) 
{
	OnEnterchannel();
		
	*pResult = 0;
}

void CChannelDlg::EnterSelectedChannel()
{
	POSITION pos = m_ctrlChannelList.GetFirstSelectedItemPosition();
	
	CHANNEL channel;

	if(pos != NULL)
	{
		int nItem = m_ctrlChannelList.GetNextSelectedItem(pos);

		CString strChannelNo = m_ctrlChannelList.GetItemText(nItem, 0);
		CString strPresent = m_ctrlChannelList.GetItemText(nItem, 1);
		CString strMaximum = m_ctrlChannelList.GetItemText(nItem, 2);

		channel.channelno = atoi(strChannelNo) - 1;
		channel.present = atoi(strPresent);
		channel.maximum = atoi(strMaximum);
	}

	else
	{
		MessageBox("채널을 선택해주세요.", "알림", MB_OK | MB_ICONEXCLAMATION);
		return;
	}

	if(channel.present <= channel.maximum)
	{
	 	m_pParent->EnterChannel(channel.channelno);
	}

	else
	{
		MessageBox("선택하신 채널에 입장하실 수 없습니다.\n다른 채널을 선택해주세요.");
	}
}

BOOL CChannelDlg::PreTranslateMessage(MSG* pMsg) 
{
	if(pMsg->message == WM_KEYDOWN)
	{
		if(pMsg->wParam == VK_RETURN) 
		{
			EnterSelectedChannel();
			return TRUE;
		}

		else if(pMsg->wParam == VK_ESCAPE) return TRUE;
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}

void CChannelDlg::OnExit() 
{
	m_pParent->Exit();
}

HBRUSH CChannelDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	if(nCtlColor == CTLCOLOR_DLG)
		hbr = CreateSolidBrush(RGB(244, 244, 244));
	
	// TODO: Return a different brush if the default is not desired
	return hbr;
}

void CChannelDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CString str;

	CFont font;
	font.CreatePointFont(90, "굴림체");
	CFont *pOldFont = dc.SelectObject(&font);

	dc.SetBkMode(TRANSPARENT);
	
	str.Format("총 : %d명", m_nTotalConnection);
//	dc.TextOut(10, 348, str);

	dc.SelectObject(pOldFont);

	font.DeleteObject();
	
	// Do not call CDialog::OnPaint() for painting messages
}

BOOL CChannelDlg::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	if(pWnd == &m_ctrlRefresh || pWnd == &m_ctrlEnterChannel || pWnd == &m_ctrlExit)
	{
		SetCursor(AfxGetApp()->LoadCursor(IDC_HAND));
		return TRUE;
	}
	
	return CDialog::OnSetCursor(pWnd, nHitTest, message);
}
