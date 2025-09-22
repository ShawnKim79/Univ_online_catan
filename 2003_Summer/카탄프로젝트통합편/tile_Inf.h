// tile_Inf.h: interface for the Ctile_Inf class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TILE_INF_H__E435C533_6EB6_4B51_8B36_3287410CD8E8__INCLUDED_)
#define AFX_TILE_INF_H__E435C533_6EB6_4B51_8B36_3287410CD8E8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class Ctile_Inf  
{
public:
	char *load_bmp(char *filename);
	struct tile_sub {
		char filename[100];		// 파일이름
		char *bitmap;			// 비트맵 파일
		tile_sub *next;
	} *dat;

	int datn;

	Ctile_Inf();
	virtual ~Ctile_Inf();

};

#endif // !defined(AFX_TILE_INF_H__E435C533_6EB6_4B51_8B36_3287410CD8E8__INCLUDED_)
