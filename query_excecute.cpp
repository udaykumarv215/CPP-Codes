#include"Headers.h"
#include<iostream>
#include<qry/qry.h>
#include<tccore/aom_prop.h>

using namespace std;

int iQueryExcecute()
{
	tag_t tQuery = NULLTAG;
	int iN_entries = 0, iN_found=0;
	const char* cQueryName = ITK_ask_cli_argument("-q=");
	char** cEntries = NULL;
	char** cValues=NULL;
	tag_t* tResults = NULL;
	iCheckError(QRY_find2(cQueryName, &tQuery));

	iCheckError(QRY_find_user_entries(tQuery, &iN_entries, &cEntries, &cValues));

	iCheckError(QRY_execute(tQuery, iN_entries, cEntries, cValues, &iN_found, &tResults));

	for (int i = 0; i < iN_found; i++)
	{
		char* cValue;
		iCheckError(AOM_ask_value_string(tResults[i], "object_name", &cValue));
		cout << cValue << endl;
	}

	return 0;
}