#include"Headers.h"
#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<tc/tc_startup.h>
#include<tcinit/tcinit.h>
#include<tc/emh.h>
#include<tccore/item.h>
#include<tccore/aom.h>
#include<fclasses/tc_date.h>
#include<tccore/aom_prop.h>
#include<tccore/grm.h>

using namespace std;

int iSet_value_property()
{

	int iCount=0;

	tag_t tItem=NULLTAG, tRelation = NULLTAG, tMasterform = NULLTAG;
	tag_t* tSecondary_objects=nullptr;

	char* cItem_id = ITK_ask_cli_argument("-i=");

	iCheckError(ITEM_find_item(cItem_id, &tItem));

	iCheckError(GRM_find_relation_type("IMAN_master_form", &tRelation));
	iCheckError(GRM_list_secondary_objects_only(tItem, tRelation, &iCount, &tSecondary_objects));

	iCheckError(AOM_refresh(tSecondary_objects[0], true));

	iCheckError(AOM_set_value_string(tSecondary_objects[0], "object_desc", "little"));
	iCheckError(AOM_save_without_extensions(tSecondary_objects[0]));
	iCheckError(AOM_refresh(tSecondary_objects[0], false));

	MEM_free(tSecondary_objects);
	return 0;
}