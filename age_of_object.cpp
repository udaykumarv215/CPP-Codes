#include"Headers.h"
#include <tc/tc_startup.h>
#include <tcinit/tcinit.h>
#include <tccore/item.h>
#include <tc/emh.h>
#include <tccore/aom_prop.h>
#include <fclasses/tc_string.h>
#include <tc\emh.h>
#include <stdlib.h>
#include <string>
#include <string.h>
#include<iostream>
#include <chrono>
#include <ctime>
using namespace std;

struct sAge {
	int iYears;
	int iMonths;
	int iDays;
};

sAge sCalculate_age(const date_t& dCreation_date) {
	// Get current date
	time_t tNow = time(nullptr);
	tm* current_tm = localtime(&tNow);

	// Calculate age
	int iYears = current_tm->tm_year + 1900 - dCreation_date.year;
	int iMonths = current_tm->tm_mon - dCreation_date.month;
	int iDays = current_tm->tm_mday - dCreation_date.day;

	if (iMonths < 0 || (iMonths == 0 && iDays < 0)) {
		iYears--;
		iMonths += 12;
		if (iDays < 0) {
			iMonths--;
			iDays += 30;
		}
	}

	return { iYears, iMonths, iDays };
}

int iCurrentDateTime()
{
	int iFail = 0;
	tag_t tItem = NULLTAG;
	tag_t tRevision = NULLTAG;
	date_t dCreation_date =NULLDATE;

	const char* cItem_id = ITK_ask_cli_argument("-i=");

	iCheckError(ITEM_find_item(cItem_id, &tItem));

	iCheckError(ITEM_ask_latest_rev(tItem, &tRevision));

	iCheckError(AOM_ask_value_date(tRevision, "creation_date", &dCreation_date));

	sAge age = sCalculate_age(dCreation_date);

	cout << "Age of the item: " << age.iYears << " years, " << age.iMonths << " months, " << age.iDays << " days" << std::endl;

	return 0;
}
