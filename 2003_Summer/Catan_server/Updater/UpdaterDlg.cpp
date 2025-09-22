// UpdaterDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Updater.h"
#include "UpdaterDlg.h"
#include "process.h"
#include "wininet.h"
#include "mmsystem.h"
#include "afxmt.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CEvent g_Event;

/////////////////////////////////////////////////////////////////////////////
// CUpdaterDlg dialog


CUpdaterDlg::CUpdaterDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CUpdaterDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CUpdaterDlg)
	m_strPath = _T("C:\\HGRGame");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_strTitle.LoadString(IDS_GAMETITLE);
}

void CUpdaterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CUpdaterDlg)
	DDX_Control(pDX, IDC_ALLFILES, m_ctrlAllFiles);
	DDX_Control(pDX, IDC_CURRENTFILE, m_ctrlCurFile);
	DDX_Control(pDX, IDC_SELECTFOLDER, m_ctrlFolder);
	DDX_Control(pDX, IDC_INSTALL, m_ctrlInstall);
	DDX_Text(pDX, IDC_PATH, m_strPath);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CUpdaterDlg, CDialog)
	//{{AFX_MSG_MAP(CUpdaterDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_INSTALL, OnInstall)
	ON_BN_CLICKED(IDC_SELECTFOLDER, OnSelectfolder)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUpdaterDlg message handlers

BOOL CUpdaterDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	m_strPath += "\\" + m_strGameType;

	UpdateData(FALSE);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CUpdaterDlg::OnPaint() 
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
HCURSOR CUpdaterDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CUpdaterDlg::OnSelectfolder() 
{
	CString strPath;
	CString strYourCaption(_T("폴더 선택"));
	CString strYourTitle(_T("설치할 폴더를 선택하세요."));

	UpdateData();

	CPathDialog dlg(strYourCaption, strYourTitle, "C:\\");

	if(dlg.DoModal() == IDOK)
	{
		CString strPathName = dlg.GetPathName();

		if(strPathName.Right(1) == "\\") 
			m_strPath = strPathName;// + "HGRGame\\";

		else m_strPath = dlg.GetPathName();// + "\\HGRGame";

		m_strPath += "\\" + m_strGameType;

		UpdateData(FALSE);
	}
}

void CUpdaterDlg::OnInstall() 
{
	UpdateData();

	GetDlgItem(IDC_STATIC1)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC2)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC3)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC4)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_PATH)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_SELECTFOLDER)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_INSTALL)->ShowWindow(SW_HIDE);

	GetDlgItem(IDC_STATIC11)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_STATIC12)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_STATIC13)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_CURRENTFILE)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_ALLFILES)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_CURFILE)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_TIMEREMAINED)->ShowWindow(SW_SHOW);
//	GetDlgItem(IDC_TRANSFERSPEED)->ShowWindow(SW_SHOW);

	AfxBeginThread(threadDownload, this);
}


UINT CUpdaterDlg::threadDownload(LPVOID arg)
{
	CUpdaterDlg *pDlg = (CUpdaterDlg *)arg;

	HINTERNET hInternet, hFtp;

	if(pDlg->m_strPath.ReverseFind('\\') != pDlg->m_strPath.GetLength() - 1)
		pDlg->m_strPath += "\\";

	CString strRemainedFolder = pDlg->m_strPath.Right(pDlg->m_strPath.GetLength() - pDlg->m_strPath.Find('\\') - 1);
	CString strFolderName = pDlg->m_strPath.Left(pDlg->m_strPath.Find('\\'));
	strFolderName += "\\";
	CString strCurFolder = strFolderName;

	while(strRemainedFolder.GetLength())
	{
		strCurFolder += strRemainedFolder.Left(strRemainedFolder.Find('\\')) + "\\";
		strRemainedFolder = strRemainedFolder.Right(strRemainedFolder.GetLength() - strRemainedFolder.Find('\\') - 1);
		::CreateDirectory(strCurFolder, NULL);
	}

	::SetCurrentDirectory(pDlg->m_strPath);


	// 인터넷 세션 열기
	hInternet = InternetOpen("TESTFTP", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	if(hInternet == NULL)
	{
		pDlg->MessageBox("게임 다운로드에 실패했습니다.");
		return 0;
	}

	// FTP 서버에 연결
	hFtp = InternetConnect(hInternet, "203.230.107.39", INTERNET_DEFAULT_FTP_PORT, "download", "ddrnow", INTERNET_SERVICE_FTP, 0, 0);
	if(hFtp == NULL)
	{
		InternetCloseHandle(hInternet);
		pDlg->MessageBox("게임 다운로드에 실패했습니다.");
		return 0;
	}

	CString strFileList = pDlg->m_strPath + "\\filelist.log";
	CString strRemoteFileList = "\\filelist.log";

	BOOL bSucceed = FtpGetFile(hFtp, strRemoteFileList, strFileList, FALSE, FILE_ATTRIBUTE_NORMAL, FTP_TRANSFER_TYPE_BINARY | INTERNET_FLAG_RELOAD, 0);

	AfxMessageBox(strFileList);

	FILE *fp;
	int nNumofFile = 0;
	int nTotalSize = 0;
	DWORD dwTotalBytes = 0;
	char filename[255] = {0, };
	char version[255] = {0, };

	if((fp = fopen(strFileList, "rt")) == NULL)
	{
		pDlg->MessageBox("게임을 설치할 수 없습니다.\nfilelist.log 열기 실패", "오류");
		return 0;
	}

	fscanf(fp, "%d", &nNumofFile);
	fscanf(fp, "%d", &nTotalSize);

	pDlg->m_ctrlAllFiles.SetRange32(0, nTotalSize);

	for(int i = 0 ; i < nNumofFile ; i++)
	{
		HINTERNET hRemote;
		HGLOBAL hLocal;
		DWORD dwRead = 0, dwWritten, Prog = 0;
		DWORD dwFileSize;
		DWORD dwMaxSize = ULONG_MAX;
		BOOL bResult;
		char buff[1000];

		// 이부분을 수정할 것 -> 2중의 파일 경로에도 대응할 수 있도록..
		fscanf(fp, "%s", filename);
		CString strFilename = filename; // 파일 네임 얻어오기.

		CString strSubFolder = strFilename.Left(strFilename.Find("*"));
		// 파일 네임 이전에 붙은 \\ 찾는다....이게 있으면 그 뒤로는 파일 이름.

		if(strSubFolder.GetLength()) strSubFolder += "\\";
		// 서브 폴더 못찾으면 \\ 붙여준다....(파일 생성 위해)

		::CreateDirectory(strSubFolder, NULL);
		

		fscanf(fp, "%d", &dwFileSize);
//		fscanf(fp, "%s", version);

		strFilename.Remove('*');
/*
		CString str("This is a test.");
		int n = str.Remove('t');
		ASSERT(n == 2);
		ASSERT(str == "This is a es.");
*/
		CString strFilePath = pDlg->m_strGameType + "\\" + strFilename;

		hRemote = FtpOpenFile(hFtp, strFilePath, GENERIC_READ, FTP_TRANSFER_TYPE_BINARY, 0);

		pDlg->m_ctrlCurFile.SetRange32(0, dwFileSize);

		if(hRemote == NULL)
		{
			fclose(fp);
			InternetCloseHandle(hFtp);
			InternetCloseHandle(hInternet);
		}

		CString strNewFile = pDlg->m_strPath + strFilename;

		hLocal = CreateFile(strNewFile, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		pDlg->GetDlgItem(IDC_CURFILE)->SetWindowText("수신 파일 : " + strNewFile);

		while(1)
		{
			DWORD dwTime1 = timeGetTime();
			DWORD dwTime2;

			bResult = InternetReadFile(hRemote, buff, 1000, &dwRead);
			if((bResult == TRUE) && (dwRead == 0)) break; // 끝까지 다 받았슴

			WriteFile(hLocal, buff, dwRead, &dwWritten, NULL);

			dwTime2 = timeGetTime();

			DWORD dwTransferTime = dwTime2 - dwTime1;

			Prog += dwWritten;
			dwTotalBytes += dwWritten;
			pDlg->m_ctrlCurFile.SetPos(Prog);
			pDlg->m_ctrlAllFiles.SetPos(dwTotalBytes);

			CString strRemained;
			int nRemained = nTotalSize / ((float)(dwTransferTime) / 1000.0f);
			float fTransfered = ((float)dwTotalBytes) / 1000000.0f;
//			int nTransferRate = 1000 / dwTransferTime;
			strRemained.Format("수신량 : [ %3.1fMB 받음 ]", fTransfered);

			pDlg->GetDlgItem(IDC_TIMEREMAINED)->SetWindowText(strRemained);
		}

		CloseHandle(hLocal);
		InternetCloseHandle(hRemote);
	}

	fclose(fp);

	HKEY hKey;
	DWORD dwDisp;

	CString strRootKey = "SOFTWARE\\HGRGame\\" + pDlg->m_strGameType + "\\Setting";

	RegCreateKeyEx(HKEY_CURRENT_USER, strRootKey, 0, NULL,
		REG_OPTION_NON_VOLATILE, KEY_SET_VALUE, NULL, &hKey, &dwDisp);

	RegSetValueEx(hKey, "installedpath", 0, REG_SZ, (LPBYTE)(LPCTSTR)pDlg->m_strPath, pDlg->m_strPath.GetLength());

	CString strParam;
	strParam.Format("%s %s", pDlg->m_strID, pDlg->m_strGameType);

	ShellExecute(NULL, "open", pDlg->m_strPath + "\\Lobby.exe", strParam, pDlg->m_strPath, SW_SHOW);

	pDlg->OnOK();

	return 0;
}

BOOL CUpdaterDlg::PreTranslateMessage(MSG* pMsg) 
{
	if(pMsg->message == WM_KEYDOWN)
	{
		if(pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE) return TRUE;
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}
