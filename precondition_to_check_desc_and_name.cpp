#include"Headers.h"
#include<string>
#include<iostream>
#include<tccore/custom.h>
#include<tccore/method.h>
#include<tccore/aom.h>
#include<tccore/aom_prop.h>
#include<tccore/item.h>
#include<fclasses/tc_string.h>
#include<tccore/tctype.h>

using namespace std;

#define DLLAPI _declspec(dllexport)
#define PLM_error (EMH_USER_error_base +13)

int iStatus = 0;
METHOD_id_t method_id;

extern"C"
{
	extern DLLAPI int DLL_register_callbacks();
	extern DLLAPI int PLM_execute_callbacks1(int *decision, va_list argv);
	extern DLLAPI int precondition_to_check_desc_and_name(int *decision, va_list argv);

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

		iCheckError(METHOD_find_method("A2DistrictsRevision", "ITEM_create_rev", &method_id));

		iCheckError(METHOD_add_pre_condition(method_id, (METHOD_function_t)precondition_to_check_desc_and_name, NULL));
		return 0;
	}
	extern DLLAPI int precondition_to_check_desc_and_name(int *decision, va_list argv)
	{
		int ifail = ITK_ok;
		tag_t item = va_arg(argv, tag_t);

		char *objDesc = NULL;
		char *item_name = NULL;


		AOM_UIF_ask_value(item, "object_desc", &objDesc);
		AOM_UIF_ask_value(item, "object_name", &item_name);

		TC_write_syslog("Entering to if\n");
		if (tc_strcmp(item_name, objDesc) == 0)
		{
			TC_write_syslog("if started\n");
			EMH_store_error(EMH_severity_error, PLM_error);
			TC_write_syslog("if end\n");
			MEM_free(objDesc);
			return PLM_error;
		}
		MEM_free(objDesc);
		TC_write_syslog("existed function A4checkingItemNameDesc_Precondition\n");
		return ifail;
	}
}