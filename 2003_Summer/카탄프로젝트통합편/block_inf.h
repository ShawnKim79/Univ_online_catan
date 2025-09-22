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
		link_type *point[6];	//  ��,�� ,	//
								// �� , ��,	//
								//  ��, ��,	//
		
		int harbor[6];	// ����Ʈ�� �ױ�������ġ
		
		char resource;	// 0 : ����
						// 1 : ����	(4)
						// 2 : ����	(3)
						// 3 : ��	(4)
						// 4 : ��	(4)			
						// 5 : ����	(3)
						// 6 : �縷 (1)
						// 7 : �ٴ�
		
		char num;

	} block[7][7];

	CPoint plague;		// ����

	Cblock_inf();
	virtual ~Cblock_inf();

};

#endif // !defined(AFX_BLOCK_INF_H__A5002967_92A3_4894_B048_59AFDB234B25__INCLUDED_)
