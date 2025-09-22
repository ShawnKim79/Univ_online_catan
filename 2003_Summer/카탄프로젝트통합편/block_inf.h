// block_inf.h: interface for the Cblock_inf class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BLOCK_INF_H__A5002967_92A3_4894_B048_59AFDB234B25__INCLUDED_)
#define AFX_BLOCK_INF_H__A5002967_92A3_4894_B048_59AFDB234B25__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "link_type.h"

class Cblock_inf  
{
public:
	int longest_road(int target);
	void set_map(char *temp);
	void get_map(char *temp);
	void mapxy_setting(int sx,int sy,int mid);
	void clear_setting();
	void normal_setting();
	void copy_map(Cblock_inf *source);
	
	link_type link[96];
	
	struct block_type {
		link_type *point[6];	//  ／,＼ ,	//
								// │ , │,	//
								//  ＼, ／,	//
		
		int harbor[6];	// 포인트에 항구존재위치
		
		char resource;	// 0 : 없음
						// 1 : 나무	(4)
						// 2 : 벽돌	(3)
						// 3 : 양	(4)
						// 4 : 밀	(4)			
						// 5 : 광석	(3)
						// 6 : 사막 (1)
						// 7 : 바다
		
		char num;

	} block[7][7];

	CPoint plague;		// 도둑

	Cblock_inf();
	virtual ~Cblock_inf();

};

#endif // !defined(AFX_BLOCK_INF_H__A5002967_92A3_4894_B048_59AFDB234B25__INCLUDED_)
