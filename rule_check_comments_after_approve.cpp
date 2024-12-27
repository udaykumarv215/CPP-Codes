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
#include<tccore/aom_prop.h>
#include<sa/sa.h>

#define DLLAPI _declspec(dllexport)
#define error (EMH_USER_error_base +6)

using namespace std;
int iStatus = 0;

extern"C"
{
	extern DLLAPI int DLL_register_callbacks();
	extern DLLAPI int Handler_execute_callback_1(int *decision, va_list argv);
	extern DLLAPI int Handler_add_Rule_Handler(EPM_rule_message_t msg);

	extern DLLAPI int DLL_register_callbacks()
	{
		iStatus = CUSTOM_register_exit("DLL", "USER_init_module", (CUSTOM_EXIT_ftn_t)Handler_execute_callback_1);
		return iStatus;
	}
	extern DLLAPI int Handler_execute_callback_1(int *decision, va_list argv)
	{
		*decision = ALL_CUSTOMIZATIONS;
		cout << "\n **************** Rule Handler Registerd Sucessfull **************\n " << endl;
		EPM_register_rule_handler("checkcommentsafterreject", "handler for checking comments after reject", (EPM_rule_handler_t)Handler_add_Rule_Handler);
		return iStatus;
	}
	extern DLLAPI int Handler_add_Rule_Handler(EPM_rule_message_t msg)
	{
		tag_t tTask = msg.task;
		EPM_state_t State;
		EPM_signoff_decision_t decision;
		date_t dDecisionDate;
		char* comments;
		char* cResult = NULL;
		char* cValue1 = NULL;
		char* cName = NULL;
		char* cValue = NULL;
		char* cUser = NULL;
		tag_t tUser = NULLTAG;
		logical out;
	
		AOM_ask_value_string(tTask, "resp_party", &cUser);
		SA_find_user2(cUser, &tUser);
		EPM_ask_decision(tTask, tUser, &decision, &comments, &dDecisionDate);

		if (decision == EPM_reject_decision)
		{
			
			if (*comments != NULL)
				return EPM_go;
			else
				EMH_store_error_s1(EMH_severity_error, error, " ");
			return error;
		}
		return 0;
	}
}