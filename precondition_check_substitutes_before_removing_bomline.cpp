//Remove option in Structure manager should work only when the Child BOM line doesnt have any Substitute

#include"Headers.h"
#include<string>
#include<iostream>
#include<tccore/custom.h>
#include<tccore/method.h>
#include<tccore/aom.h>
#include<tccore/aom_prop.h>
#include<fclasses/tc_string.h>

using namespace std;

#define DLLAPI _declspec(dllexport)
#define error (EMH_USER_error_base +11)

int iStatus = 0;
METHOD_id_t method_id;

extern"C"
{
	extern DLLAPI int DLL_register_callbacks();
	extern DLLAPI int PLM_execute_callbacks1(int *decision, va_list argv);
	extern DLLAPI int check_substitiues_before_removing_bomline(int *decision, va_list argv);

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

		iCheckError(METHOD_find_method("BOMLine", "BOMLine_cut", &method_id));

		iCheckError(METHOD_add_pre_condition(method_id, (METHOD_function_t)check_substitiues_before_removing_bomline, NULL));
		return 0;
	}

	extern DLLAPI int check_substitiues_before_removing_bomline(int *decision, va_list argv)
	{
		tag_t tBomLine = va_arg(argv, tag_t);
		int iNum = 0;
		char** cValues = NULL;

		iCheckError(AOM_ask_displayable_values(tBomLine, "bl_substitute_list", &iNum, &cValues));
		if (iNum)
		{
			return ITK_ok;
		}
		else
		{
			EMH_store_error_s1(EMH_severity_error, error,"");
			return error;
		}
	}
}