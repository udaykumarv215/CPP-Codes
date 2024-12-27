#include"Headers.h"
#include<iostream>
#include<stdio.h>
#include<fstream>
#include<fclasses/tc_date.h>
#include<string>
#include<tc/tc_startup.h>
#include<tcinit/tcinit.h>
#include<tccore/tctype.h>
#include<tccore/aom_prop.h>
#include<tccore/aom.h>

using namespace std;

int date_property()
{
	int iFail, mon, day, year;
	tag_t object_type, instance;
	iFail = TCTYPE_ask_type("ITK Item", &object_type);
	checkError(iFail);

	iFail = TCTYPE_create_object(object_type, &instance);
	checkError(iFail);

	iFail = AOM_set_value_string(instance, "object_name", "ITK-item");
	checkError(iFail);

	cout << "please provide the input in following format dd-mmm-yyyy" << endl;
	char*date1 = ITK_ask_cli_argument("-date=");
	char*date2 = date1 + 30;

	iFail = DATE_string_to_date(date1, "dd-mmm-yyyy", &mon, &day, &year, NULL, NULL, NULL);
	checkError(iFail);


	iFail = AOM_set_value_string(instance, "a2date_1", date1);
	checkError(iFail);

}

int mon_to_num(char month_name)
{
	int numbers;
}