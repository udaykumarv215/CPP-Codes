// Action Handler with Multiple Arguments. // Workflow Handler with multiple arguments//
//Read property values from the arguments and update the properties.

#include"Headers.h"
#include<iostream>
#include<tccore/aom_prop.h>
#include<tccore/custom.h>
#include<tc/emh.h>
#include<epm/epm.h>
#include<tc/tc_arguments.h>

using namespace std;

#define DLLAPI _declspec(dllexport)
int iStatus = 0;

extern"C"
{
	extern DLLAPI int DLL_register_callbacks();
	extern DLLAPI int PLM_execute_callbacks1(int *decision, va_list argv);
	extern DLLAPI int PLM_execute_callbacks2(int *decision, va_list argv);
	extern DLLAPI int action_hndlr_multiple_args(EPM_action_message_t msg);

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

		EPM_register_action_handler("ITKhandlermultipleargs", "This handler is created using ITK", (EPM_action_handler_t)action_hndlr_multiple_args);
		return iStatus;
	}
	extern DLLAPI int action_hndlr_multiple_args(EPM_action_message_t msg)
	{
		tag_t tTask = msg.task;
		char* cName = NULL;
		char* cValue = NULL;
		char* cName1 = NULL;
		char* cValue1 = NULL;
		int iCount1 = 0;
		tag_t* tAttachments = NULL;
		tag_t tRootTask = NULLTAG;
		char* cPropertyName = NULL;
		char* cPropertyValue = NULL;

		int iCount = TC_number_of_arguments(msg.arguments);
		if (iCount > 0)
		{
			for (int i = 0; i < iCount; i++)
			{
				iCheckError(ITK_ask_argument_named_value(TC_next_argument(msg.arguments), &cName, &cValue));
				if (tc_strcmp(cName, "property") == 0)
				{
					cPropertyName = cValue;
				}
				else
				{
					cPropertyValue = cValue;
				}
			}
		}
		iCheckError(EPM_ask_root_task(tTask, &tRootTask));
		iCheckError(EPM_ask_attachments(tRootTask, EPM_target_attachment, &iCount1, &tAttachments));

		for (int i = 0; i < iCount1; i++)
		{
			iCheckError(AOM_set_value_string(tAttachments[i], cPropertyName, cPropertyValue));
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