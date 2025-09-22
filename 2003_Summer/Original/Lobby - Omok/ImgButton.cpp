// ImgButton.cpp : implementation file
//

#include "stdafx.h"
#include "Lobby.h"
#include "ImgButton.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CImgButton

CImgButton::CImgButton()
{
	m_nImgNormal = 0;
	m_nImgClicked = 0;
	m_nImgOver = 0;
	m_nImgDisabled = 0;

	m_bClicked = FALSE;

	m_clrLeftTop = GetSysColor(COLOR_BTNFACE);
	m_clrRightBottom = GetSysColor(COLOR_BTNFACE);
}

CImgButton::~CImgButton()
{
}


BEGIN_MESSAGE_MAP(CImgButton, CButton)
	//{{AFX_MSG_MAP(CImgButton)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_TIMER()
	ON_WM_ENABLE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CImgButton message handlers

void CImgButton::DrawItem(LPDRAWITEMSTRUCT lpDIS) 
{
	CDC *pDC = CDC::FromHandle(lpDIS->hDC);
		
	if(m_nImgNormal || m_nImgOver || m_nImgClicked || m_nImgDisabled) // �̹����� �ϳ��� ��ϵǾ��� ���
	{
		CDC MemDC;
		CBitmap BtnImg;
		
		switch(m_nBtnState) // ��ư ���¿� ���� ��Ʈ�� �̹����� ����
		{
			case BS_NORMAL : BtnImg.LoadBitmap(m_nImgNormal); break;
			case BS_OVER : BtnImg.LoadBitmap(m_nImgOver);	break;
			case BS_CLICK : BtnImg.LoadBitmap(m_nImgClicked); break;
			case BS_DISABLED : BtnImg.LoadBitmap(m_nImgDisabled); break;
		}
		
		MemDC.CreateCompatibleDC(pDC);
		CBitmap *pOld = (CBitmap *)MemDC.SelectObject(&BtnImg);
		
		BITMAP bmp;
		BtnImg.GetBitmap(&bmp);
		
		SetWindowPos(NULL, 0, 0, bmp.bmWidth, bmp.bmHeight, SWP_NOZORDER|SWP_NOMOVE);
		
		pDC->BitBlt(0, 0, bmp.bmWidth, bmp.bmHeight, &MemDC, 0, 0, SRCCOPY);
		BtnImg.DeleteObject();
	}

	else // �̹��� ����� ���� ���
	{
		CString caption;
		GetWindowText(caption);

		pDC->FillSolidRect(&lpDIS->rcItem, GetSysColor(COLOR_BTNFACE));
		pDC->SetBkMode(TRANSPARENT);

		switch(m_nBtnState)
		{
			case BS_NORMAL : pDC->Draw3dRect(0, 0, lpDIS->rcItem.right, lpDIS->rcItem.bottom, m_clrLeftTop, m_clrRightBottom);
				pDC->DrawText(caption, &lpDIS->rcItem, DT_SINGLELINE|DT_VCENTER|DT_CENTER);
				break;

			case BS_OVER : pDC->Draw3dRect(0, 0, lpDIS->rcItem.right, lpDIS->rcItem.bottom, GetSysColor(COLOR_3DHILIGHT), GetSysColor(COLOR_3DSHADOW));
				pDC->DrawText(caption, &lpDIS->rcItem, DT_SINGLELINE|DT_VCENTER|DT_CENTER);
				break;

			case BS_CLICK : 

				pDC->Draw3dRect(0, 0, lpDIS->rcItem.right, lpDIS->rcItem.bottom, GetSysColor(COLOR_3DSHADOW), GetSysColor(COLOR_3DHILIGHT));
				
				lpDIS->rcItem.right+=2; // ���ڰ� ���� ȿ���� ��������
				lpDIS->rcItem.bottom+=2;
				
				pDC->DrawText(caption, &lpDIS->rcItem, DT_SINGLELINE|DT_VCENTER|DT_CENTER);
				break;

			case BS_DISABLED : pDC->Draw3dRect(0, 0, lpDIS->rcItem.right, lpDIS->rcItem.bottom, GetSysColor(COLOR_3DSHADOW), GetSysColor(COLOR_3DSHADOW));

				RECT rect = lpDIS->rcItem;

				pDC->SetTextColor(GetSysColor(COLOR_HIGHLIGHTTEXT));
				pDC->DrawText(caption, &lpDIS->rcItem, DT_SINGLELINE|DT_VCENTER|DT_CENTER);

				rect.right -= 2; // Disable�� �ؽ�Ʈ ���
				rect.bottom -= 2;

				pDC->SetTextColor(GetSysColor(COLOR_GRAYTEXT));
				pDC->DrawText(caption, &rect, DT_SINGLELINE|DT_VCENTER|DT_CENTER);

				break;
		}
	}
}

void CImgButton::PreSubclassWindow() 
{
	ModifyStyle(0, BS_OWNERDRAW);

	if(GetStyle() & WS_DISABLED) m_nBtnState = BS_DISABLED;
	else m_nBtnState = BS_NORMAL;

	m_ToolTip.Create(this);

	CButton::PreSubclassWindow();
}

void CImgButton::SetImages(UINT ResourceIDNormal, UINT ResourceIDSel, UINT ResourceIDOver, UINT ResourceIDDisable)
{
	// ���ҽ� ���̵� ��ư Ŭ������ �ɹ� ������ ����Ѵ�.
	m_nImgNormal = ResourceIDNormal;
	m_nImgClicked = !ResourceIDSel ? m_nImgNormal : ResourceIDSel;
	m_nImgOver = !ResourceIDOver ? m_nImgNormal : ResourceIDOver;
	m_nImgDisabled = !ResourceIDDisable ? m_nImgNormal : ResourceIDDisable;
}

void CImgButton::OnMouseMove(UINT nFlags, CPoint point) 
{
	if(GetStyle() & WS_DISABLED) m_nBtnState = BS_DISABLED;

	SetTimer(0, 10, NULL); // ���콺�� ��ư�� ���� �ִ����� �Ǵ��ϱ� ���� Ÿ�̸� �۵�

	CButton::OnMouseMove(nFlags, point);
}

void CImgButton::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if(m_nBtnState != BS_DISABLED) // ��ư�� ���°� Disabled�� �ƴϸ� 
	{
		m_nBtnState = BS_CLICK; // ��ư�� ���´� ���� ������
		m_bClicked = TRUE;

		InvalidateRect(NULL, FALSE);
	}

	CButton::OnLButtonDown(nFlags, point);
}

void CImgButton::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if(m_bClicked)
	{
		CPoint p; // ���콺 Ŀ���� ��ġ
		CRect rcButton; // ��ư�� ���� (��ũ�� ����)
		
		GetCursorPos(&p); // ���콺 Ŀ���� ��ġ ����
		GetWindowRect(rcButton); // ��ư�� ���� ����
		
		if(rcButton.PtInRect(p)) // ���콺 Ŀ���� ��ư ������ ���� ���
			m_nBtnState = BS_OVER;
		
		else m_nBtnState = BS_NORMAL;
		
		m_bClicked = FALSE; // ��ưŬ�� ������
		InvalidateRect(NULL, FALSE);
	}

	CButton::OnLButtonUp(nFlags, point);
}

void CImgButton::OnTimer(UINT nIDEvent) 
{
	static bRedraw = TRUE; // �ѹ��� �׷��ֱ� ����, ��ӱ׸��� ������

	CPoint p; // ���콺 Ŀ���� ��ġ
	CRect rcButton; // ��ư�� ���� (��ũ�� ����)

	GetCursorPos(&p); // ���콺 Ŀ���� ��ġ ����
	GetWindowRect(rcButton); // ��ư�� ���� ����

	if(!rcButton.PtInRect(p)) // ���콺 Ŀ���� ��ư ���� ��� �ܿ�
	{
		KillTimer(0); // Ÿ�̸� ����
		if(m_bClicked) m_nBtnState = BS_OVER; // ��ư�� ���´� �븻���·�
		else
		{
			if(m_nBtnState != BS_DISABLED) m_nBtnState = BS_NORMAL;
		}
		bRedraw = TRUE; // �ٽñ׸��� Ȱ��ȭ

		InvalidateRect(NULL, FALSE);
	}

	else // ���콺 Ŀ���� ��ư ���� �ִ� ���
	{
		if(bRedraw) // �ٽñ׸��Ⱑ Ȱ��ȭ�� ��쿡�� �׸�
		{
			if(m_bClicked) m_nBtnState = BS_CLICK; // ��ư�� ���ȴ� ���¿��� �ٽ� �ö���� ��쿡�� �������¸� �׸�
			else m_nBtnState = BS_OVER; // �׷��� ���� ��쿡�� �ö�� ���� �׸�
			bRedraw = FALSE; // �ٽñ׸��� ��Ȱ��ȭ

			InvalidateRect(NULL, FALSE);
		}
	}

	CButton::OnTimer(nIDEvent);
}

void CImgButton::OnEnable(BOOL bEnable) 
{
	CButton::OnEnable(bEnable);
	
	if(m_nBtnState == BS_DISABLED && bEnable)
	{
		m_nBtnState = BS_NORMAL;
		InvalidateRect(NULL, FALSE);
	}

	else if(!bEnable)
	{
		m_nBtnState = BS_DISABLED;
		InvalidateRect(NULL, FALSE);
	}
}

void CImgButton::SetToolTip(UINT nID)
{
	CString tip;
	tip.LoadString(nID);

	SetToolTip(tip);
}

void CImgButton::SetToolTip(CString strTip)
{
	CRect rect;
	GetClientRect(rect);

	m_ToolTip.AddTool(this, strTip, &rect, 1);
	m_ToolTip.UpdateTipText(strTip, this, 1);
	m_ToolTip.Activate(TRUE);
}

BOOL CImgButton::PreTranslateMessage(MSG* pMsg) 
{
	m_ToolTip.RelayEvent(pMsg);

	return CButton::PreTranslateMessage(pMsg);
}
