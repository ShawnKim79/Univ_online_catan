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

#define GSM_CONNECTED		0x0001 // 연결 확인,				Game Server  -->  Lobby
#define GSM_SERVERINFO		0x0002 // 서버 정보,				Game Server  -->  Channel Server
#define GSM_FAILURE			0x0003 // 연결 실패,				Game Server  -->  Lobby (연결 실패 코드 붙음)
#define GSM_CLIENTINFO		0x0004 // 사용자 정보,				Game Server  -->  Lobby
#define GSM_ALLUSERSINFO	0x0005 // 전체 사용자 정보,			Game Server  -->  Lobby
#define GSM_ALLROOMSINFO	0x0006 // 전체 방 정보,				Game Server  -->  Lobby
#define GSM_ROOMUPDATED		0x0007 // 방 정보 갱신됨,			Game Server  -->  Lobby

#define CSM_CONNECTED		0x0080 // 연결 확인,				Channel Server  -->  Game Server
#define CSM_CHINFO			0x0081 // 채널 정보,				Channel Server  -->  Lobby
#define CSM_DESTSVRINFO		0x0082 // 접속할 게임 서버 정보,	Channel Server  -->  Lobby
#define CSM_REQCHINFO		0x0083 // 채널 정보 요청,			Channel Server  -->  Game Server

#define CM_REQCHINFO		0x0100 // 채널 정보 요구,			Lobby  -->  Channel Server
#define CM_ENTCHANNEL		0x0200 // 채널 입장 요청,			Lobby  -->  Channel Server

#define CM_REQLOGIN			0x8000 // 로그인 요청,				Lobby  -->  Game Server
#define CM_REQALLUSERS		0x8100 // 전체 사용자 요청,			Lobby  -->  Game Server
#define CM_REQALLROOMS		0x8200 // 전체 방 요청,				Lobby  -->  Game Server
#define CM_REQMAKEROOM		0x8300 // 방 개설 요청,				Lobby  -->  Game Server
#define CM_REQJOINROOM		0x8400 // 방 입장 요청,				Lobby  -->  Game Server
#endif