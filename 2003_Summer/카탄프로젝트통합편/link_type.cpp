// link_type.cpp: implementation of the link_type class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "īź.h"
#include "link_type.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

link_type::link_type()
{
	int i;
	for(i=0;i<3;i++)road[i]=0;
	link_max=0;
	city=0;
}

link_type::~link_type()
{

}

void link_type::add_point(link_type *p)
{
	for(int i=0;i<link_max;i++)if(link[i]==p) return;
	link[link_max++] = p;
}

int link_type::road_num(link_type *p)
{
	for(int i=0;i<link_max;i++) if(link[i] == p) return i;
	return -1;
}

