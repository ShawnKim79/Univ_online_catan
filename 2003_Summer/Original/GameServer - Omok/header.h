/////////////////////////////////////////////////
//
// GSM_ : Game Server Message
// CSM_ : Channe Server Message
// CM_  : Client Message
//
//

#ifndef _HEADER_H_
#define _HEADER_H_


#define WM_SOCKETMSGCHSERVER	(WM_USER + 30)
#define WM_SOCKETMSGCLIENT		(WM_USER + 40)

enum FAILURE { 
	/*�̹� �α��εǾ���*/	F_ALREADYLOGIN = 1,
	/*���� DB ����*/		F_DBERROR = 2, 
	/*���� ���� ����*/		F_SERVERERROR = 3, 
	/*ä�� ���� �Ұ�*/		F_CHANNELFULL = 4, 
	/*�̹� �����Ǿ���*/		F_ROOMOCCUPIED = 5, 
	/*������ ���� ����*/	F_NOROOM = 6, 
	/*�� ���� �Ұ�*/		F_JOINROOM = 7 
};

enum CURSEND { ANY, ALLUSERS, ROOMSINFO };
enum { MAKEROOM = 100, JOINROOM = 200 };

#define GSM_CONNECTED		0x0001 // ���� Ȯ��,				Game Server  -->  Lobby
#define GSM_SERVERINFO		0x0002 // ���� ����,				Game Server  -->  Channel Server
#define GSM_FAILURE			0x0003 // ���� ����,				Game Server  -->  Lobby (���� ���� �ڵ� ����)
#define GSM_CLIENTINFO		0x0004 // ����� ����,				Game Server  -->  Lobby
#define GSM_ALLUSERSINFO	0x0005 // ��ü ����� ����,			Game Server  -->  Lobby
#define GSM_ALLROOMSINFO	0x0006 // ��ü �� ����,				Game Server  -->  Lobby
#define GSM_ROOMUPDATED		0x0007 // �� ���� ���ŵ�,			Game Server  -->  Lobby
#define GSM_CLIENTUPDATED	0x0008 // �� Ŭ���̾�Ʈ ���ŵ�,		Game Server  -->  Lobby
#define GSM_ALLUPDATED		0x0009 // ������ �� Ŭ���̾�Ʈ ���� Game Server  -->  Lobby
#define GSM_OWNTHISROOM		0x000A // ���� ������ ����,			Game Server  -->  Lobby
#define GSM_JOINTHISROOM	0x000B // �濡 ����,				Game Server  -->  Lobby
#define GSM_CHATMESSAGE		0x000C // ä�� �޼��� bloadcast		Game Server  -->  Lobby
#define GSM_COMPETITINFO	0x000D // ���� ����,				Game Server  -->  Lobby
#define GSM_REQNEWCHAT		0x000E // ä�� ��û,				Game Server  -->  Lobby
#define GSM_STARTNEWCHAT	0x000F // ä�� ����,				Game Server  -->  Lobby
#define GSM_REJECTNEWCHAT	0x0010 // ä�� ����, 				Game Server  -->  Lobby
#define GSM_REQNEWGAME		0x0011 // ���� ��û,				Game Server  -->  Lobby
#define GSM_REJECTNEWGAME	0x0012 // ���� ��û ����,			Game Server  -->  Lobby

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
#define CM_REQGETOUTROOM	0x8500 // �����κ��� ����,			Lobby  -->  Game Server
#define CM_CHATMESSAGE		0x8600 // ä�� �޼���,				Lobby  -->  Game Server
#define CM_UPDATEINFO		0x8700 // ����� ���� ����,			Lobby  -->  Game Server
#define CM_REQNEWCHAT		0x8800 // ä�� ��û,				Lobby  -->  Game Server
#define CM_STARTNEWCHAT		0x8900 // ä�� ����,				Lobby  -->  Game Server
#define CM_REJCTNEWCHAT		0x8A00 // ä�� ����, 				Lobby  -->  Game Server
#define CM_REQNEWGAME		0x8B00 // ���� ��û,				Lobby  -->  Game Server
#define CM_REJECTNEWGAME	0x8C00 // ���� ��û ����,			Lobby  -->  Game Server
#define CM_GAMESTATE		0x8D00 // ���� ��Ȳ ����,			Lobby  -->  Game Server

#endif