#include"Headers.h"
#include<iostream>
#include<tccore/item.h>
#include<tccore/grm.h>
#include<fclasses/tc_string.h>
#include<ae/dataset.h>
#include<tc/folder.h>
#include<tccore/aom_prop.h>
#include<tccore/aom.h>
#include<bom/bom.h>
#include<sa/user.h>
#include<sa/tcfile.h>
#include<string>

using namespace std;
string sPath = "D:";
string sFullPath = "";

int iAskChildLines(char* cUserId)
{
	tag_t tItemTag = NULLTAG, tItemRev = NULLTAG, *tBvList=NULLTAG, tWindow=NULLTAG, tTopLine=NULLTAG, *tChildren=NULLTAG, tRootParentFolder =NULLTAG, tUserId =NULLTAG, tHomeFolder=NULLTAG;
	int iCount = 0, iNum=0;
	char *cName = NULL;

	const char* cItemId = ITK_ask_cli_argument("-id=");

	ITEM_find_item(cItemId, &tItemTag);
	ITEM_ask_latest_rev(tItemTag, &tItemRev);

	ITEM_list_bom_views(tItemTag, &iCount, &tBvList);
	SA_find_user2(cUserId, &tUserId);
	SA_ask_user_home_folder(tUserId, &tHomeFolder);

	for (int i = 0; i < iCount; i++)
	{
		AOM_ask_value_string(tItemTag, "object_name", &cName);
		FL_create2(cName, "New fodler", &tRootParentFolder);
		AOM_save_without_extensions(tRootParentFolder);
		cout << cName << endl;

		BOM_create_window(&tWindow);
		BOM_save_window(tWindow);
		BOM_set_window_top_line(tWindow, tItemTag, tItemRev, tBvList[i], &tTopLine);

		iReturn_children(tTopLine, tRootParentFolder);
		FL_insert(tHomeFolder, tRootParentFolder, 999);
		AOM_save_without_extensions(tHomeFolder);
	}
	return ITK_ok;
}

int iReturn_children(tag_t tLine, string tFolder)
{
	int iCount = 0;
	tag_t x = NULLTAG, tParentFolder = NULLTAG;
	string tMainFolderPath = NULLTAG;
	tag_t* tChildren = nullptr;
	int i = 0;
	char* cId = nullptr, *cName =NULL;
	char* cParent_id = nullptr;

	iCheckError(BOM_line_ask_all_child_lines(tLine, &iCount, &tChildren));

	if (iCount != 0)
	{
		for (i = 0; i < iCount; i++)
		{
			tMainFolderPath = iAttachNamedReferenceToFolder(tChildren[i]);
			iReturn_children(tChildren[i], tMainFolderPath);
			FL_insert(tFolder, tMainFolder, 999);
			AOM_save_without_extensions(tFolder);
		}
	}
	return 0;
}

string iAttachNamedReferenceToFolder(tag_t tRev)
{
	tag_t *SecondaryObjects = NULLTAG, tClassId = NULLTAG, *tNamedRefs = NULLTAG, tFolder=NULLTAG, tMainRev=NULLTAG;
	int iCount = 0, iFound = 0;
	char* cClassName = NULL, *cName = NULL, *cItemId=NULL, *cItemRevId=NULL;
	string sFullPath = "";

	AOM_ask_value_string(tRev, "bl_item_object_name", &cName);
	sFullPath = sPath + "\\" + cName;
	

	AOM_ask_value_string(tRev, "bl_item_item_id", &cItemId);
	AOM_ask_value_string(tRev, "bl_rev_item_revision_id", &cItemRevId);
	ITEM_find_rev(cItemId, cItemRevId, &tMainRev);

	GRM_list_secondary_objects_only(tMainRev, NULLTAG, &iCount, &SecondaryObjects);
	for (int i = 0; i < iCount; i++)
	{
		POM_class_of_instance(SecondaryObjects[i], &tClassId);
		POM_name_of_class(tClassId, &cClassName);
		if (tc_strcmp(cClassName, "Dataset") == 0)
		{
			AOM_ask_value_tags(SecondaryObjects[i], "ref_list", &iFound, &tNamedRefs);
			for (int j = 0; j < iFound; j++)
			{
				IMF_export_file(tNamedRefs[j], sFullPath.c_str());
			}
		}
	}
	return sFullPath;
}