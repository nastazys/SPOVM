#define _CRT_SECURE_NO_WARNINGS

#define CURL_STATICLIB
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iterator>
#include <Windows.h>
#include <curl/curl.h>

using namespace std;
size_t WriteCallback(char *contents, size_t size, size_t nmemb, void *userp)
{
	((std::string*)userp)->append((char*)contents, size * nmemb);
	return size * nmemb;
}
string scanWeather(string weather, int n, string parameter);
string getWeatherInfo(string city)
{
	curl_global_init(CURL_GLOBAL_ALL);

	CURL* easyhandle = curl_easy_init();
	std::string readBuffer;
	string url = "https://api.openweathermap.org/data/2.5/weather?q=";
	url.append(city);
	url.append("&appid=e3e722a4ac4b08a4a9423f30a817a398");
	curl_easy_setopt(easyhandle, CURLOPT_URL, url.c_str());
	curl_easy_setopt(easyhandle, CURLOPT_WRITEFUNCTION, WriteCallback);
	curl_easy_setopt(easyhandle, CURLOPT_WRITEDATA, &readBuffer);
	CURLcode ret = curl_easy_perform(easyhandle);
	if (ret != CURLE_OK)
		throw 2;
	//cout << readBuffer;
	if (scanWeather(readBuffer, 3, "cod") == "\"404\"" || scanWeather(readBuffer, 3, "cod") == "\"400\"")
		throw 3;
	return readBuffer;
}
string getSubstr(string str)
{
	int i = 0;
	for (i; i< str.size(); i++)
	{
		if (str[i] == ',')
			if (str[i - 1] == (char)"\"")
				return str.substr(1, i - 1);
			else return str.substr(0, i);
	}
	throw 1;
}
string scanWeather(string weather, int n, string parameter)
{
	int i = 0;
	for (i = weather.find(parameter.substr(0, n - 1), i++); i != string::npos; i =weather.find(parameter.substr(0, n - 1), i + 1))
	{
		return getSubstr(weather.substr(i + n + 2, 100));
	}
	return 0;
}

using namespace std;
int main()
{
/*HANDLE CreateFile(
  LPCTSTR lpFileName,   
  DWORD  dwDesiredAccess, 
  DWORD  dwShareMode,
  LPSECURITY_ATTRIBUTES lpSecurityAttributes,  
  DWORD  dwCreationDistribution, 
  DWORD  dwFlagsAndAttributes,  
  HANDLE hTemplateFile); */

	HANDLE hWeathPipe = CreateFile(TEXT("\\\\.\\pipe\\WeatherSynchro"),
		GENERIC_READ | GENERIC_WRITE,
		0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	DWORD  cbRead;
	DWORD  cbWrite;
	string weatherInfo;
	while (1)
	{
		char city[100] = { '\0' };
		char parameter[100] = { '\0' };
		try {
			ReadFile(hWeathPipe, city, 100, &cbRead, NULL);
			weatherInfo = getWeatherInfo(string(city));
			WriteFile(hWeathPipe, "noterror", 5, &cbWrite, NULL);
			for (int i = 0; i < 4; i++)
			{
				ReadFile(hWeathPipe, parameter, 100, &cbRead, NULL);
				string info = scanWeather(weatherInfo, cbRead, parameter);
				WriteFile(hWeathPipe, info.c_str(), info.length(), &cbWrite, NULL);
			}
		}
		catch (int e)
		{
			switch (e) {
			case 1: {
				cout << "Error with parsing JSON!";
				WriteFile(hWeathPipe, "error1", 5, &cbWrite, NULL);
				Sleep(3000);
				continue;
				break;
			}
			case 2: {
				cout << "Can't get answer from OpenWeather!";
				WriteFile(hWeathPipe, "error2", 5, &cbWrite, NULL);
				Sleep(3000);
				continue;
				break;
			}
			case 3: {
				cout << "Entered city not founded!";
				WriteFile(hWeathPipe, "error3", 5, &cbWrite, NULL);
				Sleep(3000);
				continue;
				break;
			}
			}
		}
	}
	DisconnectNamedPipe(hWeathPipe);
	CloseHandle(hWeathPipe);
	//system("pause");
	return 0;
}