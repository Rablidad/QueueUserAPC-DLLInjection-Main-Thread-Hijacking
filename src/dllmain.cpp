// dllmain.cpp : Define o ponto de entrada para o aplicativo DLL.
#include "stdafx.h"

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
		MessageBox(NULL, TEXT("Hooked!"), TEXT("Hooked!"), MB_OK);
		break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
		MessageBox(NULL, TEXT("De-Hooking"), TEXT("De-hooking"), MB_OK);
        break;
    }
    return TRUE;
}

