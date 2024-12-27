// DLL.cpp : This file contains the 'main' function. Program execution begins and ends there.
//.pre-condition check the release status of item revision before revising,if it is TCM Released you can revise else error message.

#include"Headers.h"
#include<epm/epm.h>
#include<iostream>
#include<tccore/custom.h>
#include<tc/emh.h>
#include<tccore/aom.h>
#include<tccore/aom_prop.h>

#define error (EMH_USER_error_base +2)
#define error1 (EMH_USER_error_base +3)

using namespace std;

#define DLLAPI _declspec(dllexport)
int iStatus = 0;
METHOD_id_t method_id;

extern "C"
{
	extern DLLAPI int DLL_register_callbacks();
	extern DLLAPI int PLM_execute_callbacks1(int *decisison, va_list argv);
	extern DLLAPI int PLM_execute_callbacks2(int *decisison, va_list argv);
	extern DLLAPI int PLM_check_release_status(int *decisison, va_list argv);

	extern DLLAPI int DLL_register_callbacks()
	{
		iStatus = CUSTOM_register_exit("DLL", "USER_init_module", (CUSTOM_EXIT_ftn_t)PLM_execute_callbacks1);
		iStatus = CUSTOM_register_exit("DLL", "USER_exit_module", (CUSTOM_EXIT_ftn_t)PLM_execute_callbacks2);
		return iStatus;
	}

	extern DLLAPI int PLM_execute_callbacks1(int *decision, va_list argv)
	{
		*decision = ALL_CUSTOMIZATIONS;
		cout << "\n Welcome to DLL Regestration process \n";
		cout << "\n  Login Sucessfull \n ";
		METHOD_find_method("ItemRevision", "ITEM_copy_rev", &method_id);
		METHOD_add_pre_condition(method_id, (METHOD_function_t)PLM_check_release_status, NULL);
		return iStatus;
	}
	extern DLLAPI int PLM_execute_callbacks2(int *decision, va_list argv)
	{
		*decision = ALL_CUSTOMIZATIONS;
		cout << "\n Logout Sucessfull \n ";
		return iStatus;
	}
	extern DLLAPI int PLM_check_release_status(int *decisison, va_list argv)
	{
		tag_t tItem=NULLTAG;
		int iNum = 0;
		char* cValue = NULL;
		char** cValues = NULL;
		tItem = va_arg(argv, tag_t);
		iCheckError(AOM_ask_displayable_values(tItem, "release_status_list", &iNum, &cValues));
		iCheckError(AOM_ask_value_string(tItem, "object_name", &cValue));

		if (iNum == 0)
		{
			EMH_store_error_s1(EMH_severity_error, error, cValue);
			return error;
		}
		else
		{
			int iCount = 0;
			for (int i = 0; i < iNum; i++)
			{
				if (tc_strcmp(cValues[i], "TCM Released") == 0)
					iCount = 1;
				else
				{
					EMH_store_error_s1(EMH_severity_error, error1, cValue);
					iCount = 2;
				}
			}
			if (iCount == 1)
				return iStatus;
			else
				return error1;
		}
	}
}
