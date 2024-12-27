//Adding Item revision to Parent Item Revision in a Structure only when they are of same object type.

#include"Headers.h"
#include<iostream>
#include<tccore/aom.h>
#include<tccore/aom_prop.h>
#include<tccore/custom.h>
#include<tccore/method.h>
#include<tccore/item.h>
#include<fclasses/tc_string.h>

using namespace std;

#define DLLAPI _declspec(dllexport)
#define error (EMH_USER_error_base +8)

int iStatus = 0;
METHOD_id_t method_id;

extern"C"
{
	extern DLLAPI int DLL_register_callbacks();
	extern DLLAPI int PLM_execute_callbacks1(int *decision, va_list argv);
	extern DLLAPI int attach_child_of_same_type_to_parent(int *decision, va_list argv);

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

		iCheckError(METHOD_find_method("BOMLine", "BOMLine_add", &method_id));

		iCheckError(METHOD_add_pre_condition(method_id,(METHOD_function_t)attach_child_of_same_type_to_parent, NULL));
		return 0;
	}

	extern DLLAPI int attach_child_of_same_type_to_parent(int *decision, va_list argv)
	{
		tag_t tParent = va_arg(argv, tag_t);
		tag_t tChildItem = va_arg(argv, tag_t);
		tag_t tChildItemRevision = va_arg(argv, tag_t);
		char* cValueChild = NULL;
		char* cValue = NULL;
		tag_t tParentClassId = NULL;
		char* cParentClassName = NULL;
		char* cChildItem = NULL;
		tag_t tChildClassId = NULL;
		char* cChildClassName = NULL;
		tag_t tItemTag = NULLTAG;

		iCheckError(AOM_ask_value_string(tParent, "bl_item_object_type", &cValue));
		iCheckError(ITEM_ask_item_of_rev(tChildItemRevision, &tItemTag));
		iCheckError(AOM_ask_value_string(tItemTag, "object_type", &cValueChild));

		if (tc_strcmp(cValue, cValueChild) == 0)
			return 0;
		else
		{
			char* cParentName = NULL;
			char* cChildName = NULL;

			iCheckError(AOM_ask_value_string(tParent, "bl_rev_object_name", &cParentName));
			iCheckError(AOM_ask_value_string(tChildItemRevision, "object_name", &cChildName));
			EMH_store_error_s2(EMH_severity_error, error, cParentName, cChildName);
			return error;
		}

	}
}