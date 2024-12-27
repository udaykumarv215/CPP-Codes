//Revise Operation Should be performed only when the Revision is released, else should throw an error. If the Revision is Released, object_desc should be updated with "Revised" value on the old Revision (on which Revise is performed)

#include"Headers.h"
#include<iostream>
#include<tccore/custom.h>
#include<tccore/method.h>
#include<epm/epm.h>
#include<tccore/aom.h>
#include<tccore/aom_prop.h>
#include<tccore/item.h>

using namespace std;

#define DLLAPI _declspec(dllexport)
#define error (EMH_USER_error_base +10)

int iStatus = 0;
METHOD_id_t method_id;

extern"C"
{
	extern DLLAPI int DLL_register_callbacks();
	extern DLLAPI int PLM_execute_callbacks1(int *decision, va_list argv);
	extern DLLAPI int revise_when_released_update_object_desc(int *decision, va_list argv);

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

		iCheckError(METHOD_find_method("ItemRevision", "ITEM_copy_rev", &method_id));

		iCheckError(METHOD_add_action(method_id, METHOD_post_action_type, (METHOD_function_t)revise_when_released_update_object_desc, NULL));
		return 0;
	}
	extern DLLAPI int revise_when_released_update_object_desc(int *decision, va_list argv)
	{
		tag_t tSource = va_arg(argv, tag_t);
		char* cRevId = va_arg(argv, char*);
		tag_t tNew = va_arg(argv, tag_t);

		int iIsReleased = 0;

		EPM_ask_if_released(tSource, &iIsReleased);

		if (iIsReleased)
		{
			AOM_refresh(tSource,true);
			AOM_set_value_string(tSource, "object_desc", "updated");
			AOM_save_without_extensions(tSource);
			AOM_refresh(tSource, false);
			return ITK_ok;
		}
		else
		{
			EMH_store_error(EMH_severity_error, error);
			return error;
		}
	}
}