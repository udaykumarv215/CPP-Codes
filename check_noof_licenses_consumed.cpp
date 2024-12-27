#include"Headers.h"
#include<iostream>
#include<tccore/license.h>
#include<sa/user.h>

using namespace std;

int iCheckConsumedLicense()
{
	tag_t tLicense = NULLTAG, *tUsers=NULLTAG;
	int iCount = 0;
	char* cUserName = NULL;

	const char* cLicenseId = ITK_ask_cli_argument("Id=");
	ADA_find_license(cLicenseId, &tLicense);
	ADA_list_license_users(tLicense, &iCount, &tUsers);
	for (int i = 0; i < iCount; i++)
	{
		SA_ask_os_user_name2(tUsers[i], &cUserName);
		cout << cUserName << endl;
		MEM_free(cUserName);
	}
	return 0;
}