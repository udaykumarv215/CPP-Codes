#include"Headers.h"
#include<iostream>
#include<tccore/custom.h>
#include<tccore/method.h>
#include<tccore/item.h>
#include<fclasses/tc_string.h>
#include<tc/emh.h>

using namespace std;
#define DLLAPI _declspec(dllexport)
METHOD_id_t method_id;
int iStatus = ITK_ok;
#define error (EMH_USER_error_base +10)

extern"C"
{
	extern DLLAPI int DLL_register_callbacks();
	extern DLLAPI int PLM_execute_callbacks1(int *decision, va_list argv);
	extern DLLAPI int PLM_execute_callbacks2(int *decisison, va_list argv);
	extern DLLAPI int iCheckPropertyValue(int *decision, va_list argv);

	extern DLLAPI int DLL_register_callbacks()
	{
		cout << "\n  Entered DLL_register_callbacks \n ";
		iStatus = CUSTOM_register_exit("DLL", "USER_init_module", (CUSTOM_EXIT_ftn_t)PLM_execute_callbacks1);
		iStatus = CUSTOM_register_exit("DLL", "USER_exit_module", (CUSTOM_EXIT_ftn_t)PLM_execute_callbacks2);
		cout << "\n  Exit DLL_register_callbacks \n ";
		return iStatus;
	}

	extern DLLAPI int PLM_execute_callbacks1(int *decision, va_list argv)
	{
		cout << "\n Entered PLM_execute_callbacks1 \n ";
		*decision = ALL_CUSTOMIZATIONS;
		cout << "\n Welcome to DLL Registration process \n";
		cout << "\n Login Successfull \n ";

		iStatus = METHOD_find_prop_method("A2CustomItemRevision","A2EmailId", "getA2EmailId", &method_id);
		iStatus = METHOD_add_action(method_id, METHOD_post_action_type, (METHOD_function_t)iCheckPropertyValue, NULL);

		cout << "\n Exit PLM_execute_callbacks1 \n ";
		return iStatus;
	}
	extern DLLAPI int iCheckPropertyValue(int *decision, va_list argv)
	{
		char* cValue = va_arg(argv, char*);
		char* cUserName = NULL;
		char* cUserId = NULL;
		cUserName = tc_strtok(cValue, "@");
		cUserId = tc_strtok(NULL, "@");

		if (tc_strcmp(cUserId, "intelizign.com") != 0)
		{
			EMH_store_error(EMH_severity_error, error);
			return error;
		}
		return ITK_ok;
	}
	extern DLLAPI int PLM_execute_callbacks2(int *decision, va_list argv)
	{
		cout << "\n  Entered PLM_execute_callbacks2 \n ";
		*decision = ALL_CUSTOMIZATIONS;
		cout << "\n Logout Sucessfull \n ";
		cout << "\n  Exit PLM_execute_callbacks2 \n ";
		return iStatus;
	}
}