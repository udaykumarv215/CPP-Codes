#pragma once
#include<iostream>
#include <stdio.h>
#include <stdlib.h>
#include <tc/emh.h>

using namespace std;

int iCheckError(int iStatus)
{
	char* cMessage = NULLTAG;
	if (iStatus != ITK_ok)
	{
		EMH_ask_error_text(iStatus, &cMessage);
		if (cMessage)
		{
			cout << "Error: " << cMessage << endl;
			return ITK_ok;
		}
		else
		{
			cout << "Unknown error occurred with status code: " << iStatus << endl;
		}
	}
	return ITK_ok;
}