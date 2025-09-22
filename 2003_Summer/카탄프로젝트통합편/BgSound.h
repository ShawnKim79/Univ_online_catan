// BgSound.h: interface for the CBgSound class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BGSOUND_H__FEEC4246_E09A_49B5_9E6C_9EC5DD09027C__INCLUDED_)
#define AFX_BGSOUND_H__FEEC4246_E09A_49B5_9E6C_9EC5DD09027C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include<mmsystem.h>


class CBgSound  
{
public:
	CBgSound();
	virtual ~CBgSound();

	bool BS_Play();
	bool BS_Stop();
	bool BS_Pause();
	bool BS_Close();
	bool BS_Resume();
	void BS_Initializemci();
	bool BS_Openfile(char* filename);

	DWORD BS_Getlength();

	DWORD m_currentposition;

protected:
	MCI_SET_PARMS mcisetparms;
	MCI_PLAY_PARMS mciPlayParms;
	MCI_OPEN_PARMS mciOpenParms;
	MCI_SAVE_PARMS mciSaveParms;
	MCI_GENERIC_PARMS mciGenericParms;
};

class CEfSound
{
	public:
		CEfSound(){};
		~CEfSound(){};
		bool ES_PlayWave(char* filename);
		bool ES_PlayWaveAtmemory(char* buffer);

};

class CComMusic : public CBgSound, public CEfSound
{
	public:
		CComMusic(){};
		~CComMusic(){};
};

#endif // !defined(AFX_BGSOUND_H__FEEC4246_E09A_49B5_9E6C_9EC5DD09027C__INCLUDED_)
