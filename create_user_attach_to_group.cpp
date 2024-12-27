#include"Headers.h"
#include<iostream>
#include<sa/user.h>
#include<sa/group.h>
#include<tccore/aom.h>

using namespace std;

int create_user_attach_to_group()
{
	tag_t tNewUser = NULLTAG, tGroup =NULLTAG;
	iCheckError(SA_create_user2("udayizn2", "uday1", "udayizn2", &tNewUser));
	AOM_save_without_extensions(tNewUser);
	iCheckError(SA_find_group("dba", &tGroup));
	iCheckError(SA_set_user_login_group(tNewUser, tGroup));
	AOM_save_without_extensions(tGroup);
	iCheckError(SA_set_os_user_name2(tNewUser, "udayizn2"));
	AOM_save_without_extensions(tNewUser);
	return 0;
}