//pre-action, when you delete any revision,before deleting attach the dataset to earlier revision.

#include"Headers.h"
#include<iostream>
#include<tccore/item.h>
#include<tccore/custom.h>
#include<tccore/aom_prop.h>
#include<ae/dataset.h>
#include<tc/emh.h>
#include<epm/epm.h>
#include<ae/datasettype.h>
#include<tccore/grm.h>

using namespace std;

#define error (EMH_USER_error_base +5)

#define DLLAPI _declspec(dllexport)
int iStatus = 0;
METHOD_id_t method_id;

extern "C"
{
	extern DLLAPI int DLL_register_callbacks();
	extern DLLAPI int PLM_execute_callbacks1(int *decisison, va_list argv);
	extern DLLAPI int PLM_execute_callbacks2(int *decisison, va_list argv);
	extern DLLAPI int attach_dataset_previous_revision(int *decisison, va_list argv);

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
		cout << "\n  Login Successfull \n ";

		METHOD_find_method("ItemRevision", "IMAN_delete", &method_id);
		METHOD_add_action(method_id, METHOD_pre_action_type, (METHOD_function_t)attach_dataset_previous_revision, NULL);
		return iStatus;
	}
	extern DLLAPI int attach_dataset_previous_revision(int *decisison, va_list argv)
	{
		tag_t tPrimaryObjectRev = NULLTAG, tPrimaryObjectItem=NULLTAG, tText = NULLTAG, tDataset=NULLTAG, tRelation_type=NULLTAG, tRelation=NULLTAG, tRev=NULLTAG;
		char* cValue = NULL;
		char* cRevision = NULL;
		int iCount = 0;
		tag_t* tRevList = NULL;
		tPrimaryObjectRev = va_arg(argv, tag_t);

		iCheckError(ITEM_ask_item_of_rev(tPrimaryObjectRev, &tPrimaryObjectItem));

		iCheckError(AOM_ask_value_string(tPrimaryObjectItem, "object_name", &cValue));
		iCheckError(AOM_ask_value_string(tPrimaryObjectRev, "current_revision_id", &cRevision));
		{
			int iCount1 = 0;
			tag_t tClassId = NULLTAG;
			tag_t* tSecondaryObjects = NULL;
			char* cClsDataset = NULL;
			iCheckError(GRM_list_secondary_objects_only(tPrimaryObjectRev, NULLTAG, &iCount1, &tSecondaryObjects));
			if (iCount1 >= 2)
			{
				for (int i = 0; i < iCount1; i++)
				{
					iCheckError(POM_class_of_instance(tSecondaryObjects[i], &tClassId));
					iCheckError(POM_name_of_class(tClassId, &cClsDataset));
					if (tc_strcmp(cClsDataset,"Dataset")==0)
					{
						(*cRevision)--; // decrementing the rev value as we have to attach the dataset to previous revision.
						char* cNewRev = cRevision;
						ITEM_find_revision(tPrimaryObjectItem, cNewRev, &tRev);
						if (tRev != NULLTAG)
						{
							iCheckError(AE_find_datasettype2("Text", &tText));
							iCheckError(GRM_find_relation_type("IMAN_requirement", &tRelation_type));
							iCheckError(GRM_create_relation(tRev, tSecondaryObjects[i], tRelation_type, NULLTAG, &tRelation));
							iCheckError(GRM_save_relation(tRelation));
						}
						else
							EMH_store_error_s1(EMH_severity_error, error, cNewRev);
							return error;
					}
				}
			}
			else
				return 0;
		}
		return 0;
	}
	extern DLLAPI int PLM_execute_callbacks2(int *decision, va_list argv)
	{
		*decision = ALL_CUSTOMIZATIONS;
		cout << "\n Logout Successfull \n ";
		return iStatus;
	}
}