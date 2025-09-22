#ifndef _STRUCT_H_
#define _STRUCT_H_

#include <string>
#include <list>

using namespace std;

#pragma pack(push)
#pragma pack(1) // �� ����ü, 1����Ʈ �޸� ����
/***************************************************************
1����Ʈ �޸� ������ �ϴ� ���� :
����ü ������ ������� ������ ���� ���� ������, 
�޸� ������ ������ ������ �ϳ��� ����ü�� ��Ŷ���� ��ȯ�ɶ�
�޸� �＼�� �ϴ� �κп� �־ �� ������ �߻��ϰ� �ȴ�.
���� ����ü�� memcpy�� ������ ��, �޸� ������ �� �� ������ �뷫 ����.
***************************************************************/

struct ROOM; // ROOM �� ����ü ����
typedef ROOM FAR *LPROOM; // ROOM FAR == *LPROOM, type define to pointer

typedef struct USER // USER ���� ����ü, D/B ���� ��������� ����� �� ������ ����.
{
	unsigned char id[32]; // ������ ID
	unsigned char name[32]; // ������ �̸�
	unsigned char alias[32]; // ������ ��
	unsigned char ip[32]; // ������ IP
	unsigned short channelno; // �����ڰ� ������ ä�� ��ȣ.
	short roomno; // ���� �����ڰ� �� �ִ� ���ȣ : -1�� ����

	unsigned long socket; // ����

	unsigned long score; // ����
	unsigned long playcount; // ���� ��
	unsigned long victorycount; // �¼�
	unsigned long losscount; // �м�
	unsigned long drawcount; // ��� ��

	unsigned long money;

	LPROOM lproom; // �� ������
	USER* talkwith; // �׷�(?)
} USER, FAR *LPUSER;


// ������ �� ����(�ϳ��� ���� �ϳ��� �������� Ŭ���̾�Ʈ)
struct ROOM
{
	unsigned short id; //�� ��ȣ
	unsigned short channelno; // ���� �Ҽӵ� ä�� ��ȣ
	unsigned short status; // �� ����
	unsigned short maximum; // �ִ� ���� �ο�
	unsigned short present; // ���� ���� �ο�
	unsigned short locked; //��й� 
	unsigned char title[255]; // �� ����
	unsigned char passwd[16]; // ��й� �н�����

	LPUSER users[4]; // [0]�� ����
};

// ä�� ���� ����ü
typedef struct
{
	unsigned short id; // ä�� ��ȣ
	unsigned short maximum; // �ִ� ���� �� ����
	unsigned short present; // ���� ������ �� ����
	list<ROOM> roomlist; // ����� ����Ʈ
} CHANNEL, FAR* LPCHANNEL;


//////////////////////////////////////////////////////
// ������ ��� ����ü

typedef struct
{
	unsigned short header; // ��� ����
	unsigned long length;  // ��ü ��Ŷ ����
} COMMON_PACKET;

typedef struct
{
	COMMON_PACKET cmnpacket;
} CONNECTED;

typedef struct
{
	char chatmsg[1000];
	unsigned short msgtype;

	CHARFORMAT cf;
} CHATINFO, FAR* LPCHATINFO;

typedef struct
{
	COMMON_PACKET cmnpacket;

	unsigned short id; // id
	unsigned char ip[32]; // ip �ּ�
	unsigned short port; // ��Ʈ ��ȣ
	unsigned short channels; // ä�� ����
	unsigned short rooms; //
} SERVERINFO;

typedef struct
{
	unsigned short channelno; // ä�� ��ȣ
	unsigned short maximum; // �ִ� ����ο�
	unsigned short present; // ���� ������ ��
} CHANNELINFO;

typedef struct
{
	unsigned char id[32]; // ������ ID
	unsigned char name[32]; // ������ �̸�
	unsigned char alias[32]; // ������ ��
	unsigned char ip[32]; // ������ IP
	unsigned short channelno; // �����ڰ� ������ ä�� ��ȣ.
	short roomno; // ���� �����ڰ� �� �ִ� ���ȣ : -1�� ����

	unsigned long socket; // ����

	unsigned long score; // ����
	unsigned long playcount; // ���� ��
	unsigned long victorycount; // �¼�
	unsigned long losscount; // �м�
	unsigned long drawcount; // ��� ��

	unsigned long money;
} USERINFO, FAR* LPUSERINFO;

typedef struct
{
	unsigned short id; //�� ��ȣ
	unsigned short channelno; // ���� �Ҽӵ� ä�� ��ȣ
	unsigned short status; // �� ����
	unsigned short maximum; // �ִ� ���� �ο�
	unsigned short present; // ���� ���� �ο�
	unsigned short locked; //��й� 
	unsigned char title[255]; // �� ����
	unsigned char passwd[16]; // ��й� �н�����
} ROOMINFO, FAR* LPROOMINFO;

#pragma pack(pop)
#endif