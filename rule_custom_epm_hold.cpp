#include"Headers.h"
#include<stdio.h>
#include<conio.h>
#include<iostream>
#include<tccore/method.h>
#include<tccore/custom.h>
#include<tccore/aom_prop.h>
#include<fclasses/tc_string.h>
#include<epm/epm.h>
#include<tccore/grm.h>
#include<tccore/workspaceobject.h>
#include<tc/emh.h>

#define DLLAPI _declspec(dllexport)
#define customerror (EMH_USER_error_base+2)

using namespace std;
int iStatus = 0;

extern "C"
{
	extern DLLAPI int DLL_register_callbacks();
	extern DLLAPI int Handler_execute_callback_1(int *decision, va_list argv);
	extern DLLAPI int Handler_execute_callback_2(int *decision, va_list argv);
	extern DLLAPI int Handler_add_Rule_Handler(EPM_rule_message_t msg);

	extern DLLAPI int DLL_register_callbacks()
	{
		iStatus = CUSTOM_register_exit("DLL", "USER_init_module", (CUSTOM_EXIT_ftn_t)Handler_execute_callback_1);
		iStatus = CUSTOM_register_exit("DLL", "USER_exit_module", (CUSTOM_EXIT_ftn_t)Handler_execute_callback_2);
		return iStatus;
	}

	extern DLLAPI int Handler_execute_callback_1(int *decision, va_list argv)
	{
		*decision = ALL_CUSTOMIZATIONS;
		cout << "\n **************** Rule Handler Registration Successfull **************\n " << endl;
		EPM_register_rule_handler("CustomEPMhold", "RHandlerCheckAttachment", (EPM_rule_handler_t)Handler_add_Rule_Handler);
		return iStatus;
	}

	extern DLLAPI int Handler_add_Rule_Handler(EPM_rule_message_t msg)
	{
		tag_t tTask = msg.task;
		char* cResult = NULL;

		iCheckError(EPM_get_task_result(tTask, &cResult));

		if (tc_strcmp(cResult,"Completed")==0)
			return EPM_go;
		else
			return EPM_nogo;

		tTask = NULLTAG;
		cResult = NULL;
	}
	extern DLLAPI int Handler_execute_callback_2(int *decision, va_list argv)
	{
		*decision = ALL_CUSTOMIZATIONS;
		cout << "\n **************** Logout Sucessfull **************\n " << endl;
		return iStatus;

	}
}