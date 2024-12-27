#include"Headers.h"
#include<iostream>
#include<stdio.h>
#include<tc/tc_startup.h>
#include<tcinit/tcinit.h>
#include<tccore/item.h>
#include<tccore/aom.h>
#include<tccore/aom_prop.h>
#include<tccore/grm.h>

using namespace std;

int iUpdate_named_reference()
{
	tag_t tRevision=NULLTAG, tRelation_type=NULLTAG;
	tag_t* tSecondary_objects=nullptr;
	int iCount=0;

	char* cItem_id = ITK_ask_cli_argument("-i=");
	char* cItem_revision_id = ITK_ask_cli_argument("-ir=");

	iCheckError(ITEM_find_rev(cItem_id, cItem_revision_id, &tRevision));

	iCheckError(GRM_find_relation_type("IMAN_requirement", &tRelation_type));
	iCheckError(GRM_list_secondary_objects_only(tRevision, tRelation_type, &iCount, &tSecondary_objects));

	iCheckError(AOM_refresh(tSecondary_objects[0], TRUE));
	iCheckError(AOM_set_value_string(tSecondary_objects[0], "object_name", "Testing"));

	iCheckError(AOM_save(tSecondary_objects[0]));
	iCheckError(AOM_refresh(tSecondary_objects[0], FALSE));

	return 0;
}