#include"Headers.h"
#include<iostream>
#include<stdio.h>
#include<tc/tc_startup.h>
#include<tcinit/tcinit.h>
#include<tccore/tctype.h>
#include<tccore/aom.h>
#include<ps/ps.h>
#include<bom/bom.h>
#include<tccore/aom_prop.h>

using namespace std;
tag_t  tParent = NULLTAG;

int iFetch_child()
{
	tag_t tWindow = NULLTAG, tTop_line = NULLTAG, tBom_window = NULLTAG, tRevision = NULLTAG;
	tag_t* tBv_list=NULL;
	int r = 0;;
	char* cItem_id = ITK_ask_cli_argument("-i=");

	iCheckError(ITEM_find_item(cItem_id, &tParent));

	iCheckError(ITEM_find_rev(cItem_id, "A", &tRevision));

	iCheckError(ITEM_list_bom_views(tParent, &r, &tBv_list));

	for (int i = 0; i < r; i++)
	{
		iCheckError(BOM_create_window(&tWindow));

		iCheckError(BOM_set_window_top_line(tWindow, tParent, tRevision, tBv_list[i], &tTop_line));

		iReturn_children1(tTop_line);

	}
	return 0;
}

int iReturn_children1(tag_t tLine)
{
	int iCount = 0;
	tag_t x = NULLTAG;
	tag_t* tChildren = nullptr;
	int i = 0;
	char* cId =nullptr;
	char* cParent_id=nullptr;

	iCheckError(BOM_line_ask_all_child_lines(tLine, &iCount, &tChildren));

	if (iCount != 0)
	{
		tParent = tLine;
		iCheckError(AOM_ask_value_string(tParent, "bl_item_item_id", &cParent_id));

		for (i = 0; i < iCount; i++)
		{ 
			iCheckError(AOM_ask_value_string(tChildren[i], "bl_item_item_id", &cId));

			cout << "children for " << cParent_id << " are " << cId << endl;

			iReturn_children1(tChildren[i]);
		}
	}

	else
	{
		tParent = tLine;
		iCheckError(AOM_ask_value_string(tParent, "bl_item_item_id", &cParent_id));

		cout << "No Children found for "  << cParent_id << endl;
	}

	return 0;
}


