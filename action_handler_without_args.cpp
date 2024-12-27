// Action-Handler  (Without Arguments) //creating custom status //Workflow Handler for creation of custom Status//

#include"Headers.h"
#include<iostream>
#include<tc/emh.h>
#include<epm/epm.h>
#include<tccore/custom.h>
#include<tc/emh.h>
#include<tccore/aom_prop.h>
#include<tccore/grm.h>
#include<tccore/aom.h>

using namespace std;

#define DLLAPI _declspec(dllexport)
#define va_arg   __crt_va_arg
int iStatus = 0;
METHOD_id_t method_id;

extern "C"
{
	extern DLLAPI int DLL_register_callbacks();
	extern DLLAPI int PLM_execute_callbacks1(int *decisison, va_list argv);
	extern DLLAPI int PLM_execute_callbacks2(int *decisison, va_list argv);
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

		EPM_register_action_handler("ITKActionHandler", "This handler is created using ITK", (EPM_action_handler_t)Handler_add_custom_Handler);

		return iStatus;
	}
	extern DLLAPI int Handler_add_custom_Handler(EPM_action_message_t msg)
	{
		tag_t tTask = msg.task;
		tag_t tRootTask = NULLTAG, tClassId=NULLTAG, tReleasedStatus=NULLTAG;
		char* cClassName = NULL;
		int iCount = 0;
		tag_t* tAttachments = NULLTAG;
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
					iCheckError(RELSTAT_create_release_status("CustomReleaseStatusITK", &tReleasedStatus));
					iCheckError(RELSTAT_add_release_status(tReleasedStatus, 1, &tAttachments[i], TRUE));
				}
			}
		}
		return 0;
	}
	extern DLLAPI int PLM_execute_callbacks2(int *decision, va_list argv)
	{
		*decision = ALL_CUSTOMIZATIONS;
		cout << "\n Logout Successfull \n ";
		return iStatus;
	}
}