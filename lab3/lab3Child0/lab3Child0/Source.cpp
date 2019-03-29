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
string getWeatherInfo(string city)
{
	curl_global_init(CURL_GLOBAL_ALL);

	CURL* easyhandle = curl_easy_init();
	std::string readBuffer;
	string url = "https://api.openweathermap.org/data/2.5/weather?q=";//Minsk&appid=e3e722a4ac4b08a4a9423f30a817a398";
	url.append(city);
	url.append("&appid=e3e722a4ac4b08a4a9423f30a817a398");
	//cout << url;
	curl_easy_setopt(easyhandle, CURLOPT_URL, url.c_str());
	curl_easy_setopt(easyhandle, CURLOPT_WRITEFUNCTION, WriteCallback);
	curl_easy_setopt(easyhandle, CURLOPT_WRITEDATA, &readBuffer);

	curl_easy_perform(easyhandle);
	//cout << readBuffer;
	return readBuffer;
}
string getSubstr(string str)
{
	int i = 0;
	for (i; ; i++)
	{
		if (str[i] == ',')
			if (str[i - 1] == (char)"\"")
				return str.substr(1, i - 1);
			else return str.substr(0, i);
	}
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
	HANDLE hWeathPipe = CreateFile(TEXT("\\\\.\\pipe\\WeatherSynchro"),
		GENERIC_READ | GENERIC_WRITE,
		0, NULL, OPEN_EXISTING, 0, NULL);
	DWORD  cbRead;
	char city[100] = { '\0' };
	char parameter[100] = { '\0' };
	string weatherInfo;
	while (1)
	{
		ReadFile(hWeathPipe, city, 100, &cbRead, NULL);
		weatherInfo = getWeatherInfo(string(city));
		for (int i = 0; i < 4; i++)
		{
			ReadFile(hWeathPipe, parameter, 100, &cbRead, NULL);
			string info = scanWeather(weatherInfo, cbRead, parameter);
			DWORD  cbWrite;
			WriteFile(hWeathPipe, info.c_str(), info.length(), &cbWrite, NULL);
		}
	}
	CloseHandle(hWeathPipe);
	system("pause");
	return 0;
}