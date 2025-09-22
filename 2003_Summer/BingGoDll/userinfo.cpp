#include "userinfo.h"

USERINFOEX::~USERINFOEX()
{
	if(strpoint != NULL)
		delete []strpoint;

	if(strinfo != NULL)
		delete []strinfo;
}
