#define numProcs 3
#include <windows.h>
#include <iostream>
#include <stdio.h>
#include <process.h>
#include <vector>
#include <string>

using namespace std;
void main(VOID)
{
	STARTUPINFO StartupInfo[numProcs];
	PROCESS_INFORMATION ProcInfo[numProcs];
	TCHAR CommandLine1[] = TEXT("c:/Users/nasta/source/repos/spovm/lab1/child0/Debug/child0.exe");
	TCHAR CommandLine2[] = TEXT("c:/Users/nasta/source/repos/spovm/lab1/child1/Debug/child1.exe");
	DWORD consoleParam = 0;
	HANDLE processes[numProcs];
	for (int i = 0; i < numProcs; i++)
	{
		ZeroMemory(&StartupInfo[i], sizeof(StartupInfo[i]));
		StartupInfo[i].cb = sizeof(StartupInfo[i]);
		ZeroMemory(&ProcInfo[i], sizeof(ProcInfo[i]));
		if (CreateProcess(NULL,
			CommandLine1,
			NULL, // Дескриптор процесса не наследуется.
			NULL, // Дескриптор потока не наследуется.
			FALSE, // Установка описателей наследования
			consoleParam, // флаг создания процесса
			NULL, // Блок переменных окружения родительского процесса
			NULL, // Использовать текущий каталог родительского процесса
			&StartupInfo[i],
			&ProcInfo[i]))
		{
			processes[i] = ProcInfo[i].hProcess;
		}
		else
		{
			printf("CreateProcess failed.");
			cout << endl << endl << endl;
			return;
		}
		lstrcpy(CommandLine1, CommandLine2);
		consoleParam = CREATE_NEW_CONSOLE;
	}
	SYSTEMTIME lt; int k = 0;
	while (WaitForMultipleObjects(numProcs, processes, 1, 50))
	{
		SYSTEMTIME lt;
		GetLocalTime(&lt);
		printf("					%u - %02d:%02d:%02d\r", _getpid(), lt.wHour, lt.wMinute, lt.wSecond);
	}
	cout << endl << endl;
	for (int i = 0; i < numProcs; i++)
	{
		CloseHandle(processes[i]);
	}
}