#include"Headers.h"
#include<iostream>
#include<tccore/item.h>
#include<sa/sa.h>
#include<tccore/aom.h>

using namespace std;

int iSetUserStatus()
{
	tag_t user = NULL_TAG;
	int iStatus = 0;

	const char* USERNAME = ITK_ask_cli_argument("-u=");
	const char* PASSWORD = ITK_ask_cli_argument("-p=");
	const char* GROUP = ITK_ask_cli_argument("-g=");

	int loginStatus = ITK_init_module(USERNAME, PASSWORD, GROUP);
	if (loginStatus == ITK_ok)
	{
		iCheckError(SA_find_user2("izn2", &user));
		AOM_refresh(user, true);
		iCheckError(SA_set_user_status(user, 1));
		AOM_save_without_extensions(user);
		AOM_refresh(user, false);
		cout << "User status udpated successfully";
	}
	return 0;
}