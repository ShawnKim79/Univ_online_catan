// tile_Inf.cpp: implementation of the Ctile_Inf class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "īź.h"
#include "tile_Inf.h"
#include <windows.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Ctile_Inf::Ctile_Inf()
{
	dat=NULL;
}

Ctile_Inf::~Ctile_Inf()
{
	for(int i=0;i<datn;i++) delete dat[i].bitmap;
	delete dat;
}

char *Ctile_Inf::load_bmp(char *filename)
{
	int max_x,max_y,x,y;
	char bit;
	tile_sub *link = dat;
	
	if(dat) {
		while(1) {
			if(!strcmp(link->filename,filename)) return link->bitmap;

			if(link->next==NULL) {
				link->next = new(tile_sub);
				link = link->next;
				link->next = NULL;
				break;
			}
			link = link->next;
		}
	} else {
		dat = new (tile_sub);
		link = dat;
		link->next = NULL;
	}

	strcpy(link->filename,filename);
	
	FILE *fp;

	fp=fopen(filename,"rb");
		
	fseek(fp,18,0);fread(&max_x,4,1,fp);
	fseek(fp,22,0);fread(&max_y,4,1,fp);
		
	link->bitmap = new(char [max_x*max_y*3+8]);
	memcpy(link->bitmap,&max_x,4);
	memcpy(link->bitmap+4,&max_y,4);
		
	fseek(fp,54,0);
	
	for(y=0;y<max_y;y++) {
		int bit_n=max_x*3;
		fread(link->bitmap+(max_y-y-1)*bit_n+8,max_x,3,fp);
		if(bit_n%4) for(x=0;x<4-(bit_n%4);x++) fread(&bit,1,1,fp);
	}

	fclose(fp);

	datn++;
	return link->bitmap;
}
