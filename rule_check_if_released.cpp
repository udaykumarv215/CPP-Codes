//Workflow Should be initiated only when Item Revision having 'TCM Released' Status, else should throw an error.

#include"Headers.h"
#include<iostream>
#include<epm/epm.h>
#include<tc/emh.h>
#include<tccore/custom.h>

using namespace std;
#define DLLAPI _declspec(dllexport)
#define error (EMH_USER_error_base+12)

extern"C"
{
	extern DLLAPI int DLL_register_callbacks();
	extern DLLAPI int PLM_execute_callbacks1(int* decision, va_list argv);
	extern DLLAPI int check_if_released(EPM_rule_message_t msg);

	extern DLLAPI int DLL_register_callbacks()
	{
		iCheckError(CUSTOM_register_exit("DLL", "USER_init_module", (CUSTOM_EXIT_ftn_t)PLM_execute_callbacks1));
		return ITK_ok;
	}
	extern DLLAPI int PLM_execute_callbacks1(int *decision, va_list argv)
	{
		*decision =  ALL_CUSTOMIZATIONS;
		EPM_register_rule_handler("CheckIfReleased", "handler from ITK", (EPM_rule_handler_t)check_if_released);
		return ITK_ok;
	}
	extern DLLAPI int check_if_released(EPM_rule_message_t msg)
	{
		tag_t tTask = msg.task;
		tag_t tRootTask = NULLTAG;
		int iCount = 0;
		tag_t* tAttachments = NULLTAG;
		int isReleased = 0;
		tag_t tClassId = NULLTAG;
		char* cClassName = NULL;

		iCheckError(EPM_ask_root_task(tTask, &tRootTask));
		iCheckError(EPM_ask_attachments(tRootTask, EPM_target_attachment, &iCount, &tAttachments));
		if (iCount > 0)
		{
			for (int i = 0; i < iCount; i++)
			{
				iCheckError(POM_class_of_instance(tAttachments[i], &tClassId));
				iCheckError(POM_name_of_class(tClassId, &cClassName));

				if (tc_strcmp("ItemRevision", cClassName) == 0)
				{
					EPM_ask_if_released(tAttachments[i], &isReleased);
					if (isReleased)
						EPM_go;
					else
					{
						EMH_store_error(EMH_severity_error, error);
						return error;
					}
				}
			}
		}
		return 0;
	}
}
