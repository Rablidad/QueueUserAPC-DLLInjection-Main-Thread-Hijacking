
#include "pch.h"
#include <iostream>
#include <windows.h>
#include <cstdio>

int main()
{
	DWORD PID = GetCurrentProcessId();
	ULONG parameter = 5;

	while (true)
	{
		printf("Waiting to be hooked!: PID: %d\n", PID);
		SleepEx(1000, TRUE);
	}
	return (EXIT_SUCCESS);
}