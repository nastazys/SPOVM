#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <iostream>
#include <thread>

using namespace std;

typedef void(*DLL_LIB_FUNC)();
HMODULE dllLib;

void read();
void write();

int main(int args, char* argv[]) {

	//loading library
	dllLib = LoadLibrary("c:/Users/nasta/source/repos/spovm/lab5/myDll/Debug/myDll.dll");

	if (dllLib == NULL) {
		cout << "Cannot load dll library!" << endl;
		_getch();
		exit(0);
	}
	else {
		cout << "Dll library has been loaded." << endl;
	}

	/*HANDLE reader = CreateThread(
		NULL,					// дескриптор защиты
		NULL,                   // начальный размер стека
		(LPTHREAD_START_ROUTINE)read,		// функция потока
		NULL,                   // параметр потока
		NULL,                   //  опции создания
		NULL                    // идентификатор потока
	);
	if (!reader)
	{
		cout << "Can't create thread: Error " << GetLastError() << endl;
		system("pause");
		return -1;
	}

	HANDLE writer = CreateThread(
		NULL,					// дескриптор защиты
		NULL,                   // начальный размер стека
		(LPTHREAD_START_ROUTINE)write,		// функция потока
		NULL,                   // параметр потока
		NULL,                   //  опции создания
		NULL                    // идентификатор потока
	);
	if (!writer)
	{
		cout << "Can't create thread: Error " << GetLastError() << endl;
		system("pause");
		return -1;
	}*/

	thread reader(read);
	thread writer(write);

	reader.join();
	writer.join();

	cout << "Finished!" << endl;

	FreeLibrary(dllLib);
	_getch();
	return 0;
}

void read()
{
	DLL_LIB_FUNC dll_read = (DLL_LIB_FUNC)GetProcAddress(dllLib, "read");
	dll_read();
	ExitThread(0);
}

void write()
{
	DLL_LIB_FUNC dll_write = (DLL_LIB_FUNC)GetProcAddress(dllLib, "write");
	dll_write();
	ExitThread(0);
}