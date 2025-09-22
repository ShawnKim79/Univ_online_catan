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
		
	if(m_nImgNormal || m_nImgOver || m_nImgClicked || m_nImgDisabled) // 이미지가 하나라도 등록되었을 경우
	{
		CDC MemDC;
		CBitmap BtnImg;
		
		switch(m_nBtnState) // 버튼 상태에 따른 비트맵 이미지를 선택
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

	else // 이미지 등록이 없을 경우
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
				
				lpDIS->rcItem.right+=2; // 글자가 눌린 효과를 내기위해
				lpDIS->rcItem.bottom+=2;
				
				pDC->DrawText(caption, &lpDIS->rcItem, DT_SINGLELINE|DT_VCENTER|DT_CENTER);
				break;

			case BS_DISABLED : pDC->Draw3dRect(0, 0, lpDIS->rcItem.right, lpDIS->rcItem.bottom, GetSysColor(COLOR_3DSHADOW), GetSysColor(COLOR_3DSHADOW));

				RECT rect = lpDIS->rcItem;

				pDC->SetTextColor(GetSysColor(COLOR_HIGHLIGHTTEXT));
				pDC->DrawText(caption, &lpDIS->rcItem, DT_SINGLELINE|DT_VCENTER|DT_CENTER);

				rect.right -= 2; // Disable된 텍스트 출력
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
	// 리소스 아이디를 버튼 클레스의 맴버 변수에 등록한다.
	m_nImgNormal = ResourceIDNormal;
	m_nImgClicked = !ResourceIDSel ? m_nImgNormal : ResourceIDSel;
	m_nImgOver = !ResourceIDOver ? m_nImgNormal : ResourceIDOver;
	m_nImgDisabled = !ResourceIDDisable ? m_nImgNormal : ResourceIDDisable;
}

void CImgButton::OnMouseMove(UINT nFlags, CPoint point) 
{
	if(GetStyle() & WS_DISABLED) m_nBtnState = BS_DISABLED;

	SetTimer(0, 10, NULL); // 마우스가 버튼의 위에 있는지를 판단하기 위해 타이머 작동

	CButton::OnMouseMove(nFlags, point);
}

void CImgButton::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if(m_nBtnState != BS_DISABLED) // 버튼의 상태가 Disabled가 아니면 
	{
		m_nBtnState = BS_CLICK; // 버튼의 상태는 눌린 상태임
		m_bClicked = TRUE;

		InvalidateRect(NULL, FALSE);
	}

	CButton::OnLButtonDown(nFlags, point);
}

void CImgButton::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if(m_bClicked)
	{
		CPoint p; // 마우스 커서의 위치
		CRect rcButton; // 버튼의 영역 (스크린 영역)
		
		GetCursorPos(&p); // 마우스 커서의 위치 얻어옴
		GetWindowRect(rcButton); // 버튼의 영역 얻어옴
		
		if(rcButton.PtInRect(p)) // 마우스 커서가 버튼 위에서 놓인 경우
			m_nBtnState = BS_OVER;
		
		else m_nBtnState = BS_NORMAL;
		
		m_bClicked = FALSE; // 버튼클릭 해제됨
		InvalidateRect(NULL, FALSE);
	}

	CButton::OnLButtonUp(nFlags, point);
}

void CImgButton::OnTimer(UINT nIDEvent) 
{
	static bRedraw = TRUE; // 한번만 그려주기 위해, 계속그리면 깜박임

	CPoint p; // 마우스 커서의 위치
	CRect rcButton; // 버튼의 영역 (스크린 영역)

	GetCursorPos(&p); // 마우스 커서의 위치 얻어옴
	GetWindowRect(rcButton); // 버튼의 영역 얻어옴

	if(!rcButton.PtInRect(p)) // 마우스 커서가 버튼 위를 벋어난 겨우
	{
		KillTimer(0); // 타이머 정지
		if(m_bClicked) m_nBtnState = BS_OVER; // 버튼의 상태는 노말상태로
		else
		{
			if(m_nBtnState != BS_DISABLED) m_nBtnState = BS_NORMAL;
		}
		bRedraw = TRUE; // 다시그리기 활성화

		InvalidateRect(NULL, FALSE);
	}

	else // 마우스 커서가 버튼 위에 있는 경우
	{
		if(bRedraw) // 다시그리기가 활성화된 경우에만 그림
		{
			if(m_bClicked) m_nBtnState = BS_CLICK; // 버튼이 눌렸던 상태에서 다시 올라왔을 경우에는 눌린상태를 그림
			else m_nBtnState = BS_OVER; // 그렇지 않을 경우에는 올라온 상태 그림
			bRedraw = FALSE; // 다시그리기 비활성화

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
