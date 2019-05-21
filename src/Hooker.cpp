#include "pch.h"
#include <windows.h>
#include <cstdlib>
#include <iostream>
#include <cstdio>
#include <TlHelp32.h>

DWORD ProcessInfoo(LPWSTR PName)
{
	HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	PROCESSENTRY32 Process;
	Process.dwSize = sizeof(PROCESSENTRY32);
	if (Process32First( snap , &Process))
	{
		while (Process32Next(snap , &Process))
		{
			if (wcsncmp(PName, Process.szExeFile, lstrlen(PName)) == 0)
			{
				std::wcout << TEXT("Found!") << std::endl;
				CloseHandle(snap);
				return Process.th32ProcessID;
			}
		}
	}
	CloseHandle(snap);
	return (EXIT_FAILURE);
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
				CloseHandle(snap);
				return thread.th32ThreadID;
			}
		}
	}

	CloseHandle(snap);
	return (EXIT_FAILURE);
}

int wmain(int argc, LPWSTR * argv)
{
	// usage Hooker.exe <processname.exe> <C:\\Path\\To\\DLL.dll>

	if (argc != 3)
	{
		std::wcout << TEXT("USAGE: Hooker.exe <processname.exe> <C:\\Path\\To\\DLL.dll>") << std::endl;
		ExitProcess(1);
	}

	WCHAR * DllPath = argv[2];
	
	DWORD dwSize = ( lstrlenW(DllPath) + 1 ) * sizeof(wchar_t);
	DWORD PID = ProcessInfoo(argv[1]);
	DWORD TID = GetThreadInfoo(PID);

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
