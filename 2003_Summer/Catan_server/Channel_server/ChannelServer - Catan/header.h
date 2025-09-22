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


// Section #1 : Channel Server �߼�.
// ����Ʈ ������ CSM_ ���� ����.
//////////////////////////////////////////////////////////////////////
// 0x0100 ~ 0x01ff : Channel Server -> Game Server 
#define CSM_CONNECTGS	0x0100	// ���� Ȯ��
#define CSM_SERVERINFO	0x0101	// ä�� ���� ���� ���� (���� ������, IP, Port no...)
#define CSM_REQUESTINFO	0x0102	// ���� ������ ������û(�ִ� ���, ������ ��)
#define CSM_REQUESTMAN	0x0103	// ���� ������ ������ ��� �� ��û(���� �ð����� �ֱ��� �߻�)

//////////////////////////////////////////////////////////////////////
// 0x0200 ~ 0x02ff : Channel Server -> Lobby Client
#define CSM_CONNECTLB	0x0200	// ���� Ȯ��
#define CSM_SENDINFO	0x0201	// ���� ä�� ���� ����(������ ��, �ִ� ������ ��)
#define CSM_DATAMAN		0x0202	// ���� ������ ��(���� �ֱ�� �߻�-����?)
#define CSM_DESTSVRINFO 0x0203	//


/************************************************************************************************/


// Section #2 : Game Server �߼�.
// ����Ʈ ������ GSM_ ���� ����.
//////////////////////////////////////////////////////////////////////
// 0x0300 ~ 0x03ff : Game Server -> Channel Server
#define GSM_CONNECTCS	0x0300	// ���� Ȯ��.
#define GSM_SERVERINFO	0x0301	// ���� ���� ����(�ִ� ������, ���� ������, IP���...)
//#define GSM_DATAINFO	0x0302	// ���� ���� ����(�ִ� ���, ������ ��)
#define GSM_DATAMAN		0x0303	// ���� ���� ������ ������ ��� �� 

//////////////////////////////////////////////////////////////////////
// 0x0400 ~ 0x04ff : Game Server -> Lobby Client
#define GSM_CONNECTLB	0x0400	// ���� Ȯ��
// #define GSM_DATAMAN			// ���� ���� ������ ������ ��� ��, 0x03xx �迭�� ����.
#define GSM_ROOMCOUNT	0x0401	// ������ �� ����
#define GSM_ROOMNO		0x0402	// �� ��ȣ
#define GSM_ROOMNAME	0x0403	// �� �̸�
#define	GSM_ROOMMAN		0x0404	// �濡 �� ��� ��
#define GSM_ROOMLIMIT	0x0405	// �濡 �����ϴ� ��� �Ѱ�(2~4)
#define GSM_ROOMHIDN	0x0406	// ��� �� ����

#define GSM_ALLMANDATA	0x0410	// ������ ������ ��� ����� ����
#define GSM_ALLROOMDATA	0x0411	// ������ ��� �� ����
#define GSM_UPDATEMAN	0x0412	// ����� ���� ����
#define GSM_UPDATEROOM	0x0413	// �� ���� ����
#define GSM_ALLUPDATE	0x0415	// ��� ���� �α׸� ����.
#define GSM_COMPETINFO	0x0416

#define GSM_REQNEWGAME	0x0417	// �� ����
#define GSM_REJECTGAME	0x0418	// ������ ���

#define GSM_GMNGACK		0x0420	// ���� ����
#define GSM_GMNNOT		0x0421	// ���� ���� �ź�
#define GSM_OWNROOM		0x0422	// �������� �Ӹ��...

#define GSM_JOINACK		0x0430	// �� ���� ����
#define GSM_JOINNOT		0x0431	// �� ���� �ź�

#define GSM_INVITATE	0x0440	// �� �ʴ�
#define GSM_INVITACK	0x0441	// �� �ʴ� ����
#define GSM_INVITNOT	0x0442	// �� �ʴ� �ź�

#define GSM_FAILURE		0x0450	// ���� �߻�.

#define GSM_CHATMSG		0x04ff	// ä�� �޼��� (���� ������)
#define GSM_TGTMSG		0x040f	// ä�� �޼��� (Ÿ������)


/************************************************************************************************/


// Section #3 : Lobby �߼�.
// ����Ʈ ������ LM_..
////////////////////////////////////////////////////////////////////////////////
// 0x0500 ~ 0x05ff : Lobby Client -> Channel Server
#define LM_LOGIN		0x0500	// Log In ��û
#define LM_REQCHINFO	0x0501	// ä�� ���� �䱸
#define LM_REQGSMSVR	0x0502	// ���Ӽ��� ����

////////////////////////////////////////////////////////////////////////////////
// 0x0600 ~ 0x06ff : Lobby Client -> Game Server
#define LM_CONNECTGS	0x0600	// Game Server�� ����
#define LM_REQROOMINFO	0x0601	// Game Server ���� ��û
#define LM_REQMANINFO	0x0602	// ��ü ����� ��û

#define LM_REQGMN		0x0610	// ���� ��û
#define LM_REQJOIN		0x0611	// �� ���� ��û
#define LM_REQOUT		0x0612	// �� ������ ��û
#define LM_INVITACK		0x0613	// �ʴ� ���
#define LM_INVITNOT		0x0614	// �ʴ� �ź�

#define LM_REQNEWGAME	0x0620	// ������.
#define LM_REQRJTGAME	0x0621	// ���� ������.

#define LM_USERUPDATE	0x0630	// ������ ���� ����....


#define LM_CHATMSG		0x06ff	// ä�� �޼���
#define LM_TGTMSG		0x060f	// Ÿ�� ���� ä�� �޼���



/************************************************************************************************/

#endif