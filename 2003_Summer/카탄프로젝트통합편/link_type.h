// link_type.h: interface for the link_type class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LINK_TYPE_H__D36C1AD6_1736_4B7E_8F1D_0BB366B5E083__INCLUDED_)
#define AFX_LINK_TYPE_H__D36C1AD6_1736_4B7E_8F1D_0BB366B5E083__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class link_type  
{
public:
	int road_num(link_type *p);
	void add_point(link_type *p);
	
	link_type *link[3];				// 꼭지점과 연결된 다른 꼭지점들
	int number;						// 꼭지점의 고유번호
	
	int road[3];					// 꼭지점 사이의 길

	int x;							// 꼭지점의 X 좌표
	int y;							// 꼭지점의 Y 좌표

	int link_max;					// 꼭지점과 연결된 다른 꼭지점의 최대수
	
	unsigned char city;				// 꼭지점에 존재하는 건물
									// 1~4 는 세틀러먼트
									// 6~9 는 시티

	link_type();
	virtual ~link_type();

};

#endif // !defined(AFX_LINK_TYPE_H__D36C1AD6_1736_4B7E_8F1D_0BB366B5E083__INCLUDED_)
