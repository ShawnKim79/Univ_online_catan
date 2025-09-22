#pragma once

// main window
#define CLIENTWIDTH		560 // 560 800
#define CLIENTHEIGHT	550 // 550 600

// caret timer id
#define ID_CARET		0
#define IDT_TIMELIMITE	1
#define IDT_MOVETIMER	2

#define LENGTH			25			// length of side in pixel 30

#define ROWLINE			15			// rows-line count
#define COLUMNLINE		15			// column

// pixel line start-position
#define STARTX			20			// startx pos prev = 15
#define STARTY			20			// starty pos prev = 15

#define RADIUS			6			// radius

#define DOLRADIUS		10

///////////////////////////////////////////////////////////////

const int CENTERX		= ROWLINE/2;
const int CENTERY		= COLUMNLINE/2;
const int HALFLENGTH	= LENGTH/2;
const int MAXLENGTHX	= LENGTH*(COLUMNLINE+1);
const int MAXLENGTHY	= LENGTH*(ROWLINE+1);

/////////////////////////////////////////////////////////////////

//talk window
//const int EDITWIDTH		= MAXLENGTHX-STARTX;		//#define EDITWIDTH		800 // 517 // 33 
#define EDITSTARTY		517
#define EDITWIDTH		355
#define EDITHEIGHT		14 // prev = 20

// chat window
#define CHATVIEWSTARTY	408
#define CHATVIEWWIDTH	EDITWIDTH			//#define 	800 // 408
#define CHATVIEWHEIGHT	96 // 85

//notice window
//#define NOTICECAPX		20		
//#define NOTICECAPY		100

#define NOTICEWIDTH		138
#define NOTICEHEIGHT	126

const int NOTICEPOSX	=  406;
const int NOTICEPOSY	=  328;

// checker 
#define BLACK_DOL		'B'
#define WHITE_DOL		'W'
#define NONE_DOL		'N'


#define OM_PASSTURN		WM_USER+1

/////////////////////////////////////////////////////////////////

#define WM_GAMESTATEUPDATE WM_USER+33

/////////////////////////////////////////////////////////////////

#define MYINFOPOSX		500
#define MYINFOPOSY		100

#define	LIMITETIME		29 // 30 second

/////////////////////////////////////////////////////////////////
// 63/68
// 80/20

// button image info

#define BUTTONWIDTH			80
#define BUTTONHEIGHT		20

#define PUSHBUTTONSTARTX	400 

#define PUSHBUTTON1STARTY	467

#define PUSHBUTTON2STARTY	491

#define PUSHBUTTON3STARTY	515

#define BUTTONMAPWIDTH		320
#define BUTTONMAPHEIGHT		128

#define BIGBUTTONSTARTX		485
#define BIGBUTTONSTARTY     467

#define BIGBUTTONWIDTH		63
#define BIGBUTTONHEIGHT		68

// clock info

#define CLOCKWIDTH			26
#define CLOCKHEIGHT			27

#define CLOCK_POS_X			480	

#define MYCLOCK_POS_Y		116
#define COMCLOCK_POS_Y		267


////////////////////////////// omokimage

#define OMOKIMAGEWIDTH		996
#define OMOKIMAGEHEIGHT		256

// picture of color-dol		

#define DOLWIDTH			33
#define DOLHEIGHT			26

#define DOL_POS_X			416

#define MYDOL_POS_Y			114
#define COMDOL_POS_Y		265

// WIN and LOST text info

#define WINANDLOSTWIDTH		231
#define WINANDLOSTHEIGHT	99

#define WINANDLOST_POS_X	81
#define WINANDLOST_POS_Y	145

// start text

#define STARTTEXTWIDTH		278
#define STARTTEXTHEIGHT		100

#define STARTTEXT_POS_X		56
#define STARTTEXT_POS_Y		146

// start button

#define STARTWIDTH			128
#define STARTHEIGHT			128

#define START_POS_X			132
#define START_POS_Y			132


/////////// send Lobby Message
#define ONGAME				1
#define OUTGAME				0

#define DRAWGAME			0
#define VICTORYGAME			1
#define LOSEGAME			2
#define CORRECTEND			4


// text out INFO

#define NICKNAMEX			416

#define	MYNICKNAME_Y		15
#define COMNICKNAME_Y		166

#define FILEDBOX_X			460

#define MYID_Y				54
#define MYPOINT_Y			74
#define MYINFO_Y			94

#define COMID_Y				205
#define COMPOINT_Y			225
#define	COMINFO_Y			245		

// timer textout info

#define TIMETEXTWIDTH		36
#define TIMETEXTHEIGHT		27

#define TIMETEXT_POS_X		506

#define MYTIMETEXT_POS_Y	114
#define COMTIMETEXT_POS_Y	265