// ConsoleApplication1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "GameState.h"
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <string>
using namespace std;

float mathTest();

int main()
{
	

	/*cout << "This is a test. Press Enter to continue.";
	cin.get();*/

	srand(time(NULL));
	//startupAnim();
	GameState();
	//mathTest();

	//cout << "The test has been run.";
	cout << "Thank you for testing this game engine.\nPress ENTER to close the window.";
	cin.get();
	cin.get();

    return 0;
}

float mathTest()
{
	float total;
	float rate;
	float min;
	int loan;

	cout << "ENTER MIN MONTHLY PAYMENT AND LOAN NUMBER.\n";
	cin >> min;
	cin >> loan;

	switch (loan)
	{
	default:
		total = 100000;
		rate = 5.0;
		break;
	case 1:
		total = 74000;
		rate = 6.76;
		break;
	case 2:
		total = 32000;
		rate = 4.60;
		break;
	}

	for (int i = 0; i < 12; i++)
	{
		float interest = (total * (rate / 100)) / 12;
		cout << "MONTH " << i + 1 << " INTEREST: " << interest << "\n";
		total = total + interest - min;
		cout << "MONTH " << i + 1 << " REMAINDER: " << total << "\n";
	}

	return total;
}