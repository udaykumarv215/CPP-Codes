#include"Headers.h"
#include<iostream>
#include<stdio.h>
#include<tc/tc_startup.h>
#include<tccore/tctype.h>
#include<tcinit/tcinit.h>
#include<bom/bom.h>
#include<fstream>
#include<string>
#include<tccore/aom_prop.h>
#include<tccore/aom.h>

using namespace std;

int iSingleLevel_bom_from_file()
{
	string sLine;
	const char* cArray_item_id[100];
	int status = 0, i = 0;
	tag_t tItemTypeTag = NULLTAG, tCreateInputTag = NULLTAG, tItem = NULLTAG, tTop_line = NULLTAG;

	const char* cFile_path = ITK_ask_cli_argument("-file=");

	if (cFile_path != nullptr)
	{
		ifstream fin(cFile_path);
		if (fin.is_open())
		{
			cout << "opened file successfully" << endl;

			tag_t tBomWindow = NULLTAG;
			iCheckError(BOM_create_window(&tBomWindow));

			iCheckError(AOM_save_with_extensions(tBomWindow));

			for (int i = 0; getline(fin, sLine); i++)
			{
				// Expected format: item_id|item_name
				size_t pos = sLine.find('|');
				if (pos != string::npos) 
				{
					string item_id = sLine.substr(0, pos);
					string item_name = sLine.substr(pos + 1);

					const char* item_id1 = item_id.c_str();
					const char* item_name1 = item_name.c_str();					

					iCheckError(TCTYPE_ask_type("Item", &tItemTypeTag));//to get the object type

					iCheckError(TCTYPE_construct_create_input(tItemTypeTag, &tCreateInputTag));//to create input of object type

					iCheckError(AOM_set_value_string(tCreateInputTag, "item_id", item_id1));

					iCheckError(AOM_set_value_string(tCreateInputTag, "object_name", item_name1));

					iCheckError(TCTYPE_create_object(tCreateInputTag, &tItem));

					iCheckError(AOM_save_with_extensions(tItem));

					if (i == 0)
					{
						tag_t tParent=NULLTAG;

						ITEM_find_item(item_id1, &tParent);
						vCreate_BV(tParent);
						iCheckError(BOM_set_window_top_line(tBomWindow, tParent, NULLTAG, NULLTAG, &tTop_line));

						iCheckError(AOM_save_with_extensions(tTop_line));
					}

					else
					{
						tag_t tChild=NULLTAG,tChild_bom=NULLTAG;

						ITEM_find_item(item_id1, &tChild);
						iCheckError(BOM_line_add(tTop_line, tChild, NULLTAG, NULLTAG, &tChild_bom));

						iCheckError(AOM_save_with_extensions(tChild_bom));
					}

					iCheckError(BOM_save_window(tBomWindow));

					item_id1 = NULLTAG;
				}
			}
			fin.close();
			cout << "File processed successfully.\n";

			iCheckError(BOM_close_window(tBomWindow));
		}
	}
	return 0;
}

void vCreate_BV(const tag_t tParent)
{
	tag_t tBom=NULLTAG, tMain_rev=NULLTAG;

	iCheckError(ITEM_ask_latest_rev(tParent, &tMain_rev));

	iCheckError(PS_create_bom_view(NULLTAG, NULL, NULL, tParent, &tBom));

	iCheckError(AOM_save_with_extensions(tBom));
	iCheckError(AOM_save_with_extensions(tParent));

	tag_t tBom_view_revision=NULLTAG;
	iCheckError(PS_create_bvr(tBom, NULL, NULL, false, tMain_rev, &tBom_view_revision));

	iCheckError(AOM_save_with_extensions(tBom_view_revision));
	iCheckError(AOM_save_with_extensions(tMain_rev));
}