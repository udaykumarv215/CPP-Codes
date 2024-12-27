#include"Headers.h"
#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<tc/tc_startup.h>
#include<tcinit/tcinit.h>
#include<tc/emh.h>
#include<tccore/item.h>
#include<tc/folder.h>
#include<sa/user.h>
#include<tccore/aom.h>
#include<tccore/grm.h>
#include<ae/ae.h>

using namespace std;

int iItem_create()
{
	tag_t tItem=NULLTAG, tRev = NULLTAG, tUser = NULLTAG, tHome_folder = NULLTAG, tText = NULLTAG, tDataset = NULLTAG, tRelation_type = NULLTAG, tRelation = NULLTAG;

	iCheckError(ITEM_create_item("", "salaar-thesaga", "Item", "A", &tItem, &tRev));

	iCheckError(SA_find_user2("izn", &tUser));

	iCheckError(SA_ask_user_home_folder(tUser, &tHome_folder));

	iCheckError(FL_insert(tHome_folder, tItem, 999));

	iCheckError(AOM_save_without_extensions(tItem));
 
	iCheckError(AOM_save_without_extensions(tHome_folder));

	iCheckError(AE_find_datasettype2("Text", &tText));

	iCheckError(AE_create_dataset_with_id(tText, "ABC", "", "", "A", &tDataset));

	iCheckError(AE_save_myself(tDataset));

	iCheckError(GRM_find_relation_type("IMAN_requirement", &tRelation_type));

	iCheckError(GRM_create_relation(tRev, tDataset, tRelation_type, NULLTAG, &tRelation));

	iCheckError(GRM_save_relation(tRelation));

	return 0;
}

