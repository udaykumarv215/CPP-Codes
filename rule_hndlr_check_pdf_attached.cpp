#include"Headers.h"
#include<iostream>
#include<epm/epm.h>
#include<tc/emh.h>
#include<tccore/custom.h>
#include<tccore/grm.h>
#include<tccore/aom_prop.h>

using namespace std;

#define DLLAPI _declspec(dllexport)
#define error (EMH_USER_error_base +7)
int iStatus = 0;

extern"C"
{
	extern DLLAPI int DLL_register_callbacks();
	extern DLLAPI int PLM_execute_callbacks1(int *decision, va_list argv);
	extern DLLAPI int PLM_execute_callbacks2(int *decision, va_list argv);
	extern DLLAPI int check_pdf_attached(EPM_rule_message_t msg);

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

		EPM_register_rule_handler("check_pdf", "rule handler to check pdf attached", (EPM_rule_handler_t)check_pdf_attached);

		return 0;
	}

	extern DLLAPI int check_pdf_attached(EPM_rule_message_t msg)
	{
		tag_t tTask = msg.task;
		tag_t tRootTask = NULLTAG;
		int iCount = 0, iNumber=0;
		tag_t* tAttachments = NULL;
		tag_t* tSecondaryObjects = NULL;
		char* cValue = NULL;
		int iCheckEpmgo = 0, iCheckEpmnogo = 0;

		iCheckError(EPM_ask_root_task(tTask, &tRootTask));
		iCheckError(EPM_ask_attachments(tRootTask, EPM_target_attachment, &iCount, &tAttachments));

		if (iCount > 0)
		{
			for (int i = 0; i < iCount; i++)
			{
				iCheckError(GRM_list_secondary_objects_only(tAttachments[i], NULLTAG, &iNumber, &tSecondaryObjects));

				for (int j = 0; j < iNumber; j++)
				{
					iCheckError(AOM_ask_value_string(tSecondaryObjects[i], "object_type", &cValue));
					if (tc_strcmp(cValue,"PDF")==0)
						iCheckEpmgo++;
					else
						iCheckEpmnogo++;
				}
				if (iCheckEpmgo != 0)
					return EPM_go;
				else
					return EPM_nogo;
			}
		}
		else
			EMH_store_error_s1(EMH_severity_error, error, "eeeee");
			return error;
		
		return 0;
	}
	extern DLLAPI int PLM_execute_callbacks2(int *decision, va_list argv)
	{
		*decision = ALL_CUSTOMIZATIONS;
		cout << "\n **************** Logout Sucessfull **************\n " << endl;
		return iStatus;

	}
}