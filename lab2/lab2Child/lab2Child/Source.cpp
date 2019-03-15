#include <iostream>
#include <Windows.h>
#include <process.h>

#include <vector>
#include <fstream>
#include <string>
#include <iomanip>
#include <iterator>
#include <string>
using namespace std;

bool remove_line(const char *filename, size_t index)
{
	std::vector<std::string> vec;
	std::ifstream file(filename);
	if (file.is_open())
	{
		std::string str;
		while (std::getline(file, str))
			vec.push_back(str);
		file.close();
		if (vec.size() < index)
			return false;
		vec.erase(vec.begin() + index);
		std::ofstream outfile(filename);
		if (outfile.is_open())
		{
			std::copy(vec.begin(), vec.end(),
				std::ostream_iterator<std::string>(outfile, "\n"));
			outfile.close();
			return true;
		}
		return false;
	}
	return false;
}

string scanTxt(fstream& stream)
{
	string line;
	HANDLE readingFromFileEnable = OpenEvent(EVENT_ALL_ACCESS, TRUE, "readingFromFileEnable");
	if (!readingFromFileEnable)
	{
		throw 2;
		return NULL;
	}
	WaitForSingleObject(readingFromFileEnable, INFINITE);
	ResetEvent(readingFromFileEnable);
	getline(stream, line);
	remove_line("c:/Users/nasta/source/repos/spovm/lab2/strings.txt", 0);
	SetEvent(readingFromFileEnable);
	return line;
}

using namespace std;

void main()
{
	try {
		int k = 0;
		string str;
		std::fstream txt;
		txt.open("c:/Users/nasta/source/repos/spovm/lab2/strings.txt");
		if (!txt) throw std::runtime_error("Can't open file!");
		str = scanTxt(txt);
		HANDLE writingToConsoleEnable = OpenEvent(EVENT_ALL_ACCESS, TRUE, "writingToConsoleEnable");
		if (!writingToConsoleEnable)
		{
			throw 1;
		}
		DWORD dwWaitResult = 0;
		while (k < 500)
		{
			WaitForSingleObject(writingToConsoleEnable, INFINITE);
			if (!ResetEvent(writingToConsoleEnable))
				return;
			cout << str << endl;
			if (!SetEvent(writingToConsoleEnable))
				return;
			Sleep(2500);
		}
	}
	catch (int i)
	{
		switch (i)
		{
		case 1: {
			cout << "Error with writingToConsole Event!" << endl;
			system("pause");
			return;
		}
		case 2: {
			cout << "Error with readingFromFile Event!" << endl;
			system("pause");
			return;
		}
		default: {
			cout << "Error!" << endl;
			system("pause");
			return;
		}
		}
	}
	system("pause");
}