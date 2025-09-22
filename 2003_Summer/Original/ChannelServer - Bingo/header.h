/////////////////////////////////////////////////
//
// GSM_ : Game Server Message
// CSM_ : Channe Server Message
// CM_  : Client Message
//
//

#ifndef _HEADER_H_
#define _HEADER_H_

#define WM_SOCKETMSGSERVER		(WM_USER + 50)
#define WM_SOCKETMSGCLIENT		(WM_USER + 60)

enum FAILURE { F_ALREADYLOGIN, F_DBERROR, F_SERVERERROR, F_CHANNELFULL, F_ROOMOCCUPIED };
enum CURSEND { ANY, ALLUSERS, ROOMSINFO };
enum { MAKEROOM = 100, JOINROOM = 200 };

#define GSM_CONNECTED		0x0001 // ���� Ȯ��,				Game Server  -->  Lobby
#define GSM_SERVERINFO		0x0002 // ���� ����,				Game Server  -->  Channel Server
#define GSM_FAILURE			0x0003 // ���� ����,				Game Server  -->  Lobby (���� ���� �ڵ� ����)
#define GSM_CLIENTINFO		0x0004 // ����� ����,				Game Server  -->  Lobby
#define GSM_ALLUSERSINFO	0x0005 // ��ü ����� ����,			Game Server  -->  Lobby
#define GSM_ALLROOMSINFO	0x0006 // ��ü �� ����,				Game Server  -->  Lobby
#define GSM_ROOMUPDATED		0x0007 // �� ���� ���ŵ�,			Game Server  -->  Lobby

#define CSM_CONNECTED		0x0080 // ���� Ȯ��,				Channel Server  -->  Game Server
#define CSM_CHINFO			0x0081 // ä�� ����,				Channel Server  -->  Lobby
#define CSM_DESTSVRINFO		0x0082 // ������ ���� ���� ����,	Channel Server  -->  Lobby
#define CSM_REQCHINFO		0x0083 // ä�� ���� ��û,			Channel Server  -->  Game Server

#define CM_REQCHINFO		0x0100 // ä�� ���� �䱸,			Lobby  -->  Channel Server
#define CM_ENTCHANNEL		0x0200 // ä�� ���� ��û,			Lobby  -->  Channel Server

#define CM_REQLOGIN			0x8000 // �α��� ��û,				Lobby  -->  Game Server
#define CM_REQALLUSERS		0x8100 // ��ü ����� ��û,			Lobby  -->  Game Server
#define CM_REQALLROOMS		0x8200 // ��ü �� ��û,				Lobby  -->  Game Server
#define CM_REQMAKEROOM		0x8300 // �� ���� ��û,				Lobby  -->  Game Server
#define CM_REQJOINROOM		0x8400 // �� ���� ��û,				Lobby  -->  Game Server
#endif