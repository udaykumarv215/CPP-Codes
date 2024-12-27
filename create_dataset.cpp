// create a dataset and attach it to latest revision of the entered item.

#include"Headers.h"
#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<tc/tc_startup.h>
#include<tcinit/tcinit.h>
#include<tc/folder.h>
#include<tccore/aom.h>
#include<tccore/grm.h>
#include<tccore/item.h>
#include<sa/user.h>
#include<ae/ae.h>

using namespace std;

int iCreate_dataset()
{
	tag_t tDataset, tNew_dataset, tRelation_type, tItem, tItem_rev, tRelation;
	char* cD_name = ITK_ask_cli_argument("-dn=");
	char* cD_desc = ITK_ask_cli_argument("-ds=");
	char* cItem_id = ITK_ask_cli_argument("-id=");

	iCheckError(ITEM_find_item(cItem_id, &tItem));
	iCheckError(ITEM_ask_latest_rev(tItem, &tItem_rev));

	iCheckError(AE_find_datasettype2("PDF", &tDataset));
	iCheckError(AE_create_dataset_with_id(tDataset, cD_name, cD_desc, "", "A", &tNew_dataset));

	AOM_save_without_extensions(tNew_dataset);

	iCheckError(GRM_find_relation_type("IMAN_reference", &tRelation_type));
	iCheckError(GRM_create_relation(tItem_rev, tNew_dataset, tRelation_type, NULLTAG, &tRelation));

	AOM_save_without_extensions(tRelation);

	return 0;
}