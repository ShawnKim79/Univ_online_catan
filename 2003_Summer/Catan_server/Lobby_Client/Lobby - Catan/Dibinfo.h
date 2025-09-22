// Dibinfo.h: interface for the CDibinfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DIBINFO_H__74BB57AC_2814_11D6_8724_004095A1B407__INCLUDED_)
#define AFX_DIBINFO_H__74BB57AC_2814_11D6_8724_004095A1B407__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <windows.h>

class __declspec(dllexport) CDibinfo  
{
public:
	CDibinfo();
	~CDibinfo();

	BYTE*				DI_Bitmap;
	BITMAPINFO*			DI_BitmapInfo;
	LONG				DI_Width,DI_Height;
	LOGPALETTE*			DI_plogpal;

	void DI_Release();
};

#ifndef LPCDibinfo

typedef CDibinfo* LPCDibinfo;

#endif

class __declspec(dllexport) CDib
{
public:
	CDib(){};
	~CDib(){};

	LPCDibinfo GetBitmap(LPCSTR filename);
	BOOL GetBitmapDC(LPCSTR filename,HDC screenDC,HDC& hdc);
};

#endif // !defined(AFX_DIBINFO_H__74BB57AC_2814_11D6_8724_004095A1B407__INCLUDED_)
