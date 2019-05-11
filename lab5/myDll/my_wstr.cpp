#include "stdafx.h"
#include "my_wstr.h"

uString::uString(std::string &str)
{
	data = str;
}

uString::uString(const char ar[])
{
	std::string buf = ar;
	data = buf;
}

std::wstring uString::getWstring()
{
	int len;
	int slength = (int)data.length() + 1;
	len = MultiByteToWideChar(CP_ACP, 0, data.c_str(), slength, 0, 0);
	wchar_t* buf = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, data.c_str(), slength, buf, len);
	std::wstring r(buf);
	delete[] buf;
	return r;
}

LPCWSTR uString::getLpcwstr()
{
	return this->getWstring().c_str();
}