#include"Headers.h"
#include<iostream>
#include<lov/lov.h>
#include<tccore/aom_prop.h>
#include<tccore/item.h>
#include<tccore/aom.h>

using namespace std;

int iUpdate_property_with_lov_value()
{
	tag_t tItem = NULLTAG, tLov=NULLTAG, tItemRevision=NULLTAG;
	LOV_usage_t lUsage;
	int iValues;
	char** cValues;
	logical* lIsnull;
	logical* lIsempty;
	const char* cItem_id = ITK_ask_cli_argument("-i=");

	iCheckError(ITEM_find_item(cItem_id, &tItem));

	iCheckError(ITEM_ask_latest_rev(tItem, &tItemRevision));

	iCheckError(AOM_ask_lov(tItemRevision, "a2ITK_lov", &tLov));

	iCheckError(LOV_ask_disp_values(tLov,&lUsage,&iValues,&lIsnull,&lIsempty,&cValues));

	iCheckError(AOM_refresh(tItemRevision, true));

	iCheckError(AOM_set_value_string(tItemRevision, "a2ITK_lov", cValues[0]));

	iCheckError(AOM_refresh(tItemRevision,false ));

	iCheckError(AOM_save_without_extensions(tItemRevision));

	return 0;
}