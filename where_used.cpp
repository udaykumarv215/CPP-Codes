#include"Headers.h"
#include<iostream>
#include<stdio.h>
#include<tc/tc_startup.h>
#include<tcinit/tcinit.h>
#include<tccore/item.h>
#include<tccore/aom_prop.h>
#include<tccore/aom.h>
#include<ps/ps.h>

using namespace std;

int iWhere_used()
{
	int* iLevels = NULL;
	int ref =0, n = 0;
	tag_t tItem1=NULLTAG, tRevision=NULLTAG;
	tag_t* tN_ref = NULL;
	char** cRelations=NULL;
	char* cName=NULL;

	char* cItem_id = ITK_ask_cli_argument("-i=");
	char* cRevision_id = ITK_ask_cli_argument("-r=");

	iCheckError(ITEM_find_rev(cItem_id, cRevision_id, &tRevision));
	iCheckError(PS_where_used_all(tRevision, -1, &ref, &iLevels, &tN_ref));	
	for (int i=0; i< ref; i++)
	{
		iCheckError(AOM_ask_name(tN_ref[i], &cName));
		cout << "used " << iLevels << ": " << cName << endl;	
	}	
	return 0;
}
