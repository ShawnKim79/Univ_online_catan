#include "bingGoManage.h"

HINSTANCE g_hInst;
BingGoManage* BingGo = NULL;

extern "C" __declspec(dllexport) BingGoManage* getGameObject();

BOOL WINAPI DllMain(HINSTANCE hinstDLL,DWORD fdwReson,LPVOID lpvReserved)
{
	switch(fdwReson)
	{
	case DLL_PROCESS_ATTACH:
		g_hInst = hinstDLL;
		break;
	case DLL_PROCESS_DETACH:
		{
			if(BingGo != NULL)
			{
				delete BingGo;
				BingGo = NULL;
			}

		}
	}
	return TRUE;
}

extern "C" __declspec(dllexport) BingGoManage* getGameObject()
{
	BingGo = new BingGoManage();
	BingGo->hInst = g_hInst;
	return BingGo;
}