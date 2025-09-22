// ChannelManager.h: interface for the CChannelManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHANNELMANAGER_H__4A4101D2_6C45_4D3E_8080_4F048321CF94__INCLUDED_)
#define AFX_CHANNELMANAGER_H__4A4101D2_6C45_4D3E_8080_4F048321CF94__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "structs.h"

#include <string>
#include <list>

using namespace std;

class CChannelManager  
{
public:
	void ClearChannel();
	void InitializeChannel(int serverid, int channelcnt, int roomcnt, int amxuser);
	LPROOM GetRoomInfo(int channelno, int roomno);
	LPCHANNEL GetChannelInfo(int channelno);
	CChannelManager();
	virtual ~CChannelManager();

protected:

	list<CHANNEL> channellist;

};

#endif // !defined(AFX_CHANNELMANAGER_H__4A4101D2_6C45_4D3E_8080_4F048321CF94__INCLUDED_)
