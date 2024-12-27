#include"Headers.h"
#include<iostream>
#include<tccore/aom_prop.h>
#include<tccore/custom.h>
#include<tc/emh.h>
#include<epm/epm.h>
#include<tc/tc_arguments.h>
#include<tccore/item.h>

using namespace std;

#define DLLAPI _declspec(dllexport)
int iStatus = 0;

extern"C"
{
	extern DLLAPI int DLL_register_callbacks();
	extern DLLAPI int PLM_execute_callbacks1(int *decision, va_list argv);
	extern DLLAPI int action_hndlr(EPM_action_message_t msg);

	extern DLLAPI int DLL_register_callbacks()
	{
		iStatus = CUSTOM_register_exit("DLL", "USER_init_module", (CUSTOM_EXIT_ftn_t)PLM_execute_callbacks1);
		return iStatus;
	}
	extern DLLAPI int PLM_execute_callbacks1(int *decision, va_list argv)
	{
		*decision = ALL_CUSTOMIZATIONS;
		cout << "\n Welcome to DLL Registration process \n";
		cout << "\n  Login Successfull \n ";

		EPM_register_action_handler("Parameter Value Execution", "This handler is created using ITK", (EPM_action_handler_t)action_hndlr);
		return iStatus;
	}
	extern DLLAPI int action_hndlr(EPM_action_message_t msg)
	{
		tag_t tTask = msg.task;
		tag_t tRootTask = NULLTAG, tLatestRev = NULLTAG;
		int iCount = 0, iNum=0;
		tag_t* tAttachments = NULLTAG;
		char* tType = NULL;
		char** cValues = NULL;

		EPM_ask_root_task(tTask, &tRootTask);
		EPM_ask_attachments(tRootTask, EPM_target_attachment, &iCount, &tAttachments);
		if (iCount > 0)
		{
			for (int i = 0; i < iCount; i++)
			{
				AOM_ask_value_string(tAttachments[i], "object_type", &tType);
				if (tc_strcmp(tType, "A2MacroRevision") == 0)
				{
					ITEM_ask_latest_rev(tAttachments[i], &tLatestRev);
					AOM_ask_displayable_values(tLatestRev, "a2ConstantParameters", &iNum, &cValues);
				}
			}
		}
		return 0;
	}
}