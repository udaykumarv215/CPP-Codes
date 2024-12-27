#include <tcinit/tcinit.h>
#include <tccore/item.h>
#include <tccore/aom_prop.h>
#include <tccore/aom.h>
#include <iostream>

using namespace std;

int iFreeze()
{

	int iFail = 0;
	char *cDate, *cValue;
	tag_t tItem, tDate, tValue;
	date_t dDate;


	const char *username = ITK_ask_cli_argument("-u=");
	const char *password = ITK_ask_cli_argument("-p=");
	const char *group = ITK_ask_cli_argument("-g=");

	iFail = TC_init_module(username, password, group);

	ITEM_find_item("000619", &tItem);
	AOM_ask_value_tag(tItem, "last_mod_date", &tValue);

	POM_freeze(tItem);

	AOM_refresh(tItem, 1);

	AOM_set_value_string(tItem, "object_desc", "Phone");

	AOM_save_without_extensions(tItem);

	AOM_refresh(tItem, 0);

	AOM_ask_value_string(tItem, "object_desc", &cValue);

	cout << cValue << endl;

	AOM_ask_modified_date(tItem, &dDate);

	ITK_date_to_string(dDate, &cDate);

	cout << cDate << endl;


	return 0;

}