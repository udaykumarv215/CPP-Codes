//When a dataset is attached to a item revision with some specific relation, then dataset name should be in the form of  “itemid-revid-n"( where n is the n^th dataset you are attaching)

#include"Headers.h"
#include<string>
#include<iostream>
#include<tccore/custom.h>
#include<tccore/method.h>
#include<tccore/aom.h>
#include<tccore/aom_prop.h>
#include<tccore/grm.h>
#include<fclasses/tc_string.h>

using namespace std;

#define DLLAPI _declspec(dllexport)
#define error (EMH_USER_error_base +10)

int iStatus = 0;
METHOD_id_t method_id;

extern"C"
{
	extern DLLAPI int DLL_register_callbacks();
	extern DLLAPI int PLM_execute_callbacks1(int *decision, va_list argv);
	extern DLLAPI int postaction_update_name_of_dataset(int *decision, va_list argv);

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

		iCheckError(METHOD_find_method("ImanRelation", "GRM_create", &method_id));

		iCheckError(METHOD_add_action(method_id, METHOD_post_action_type, (METHOD_function_t)postaction_update_name_of_dataset, NULL));
		return 0;
	}

	extern DLLAPI int postaction_update_name_of_dataset(int *decision, va_list argv)
	{
		tag_t tPrimaryObject = va_arg(argv, tag_t);
		tag_t tSecondaryObject = va_arg(argv, tag_t);

		int iCount = 0, n=0;
		tag_t* tSecObjects = NULLTAG;
		tag_t tClassId = NULLTAG; 
		char* cClassName = NULL;
		char* id =NULL;
		char* name = NULL;
		const char* newname1;

		iCheckError(GRM_list_secondary_objects_only(tPrimaryObject, NULLTAG, &iCount, &tSecObjects));

		for (int i = 0; i < iCount; i++)
		{
			iCheckError(POM_class_of_instance(tSecObjects[i], &tClassId));
			iCheckError(POM_name_of_class(tClassId, &cClassName));

			if (tc_strcmp("Dataset", cClassName) == 0)
			{
				n++;
			}
		}
		AOM_ask_value_string(tPrimaryObject, "item_id", &id);
		AOM_ask_value_string(tPrimaryObject, "item_revision_id", &name);

		string newname = string(id) + "-" + string(name) +"-"+ to_string(n);
		newname1 = newname.c_str();

		AOM_refresh(tSecondaryObject, true);
		iCheckError(AOM_set_value_string(tSecondaryObject, "object_string", newname1));
		iCheckError(AOM_set_value_string(tSecondaryObject, "object_name", newname1));
		AOM_save_without_extensions(tSecondaryObject);
		AOM_refresh(tSecondaryObject, false);

		return ITK_ok;
	}
}