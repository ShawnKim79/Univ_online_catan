// Player_Inf.h: interface for the CPlayer_Inf class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PLAYER_INF_H__1B74A7C7_38D5_49ED_9212_7EA4EC97E8B7__INCLUDED_)
#define AFX_PLAYER_INF_H__1B74A7C7_38D5_49ED_9212_7EA4EC97E8B7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CPlayer_Inf  // 플레이어 정보를 담고 있습니다.
{
public:
	struct plist {
		char name[100];
		int cardn;
		int soldier;
		bool enable;
	} ps[4];

	int card[30];		// 가지고 있는 카드
	int cardn;			// 가지고 있는 카드갯수
	int newcard[30];	// 새로 얻은 카드
	int newcardn;		// 새로 얻은 카드갯수
	bool carduse;		// 카드사용여부
	
	int who_turn;	// 누가 턴을 가지고 있니?

	int p_n;		// 자신의 플레이어 순서번호
	bool start;		// 게임시작여부
	bool turn;		// 내턴여부

	int settle;		//세틀수
	int city;		//시티수
	int road;		// 길수

	int con;		// 현재 작업상황
	int tmp[100];	// 정보를 기억하기 위한 임시기억장소
					// 100번째는 길짓기를 이용하여 길을 짓고 있음
	
	int resource[5];// 자원카드
	
	CPlayer_Inf();
	virtual ~CPlayer_Inf();
};

#endif // !defined(AFX_PLAYER_INF_H__1B74A7C7_38D5_49ED_9212_7EA4EC97E8B7__INCLUDED_)
