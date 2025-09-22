// Dibinfo.cpp: implementation of the CDibinfo class.
//
//////////////////////////////////////////////////////////////////////
	
#include "Dibinfo.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDibinfo::CDibinfo()
{

	DI_Bitmap = NULL;
	DI_BitmapInfo = NULL;
	DI_Width = DI_Height = 0;
	DI_plogpal = NULL;
}

CDibinfo::~CDibinfo()
{

}

void CDibinfo::DI_Release()
{
	if(DI_Bitmap != NULL)
		delete DI_Bitmap;

	if(DI_BitmapInfo != NULL)
		delete DI_BitmapInfo;

	if(DI_plogpal != NULL)
		delete DI_plogpal;

	DI_Bitmap = NULL;
	DI_BitmapInfo = NULL;
	DI_plogpal = NULL;
	DI_Width = DI_Height = 0;

	delete this;
}
//////////////////////////////////////////////////////////////////////////////////////
//
//
//
//
///////////////////////////////////////////////////////////////////////////////////////

LPCDibinfo CDib::GetBitmap(LPCSTR filename)
{
	LPCDibinfo Dibinfo = new CDibinfo;

	HANDLE hfile;
	DWORD tempsize = 0;
	BITMAPFILEHEADER bmfh;
	DWORD Bitmapinfosize,Datasize;
	BYTE ByteCount;
	
	hfile = CreateFile(filename,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,NULL,NULL);

	ReadFile(hfile,&bmfh,sizeof(BITMAPFILEHEADER),&tempsize,NULL);
	
	Bitmapinfosize = bmfh.bfOffBits - sizeof(BITMAPFILEHEADER);

	Dibinfo->DI_BitmapInfo = (BITMAPINFO *) new BYTE[Bitmapinfosize];

	ReadFile(hfile,Dibinfo->DI_BitmapInfo,Bitmapinfosize,&tempsize,NULL);
	
	Dibinfo->DI_Width = Dibinfo->DI_BitmapInfo->bmiHeader.biWidth;
	Dibinfo->DI_Height = Dibinfo->DI_BitmapInfo->bmiHeader.biHeight;
	ByteCount = Dibinfo->DI_BitmapInfo->bmiHeader.biBitCount / 8;

	if(Dibinfo->DI_Height < 0)
		Dibinfo->DI_Height *= -1;
	
	Datasize = ((Dibinfo->DI_Width * ByteCount+3)&~3)*Dibinfo->DI_Height;

	Dibinfo->DI_Bitmap = new BYTE[Datasize];
	ReadFile(hfile,Dibinfo->DI_Bitmap,Datasize,&tempsize,NULL);
	CloseHandle(hfile);

	if(Dibinfo->DI_BitmapInfo->bmiHeader.biBitCount == 8)
	{
		Dibinfo->DI_plogpal = (LOGPALETTE *) new BYTE[sizeof(LOGPALETTE)+255*sizeof(PALETTEENTRY)];
		Dibinfo->DI_plogpal->palVersion = 0x0300;
		Dibinfo->DI_plogpal->palNumEntries = 256;
		
		for(int i = 0; i< 256; i++)
		{
			Dibinfo->DI_plogpal->palPalEntry[i].peRed = Dibinfo->DI_BitmapInfo->bmiColors[i].rgbRed;
			Dibinfo->DI_plogpal->palPalEntry[i].peGreen = Dibinfo->DI_BitmapInfo->bmiColors[i].rgbGreen;
			Dibinfo->DI_plogpal->palPalEntry[i].peBlue = Dibinfo->DI_BitmapInfo->bmiColors[i].rgbBlue;
			Dibinfo->DI_plogpal->palPalEntry[i].peFlags = PC_RESERVED;
		}		
	}
	return Dibinfo;
}

BOOL CDib::GetBitmapDC(LPCSTR filename,HDC screenDC,HDC& hdc)
{
	CDib dib;
	LPCDibinfo bit;

	bit = dib.GetBitmap(filename);
	hdc = CreateCompatibleDC(screenDC);
	HBITMAP bitmap = CreateCompatibleBitmap(screenDC,bit->DI_Width,bit->DI_Height);
	SelectObject(hdc,bitmap);
	SetDIBitsToDevice(hdc,0,0,bit->DI_Width,bit->DI_Height,0,0,0,bit->DI_Height,bit->DI_Bitmap,bit->DI_BitmapInfo,DIB_RGB_COLORS);
	DeleteObject(bitmap);
	bit->DI_Release();

	return TRUE;
}