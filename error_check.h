#pragma once
#include<iostream>
#include <stdio.h>
#include <stdlib.h>
#include <tc/emh.h>

using namespace std;

void checkStatus(int status)
{
	char *message = NULLTAG;
	if (status != ITK_ok)
	{
		EMH_ask_error_text(status, &message);
		if (message)
		{
			cout << "Error: " << message << endl;
		}
		else
		{
			cout << "Unknown error occurred with status code: " << status << endl;
		}
	}
}