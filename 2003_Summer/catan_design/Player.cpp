// Player.cpp: implementation of the CPlayer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "catan_design.h"
#include "Player.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPlayer::CPlayer()
{

}

CPlayer::~CPlayer()
{

}

CPlayer::SetPlayer()
{
// m_cPlayername; // �÷��̾� �̸� �ʱ�ȭ.....

m_nTimbercard = 0;
m_nBlockcard = 0;
m_nGraincard = 0;
m_nSheepcard = 0;
m_nOrecard = 0;

// Ư�� ī��....
m_nPlentycard = 0;
m_nMonopolycard = 0;
m_nRoadBuilding = 0;
m_nSoldiercard = 0;

// ���� ī�� �ΰ�...(Largest Army & Longest Way)
m_bLargestArmy = 0;
m_bLongestWay = 0;

// ���� ���� ����(?)
m_nRoadLength = 0;
	
// ���� ���ھ�...
m_nGamescore = 0;
}