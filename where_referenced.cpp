#include"Headers.h"
#include<iostream>
#include<stdio.h>
#include<tc/tc_startup.h>
#include<tcinit/tcinit.h>
#include<tccore/item.h>
#include<tccore/workspaceobject.h>
#include<tccore/aom_prop.h>
#include<tccore/aom.h>

using namespace std;

int iWhere_referenced()
{
	int* iLevels = nullptr;
	int iRef = 0, n=0;
	tag_t tItem1=NULLTAG, tRevision = NULLTAG;
	tag_t* tN_ref = nullptr;
	char** cRelations = NULL;
	char* cName=NULL;


	char* cItem_id= ITK_ask_cli_argument("-i=");
	char* cRevision_id = ITK_ask_cli_argument("-r=");

	iCheckError(ITEM_find_rev(cItem_id, cRevision_id, &tRevision));
	
	iCheckError(WSOM_where_referenced2(tRevision, -1, &iRef, &iLevels, &tN_ref, &cRelations));


	for (int i = 0; i < iRef; i++) 
	{
		iCheckError(AOM_ask_name(tN_ref[i], &cName));

		cout << "Reference " << i + 1 << ": " << cName << endl;
	}

	return 0;
}