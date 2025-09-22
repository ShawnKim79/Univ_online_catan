#include "stdafx.h"
#include "cConsol.h"
#include <stdarg.h>
#include <stdio.h>

cConsol::cConsol()
{
	hout = NULL;
#ifdef _DEBUG
	AllocConsole();
	hout = GetStdHandle(STD_OUTPUT_HANDLE);
#endif
}

cConsol::~cConsol()
{
	if(hout) FreeConsole();
}

void cConsol::writeConsole(const char *format, ...)
{
#ifdef _DEBUG
	va_list args;
	va_start(args,format);
	DWORD nBuf = _vsnprintf(szBuff, TRACEMAXSTRING, format, args);
	va_end(args);

	WriteConsole(hout, szBuff, nBuf, &nBuf, 0);
#endif
}

void cConsol::SetConsoleTitle(LPCTSTR lpszTitle)
{
	::SetConsoleTitle(lpszTitle);
}
