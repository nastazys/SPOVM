#include <iostream>
#include <Windows.h>
#include <process.h>

using namespace std;

void main()
{
	int k = 0;
	while (k < 500)
	{
		SYSTEMTIME lt;
		//cout << "								";
		GetSystemTime(&lt);
		printf("child %u - %02d:%02d:%02d\r", _getpid(), lt.wHour, lt.wMinute, lt.wSecond);
		//Sleep(1000);
		//system("cls");
	}
	
	system("pause");
}