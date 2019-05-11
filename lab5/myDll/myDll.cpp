#include "stdafx.h"
#include "my_wstr.h"
#include <vector>
#include <iostream>

#define NUM_OF_SYMBOLS 100
#define STR_SIZE 64

using namespace std;

HANDLE event_output;
HANDLE event_input;
HANDLE event_close;
uString out_filename("result.txt");
char buffer[NUM_OF_SYMBOLS + 1];
using namespace std;

vector<wstring> getFilesNames(string folder)
{
	vector<wstring> names;
	uString search_path (folder);
	WIN32_FIND_DATA fd;
	HANDLE hFind = ::FindFirstFile(search_path.getWstring().c_str(), &fd);
	if (hFind != INVALID_HANDLE_VALUE) {
		do {
			// read all (real) files in current folder
			// , delete '!' read other 2 default folder . and ..
			if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				if (fd.cFileName != out_filename.getWstring())
					names.push_back(fd.cFileName);
			}
		} while (::FindNextFile(hFind, &fd));
		::FindClose(hFind);
	}
	return names;
}

extern "C" __declspec(dllexport) void read() {
	event_output = CreateEvent(NULL, false, false, NULL);
	event_input = CreateEvent(NULL, false, true, NULL);
	event_close = CreateEvent(NULL, false, false, NULL);

	vector<wstring> names = getFilesNames("c:/Users/nasta/source/repos/temp/*.txt");
	DWORD bytes_in_file;
	OVERLAPPED overlapped;
	overlapped.hEvent = CreateEvent(NULL, true, true, NULL);

	/*BOOL ReadFile(
		HANDLE       hFile,
		LPVOID       lpBuffer,
		DWORD        nNumberOfBytesToRead,
		LPDWORD      lpNumberOfBytesRead,
		LPOVERLAPPED lpOverlapped
	);*/
	for (int i = 0; i < names.size(); i++) 
	{
		overlapped.Offset = 0;
		overlapped.OffsetHigh = 0;
		HANDLE handler_file = CreateFile(names[i].c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);

		do {
			WaitForSingleObject(event_input, INFINITE);
			memset(buffer, '\0', sizeof(buffer));
			if (ReadFile(handler_file, buffer, NUM_OF_SYMBOLS, &bytes_in_file, &overlapped) == 0) {
				break;
			}
			WaitForSingleObject(overlapped.hEvent, INFINITE);
			overlapped.Offset += bytes_in_file;
			SetEvent(event_output);

		} while (bytes_in_file == NUM_OF_SYMBOLS);
		CloseHandle(handler_file);
	}
	SetEvent(event_close);
	CloseHandle(overlapped.hEvent);

}

extern "C" __declspec(dllexport) void write() {
	TCHAR file[STR_SIZE] = TEXT("result.txt");
	HANDLE handler_file = CreateFile(file, GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
	DWORD bytes_in_file;
	OVERLAPPED overlapped;

	overlapped.hEvent = CreateEvent(NULL, true, true, NULL);
	overlapped.Offset = 0;
	overlapped.OffsetHigh = 0;

	while (true) {

		if (WaitForSingleObject(event_output, 0) == WAIT_OBJECT_0) {

			WriteFile(handler_file, &buffer, strlen(buffer), &bytes_in_file, &overlapped);

			WaitForSingleObject(overlapped.hEvent, INFINITE);
			overlapped.Offset += bytes_in_file;
			SetEvent(event_input);
		}
		if (WaitForSingleObject(event_close, 0) == WAIT_OBJECT_0) {
			break;
		}
	}

	CloseHandle(handler_file);
	CloseHandle(event_input);
	CloseHandle(event_output);
	CloseHandle(event_close);
}