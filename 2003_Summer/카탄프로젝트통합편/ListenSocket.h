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
		char name[100];		// 사용자 이름
		int turn_dice;		// 턴을 정하는 주사위합
		int resource[5];	// 가지고 있는 자원
		
		/// 상태 ///
		bool tmp;
		bool half;			// 반버리기완료?
		bool resource_move;	// 리소스이동완료?
		int soldier;		// 군대수

		int victory;		// 승리포인트점수
		bool ingame;		// 게임에 들어왔냐?
	} player[4];

	int resource[5];		// 카드남은양
	int longest_road;		// 최대길을 가진자
	int large_ami;			// 대군대를 가진자
	
	int card[25];			// 카드리스트
	int cardn;				// 남은카드갯수

	int half_resource[20];	// 반버림 자원카드이동양
	bool plague_move;		// 도둑이동가능상태

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
