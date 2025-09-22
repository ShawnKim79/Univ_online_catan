// StarterCtrl.h : Declaration of the CStarterCtrl

#ifndef __STARTERCTRL_H_
#define __STARTERCTRL_H_

#include "resource.h"       // main symbols
#include <atlctl.h>
#include "wininet.h"


#define SERVER		"203.230.107.39"
#define USERID		"download"
#define PASSWD		"ddrnow"


/////////////////////////////////////////////////////////////////////////////
// CStarterCtrl
class ATL_NO_VTABLE CStarterCtrl : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public IDispatchImpl<IStarterCtrl, &IID_IStarterCtrl, &LIBID_STARTERCOMPLib>,
	public CComControl<CStarterCtrl>,
	public IPersistStreamInitImpl<CStarterCtrl>,
	public IOleControlImpl<CStarterCtrl>,
	public IOleObjectImpl<CStarterCtrl>,
	public IOleInPlaceActiveObjectImpl<CStarterCtrl>,
	public IViewObjectExImpl<CStarterCtrl>,
	public IOleInPlaceObjectWindowlessImpl<CStarterCtrl>,
	public IConnectionPointContainerImpl<CStarterCtrl>,
	public CComCoClass<CStarterCtrl, &CLSID_StarterCtrl>,
	public IObjectSafetyImpl<CStarterCtrl, INTERFACESAFE_FOR_UNTRUSTED_CALLER|INTERFACESAFE_FOR_UNTRUSTED_DATA>
{

	
public:
	// DLL로 패키징한 파일이 압축이 풀리고 시스템에 깔리게 되어, 
	// 웹브라우저에서 ASP 코드로 해당 파일의 인스턴스가 생성되면 이부분(생성자)이 제일 먼저 실행된다.
	CStarterCtrl()
	{
		if(FtpConnect(SERVER, USERID, PASSWD))
		{
			DownloadUpdater();
			FtpDisconnect();
		}
	}

DECLARE_REGISTRY_RESOURCEID(IDR_STARTERCTRL)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CStarterCtrl)
	COM_INTERFACE_ENTRY(IStarterCtrl)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IViewObjectEx)
	COM_INTERFACE_ENTRY(IViewObject2)
	COM_INTERFACE_ENTRY(IViewObject)
	COM_INTERFACE_ENTRY(IOleInPlaceObjectWindowless)
	COM_INTERFACE_ENTRY(IOleInPlaceObject)
	COM_INTERFACE_ENTRY2(IOleWindow, IOleInPlaceObjectWindowless)
	COM_INTERFACE_ENTRY(IOleInPlaceActiveObject)
	COM_INTERFACE_ENTRY(IOleControl)
	COM_INTERFACE_ENTRY(IOleObject)
	COM_INTERFACE_ENTRY(IPersistStreamInit)
	COM_INTERFACE_ENTRY2(IPersist, IPersistStreamInit)
	COM_INTERFACE_ENTRY(IConnectionPointContainer)
	COM_INTERFACE_ENTRY(IObjectSafety)
END_COM_MAP()

BEGIN_PROP_MAP(CStarterCtrl)
	PROP_DATA_ENTRY("_cx", m_sizeExtent.cx, VT_UI4)
	PROP_DATA_ENTRY("_cy", m_sizeExtent.cy, VT_UI4)
	// Example entries
	// PROP_ENTRY("Property Description", dispid, clsid)
	// PROP_PAGE(CLSID_StockColorPage)
END_PROP_MAP()

BEGIN_CONNECTION_POINT_MAP(CStarterCtrl)
END_CONNECTION_POINT_MAP()

BEGIN_MSG_MAP(CStarterCtrl)
	CHAIN_MSG_MAP(CComControl<CStarterCtrl>)
	DEFAULT_REFLECTION_HANDLER()
END_MSG_MAP()
// Handler prototypes:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);



// IViewObjectEx
	DECLARE_VIEW_STATUS(VIEWSTATUS_SOLIDBKGND | VIEWSTATUS_OPAQUE)

// IStarterCtrl
public:

	HRESULT OnDraw(ATL_DRAWINFO& di)
	{
		RECT& rc = *(RECT*)di.prcBounds;
		Rectangle(di.hdcDraw, rc.left, rc.top, rc.right, rc.bottom);

		SetTextAlign(di.hdcDraw, TA_CENTER|TA_BASELINE);
		LPCTSTR pszText = _T("ATL 3.0 : StarterCtrl");
		TextOut(di.hdcDraw, 
			(rc.left + rc.right) / 2, 
			(rc.top + rc.bottom) / 2, 
			pszText, 
			lstrlen(pszText));

		return S_OK;
	}

public:
	STDMETHOD(StartUpdater)(BSTR id, short type);
	BOOL DownloadUpdater();
	void FtpDisconnect();
	BOOL FtpConnect(LPCTSTR server, LPCTSTR userid, LPCTSTR passwd);
	HINTERNET hInternet, hFtp;
};

#endif //__STARTERCTRL_H_
