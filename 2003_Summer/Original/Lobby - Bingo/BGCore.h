#define BG_B_EMPTY		0x00
#define BG_B_NORMAL		0x00 // not select
#define BG_B_PUSHED		0x01 // selected 

#define BOARDWIDTH		350
#define BOARDHEIGHT		350

#define BOARD_POS_X		21
#define BOARD_POS_Y		22

#define BOARDLENGTH		70

#define PENWIDTH		3

#define BG_B_WIDTH		5
#define BG_B_HEIGHT		5

#define CLIENTWIDTH		560 // 560 800
#define CLIENTHEIGHT	550 // 550 600

#define BG_D_COUNT		5

#define STARTX			20			// startx pos prev = 15
#define STARTY			20			// starty pos prev = 15

//talk window
//const int EDITWIDTH		= MAXLENGTHX-STARTX;		//#define EDITWIDTH		800 // 517 // 33 
#define EDITSTARTY		517
#define EDITWIDTH		355
#define EDITHEIGHT		14 // prev = 20

// chat window
#define CHATVIEWSTARTY	408
#define CHATVIEWWIDTH	EDITWIDTH			//#define 	800 // 408
#define CHATVIEWHEIGHT	96 // 85

#define NOTICEWIDTH		138
#define NOTICEHEIGHT	126

//const int NOTICEPOSX	=  406;
//const int NOTICEPOSY	=  328;

#define NOTICEPOSX 406
#define NOTICEPOSY 328



///////////////////////////////////////////////////////////////////////////////////////////
// Socket
#define OMOKPORT			7777
#define SERVERADDRESS		"203.230.107.35"
#define WM_SOCKETMSG		WM_USER+1
#define FD_SERVERTYPE		FD_READ|FD_ACCEPT|FD_CLOSE
#define FD_CLIENTTYPE		FD_READ|FD_CONNECT|FD_CLOSE
#define DEFAULTSOCKET		0
#define MAXCLIENTSIZE		8
#define WAITQUEUESIZE		8
#define RECVMAXSIZE			65536

#define CHATSTRING			0x43535452		// "CSTR" totalsize(4) | thismsg(4) | stringsize(4)| buff(1..)
#define STARTGAME			0x53545254		// "STRT" totalsize(4) | thismsg(4) | DOLFLAG
#define ENDTURN				0x454e4454		// "ENDT" totalsize(4) | thismsg(4)
#define ENDGAME				0x454e4447		// "ENDG" totalsize(4) | thismsg(4) | 
#define SELECTNUMBER		0x53544E4C		// "STNM" totalsize(4) | thismsg(4) | index(i) (X|Y)

#define ENDTURNSIZE			8
#define STARTGAMESIZE		8
#define ENDGAMESIZE			8
#define SELECTNUMBERSIZE	16
///////////////////////////////////////////////////////////////////////////////////////////

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

// START BUTTON 

#define STARTWIDTH			128
#define STARTHEIGHT			128

#define START_POS_X			111		//132
#define START_POS_Y			110		//132

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

// sbutton
// NORMAL
// OVER
// DOWN
// DISABLE

#define SBUTTONWIDTH			80
#define SBUTTONHEIGHT			20

#define DRAWBUTTON_POS_X		400
#define DRAWBUTTON_POS_Y		480

#define GIVEUP_POS_X			400
#define GIVEUP_POS_Y			504	

// ebutton
// NORMAL
// OVER
// DOWN

#define EBUTTONWIDTH			63
#define EBUTTONHEIGHT			68

#define EBUTTON_POS_X			484
#define EBUTTON_POS_Y			466

/////////// send Lobby Message
#define ONGAME				1
#define OUTGAME				0

#define DRAWGAME			0
#define VICTORYGAME			1
#define LOSEGAME			2
#define CORRECTEND			4