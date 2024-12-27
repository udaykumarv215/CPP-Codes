//Check properties listed in the preference and check they are not null, then only workflow has to be initiated.

#include"Headers.h"
#include<iostream>
#include<tccore/custom.h>
#include<tccore/method.h>
#include<tccore/aom.h>
#include<tccore/aom_prop.h>
#include<tc/preferences.h>
#include<epm/epm.h>

#define DLLAPI _declspec(dllexport)
#define error (EMH_USER_error_base+12)
using namespace std;

int iStatus = 0;

extern"C"
{
	extern DLLAPI int DLL_register_callbacks();
	extern DLLAPI int PLM_execute_callbacks1(int *decision, va_list argv);
	extern DLLAPI int checkpropertyinpreference(EPM_rule_message_t msg);

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

		iCheckError(EPM_register_rule_handler("checkpropertyinpreference", "", (EPM_rule_handler_t)checkpropertyinpreference));
		return 0;
	}

	extern DLLAPI int checkpropertyinpreference(EPM_rule_message_t msg)
	{
		tag_t tTask = msg.task;
		tag_t tRootTask = NULLTAG;
		int iCount = 0, iNum=0, iTemp=0;
		char** cValues = NULL;
		tag_t* tAttachments = NULL;

		iCheckError(EPM_ask_root_task(tTask, &tRootTask));
		iCheckError(EPM_ask_attachments(tRootTask, EPM_target_attachment, &iNum, &tAttachments));

		iCheckError(PREF_ask_char_values("rule_ITK_property", &iCount, &cValues));

		for (int i = 0; i < iCount; i++)
		{
			for (int j = 0; j < iNum; j++)
			{
				char* cPropValue = NULL;
				iCheckError(AOM_UIF_ask_value(tAttachments[j], cValues[i], &cPropValue));
				if (tc_strcmp(cPropValue, " ") == 0)
					iTemp++;
				else
				{
					iTemp = 0;
					EMH_store_error(EMH_severity_error, error);
					return error;
				}
			}
		}
		if (iTemp != 0)
			return EPM_go;
		else
			return EPM_nogo;
	}
}

