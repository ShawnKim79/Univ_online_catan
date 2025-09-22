// ChannelManager.h: interface for the CChannelManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHANNELMANAGER_H__5678998A_8ED0_44EB_B8F3_D646C47536E6__INCLUDED_)
#define AFX_CHANNELMANAGER_H__5678998A_8ED0_44EB_B8F3_D646C47536E6__INCLUDED_

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
	LPCHANNEL GetChannelInfo(int channelno);
	LPROOM GetRoomInfo(int channelno, int roomno);
	void InitializeChannel(int serverid, int channelcnt, int roomcnt, int amxuser);
	void ClearChannel();
	CChannelManager();
	virtual ~CChannelManager();

public:
	list<CHANNEL> channellist;

};

#endif // !defined(AFX_CHANNELMANAGER_H__5678998A_8ED0_44EB_B8F3_D646C47536E6__INCLUDED_)
