//Update the object_desc property on an ItemRevision when that ItemRevision is assigned to a Project.

#include"Headers.h"
#include<iostream>
#include<tccore/aom.h>
#include<tccore/aom_prop.h>
#include<tc/emh.h>
#include<tccore/custom.h>
#include<tccore/method.h>
#include<tccore/item.h>

using namespace std;

#define DLLAPI _declspec(dllexport)
int iStatus = 0;
METHOD_id_t method_id;

extern"C"
{
	extern DLLAPI int DLL_register_callbacks();
	extern DLLAPI int PLM_execute_callbacks1(int *decision, va_list argv);
	extern DLLAPI int update_desc_after_assign(int *decision, va_list argv);

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

		iCheckError(METHOD_find_method("ItemRevision", "TC_assign_primary_obj_to_project", &method_id));

		iCheckError(METHOD_add_action(method_id, METHOD_post_action_type, (METHOD_function_t)update_desc_after_assign, NULL));
		return 0;
	}
	
	extern DLLAPI int update_desc_after_assign(int *decision, va_list argv)
	{
		tag_t tPrimaryObject=NULLTAG;
		char* value;
		tPrimaryObject = va_arg(argv, tag_t);

		iCheckError(AOM_refresh(tPrimaryObject, true));
		iCheckError(AOM_set_value_string(tPrimaryObject, "object_desc", "updated"));
		iCheckError(AOM_save_without_extensions(tPrimaryObject));
		iCheckError(AOM_refresh(tPrimaryObject, false));

		return 0; 
	}
}