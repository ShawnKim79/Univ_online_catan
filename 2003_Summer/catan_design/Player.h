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

	void SetPlayer(); // ���� �ʱ�ȭ

	// �ڿ� �߰�....
	void GetResourceCard(unsigned int, unsigned int, unsigned int, unsigned int, unsigned int); // ī�� ���
	void UseResourceCard(unsigned int, unsigned int, unsigned int, unsigned int, unsigned int); // ī�� �Һ� �ϱ�

	void BuySpecialCard(unsigned int, unsigned int, unsigned int, unsigned int); // Ư�� ī�� ���
	void UseSpecialCard(unsigned int, unsigned int, unsigned int, unsigned int); // Ư��ī�� �Һ��ϱ�

	void DemandLongestRoad(); // Longest Road �䱸
	void DemandLargestArmy(); // Largest Army �䱸

	int calculateRoadLength(); // ���ӵ� ���� ���� ���...(�ڽ��� �Ͽ� ������ ���ش�)
	int CalculateScore(); // ���� ���...(�ڽ��� �Ͽ� ������ ���ش�)

private:
	char m_cPlayername; // �÷��̾� ��.

	// �ڿ�ī��...
	unsigned int m_nTimbercard;
	unsigned int m_nBlockcard;
	unsigned int m_nGraincard;
	unsigned int m_nSheepcard;
	unsigned int m_nOrecard;

	// Ư�� ī��....
	unsigned int m_nPlentycard;
	unsigned int m_nMonopolycard;
	unsigned int m_nRoadBuilding;
	unsigned int m_nSoldiercard;

	// ���� ī�� �ΰ�...(Largest Army & Longest Way)
	int m_bLargestArmy;
	int m_bLongestWay;

	// ���� ���� ����(?)
	unsigned int m_nRoadLength;
	
	// ���� ���ھ�...
	unsigned int m_nGamescore;


};

#endif // !defined(AFX_PLAYER_H__0BA7C6D9_EF90_4F2E_9DFF_59BA9E530718__INCLUDED_)
