#pragma once
#include <windows.h>

class cConsol{

public:
	HANDLE hout;
	cConsol();
	~cConsol();
	void writeConsol(LPSTR text);	
};

