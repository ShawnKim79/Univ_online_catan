// header.h: interface for the CMessage class.
//	and, Use on Message Map
//////////////////////////////////////////////////////////////////////


#ifndef _HEADER_H_
#define _HEADER_H_


#define WM_SOCKETMSGSERVER		(WM_USER + 50)
#define WM_SOCKETMSGCLIENT		(WM_USER + 60)

enum FAILURE
{
	F_ALREADYLOGIN = 1,
	F_DBERROR = 2,
	F_SERVERERROR = 3,
	F_CHANNELFULL = 4, 
	F_ROOMOCCUPIED = 5,
	F_NOROOM = 6, 
	F_JOINROOM = 7
};

enum CURSEND { ANY, ALLUSERS, ROOMSINFO };
enum { MAKEROOM = 100, JOINROOM = 200 };


/************************************************
0x0000 ~ 0x00ff : for broadcasting Mssages

0x0100 ~ 0x01ff : Channel Server -> Game Server 
0x0200 ~ 0x02ff : Channel Server -> Lobby Client

0x0300 ~ 0x03ff : Game Server -> Channel Server
0x0400 ~ 0x04ff : Game Server -> Lobby Client

0x0500 ~ 0x05ff : Lobby Client -> Game Server
0x0600 ~ 0x06ff : Lobby Client -> Channel Server
*************************************************/


// Section #1 : Channel Server 발송.
// 디폴트 선언은 CSM_ 으로 시작.
//////////////////////////////////////////////////////////////////////
// 0x0100 ~ 0x01ff : Channel Server -> Game Server 
#define CSM_CONNECTGS	0x0100	// 연결 확인
#define CSM_SERVERINFO	0x0101	// 채널 서버 정보 제공 (연결 유지용, IP, Port no...)
#define CSM_REQUESTINFO	0x0102	// 게임 서버에 정보요청(최대 방수, 접속자 수)
#define CSM_REQUESTMAN	0x0103	// 게임 서버에 접속한 사람 수 요청(일정 시간마다 주기적 발생)

//////////////////////////////////////////////////////////////////////
// 0x0200 ~ 0x02ff : Channel Server -> Lobby Client
#define CSM_CONNECTLB	0x0200	// 연결 확인
#define CSM_SENDINFO	0x0201	// 현재 채널 서버 정보(접속자 수, 최대 접속자 수)
#define CSM_DATAMAN		0x0202	// 현재 접속자 수(일정 주기로 발생-보냄?)
#define CSM_DESTSVRINFO 0x0203	//


/************************************************************************************************/


// Section #2 : Game Server 발송.
// 디폴트 선언은 GSM_ 으로 시작.
//////////////////////////////////////////////////////////////////////
// 0x0300 ~ 0x03ff : Game Server -> Channel Server
#define GSM_CONNECTCS	0x0300	// 연결 확인.
#define GSM_SERVERINFO	0x0301	// 게임 서버 정보(최대 접속자, 현재 접속자, IP등등...)
//#define GSM_DATAINFO	0x0302	// 게임 서버 정보(최대 방수, 접속자 수)
#define GSM_DATAMAN		0x0303	// 현재 게임 서버에 접속한 사람 수 

//////////////////////////////////////////////////////////////////////
// 0x0400 ~ 0x04ff : Game Server -> Lobby Client
#define GSM_CONNECTLB	0x0400	// 연결 확인
// #define GSM_DATAMAN			// 현재 게임 서버에 접속한 사람 수, 0x03xx 계열에 존재.
#define GSM_ROOMCOUNT	0x0401	// 생성된 방 갯수
#define GSM_ROOMNO		0x0402	// 방 번호
#define GSM_ROOMNAME	0x0403	// 방 이름
#define	GSM_ROOMMAN		0x0404	// 방에 들어간 사람 수
#define GSM_ROOMLIMIT	0x0405	// 방에 참가하는 사람 한계(2~4)
#define GSM_ROOMHIDN	0x0406	// 비밀 방 여부

#define GSM_ALLMANDATA	0x0410	// 서버에 접속한 모든 사람들 정보
#define GSM_ALLROOMDATA	0x0411	// 서버의 모든 방 정보
#define GSM_UPDATEMAN	0x0412	// 사람들 정보 업뎃
#define GSM_UPDATEROOM	0x0413	// 방 정보 업뎃
#define GSM_ALLUPDATE	0x0415	// 모든 정보 싸그리 업뎃.
#define GSM_COMPETINFO	0x0416

#define GSM_REQNEWGAME	0x0417	// 새 게임
#define GSM_REJECTGAME	0x0418	// 새게임 취소

#define GSM_GMNGACK		0x0420	// 방장 승인
#define GSM_GMNNOT		0x0421	// 방장 승인 거부
#define GSM_OWNROOM		0x0422	// 방장으로 임명됨...

#define GSM_JOINACK		0x0430	// 방 입장 승인
#define GSM_JOINNOT		0x0431	// 방 입장 거부

#define GSM_INVITATE	0x0440	// 방 초대
#define GSM_INVITACK	0x0441	// 방 초대 승인
#define GSM_INVITNOT	0x0442	// 방 초대 거부

#define GSM_FAILURE		0x0450	// 에러 발생.

#define GSM_CHATMSG		0x04ff	// 채팅 메세지 (전부 보내기)
#define GSM_TGTMSG		0x040f	// 채팅 메세지 (타겟지정)


/************************************************************************************************/


// Section #3 : Lobby 발송.
// 디폴트 선언은 LM_..
////////////////////////////////////////////////////////////////////////////////
// 0x0500 ~ 0x05ff : Lobby Client -> Channel Server
#define LM_LOGIN		0x0500	// Log In 요청
#define LM_REQCHINFO	0x0501	// 채널 정보 요구
#define LM_REQGSMSVR	0x0502	// 게임서버 입장

////////////////////////////////////////////////////////////////////////////////
// 0x0600 ~ 0x06ff : Lobby Client -> Game Server
#define LM_CONNECTGS	0x0600	// Game Server와 연결
#define LM_REQROOMINFO	0x0601	// Game Server 정보 요청
#define LM_REQMANINFO	0x0602	// 전체 사용자 요청

#define LM_REQGMN		0x0610	// 방장 신청
#define LM_REQJOIN		0x0611	// 방 들어가기 신청
#define LM_REQOUT		0x0612	// 방 나가기 신청
#define LM_INVITACK		0x0613	// 초대 허락
#define LM_INVITNOT		0x0614	// 초대 거부

#define LM_REQNEWGAME	0x0620	// 새게임.
#define LM_REQRJTGAME	0x0621	// 게임 나가기.

#define LM_USERUPDATE	0x0630	// 유저의 정보 변경....


#define LM_CHATMSG		0x06ff	// 채팅 메세지
#define LM_TGTMSG		0x060f	// 타켓 지정 채팅 메세지



/************************************************************************************************/

#endif