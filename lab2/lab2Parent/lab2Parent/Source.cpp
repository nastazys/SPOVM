#define _CRT_SECURE_NO_WARNINGS

#include <Windows.h>
#include <conio.h>

#include <iostream>
#include <string>
#include <fstream>
#include <iterator>
#include <vector>

#include "ProcessInfo.h"

using namespace std;

vector<ProcessInfo*> processes;

HANDLE writingToConsoleEnable = CreateEvent(NULL, TRUE, TRUE, TEXT("writingToConsoleEnable"));
HANDLE readingFromFileEnable = CreateEvent(NULL, TRUE, TRUE, TEXT("readingFromFileEnable"));

void copyJackPoem() {
	std::vector<std::string> vec;
	std::ifstream file("c:/Users/nasta/source/repos/spovm/lab2/TheHouseJackBuilt.txt");
	if (file.is_open())
	{
		std::string str;
		while (std::getline(file, str))
			vec.push_back(str);
		file.close();
		std::ofstream outfile("c:/Users/nasta/source/repos/spovm/lab2/strings.txt");
		if (outfile.is_open())
		{
			std::copy(vec.begin(), vec.end(),
				std::ostream_iterator<std::string>(outfile, "\n"));
			outfile.close();
		}
	}
}

void deleteAllProcesses() {
	while (processes.size() > 0) {
		cout << endl << "Terminating of all running processes" << endl;
		ProcessInfo* procInfo = processes.back();
		processes.pop_back();
		procInfo->deleteProcess();
	}
	copyJackPoem();
	CloseHandle(writingToConsoleEnable);
	CloseHandle(readingFromFileEnable);
	exit(0);
}

BOOL WINAPI ConsoleHandler(DWORD CEvent) {

	switch (CEvent) {
	case CTRL_CLOSE_EVENT:
		deleteAllProcesses();
	}
	return TRUE;
}

int main(int args, char** argv) {

	SetConsoleCtrlHandler((PHANDLER_ROUTINE)ConsoleHandler, TRUE);

	cout << "Enter to create new process: '+', to delete last created: '-', quit: 'q'" << endl;
	while (true) {
		char iSymbol;

		if (_kbhit()) {
			iSymbol = _getch();
		}
		else {
			iSymbol = '.';
		}

		WaitForSingleObject(writingToConsoleEnable, INFINITE);
		ResetEvent(writingToConsoleEnable);
		switch (iSymbol) {

		case '=': case '+': { 
			ProcessInfo* process_info = new ProcessInfo(NULL);
			if (process_info->createProcess() == 0) {
				processes.push_back(process_info);
			}
			break;
		}

		case '-': { 
			if (processes.size() > 0) {
				cout << "Terminating of last created process: " << endl;
				ProcessInfo* process_info = processes.back();
				processes.pop_back();
				process_info->deleteProcess();
			}
			else {
				cout << "There are no running processes." << endl;
			}
			break;
		}

		case 'q': { 
			deleteAllProcesses();
			exit(0);
			break;
		}

		default:
			break;
		}
		SetEvent(writingToConsoleEnable);
	}

	return 0;
}