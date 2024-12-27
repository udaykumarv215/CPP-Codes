// update the properties on the latest revisions. In case the latest revision is WIP, they want us to update the previous revision as well. So, in case of a working revision, we will have to update the property on 2 revisions.
#include"Headers.h"
#include<iostream>
#include<tccore/item.h>
#include<tccore/aom.h>
#include<tccore/aom_prop.h>
#include<epm/epm.h>

using namespace std;
const char* cObjectDesc;

int iUpdatePropertiesWip()
{
	tag_t tItem = NULLTAG;
	tag_t* tRevList = NULL;
	int iIsReleased = 0;
	int iCount = 0;
	int iFail = 0;
	const char* cItemId = ITK_ask_cli_argument("-i=");
	cObjectDesc = ITK_ask_cli_argument("-desc=");

	iCheckError(ITEM_find_item(cItemId, &tItem));
	iCheckError(ITEM_list_all_revs(tItem, &iCount, &tRevList));

	if (iCount == 1)
		iSetProperty(tRevList[iCount - 1]);

	if (iCount < 4)
	{
		iCheckError(EPM_ask_if_released(tRevList[iCount - 1], &iIsReleased));
		if (iIsReleased)
			iSetProperty(tRevList[iCount - 1]);
		else
		{
			for (int j = iCount - 2; j >= 0; j--)
			{
				iSetProperty(tRevList[j]);
			}
		}
	}
	else
	{
		iCheckError(EPM_ask_if_released(tRevList[iCount - 1], &iIsReleased));
		if (iIsReleased)
			iSetProperty(tRevList[iCount - 1]);
		else
		{
			for (int j = iCount - 2; j >= (iCount - 2)-1; j--)
			{
				iSetProperty(tRevList[j]);
			}
		}
	}
	return 0;
}

void iSetProperty(tag_t tRevList)
{
	iCheckError(AOM_refresh(tRevList, true));
	iCheckError(AOM_set_value_string(tRevList, "object_desc", cObjectDesc));
	iCheckError(AOM_save_without_extensions(tRevList));
	iCheckError(AOM_refresh(tRevList, false));
}