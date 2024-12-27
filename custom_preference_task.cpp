#define DESCRIPTION "object_desc"
#include"Headers.h"
#include<iostream>
#include<stdio.h>
#include<string>
#include<tc/tc_startup.h>
#include<tc/preferences.h>
#include<tccore/aom_prop.h>
#include<tccore/aom.h>
#include<tcinit/tcinit.h>
#include<fstream>
#include<tccore/tctype.h>
#include<tccore/item.h>

using namespace std;

int iSet_property_preference()
{
	char* cValues=NULL;
	tag_t tItem=NULLTAG;
	iCheckError(PREF_ask_char_value("ITK_custom_preference", 0, &cValues));

	char* cFile_name = cValues;
	string sLine;

	if (cFile_name != nullptr)
	{
		ifstream fin(cFile_name);

		if (fin.is_open())
		{
			cout << "Processing file: " << cFile_name << endl;

			while (getline(fin, sLine))
			{
				size_t sPos = sLine.find('-');
				if (sPos != string::npos)
				{
					string sItem_id = sLine.substr(0, sPos);
					string sObject_desc = sLine.substr(sPos + 1);

					const char* cItem_id1 = sItem_id.c_str();
					const char* cObject_desc1 = sObject_desc.c_str();

					iCheckError(ITEM_find_item(cItem_id1, &tItem));

					iCheckError(AOM_refresh(tItem, true));

					iCheckError(AOM_set_value_string(tItem, DESCRIPTION, cObject_desc1));

					iCheckError(AOM_save_with_extensions(tItem));

					iCheckError(AOM_refresh(tItem, false));
				}
			}
		}
	}

	return 0;
}
