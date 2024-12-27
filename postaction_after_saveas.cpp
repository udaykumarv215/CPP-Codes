//post action after save as operation on item revision, add some data into IR Master Form.

#include"Headers.h"
#include<iostream>
#include<epm/epm.h>
#include<tccore/aom.h>
#include<tccore/aom_prop.h>
#include<tccore/custom.h>
#include<tc/emh.h>
#include<tccore/grm.h>

using namespace std;

#define error4 (EMH_USER_error_base +4)

#define DLLAPI _declspec(dllexport) // using this for compailing purpose.
int iStatus = 0;
METHOD_id_t method_id;

extern "C"
{
	extern DLLAPI int DLL_register_callbacks();
	extern DLLAPI int PLM_execute_callbacks1(int *decisison, va_list argv);
	extern DLLAPI int PLM_execute_callbacks2(int *decisison, va_list argv);
	extern DLLAPI int set_property_IRMF(int *decision, va_list argv);

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

		METHOD_find_method("ItemRevision", "ITEM_create_from_rev", &method_id);
		METHOD_add_action(method_id, METHOD_post_action_type, (METHOD_function_t)set_property_IRMF, NULL);
		return iStatus;
	}
	extern DLLAPI int set_property_IRMF(int *decision, va_list argv)
	{
		tag_t tPrimaryObject = NULLTAG, tRelationType = NULLTAG;
		tag_t* tSecondaryList=NULLTAG;
		char* cValue = NULL;
		int iCount = 0;
		GRM_relation_t* gSecondaryList;
		tPrimaryObject = va_arg(argv, tag_t);

		iCheckError(GRM_find_relation_type("IMAN_master_form_rev", &tRelationType));
		iCheckError(GRM_list_secondary_objects_only(tPrimaryObject, tRelationType, &iCount, &tSecondaryList));

		if (iCount)
		{
			for (int i = 0; i < iCount; i++)
			{
				iCheckError(AOM_refresh(tSecondaryList[i], true));
				iCheckError(AOM_set_value_string(tSecondaryList[i], "object_desc", "UPDATE"));
				iCheckError(AOM_save_without_extensions(tSecondaryList[i]));
				iCheckError(AOM_refresh(tSecondaryList[i], false));
				iCheckError(AOM_ask_value_string(tPrimaryObject, "object_name", &cValue));
			}
			return ITK_ok;
		}
		else
		{
			EMH_store_error_s1(EMH_severity_error, error4, cValue);
			return error4;
		}
	}
	extern DLLAPI int PLM_execute_callbacks2(int *decision, va_list argv)
	{
		*decision = ALL_CUSTOMIZATIONS;
		cout << "\n Logout Sucessfull \n ";
		return iStatus;
	}
}