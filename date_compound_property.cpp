#include"Headers.h"
#include<iostream>
#include <tc/tc_startup.h>
#include <tcinit/tcinit.h>
#include <tccore/item.h>
#include<tccore/aom.h>
#include <tccore/aom_prop.h>
#include <fclasses/tc_string.h>
#include <chrono>
#include <ctime>
#include<time.h>
#include <iomanip>
#include <sstream>

using namespace std;
date_t date1 = NULLDATE;
date_t date2 = NULLDATE;
char *message = NULL;

// Function to add days to a date

date_t dCalc30Days(date_t date1) 
{
	char* cvalue = NULL;
	ITK_date_to_string(date1, &cvalue);
	string dateStr = cvalue;

	tm time = {};  // tm used to store date and time components.
	istringstream ss(dateStr);
	ss >> get_time(&time, "%d-%b-%Y %H:%M"); // parses the dateStr in to specified format.


	// Add 30 days to the parsed date
	time.tm_mday += 30;
	mktime(&time); // mktime-maketime, handles the overflow of the dates (leap year, feb 28 days, change of year etc).

	// Format the resulting date into the desired format
	char buffer[80]; // Buffer to store the formatted date
	strftime(buffer, 80, "%d-%b-%Y %H:%M", &time); //parses the time in to specified format.

	// Store the formatted date in another string variable
	string thirtyDaysLater = buffer;
	cout << "30days later date and time: " << buffer << endl;
	date_t date2 = NULLDATE;
	ITK_string_to_date(buffer, &date2);

	return date2;
}

int iDate_property() 
{
	int iFail;
	int status;
	tag_t cItemTag = NULLTAG;

	char* cdate = ITK_ask_cli_argument("-date=");
	char* citem = ITK_ask_cli_argument("-i=");

		status = ITEM_find_item(citem, &cItemTag);
		if (status == ITK_ok) 
		{

			status = ITK_string_to_date(cdate, &date1);
			if (status == ITK_ok) 
			{
				AOM_refresh(cItemTag, true);
				status = AOM_set_value_date(cItemTag, "a2date_1", date1);
				AOM_save_without_extensions(cItemTag);
				AOM_refresh(cItemTag, false);
				iCheckError(status);

				if (status == ITK_ok) 
				{
					date2 = dCalc30Days(date1);
					AOM_refresh(cItemTag, true);
					status = AOM_set_value_date(cItemTag, "a2date_2", date2);
					AOM_save_without_extensions(cItemTag);
					AOM_refresh(cItemTag, false);
					iCheckError(status);
				}
				else
					cout << "Failed to update the a2date_2 property" << endl;
			}
			else 
				cout << "Failed to convert cdate string to date_t" << endl;
		}
		else
			cout << "Failed to find item with id " << citem << endl;

	return 0;
}