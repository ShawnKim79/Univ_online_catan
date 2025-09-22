// ListDlg.cpp : implementation file
//

#include "stdafx.h"
#include "List.h"
#include "ListDlg.h"
#include "ItemInputDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CListDlg dialog
#define NUM_ITEMS	5

CListDlg::CListDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CListDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CListDlg)
	m_nStyle = 0;
	//}}AFX_DATA_INIT
}


void CListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CListDlg)
	DDX_Control(pDX, IDC_LIST1, m_ctrlList);
	DDX_CBIndex(pDX, IDC_COMBO1, m_nStyle);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CListDlg, CDialog)
	//{{AFX_MSG_MAP(CListDlg)
	ON_CBN_SELCHANGE(IDC_COMBO1, OnSelchangeCombo1)
	ON_BN_CLICKED(ID_DELETEITEM, OnDeleteitem)
	ON_BN_CLICKED(ID_ADDITEM, OnAdditem)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CListDlg message handlers

BOOL CListDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	LoadImageList();	// 이미지 리스트 생성
	InsertColumn();		// 컬럼 생성
	LargeIconStyle();	// 리스트 컨트롤 속성 설정
	AddSampleData();	// 샘플 데이터 추가

	return TRUE;
}

void CListDlg::LoadImageList()
{
	// 큰 아이콘 설정
	m_LargeImageList.Create(IDB_LARGE, 32, 1, RGB(0, 128, 128));
	m_ctrlList.SetImageList(&m_LargeImageList, LVSIL_NORMAL);

	// 작은 아이콘 설정
	m_SmallImageList.Create(IDB_SMALL, 16, 1, RGB(0, 128, 128));
	m_ctrlList.SetImageList(&m_SmallImageList, LVSIL_SMALL);
}

void CListDlg::InsertColumn( )
{
	CRect rect;
	m_ctrlList.GetClientRect(&rect);

	// 컬럼에 대한 정보를 저장하는 구조체
	LV_COLUMN lvcolumn;

	// 설정할 항목들을 명시함
	lvcolumn.mask = LVCF_FMT|LVCF_SUBITEM|LVCF_TEXT|LVCF_WIDTH;

	// 컬럼 헤더를 중앙에 출력하도록 함
	lvcolumn.fmt = LVCFMT_CENTER;

	for(int i = 0 ; i < 4 ; i++ )
	{
		lvcolumn.iSubItem = i;
		switch(i)
		{
		case 0:	// 첫번째 컬럼 정보 설정
			lvcolumn.cx = 100;
			lvcolumn.pszText = "이름";
			break;
		case 1:	// 두번째 컬럼 정보 설정
			lvcolumn.cx = 60;
			lvcolumn.pszText = "나이";
			break;
		case 2:	// 세번째 컬럼 정보 설정
			lvcolumn.cx = 60;
			lvcolumn.pszText = "성별";
			break;
		case 3:	// 네번째 컬럼 정보 설정
			lvcolumn.cx = rect.Width()-220;
			lvcolumn.pszText = "전화번호";
			break;
		}
		m_ctrlList.InsertColumn(i, &lvcolumn);	// 컬럼 추가
	}
}

// 큰 아이콘으로 보기
void CListDlg::LargeIconStyle()
{
	m_ctrlList.ModifyStyle(LVS_TYPEMASK, LVS_ICON);
}

// 작은 아이콘으로 보기
void CListDlg::SmallIconStyle()
{
	m_ctrlList.ModifyStyle(LVS_TYPEMASK, LVS_SMALLICON);
}

// 자세히 보기
void CListDlg::Detail()
{
	m_ctrlList.ModifyStyle(LVS_TYPEMASK,LVS_LIST);
}

// 목록으로 보기
void CListDlg::List()
{
	m_ctrlList.ModifyStyle(LVS_TYPEMASK,LVS_REPORT);
}

void CListDlg::OnSelchangeCombo1( ) 
{
	// 콤보 박스의 변경된 선택 사항을 알아냄
	UpdateData();

	// 콤보 박스에 선택된 내용에 따라, 함수를 호출
	switch(m_nStyle)
	{
		case 0: LargeIconStyle(); break;
		case 1: SmallIconStyle(); break;
		case 2: Detail(); break;
		case 3: List(); break;
	}
}

void CListDlg::AddItem( )
{
	LV_ITEM lvitem;

	lvitem.iItem = m_ctrlList.GetItemCount();

	lvitem.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_STATE;
	lvitem.iSubItem = 0;
	lvitem.iImage = m_nImage;
	lvitem.stateMask = LVIS_STATEIMAGEMASK;
	lvitem.state = INDEXTOSTATEIMAGEMASK(1);
	lvitem.pszText = (LPSTR)(LPCTSTR)m_strName;

	// 새로운 행 등록
	m_ctrlList.InsertItem(&lvitem); 
	
	// 각 컬럼의 텍스트 설정
	m_ctrlList.SetItemText(lvitem.iItem, 1, m_strAge);
	m_ctrlList.SetItemText(lvitem.iItem, 2, m_strSex);
	m_ctrlList.SetItemText(lvitem.iItem, 3, m_strPhone);
}

void CListDlg::OnDeleteitem() 
{
	LV_ITEM lvitem;
	int count = m_ctrlList.GetItemCount();
	int i;
	lvitem.mask = LVIF_STATE;

	// 모든 항목을 조사하여
	for(i=count-1 ; i>=0 ; i--)
	{
		lvitem.iItem = i;
		if(m_ctrlList.GetItemState(i, LVIS_SELECTED) != 0)
			// 선택된 항목을 삭제
			m_ctrlList.DeleteItem(i);
	}
}


void CListDlg::OnAdditem() 
{
	CItemInputDlg dlg;
	if(dlg.DoModal() == IDOK)
	{
		m_strName = dlg.m_strName;
		m_strPhone = dlg.m_strPhone;
		m_strAge.Format("%d", dlg.m_nAge);
		if(dlg.m_nSex == 0)
			m_strSex = "남자";
		else
			m_strSex = "여자";
		m_nImage = rand()%5;

		AddItem();
	}
}

char *sampleName[] = {"김용성", "최정식", "이상엽", "추현곤", "황선규", "이정수", "황병진", "송상호", "유광석", "한기태"};

void CListDlg::AddSampleData()
{
	for(int i=0 ; i<10 ; i++)
	{
		m_strName = sampleName[i];
		m_strPhone.Format("%d%d%d - %d%d%d%d", 
			rand()%10,rand()%10,rand()%10,rand()%10,rand()%10,rand()%10,rand()%10);
		m_strAge.Format("%d", rand()%100);
		if(rand()%2 == 0)
			m_strSex = "남자";
		else
			m_strSex = "여자";
		m_nImage = rand()%5;

		AddItem();
	}
}

