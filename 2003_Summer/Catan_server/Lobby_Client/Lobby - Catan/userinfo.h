#pragma once
#include <windows.h>
/*
typedef struct __declspec(dllexport)
{
	unsigned char	id[32];		// ������ ���̵�
	unsigned char	name[32];	// ������ ����
	unsigned char	alias[32];	// ������ �г���
	unsigned char	ip[32];		// ������ ������
	unsigned short	channelno;	// ������ ä�� ��ȣ
	short			roomno;		// ������ �� ��ȣ -1 : ����
	unsigned long	socket;		// ����
	unsigned long	score;		// ����
	unsigned long	playcount;	// �� ���� ��
	unsigned long	victorycount;	// �� ��
	unsigned long	losscount;	// �� 
	unsigned long	drawcount;	// ��� ��
	unsigned long	money;

} USERINFO;
*/
class  __declspec(dllexport) USERINFOEX : public USERINFO
{
public:
	~USERINFOEX();
	USHORT the_length_of_nickname;
	USHORT the_length_of_id;
	USHORT the_length_of_info;
	USHORT the_length_of_point;

	char* strpoint;
	char* strinfo;
	COLORREF	selfcolor;
};
