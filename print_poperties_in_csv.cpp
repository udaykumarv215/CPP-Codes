//Printing properties of item revision in a CSV

#include"Headers.h"
#include<iostream>
#include<tccore/aom_prop.h>
#include<tccore/item.h>
#include<fstream>
#include<sstream>

using namespace std;

int iPrint_csv()
{
	tag_t tItem=NULLTAG;
	int iCount=0;
	char** cProp_names= nullptr;
	const char* cItem_r = ITK_ask_cli_argument("-i=");
	const char* cFile = ITK_ask_cli_argument("-file=");

	ofstream oNew_file(cFile);
	if (!oNew_file.is_open())
	{
		cout << "cannot open file" << endl;
		return -1;
	}

	iCheckError(ITEM_find_item(cItem_r, &tItem));

	iCheckError(AOM_ask_prop_names(tItem, &iCount, &cProp_names));

	if (iCount != 0 && cProp_names != NULL)
	{
		for (int i = 0; i < iCount; i++)
		{
			int iNum_values = 0;
			char** cValues = nullptr;

			iCheckError(AOM_ask_displayable_values(tItem, cProp_names[i], &iNum_values, &cValues));

			if (iNum_values != 0 && cValues != NULL)
			{
				for (int j = 0; j < iNum_values; j++)
				{
					oNew_file << cProp_names[i] << " = " << cValues[j] << endl;
					cout << cProp_names[i] << " = " << cValues[j] << endl;
				}
			}
		}
	}

	oNew_file.close();
	return 0;
}