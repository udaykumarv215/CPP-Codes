// Action Handler with single Argument //Action Handler to create custom status with single argument //Workflow Handler with single argument for creation of custom Status//

#include"Headers.h"
#include<iostream>
#include<tc/emh.h>
#include<epm/epm.h>
#include<tccore/custom.h>
#include<tc/tc_arguments.h>

using namespace std;

#define DLLAPI _declspec(dllexport)
int iStatus = 0;

extern"C"
{
	extern DLLAPI int DLL_register_callbacks();
	extern DLLAPI int PLM_execute_callbacks1(int *decision, va_list argv);
	extern DLLAPI int PLM_execute_callbacks2(int *decision, va_list argv);
	extern DLLAPI int Handler_add_custom_Handler(EPM_action_message_t msg);

	extern DLLAPI int DLL_register_callbacks()
	{
		iStatus = CUSTOM_register_exit("DLL", "USER_init_module", (CUSTOM_EXIT_ftn_t)PLM_execute_callbacks1);
		iStatus = CUSTOM_register_exit("DLL", "USER_exit_module", (CUSTOM_EXIT_ftn_t)PLM_execute_callbacks2);
		return iStatus;
	}
	extern DLLAPI int PLM_execute_callbacks1(int *decision, va_list argv)
	{
		*decision = ALL_CUSTOMIZATIONS;
		cout << "\n Welcome to DLL Registration process \n";
		cout << "\n  Login Successfull \n ";

		EPM_register_action_handler("ITKActionHandler2", "This handler is created using ITK", (EPM_action_handler_t)Handler_add_custom_Handler);
		return iStatus;
	}
	extern DLLAPI int Handler_add_custom_Handler(EPM_action_message_t msg)
	{
		tag_t tTask = msg.task;
		tag_t tRootTask = NULLTAG, tClassId = NULLTAG, tReleasedStatus = NULLTAG;
		tag_t* tAttachments = NULL;
		char* cClassName = NULL;
		int iCount = 0, iCount1=0;
		
		iCount = TC_number_of_arguments(msg.arguments);
		char* cName =NULL;
		char* cValue = NULL;
		if (iCount > 0)
		{
			for (int i = 0; i < iCount; i++)
			{
				iCheckError(ITK_ask_argument_named_value(TC_next_argument(msg.arguments), &cName, &cValue));
				if (tc_strcmp(cName, "status") == 0)
				{
					iCheckError(EPM_ask_root_task(tTask, &tRootTask));
					iCheckError(EPM_ask_attachments(tRootTask, EPM_target_attachment, &iCount1, &tAttachments));

					if (iCount1 > 0)
					{
						for (int i = 0; i < iCount1; i++)
						{
							iCheckError(POM_class_of_instance(tAttachments[i], &tClassId));
							iCheckError(POM_name_of_class(tClassId, &cClassName));

							if (tc_strcmp("ItemRevision", cClassName) == 0)
							{
								iCheckError(RELSTAT_create_release_status(cValue, &tReleasedStatus));
								iCheckError(RELSTAT_add_release_status(tReleasedStatus, 1, &tAttachments[i], TRUE));
							}
						}
					}
				}
			}
		}
		return 0;
	}
	extern DLLAPI int PLM_execute_callbacks2(int *decision, va_list argv)
	{
		*decision = ALL_CUSTOMIZATIONS;
		printf("\n **************** Logout Sucessfull **************\n ");
		return iStatus;
	}
}
