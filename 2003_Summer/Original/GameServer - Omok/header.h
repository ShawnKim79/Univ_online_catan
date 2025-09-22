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
	/*이미 로그인되었음*/	F_ALREADYLOGIN = 1,
	/*서버 DB 오류*/		F_DBERROR = 2, 
	/*서버 내부 오류*/		F_SERVERERROR = 3, 
	/*채널 입장 불가*/		F_CHANNELFULL = 4, 
	/*이미 점유되었음*/		F_ROOMOCCUPIED = 5, 
	/*가능한 방이 없음*/	F_NOROOM = 6, 
	/*방 입장 불가*/		F_JOINROOM = 7 
};

enum CURSEND { ANY, ALLUSERS, ROOMSINFO };
enum { MAKEROOM = 100, JOINROOM = 200 };

#define GSM_CONNECTED		0x0001 // 연결 확인,				Game Server  -->  Lobby
#define GSM_SERVERINFO		0x0002 // 서버 정보,				Game Server  -->  Channel Server
#define GSM_FAILURE			0x0003 // 연결 실패,				Game Server  -->  Lobby (연결 실패 코드 붙음)
#define GSM_CLIENTINFO		0x0004 // 사용자 정보,				Game Server  -->  Lobby
#define GSM_ALLUSERSINFO	0x0005 // 전체 사용자 정보,			Game Server  -->  Lobby
#define GSM_ALLROOMSINFO	0x0006 // 전체 방 정보,				Game Server  -->  Lobby
#define GSM_ROOMUPDATED		0x0007 // 방 정보 갱신됨,			Game Server  -->  Lobby
#define GSM_CLIENTUPDATED	0x0008 // 새 클라이언트 갱신됨,		Game Server  -->  Lobby
#define GSM_ALLUPDATED		0x0009 // 방정보 및 클라이언트 갱신 Game Server  -->  Lobby
#define GSM_OWNTHISROOM		0x000A // 방장 소유권 가짐,			Game Server  -->  Lobby
#define GSM_JOINTHISROOM	0x000B // 방에 입장,				Game Server  -->  Lobby
#define GSM_CHATMESSAGE		0x000C // 채팅 메세지 bloadcast		Game Server  -->  Lobby
#define GSM_COMPETITINFO	0x000D // 상대방 정보,				Game Server  -->  Lobby
#define GSM_REQNEWCHAT		0x000E // 채팅 요청,				Game Server  -->  Lobby
#define GSM_STARTNEWCHAT	0x000F // 채팅 시작,				Game Server  -->  Lobby
#define GSM_REJECTNEWCHAT	0x0010 // 채팅 거절, 				Game Server  -->  Lobby
#define GSM_REQNEWGAME		0x0011 // 게임 요청,				Game Server  -->  Lobby
#define GSM_REJECTNEWGAME	0x0012 // 게임 요청 거절,			Game Server  -->  Lobby

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
#define CM_REQGETOUTROOM	0x8500 // 방으로부터 퇴장,			Lobby  -->  Game Server
#define CM_CHATMESSAGE		0x8600 // 채팅 메세지,				Lobby  -->  Game Server
#define CM_UPDATEINFO		0x8700 // 사용자 정보 갱신,			Lobby  -->  Game Server
#define CM_REQNEWCHAT		0x8800 // 채팅 요청,				Lobby  -->  Game Server
#define CM_STARTNEWCHAT		0x8900 // 채팅 시작,				Lobby  -->  Game Server
#define CM_REJCTNEWCHAT		0x8A00 // 채팅 거절, 				Lobby  -->  Game Server
#define CM_REQNEWGAME		0x8B00 // 게임 신청,				Lobby  -->  Game Server
#define CM_REJECTNEWGAME	0x8C00 // 게임 요청 거절,			Lobby  -->  Game Server
#define CM_GAMESTATE		0x8D00 // 게임 상황 전송,			Lobby  -->  Game Server

#endif