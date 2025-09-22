#if !defined(AFX_LISTENSOCKET_H__92467EE9_E960_4813_8F84_670AE22F1546__INCLUDED_)
#define AFX_LISTENSOCKET_H__92467EE9_E960_4813_8F84_670AE22F1546__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ListenSocket.h : header file
//

#include "ServerSocket.h"
#include "block_inf.h"


/////////////////////////////////////////////////////////////////////////////
// CListenSocket command target

class CListenSocket : public CAsyncSocket
{
// Attributes
public:
	Cblock_inf map;

	struct player_inf {
		char name[100];		// ����� �̸�
		int turn_dice;		// ���� ���ϴ� �ֻ�����
		int resource[5];	// ������ �ִ� �ڿ�
		
		/// ���� ///
		bool tmp;
		bool half;			// �ݹ�����Ϸ�?
		bool resource_move;	// ���ҽ��̵��Ϸ�?
		int soldier;		// �����

		int victory;		// �¸�����Ʈ����
		bool ingame;		// ���ӿ� ���Գ�?
	} player[4];

	int resource[5];		// ī�峲����
	int longest_road;		// �ִ���� ������
	int large_ami;			// �뱺�븦 ������
	
	int card[25];			// ī�帮��Ʈ
	int cardn;				// ����ī�尹��

	int half_resource[20];	// �ݹ��� �ڿ�ī���̵���
	bool plague_move;		// �����̵����ɻ���

	int game_run;
	int turn_sort[4];
	int turn;
	int turn_use[4];

// Operations
public:
	CListenSocket();
	virtual ~CListenSocket();

// Overrides
public:
	void turn_clear();
	void turn_set();
	void all_send(char *buffer, int len);
	CServerSocket *socket[4];
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CListenSocket)
	public:
	virtual void OnAccept(int nErrorCode);
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CListenSocket)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

// Implementation
protected:
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LISTENSOCKET_H__92467EE9_E960_4813_8F84_670AE22F1546__INCLUDED_)
