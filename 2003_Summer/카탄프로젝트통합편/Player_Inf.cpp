// Player_Inf.cpp: implementation of the CPlayer_Inf class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "īź.h"
#include "Player_Inf.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPlayer_Inf::CPlayer_Inf()
{
	for(int i=0;i<4;i++){
		ps[i].cardn=0;
		ps[i].soldier=0;
		ps[i].enable=false;
		strcpy(ps[i].name,"");
	}
	con=0;
	start=false;
	for(i=0;i<5;i++)resource[i]=0;
	for(i=0;i<100;i++)tmp[i]=0;

	turn=false;
	who_turn=5;
	
	for(i=0;i<25;i++){card[i]=0;}
	cardn=0;
	newcardn=0;
	carduse=false;

	settle=0;
	city=0;
	road=0;
	p_n=999;
}

CPlayer_Inf::~CPlayer_Inf()
{

}
