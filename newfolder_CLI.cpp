#include"Headers.h"
#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<tc/tc_startup.h>
#include<tc/folder.h>
#include<tcinit/tcinit.h>
#include<tccore/aom.h>
#include<tccore/item.h>
#include<sa/user.h>

using namespace std;

int iCreate_folder()
{
	tag_t tNew_folder=NULLTAG,tUser1 = NULLTAG, tHome_folder = NULLTAG;

	char* cName = ITK_ask_cli_argument("-n=");
	char* cDesc = ITK_ask_cli_argument("-d=");
	char* cUser = ITK_ask_cli_argument("-user=");

	iCheckError( FL_create2(cName, cDesc, &tNew_folder));

	iCheckError( SA_find_user2(cUser, &tUser1));

	iCheckError(SA_ask_user_home_folder(tUser1, &tHome_folder));
	iCheckError(FL_insert(tHome_folder, tNew_folder, 999)); 

	iCheckError(AOM_save_without_extensions(tNew_folder));
	iCheckError(AOM_save_without_extensions(tHome_folder));

	return 0;
}