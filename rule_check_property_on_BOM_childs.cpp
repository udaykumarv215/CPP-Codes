//Create rule handler to check any property on Parent BOM line. If property filled for all child lines ,then WF should initiate, otherwise it should show a custom error.

#include"Headers.h"
#include<iostream>
#include<epm/epm.h>
#include<tc/emh.h>
#include<tccore/item.h>
#include<tccore/custom.h>
#include<tccore/aom_prop.h>
#include<tccore/aom_prop.h>
#include<bom/bom.h>

using namespace std;
#define DLLAPI _declspec(dllexport)
#define error (EMH_USER_error_base+12)
int iTemp = 0;

extern"C"
{
	extern DLLAPI int DLL_register_callbacks();
	extern DLLAPI int PLM_execute_callbacks1(int* decision, va_list argv);
	extern DLLAPI int check_property_on_BOM_childs(EPM_rule_message_t msg);
	extern DLLAPI int return_children(tag_t topline);

	extern DLLAPI int DLL_register_callbacks()
	{
		iCheckError(CUSTOM_register_exit("DLL", "USER_init_module", (CUSTOM_EXIT_ftn_t)PLM_execute_callbacks1));
		return ITK_ok;
	}
	extern DLLAPI int PLM_execute_callbacks1(int *decision, va_list argv)
	{
		*decision = ALL_CUSTOMIZATIONS;
		EPM_register_rule_handler("CheckPropertyBomChilds", "handler from ITK", (EPM_rule_handler_t)check_property_on_BOM_childs);
		return ITK_ok;
	}
	extern DLLAPI int check_property_on_BOM_childs(EPM_rule_message_t msg)
	{
		tag_t tTask = msg.task;
		tag_t tRootTask = NULLTAG, tWindow = NULLTAG, tTopLine = NULLTAG, tRev = NULLTAG, tItem = NULLTAG;
		int iCount = 0, iNum=0;
		tag_t* tAttachments = NULLTAG;
		tag_t* tBvList = NULLTAG;

		iCheckError(EPM_ask_root_task(tTask, &tRootTask));
		iCheckError(EPM_ask_attachments(tRootTask, EPM_target_attachment, &iCount, &tAttachments));
		if (iCount > 0)
		{
			for (int i = 0; i < iCount; i++)
			{
				iCheckError(ITEM_rev_list_all_bom_view_revs(tAttachments[i], &iNum, &tBvList));
				for (int j = 0; j < iNum; j++)
				{
					iCheckError(BOM_create_window(&tWindow));
					iCheckError(BOM_save_window(tWindow));
					ITEM_ask_item_of_rev(tAttachments[i], &tItem);
					ITEM_ask_latest_rev(tItem, &tRev);
					iCheckError(BOM_set_window_top_line(tWindow, tAttachments[i], tRev, tBvList[j], &tTopLine));
					int iStatus = return_children(tTopLine);
					if (iStatus == 1)
						return EPM_go;
					else
						return EPM_nogo;
				}
			}
		}
		return ITK_ok;
	}
	extern DLLAPI int return_children(tag_t topline)
	{
		int Count = 0;
		tag_t* tChildren = NULLTAG;
		char* cValue = NULL;

		BOM_line_ask_child_lines(topline, &Count, &tChildren);
		for (int i = 0; i < Count; i++)
		{
			iCheckError(AOM_ask_value_string(tChildren[i], "bl_rev_object_desc", &cValue));
			if (tc_strcmp(cValue,"")==0)
				return 0;
			else
				iTemp = 1;

			return_children(tChildren[i]);
		}
		return iTemp;
	}
}