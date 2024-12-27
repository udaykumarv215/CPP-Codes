#include"Headers.h"
#include<iostream>
#include<tccore/aom.h>
#include<tccore/item.h>
#include<tccore/grm.h>
#include<tccore/aom_prop.h>
#include<fclasses/tc_string.h>

using namespace std;

//int iDelete_datsets()
//{
//	tag_t tItem=NULLTAG, iRev_tag = NULLTAG, tRelation = NULLTAG, tSecondary_object = NULLTAG;
//	int iCount = 0, iNum = 0;
//	tag_t* tList=NULL;
//	tag_t* tRevList = NULL;
//	char* cValue = NULL;
//	char* cItem_id = ITK_ask_cli_argument("-i=");
//
//	iCheckError(ITEM_find_item(cItem_id, &tItem));
//	iCheckError(ITEM_list_all_revs(tItem, &iNum, &tRevList));
//
//	for (int i = 0; i < iNum; i++)
//	{
//		iCheckError(GRM_list_secondary_objects_only(tRevList[i], NULLTAG, &iCount, &tList));
//
//		for (int j = 0; j < iCount; j++)
//		{
//			iCheckError(AOM_ask_value_string(tList[j], "object_type", &cValue));
//			if (tc_strcmp(cValue, "PDF") == 0)
//			{
//				iCheckError(AOM_delete_from_parent(tList[j], tRevList[i]));
//			}
//		}
//	}
//	return 0;
//}

int iDelete_datsets()
{
	tag_t tItem = NULLTAG, iRev_tag = NULLTAG, tRelation = NULLTAG, tSecondary_object = NULLTAG;
	int iCount = 0, iNum = 0;
	tag_t* tList = NULL;
	tag_t* tRevList = NULL;
	char* cValue = NULL;
	char* cItem_id = ITK_ask_cli_argument("-i=");

	iCheckError(ITEM_find_item(cItem_id, &tItem));
	iCheckError(ITEM_list_all_revs(tItem, &iNum, &tRevList));

	for (int i = 0; i < iNum; i++)
	{
		GRM_list_secondary_objects(tRevList[i], NULLTAG, &iCount, &)
		for (int j = 0; j < iCount; j++)
		{
			
		}
	}
	return 0;
}