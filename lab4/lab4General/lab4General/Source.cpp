#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <fstream>
#include <string>
#include <Windows.h>
#include <curl/curl.h>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

using namespace std;

static size_t writeFile(char *data, size_t size, size_t nmemb, FILE* pBuf) 
{
	size_t written = 0;
	if (pBuf != NULL) 
	{
		written = fwrite(data, size, nmemb, pBuf);
	}
	return written;
}

struct downloadInfo
{
	int count = 0;
	string url;
	string fileName;
	double totalToDownload;
	double nowDownloaded;
	double lastDownloaded;
	COORD position;
};

CRITICAL_SECTION csConsole;

int downloadProgress(void* ptr, double TotalToDownload, double NowDownloaded, double TotalToUpload, double NowUploaded)
{
	struct downloadInfo *progress = (struct downloadInfo *)ptr;
	progress->nowDownloaded = NowDownloaded;
	progress->totalToDownload = TotalToDownload;

	if (progress->nowDownloaded / 1024 != 0)
	{
		progress->lastDownloaded = progress->nowDownloaded / 1024;
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		CONSOLE_SCREEN_BUFFER_INFO curPosition;
		GetConsoleScreenBufferInfo(hConsole, &curPosition);
		COORD newPosition = curPosition.dwCursorPosition;
		newPosition.Y += 17;
		newPosition.X = 0;

		EnterCriticalSection(&csConsole);
		SetConsoleCursorPosition(hConsole, newPosition);
		printf("Thread #%d is downloading file: %d kbytes \n", GetThreadId(GetCurrentThread()), (int)progress->lastDownloaded);
		SetConsoleCursorPosition(hConsole, curPosition.dwCursorPosition);
		LeaveCriticalSection(&csConsole);
	}
	progress->count+=1;
	return 0;
}

FILE* outFile;
int counter = 0;
void downloader(struct downloadInfo* startInfo)
{
	static string lastFile;
	CURL *curl;
	CURLcode res;
	try
	{
		curl = curl_easy_init();
		if (curl) 
		{
			FILE* outFile = fopen(startInfo->fileName.c_str(), "wb");
				if (!outFile)
					throw 1;
			curl_easy_setopt(curl, CURLOPT_URL, startInfo->url.c_str());
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, outFile);
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeFile);
			
			HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
			CONSOLE_SCREEN_BUFFER_INFO curPosition;
			GetConsoleScreenBufferInfo(hConsole, &curPosition);
			startInfo->position = curPosition.dwCursorPosition;

			curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0L); //to make progress function actually called
			curl_easy_setopt(curl, CURLOPT_PROGRESSFUNCTION, downloadProgress);
			curl_easy_setopt(curl, CURLOPT_PROGRESSDATA, startInfo);
			res = curl_easy_perform(curl);
			if (res != CURLE_OK)
				throw 2;
			fclose(outFile);
			// always cleanup 
			curl_easy_cleanup(curl);
		}
	}
	catch (int code)
	{
		EnterCriticalSection(&csConsole);
		switch (code)
		{
		case 1: cout << GetThreadId(GetCurrentThread()) << "file error!" << endl;
			break;
		case 2: cout << GetThreadId(GetCurrentThread()) << "connection error!" << endl;
			break;
		case 3: cout << GetThreadId(GetCurrentThread()) << "writeFile error!" << endl;
			break;
		}
		LeaveCriticalSection(&csConsole);
	}
}

vector <string> getCommand()
{
	string str;
	getline(cin, str);
	while (str.length() == 0)
		getline(cin, str);
	istringstream iss(str);
	string temp;
	vector<string> result;
	while (iss >> temp)
	{
		result.push_back(temp);
	}
	return result;
}


int main()
{
	vector<HANDLE> threads;
	vector <downloadInfo> info;
	InitializeCriticalSection(&csConsole);
	while (1)
	{
		downloadInfo tempinfo;
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		CONSOLE_SCREEN_BUFFER_INFO curPosition;
		GetConsoleScreenBufferInfo(hConsole, &curPosition);
		//EnterCriticalSection(&csConsole);
		if (curPosition.dwCursorPosition.Y == 12)
		{
			system("cls");
			curPosition.dwCursorPosition.Y = 0;
			SetConsoleCursorPosition(hConsole, curPosition.dwCursorPosition);
		}
		vector<string> command = getCommand();
		//LeaveCriticalSection(&csConsole);
		if (command[0] == "exit")
		{
			for (auto thr : threads)
			{
				CloseHandle(thr);
			}
			return 0;
		}
		if (command[0] == "down")
		{
			tempinfo.fileName = command[1];
			tempinfo.url = command[2];
			info.push_back(tempinfo);
			threads.push_back(CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)downloader, &info[threads.size()], 0, NULL));
			if (!threads[threads.size() - 1])
			{
				EnterCriticalSection(&csConsole);
				cout << "CreateThread error!" << endl;
				LeaveCriticalSection(&csConsole);
			}
		}
		//WaitForSingleObject(threads[0], INFINITE);
	}
	for (auto thr : threads)
	{
		CloseHandle(thr);
	}
	system("pause");
	return 0;
}