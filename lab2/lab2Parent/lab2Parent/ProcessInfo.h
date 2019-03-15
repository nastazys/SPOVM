#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include <Windows.h>
#include <iostream>

using namespace std;


class ProcessInfo {
public:
	static int counter;
	STARTUPINFO startupInfo;
	PROCESS_INFORMATION prInformation;
	char* eventName;

	bool isRunning();

	void init();

	ProcessInfo(char* str);

	~ProcessInfo();

	STARTUPINFO getStartupInfo();

	PROCESS_INFORMATION getProcessInformation();

	int createProcess();

	void deleteProcess();

	HANDLE getProcessHandle();
};