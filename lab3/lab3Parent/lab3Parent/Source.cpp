#define _CRT_SECURE_NO_WARNINGS
#define numProcs 1

#include <iostream>
#include <fstream>
#include <string>
#include <Windows.h>
#include <curl/curl.h>

using namespace std;

struct Weather
{
	char temp[100] = { '\0' };
	char description[100] = { '\0' };
	char humidity[100] = { '\0' };
	char pressure[100] = { '\0' };

};

/*HANDLE CreateNamedPipe(
  LPCTSTR lpName,   
  DWORD   dwOpenMode,     
  DWORD   dwPipeMode,      
  DWORD   nMaxInstances,   
  DWORD   nOutBufferSize,  
  DWORD   nInBufferSize,   
  DWORD   nDefaultTimeOut, 
  LPSECURITY_ATTRIBUTES lpSecurityAttributes); */

HANDLE hWeathPipe = CreateNamedPipe("\\\\.\\pipe\\WeatherSynchro",
	PIPE_ACCESS_DUPLEX,
	PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,
	PIPE_UNLIMITED_INSTANCES,
	100, 100, 500, NULL);

DWORD WritePipe(string request)
{
	DWORD  cbWrite;
	WriteFile(hWeathPipe, request.c_str(), request.length(), &cbWrite, NULL);
	return cbWrite;
}

DWORD ReadPipe(char* array)
{
	DWORD  cbRead;
	ReadFile(hWeathPipe, array, 100, &cbRead, NULL);
	if (array[0] == 'e' && array[1] == 'r' && array[2] == 'r' && array[3] == 'o' && array[4] == 'r')
	{
		throw 1;
	}
	return cbRead;
}


void main(int argc, char* argv[])
{
	STARTUPINFO StartupInfo[numProcs];
	PROCESS_INFORMATION ProcInfo[numProcs];
	TCHAR CommandLine[] = TEXT("C:/Users/nasta/source/repos/spovm/lab3/lab3Child0/Debug/lab3Child0.exe");
	for (int i = 0; i < numProcs; i++)
	{
		ZeroMemory(&StartupInfo[i], sizeof(StartupInfo[i]));
		StartupInfo[i].cb = sizeof(StartupInfo[i]);
		ZeroMemory(&ProcInfo[i], sizeof(ProcInfo[i]));
		if (!CreateProcess(NULL,
			CommandLine,
			NULL,
			NULL,
			FALSE,
			0,
			NULL,
			NULL,
			&StartupInfo[i],
			&ProcInfo[i]))
		{
			printf("CreateProcess failed.");
			cout << endl << endl << endl;
			return;
		}
	}
	if (!ConnectNamedPipe(hWeathPipe, NULL))
		cout << "er";
	char k = '+';
	char message[] = "";
	while (k != 'q')
	{
		try {
			system("cls");
			string city = "";
			Weather currentWeath;
			cout << "Enter city: ";
			cin >> city;
			WritePipe(city);
			ReadPipe(message);
			WritePipe("description");
			ReadPipe(currentWeath.description);
			cout << "Weather info: " << currentWeath.description << endl;

			WritePipe("temp");
			ReadPipe(currentWeath.temp);
			cout << "Current temperature: " << (atoi(currentWeath.temp) - 273) << " 'C" << endl;

			WritePipe("humidity");
			ReadPipe(currentWeath.humidity);
			cout << "Humidity: " << currentWeath.humidity << "%" << endl;

			WritePipe("pressure");
			ReadPipe(currentWeath.pressure);
			cout << "Pressure: " << currentWeath.pressure << " hPa" << endl;
			cout << "Press q to exit or + to continue: ";
			cin >> k;
		}
		catch (int e)
		{
			switch (e) {
			case 1: {
				Sleep(2500);
				continue;
				break;
			}
			}
		}
	}
	delete[] message;
	CloseHandle(hWeathPipe);
	for (int i = 0; i < numProcs; i++)
		CloseHandle(ProcInfo[i].hProcess);
	return;
}