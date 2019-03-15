#include "ProcessInfo.h"
#include <conio.h>

void getRandomString(char* &str, const int length) {
	static const char characters[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
	str = new char[length];

	for (int i = 0; i < length; i++) {
		str[i] = characters[rand() % (sizeof(characters) - 1)];
	}
	str[length - 1] = '\0';
}

ProcessInfo::ProcessInfo(char* str) {

	eventName = new char[10];
	getRandomString(eventName, 10);
	init();
}

ProcessInfo::~ProcessInfo() {
	deleteProcess();
}

void ProcessInfo::init() {
	ZeroMemory(&startupInfo, sizeof(startupInfo));
	startupInfo.cb = sizeof(startupInfo);
	ZeroMemory(&prInformation, sizeof(prInformation));
}

STARTUPINFO ProcessInfo::getStartupInfo() {
	return startupInfo;
}

PROCESS_INFORMATION ProcessInfo::getProcessInformation() {
	return prInformation;
}

int ProcessInfo::createProcess() {

	int index = CreateProcess(
		TEXT("c:/Users/nasta/source/repos/spovm/lab2/lab2Child/Debug/lab2Child.exe"),
		TEXT(eventName), NULL, NULL, FALSE, /*CREATE_NEW_CONSOLE*/ 0, NULL, NULL, &startupInfo, &prInformation);

	if (index <= 0) {
		cout << "Create Process failed, error code: " << GetLastError() << "." << endl;
		_getch();
		return -1;
	}
	else {
		cout << "New Process has been created, str: ";
		cout << eventName << endl;
		return 0;
	}

}

void ProcessInfo::deleteProcess() {
	int info = 0;
	TerminateProcess(prInformation.hProcess, info);
	CloseHandle(prInformation.hProcess);
	CloseHandle(prInformation.hThread);
	cout << "Process has been stopped. Code: " << info << endl;
}

bool ProcessInfo::isRunning() {
	DWORD exit_code = 0;
	GetExitCodeProcess(prInformation.hProcess, &exit_code);
	if (exit_code == STILL_ACTIVE) {
		return true;
	}
	else {
		return false;
	}
}

HANDLE ProcessInfo::getProcessHandle() {
	return prInformation.hProcess;
}