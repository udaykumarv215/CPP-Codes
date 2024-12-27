//Insert a new item as a parent to the second child of the structure.

#include"Headers.h"
#include<iostream>
#include<tccore/item.h>
#include<bom/bom.h>
#include<ps/ps.h>
#include<tccore/aom.h>

using namespace std;

int iAddImmediateParent()
{
	tag_t tItem = NULLTAG, tRevision=NULLTAG, tWindow =NULLTAG, tTopLine =NULLTAG, tParentItem =NULLTAG, tParentRevision=NULLTAG;
	int iCount = 0 , iCount1 =0;
	tag_t* tBvList = NULLTAG;
	tag_t* tChildren = NULLTAG;

	const char* cItemId = ITK_ask_cli_argument("-id=");
	const char* cParentItemId = ITK_ask_cli_argument("-pid=");

	iCheckError(ITEM_find_item(cItemId, &tItem));
	iCheckError(ITEM_find_rev(cItemId, "A", &tRevision));

	iCheckError(ITEM_find_item(cParentItemId, &tParentItem));
	iCheckError(ITEM_find_rev(cParentItemId, "A", &tParentRevision));

	iCheckError(ITEM_list_bom_views(tItem, &iCount, &tBvList));

	for (int i = 0; i < iCount; i++)
	{
		iCheckError(BOM_create_window(&tWindow));
		iCheckError(BOM_set_window_top_line(tWindow, tItem, tRevision, tBvList[i], &tTopLine));

		iCheckError(BOM_line_ask_all_child_lines(tTopLine, &iCount1, &tChildren));
		for (int j = 0; j < iCount1; j++)
		{
			if (j == 1)
			{
				tag_t tBom = NULLTAG, tBom_view_revision =NULLTAG;

				iCheckError(PS_create_bom_view(NULLTAG, NULL, NULL, tParentItem, &tBom));
				iCheckError(PS_create_bvr(tBom, NULL, NULL, false, tParentRevision, &tBom_view_revision));
				AOM_save_without_extensions(tBom);
				AOM_save_without_extensions(tBom_view_revision);

				iCheckError(BOM_line_insert_level(iCount1, tChildren, tParentRevision, tBom));
				iCheckError(BOM_save_window(tWindow));
				return 0;
			}
		}
	}
	return 0;
}