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

void vImport_named_reference()
{
	char* cDataset = ITK_ask_cli_argument("-dn=");
	char* cPath = ITK_ask_cli_argument("-path=");
	tag_t tDataset_name = NULLTAG;

	iCheckError(AE_find_dataset2(cDataset, &tDataset_name));
	
	iCheckError(AOM_refresh(tDataset_name, TRUE));

	iCheckError(AE_import_named_ref(tDataset_name, "Text", cPath, "newDataset1", SS_TEXT));

	iCheckError(AOM_save(tDataset_name));

	iCheckError(AOM_refresh(tDataset_name, FALSE));
}