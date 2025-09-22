// Player_Inf.h: interface for the CPlayer_Inf class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PLAYER_INF_H__1B74A7C7_38D5_49ED_9212_7EA4EC97E8B7__INCLUDED_)
#define AFX_PLAYER_INF_H__1B74A7C7_38D5_49ED_9212_7EA4EC97E8B7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CPlayer_Inf  // �÷��̾� ������ ��� �ֽ��ϴ�.
{
public:
	struct plist {
		char name[100];
		int cardn;
		int soldier;
		bool enable;
	} ps[4];

	int card[30];		// ������ �ִ� ī��
	int cardn;			// ������ �ִ� ī�尹��
	int newcard[30];	// ���� ���� ī��
	int newcardn;		// ���� ���� ī�尹��
	bool carduse;		// ī���뿩��
	
	int who_turn;	// ���� ���� ������ �ִ�?

	int p_n;		// �ڽ��� �÷��̾� ������ȣ
	bool start;		// ���ӽ��ۿ���
	bool turn;		// ���Ͽ���

	int settle;		//��Ʋ��
	int city;		//��Ƽ��
	int road;		// ���

	int con;		// ���� �۾���Ȳ
	int tmp[100];	// ������ ����ϱ� ���� �ӽñ�����
					// 100��°�� �����⸦ �̿��Ͽ� ���� ���� ����
	
	int resource[5];// �ڿ�ī��
	
	CPlayer_Inf();
	virtual ~CPlayer_Inf();
};

#endif // !defined(AFX_PLAYER_INF_H__1B74A7C7_38D5_49ED_9212_7EA4EC97E8B7__INCLUDED_)
