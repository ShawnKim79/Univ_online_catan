#include "stdafx.h"
#include "cConsol.h"

cConsol::cConsol()
{
#ifdef _DEBUG
	AllocConsole();
	hout = GetStdHandle(STD_OUTPUT_HANDLE);
#endif
}

cConsol::~cConsol()
{
	FreeConsole();
}

void cConsol::writeConsol(LPSTR text)
{
#ifdef _DEBUG
	DWORD size  = strlen(text);
	WriteConsole(hout,text,size,&size,0);
#endif
}