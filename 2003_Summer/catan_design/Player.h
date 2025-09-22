// Player.h: interface for the CPlayer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PLAYER_H__0BA7C6D9_EF90_4F2E_9DFF_59BA9E530718__INCLUDED_)
#define AFX_PLAYER_H__0BA7C6D9_EF90_4F2E_9DFF_59BA9E530718__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

typedef s_Mapdata
{

} MAPDATA;

class CPlayer  
{
public:
	CPlayer();
	virtual ~CPlayer();

	void SetPlayer(); // 게임 초기화

	// 자원 추가....
	void GetResourceCard(unsigned int, unsigned int, unsigned int, unsigned int, unsigned int); // 카드 얻기
	void UseResourceCard(unsigned int, unsigned int, unsigned int, unsigned int, unsigned int); // 카드 소비 하기

	void BuySpecialCard(unsigned int, unsigned int, unsigned int, unsigned int); // 특수 카드 얻기
	void UseSpecialCard(unsigned int, unsigned int, unsigned int, unsigned int); // 특수카드 소비하기

	void DemandLongestRoad(); // Longest Road 요구
	void DemandLargestArmy(); // Largest Army 요구

	int calculateRoadLength(); // 연속된 길의 길이 계산...(자신의 턴에 스스로 해준다)
	int CalculateScore(); // 점수 계산...(자신의 턴에 스스로 해준다)

private:
	char m_cPlayername; // 플레이어 색.

	// 자원카드...
	unsigned int m_nTimbercard;
	unsigned int m_nBlockcard;
	unsigned int m_nGraincard;
	unsigned int m_nSheepcard;
	unsigned int m_nOrecard;

	// 특수 카드....
	unsigned int m_nPlentycard;
	unsigned int m_nMonopolycard;
	unsigned int m_nRoadBuilding;
	unsigned int m_nSoldiercard;

	// 점수 카드 두개...(Largest Army & Longest Way)
	int m_bLargestArmy;
	int m_bLongestWay;

	// 길의 길이 정보(?)
	unsigned int m_nRoadLength;
	
	// 게임 스코어...
	unsigned int m_nGamescore;


};

#endif // !defined(AFX_PLAYER_H__0BA7C6D9_EF90_4F2E_9DFF_59BA9E530718__INCLUDED_)
