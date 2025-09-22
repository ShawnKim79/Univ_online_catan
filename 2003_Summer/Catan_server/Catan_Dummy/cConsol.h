#pragma once
#include <windows.h>

#define TRACEMAXSTRING	1024

class cConsol{

public:
	void SetConsoleTitle(LPCTSTR lpszTitle);
	HANDLE hout;
	cConsol();
	~cConsol();
	void writeConsole(const char *format, ...);

protected:
	char szBuff[TRACEMAXSTRING];
};

