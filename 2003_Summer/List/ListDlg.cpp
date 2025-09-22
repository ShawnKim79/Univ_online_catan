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

	LoadImageList();	// �̹��� ����Ʈ ����
	InsertColumn();		// �÷� ����
	LargeIconStyle();	// ����Ʈ ��Ʈ�� �Ӽ� ����
	AddSampleData();	// ���� ������ �߰�

	return TRUE;
}

void CListDlg::LoadImageList()
{
	// ū ������ ����
	m_LargeImageList.Create(IDB_LARGE, 32, 1, RGB(0, 128, 128));
	m_ctrlList.SetImageList(&m_LargeImageList, LVSIL_NORMAL);

	// ���� ������ ����
	m_SmallImageList.Create(IDB_SMALL, 16, 1, RGB(0, 128, 128));
	m_ctrlList.SetImageList(&m_SmallImageList, LVSIL_SMALL);
}

void CListDlg::InsertColumn( )
{
	CRect rect;
	m_ctrlList.GetClientRect(&rect);

	// �÷��� ���� ������ �����ϴ� ����ü
	LV_COLUMN lvcolumn;

	// ������ �׸���� �����
	lvcolumn.mask = LVCF_FMT|LVCF_SUBITEM|LVCF_TEXT|LVCF_WIDTH;

	// �÷� ����� �߾ӿ� ����ϵ��� ��
	lvcolumn.fmt = LVCFMT_CENTER;

	for(int i = 0 ; i < 4 ; i++ )
	{
		lvcolumn.iSubItem = i;
		switch(i)
		{
		case 0:	// ù��° �÷� ���� ����
			lvcolumn.cx = 100;
			lvcolumn.pszText = "�̸�";
			break;
		case 1:	// �ι�° �÷� ���� ����
			lvcolumn.cx = 60;
			lvcolumn.pszText = "����";
			break;
		case 2:	// ����° �÷� ���� ����
			lvcolumn.cx = 60;
			lvcolumn.pszText = "����";
			break;
		case 3:	// �׹�° �÷� ���� ����
			lvcolumn.cx = rect.Width()-220;
			lvcolumn.pszText = "��ȭ��ȣ";
			break;
		}
		m_ctrlList.InsertColumn(i, &lvcolumn);	// �÷� �߰�
	}
}

// ū ���������� ����
void CListDlg::LargeIconStyle()
{
	m_ctrlList.ModifyStyle(LVS_TYPEMASK, LVS_ICON);
}

// ���� ���������� ����
void CListDlg::SmallIconStyle()
{
	m_ctrlList.ModifyStyle(LVS_TYPEMASK, LVS_SMALLICON);
}

// �ڼ��� ����
void CListDlg::Detail()
{
	m_ctrlList.ModifyStyle(LVS_TYPEMASK,LVS_LIST);
}

// ������� ����
void CListDlg::List()
{
	m_ctrlList.ModifyStyle(LVS_TYPEMASK,LVS_REPORT);
}

void CListDlg::OnSelchangeCombo1( ) 
{
	// �޺� �ڽ��� ����� ���� ������ �˾Ƴ�
	UpdateData();

	// �޺� �ڽ��� ���õ� ���뿡 ����, �Լ��� ȣ��
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

	// ���ο� �� ���
	m_ctrlList.InsertItem(&lvitem); 
	
	// �� �÷��� �ؽ�Ʈ ����
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

	// ��� �׸��� �����Ͽ�
	for(i=count-1 ; i>=0 ; i--)
	{
		lvitem.iItem = i;
		if(m_ctrlList.GetItemState(i, LVIS_SELECTED) != 0)
			// ���õ� �׸��� ����
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
			m_strSex = "����";
		else
			m_strSex = "����";
		m_nImage = rand()%5;

		AddItem();
	}
}

char *sampleName[] = {"��뼺", "������", "�̻�", "������", "Ȳ����", "������", "Ȳ����", "�ۻ�ȣ", "������", "�ѱ���"};

void CListDlg::AddSampleData()
{
	for(int i=0 ; i<10 ; i++)
	{
		m_strName = sampleName[i];
		m_strPhone.Format("%d%d%d - %d%d%d%d", 
			rand()%10,rand()%10,rand()%10,rand()%10,rand()%10,rand()%10,rand()%10);
		m_strAge.Format("%d", rand()%100);
		if(rand()%2 == 0)
			m_strSex = "����";
		else
			m_strSex = "����";
		m_nImage = rand()%5;

		AddItem();
	}
}

