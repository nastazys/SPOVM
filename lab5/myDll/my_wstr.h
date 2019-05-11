#pragma once
#include <Windows.h>
#include <string>

class uString
{
private:
	std::string data;
public:
	uString(const char ar[]);
	uString(std::string &str);
	std::wstring getWstring();
	LPCWSTR getLpcwstr();
};