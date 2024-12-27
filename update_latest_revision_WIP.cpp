#include"Headers.h"
#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>
#include <chrono>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <tc/tc_startup.h>
#include <tccore/aom_prop.h>
#include <tccore/aom.h>
#include <tcinit/tcinit.h>
#include <tccore/item.h>
#include <tccore/grm.h>
#include <tc/emh.h>
#include<epm/epm.h>
#include<f
es/tc_date.h>

using namespace std;
vector<tag_t> vRevs;

int iUpdate_latest_revision()
{
	int iLatest_rev=0;
	char* cItem = ITK_ask_cli_argument("-i=");
	if (cItem != nullptr)
	{
		// Convert cItem to tag_t (assuming there's a function for this conversion)
		tag_t tItem_tag=NULLTAG;
		iCheckError(ITEM_find_item(cItem, &tItem_tag));
	}
	char* CId=nullptr;
	iCheckError(AOM_ask_value_string(iLatest_rev, "item_revision_id", &CId));
	cout << CId << endl;
	return 0;
}

int	iFind_latest_revision(tag_t tItem_tag)
{
	tag_t* tRev_tags=nullptr;
	time_t tRelease_date = NULLTAG;
	date_t dDate=NULLDATE;
	time_t tLatest_date = NULL;
	int n=0, iLatest_revision=0;
	int m = 0;
	char* cDate_str=nullptr;
	iCheckError(ITEM_list_all_revs(tItem_tag, &n, &tRev_tags));

	for (int i = 0; i < n; i++)
	{
		iCheckError(EPM_ask_if_released(tRev_tags[i], &m));

		if (m !=0)
		{
			vRevs.push_back(tRev_tags[i]);
		}
	}

	int x = vRevs.size();

	for (int i = 0; i < x; i++)
	{
		iCheckError(AOM_ask_value_date(vRevs[i], "date_released", &dDate));
		iCheckError(DATE_date_to_string(dDate, "%Y-%m-%d", &cDate_str));

		tRelease_date = convert_date_to_time_t(cDate_str);

		if (tLatest_date < tRelease_date)
		{
			tLatest_date = tRelease_date;
			iLatest_revision = vRevs[i];
		}
	}
	
	return iLatest_revision;
}

time_t convert_date_to_time_t(string DateString)
{
	struct tm tm = {};
	istringstream ss(DateString);
	ss >> get_time(&tm, "%Y-%m-%d");  // Adjust format according to date_str format
	return mktime(&tm);
}