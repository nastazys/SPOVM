#include <iostream>
#include <Windows.h>
#include <process.h>

using namespace std;

void main()
{
	int k = 0;
	cout << "		                              _______________________" << endl <<
		"   _______________________-------------------                       \ "<< endl <<
		" /:--__                                                              |" << endl <<
		"||< > |                                   ___________________________/" << endl <<
		"| \__/_________________-------------------                         |" << endl <<
		"|                                                                  |" << endl <<
		"|                       THE LORD OF THE RINGS                      |" << endl <<
		"|                                                                  |" << endl <<
		"|      Three Rings for the Elven - kings under the sky,           | " << endl <<
		" |        Seven for the Dwarf-lords in their halls of stone,        |" << endl <<
		" |      Nine for Mortal Men doomed to die,                          |" << endl <<
		" |        One for the Dark Lord on his dark throne                  |" << endl <<
		" |      In the Land of Mordor where the Shadows lie.                 |" << endl <<
		"  |       One Ring to rule them all, One Ring to find them,          |" << endl <<
		"  |       One Ring to bring them all and in the darkness bind them   |" << endl <<
		"  |     In the Land of Mordor where the Shadows lie.                |" << endl <<
		" |                                              ____________________|_" << endl <<
		" |  ___________________-------------------------                       |" << endl <<
		" |/ --_                                                                 |" << endl <<
		" ||[ ]||                                            ___________________/" << endl <<
		"   \===/___________________--------------------------" << endl << endl << endl;
	while (k < 0)
	{
		SYSTEMTIME lt;
		//cout << "								";
		GetLocalTime(&lt);
		printf("child %u - %02d:%02d:%02d\r", _getpid(), lt.wHour, lt.wMinute, lt.wSecond);
		
	}
	system("pause");
}