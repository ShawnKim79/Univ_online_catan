
#ifndef __cConsole_H__
#define __cConsole_H__

#include <winbase.h>

class cConsole{
	
	HANDLE m_hout;
	DWORD m_nocw;
	static cConsole* m_Console_instance;
	
	cConsole();
	~cConsole();
	
public:
	static cConsole* GetInstance(); 
	void Write(char* buff,...);
	void DeleteConsole();
};

cConsole* cConsole::m_Console_instance = 0;

cConsole::cConsole()
{
	m_hout = NULL;
	m_hout = GetStdHandle(STD_OUTPUT_HANDLE);
}

cConsole::~cConsole()
{
	m_Console_instance = NULL;
}

cConsole* cConsole::GetInstance()
{
	
	if(!m_Console_instance)
	{
		AllocConsole();
		m_Console_instance = new cConsole;
	}
	
	return m_Console_instance;
}

void cConsole::DeleteConsole()
{
	if(m_hout)
	{
		FreeConsole();
		CloseHandle(m_hout);
		delete this;
	}
}

void cConsole::Write(char* buff,...)
{
	char buf1[256] = {0,};
	m_nocw = wvsprintf(buf1,buff,(LPSTR)(&buff + 1));
	WriteConsole(m_hout,buf1,m_nocw,&m_nocw,NULL);
	
}


#endif //__cConsole_H__