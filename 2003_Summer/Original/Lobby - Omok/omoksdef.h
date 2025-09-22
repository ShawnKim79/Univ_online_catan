
#pragma once
#include <winsock2.h>

#define OMOKPORT			7777
#define SERVERADDRESS		"203.230.96.173"
#define WM_SOCKETMSG		WM_USER+1
#define FD_SERVERTYPE		FD_READ|FD_ACCEPT|FD_CLOSE
#define FD_CLIENTTYPE		FD_READ|FD_CONNECT|FD_CLOSE
#define DEFAULTSOCKET		0
#define MAXCLIENTSIZE		8
#define WAITQUEUESIZE		8
#define RECVMAXSIZE			65536

////////////////////// msg

#define CHATSTRING			0x43535452		// "CSTR" totalsize(4) | thismsg(4) | stringsize(4)| buff(1..)
#define CHATID				0x43484944		// "CHID" totalsize(4) | thismsg(4) | chatIDsize(4)| buff(1..)
#define DOLPOS				0x444f4c50		// "DOLP" totalsize(4) | thismsg(4) | x-position (2)| y-position(2)
#define ENDTURN				0x454e4454		// "ENDT" totalsize(4) | thismsg(4)
#define STARTGAME			0x53545254		// "STRT" totalsize(4) | thismsg(4) | DOLFLAG
#define RETURNDOLREQUEST	0x5254444c		// "RTDL" totalsize(4) | thismsg(4) 
#define DRAWGAMEREQUEST		0x44524157		// "DRAW" totalsize(4) | thismsg(4) | 
#define ENDGAME				0x454e4447		// "ENDG" totalsize(4) | thismsg(4) | 

////////////////////// msg size

#define DOLPOSSIZE			12
#define ENDTURNSIZE			8
#define STARTGAMESIZE		8
#define RETURNDOLSIZE		12
#define DRAWGAMESIZE		12
#define ENDGAMESIZE			8

///////////////// return dol

#define RETURNDOL_MSG_REQUEST			0x00			// request
#define RETURNDOL_MSG_ACCEPT			0x01			// accept
#define RETURNDOL_MSG_REFUSE			0x02			// refuse

////////////////DRAWGAME

#define DRAWGAME_MSG_REQUEST		0x00
#define DRAWGAME_MSG_ACCEPT			0x01
#define DRAWGAME_MSG_REFUSE			0x02