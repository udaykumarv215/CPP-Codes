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

int iCreate_item_cli()
{
	tag_t iItem=NULLTAG, tRev=NULLTAG;
	
	char* cName = ITK_ask_cli_argument("-n=");
	char* cItem_revision = ITK_ask_cli_argument("-it=");
	char* cRt = ITK_ask_cli_argument("-rt=");

	iCheckError(ITEM_create_item("", cName, cItem_revision, cRt, &iItem, &tRev));

	iCheckError(AOM_save_without_extensions(iItem));

	return 0;
}