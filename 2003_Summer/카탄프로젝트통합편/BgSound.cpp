// BgSound.cpp: implementation of the CBgSound class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BgSound.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBgSound::CBgSound()
{
	BS_Initializemci();
}

CBgSound::~CBgSound()
{

}

void CBgSound::BS_Initializemci()
{
	m_currentposition = 0;
	mciGenericParms.dwCallback = NULL;
	mcisetparms.dwTimeFormat = MCI_FORMAT_MILLISECONDS;
}

bool CBgSound::BS_Openfile(char* filename)
{
	mciOpenParms.dwCallback = NULL;
	mciOpenParms.lpstrDeviceType = "MPEGVideo";
	mciOpenParms.lpstrElementName = filename;
	if(mciSendCommand(NULL,MCI_OPEN,MCI_OPEN_ELEMENT |MCI_OPEN_TYPE,(DWORD)(LPMCI_OPEN_PARMS)&mciOpenParms))
		return false;

	if(!mciSendCommand(mciOpenParms.wDeviceID,MCI_SET,MCI_SET_TIME_FORMAT,(DWORD)(LPMCI_SET_PARMS)&mcisetparms))
		return false;

	return true;
}

bool CBgSound::BS_Play()
{
	mciPlayParms.dwCallback = NULL;
	mciPlayParms.dwFrom = m_currentposition;
	if(!mciSendCommand(mciOpenParms.wDeviceID,MCI_PLAY,MCI_NOTIFY|MCI_FROM,(DWORD)(LPMCI_PLAY_PARMS)&mciPlayParms))
		return false;

	return true;
}

bool CBgSound::BS_Pause()
{
	m_currentposition = BS_Getlength();
	
	if(!mciSendCommand(mciOpenParms.wDeviceID,MCI_PAUSE,MCI_NOTIFY,(DWORD)(LPMCI_GENERIC_PARMS)&mciGenericParms))
		return false;

	return true;
}

bool CBgSound::BS_Close()
{
	m_currentposition = 0;
	if(!mciSendCommand(mciOpenParms.wDeviceID,MCI_CLOSE,MCI_NOTIFY,(DWORD)(LPMCI_GENERIC_PARMS)&mciGenericParms))
		return false;

	return true;
}


bool CBgSound::BS_Stop()
{
	m_currentposition = 0;
	if(!mciSendCommand(mciOpenParms.wDeviceID,MCI_STOP,MCI_NOTIFY,(DWORD)(LPMCI_GENERIC_PARMS)&mciGenericParms))
		return false;

	return true;
}

bool CBgSound::BS_Resume()
{
	if(!mciSendCommand(mciOpenParms.wDeviceID,MCI_RESUME,MCI_NOTIFY,(DWORD)(LPMCI_GENERIC_PARMS)&mciGenericParms))
		return false;

	return true;
}

DWORD CBgSound::BS_Getlength()
{
	MCI_STATUS_PARMS mcistatusparms;
	char error1[256];
	mcistatusparms.dwItem = MCI_STATUS_POSITION;
	MCIERROR error = mciSendCommand(mciOpenParms.wDeviceID,MCI_STATUS,MCI_STATUS_ITEM,(DWORD)(LPMCI_STATUS_PARMS)&mcistatusparms);
	mciGetErrorString(error,error1,sizeof(error1));

//	if(!mciSendCommand(mciOpenParms.wDeviceID,MCI_STATUS,MCI_STATUS_ITEM,(DWORD)(LPMCI_STATUS_PARMS)&mcistatusparms))
//			return 0;

	return mcistatusparms.dwReturn;
}

//////////////////////////////////////////////////////////////////////////////////////////
///
/// class CEfSound
///
//////////////////////////////////////////////////////////////////////////////////////////

bool CEfSound::ES_PlayWave(char* filename)
{
	if(!sndPlaySound(filename,SND_ASYNC))
		return false;

	return true;
}

bool CEfSound::ES_PlayWaveAtmemory(char* buffer)
{
	
	if(!sndPlaySound(buffer,SND_ASYNC|SND_MEMORY))
		return false;

	return false;
}