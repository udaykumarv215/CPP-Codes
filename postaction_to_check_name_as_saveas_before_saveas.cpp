//Save As Operation has to be performed only when the Item name is "Save As", otherwise throw an error. After Save As operation , the object_desc of new Item Master has to be updated with the name of the Item.

#include"Headers.h"
#include<iostream>
#include<tccore/custom.h>
#include<tccore/method.h>
#include<tccore/aom_prop.h>
#include<tccore/aom.h>
#include<fclasses/tc_string.h>
#include<tccore/grm.h>

#define DLLAPI _declspec(dllexport)
#define error (EMH_USER_error_base +9)

int iStatus = 0;
METHOD_id_t method_id;
using namespace std;

extern"C"
{
	extern DLLAPI int DLL_register_callbacks();
	extern DLLAPI int PLM_execute_callbacks1(int *decision, va_list argv);
	extern DLLAPI int postaction_to_check_name_as_saveas_before_saveas(int *decision, va_list argv);

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

		iCheckError(METHOD_find_method("ItemRevision", "ITEM_create_from_rev", &method_id));

		//iCheckError(METHOD_add_pre_condition(method_id, (METHOD_function_t)precondition_to_check_name_as_saveas_before_saveas, NULL));
		iCheckError(METHOD_add_action(method_id, METHOD_post_action_type, (METHOD_function_t)postaction_to_check_name_as_saveas_before_saveas, NULL));
		return 0;
	}

	extern DLLAPI int postaction_to_check_name_as_saveas_before_saveas(int *decision, va_list argv)
	{
		tag_t tParent = va_arg(argv, tag_t);
		tag_t tOldRev = va_arg(argv, tag_t);
		const char cNewItemId = va_arg(argv, tag_t);
		const char cNewRevId = va_arg(argv, tag_t);
		tag_t tNewItem = va_arg(argv, tag_t);
		tag_t tNewItemRev = va_arg(argv, tag_t);

		char* cValue = NULL;
		tag_t tRelation = NULLTAG;
		int iCount = 0; 
		tag_t* tSecondaryObjects = NULLTAG;
		char* cValue1 = NULL;

		iCheckError(AOM_ask_value_string(tParent, "object_name", &cValue));

		if (tc_strcmp(cValue, "save_as") == 0)
		{
			GRM_find_relation_type("IMAN_master_form", &tRelation);
			GRM_list_secondary_objects_only(tNewItem, tRelation, &iCount, &tSecondaryObjects);
			AOM_ask_value_string(tNewItem, "object_name", &cValue1);

			for (int i = 0; i < iCount; i++)
			{
				AOM_refresh(tSecondaryObjects[i], true);
				AOM_set_value_string(tSecondaryObjects[i], "object_desc", cValue1);
				AOM_save_without_extensions(tSecondaryObjects[i]);
				AOM_refresh(tSecondaryObjects[i], false);
			}
			return ITK_ok;
		}
			
		else
		{
			EMH_store_error_s1(EMH_severity_error, error, "save_as");
			return error;
		}
	}
}