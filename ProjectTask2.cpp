// Working on project related task on ITK, To add 2 custom business objects on creation of one custom business object and add a custom status and attaching naming rules for all.

#include"Headers.h"
#include<iostream>
#include<tccore/custom.h>
#include<tccore/tctype.h>
#include<tccore/grm.h>
#include<tccore/item.h>
#include<tccore/aom.h>

using namespace std;

#define DLLAPI _declspec(dllexport)
int iStatus = ITK_ok;
METHOD_id_t method_id;

extern"C"
{
	extern DLLAPI int DLL_register_callbacks();
	extern DLLAPI int PLM_execute_callbacks1(int *decision, va_list argv);
	extern DLLAPI int PLM_execute_callbacks2(int *decisison, va_list argv);
	extern DLLAPI int add_macro_and_constraint(int *decision, va_list argv);
	extern DLLAPI int create_objects(const char* type);
	extern DLLAPI int create_relation(tag_t ParentRev, tag_t ChildRev);

	extern DLLAPI int DLL_register_callbacks()
	{
		cout << "\n  Entered DLL_register_callbacks \n ";
		iStatus = CUSTOM_register_exit("DLL", "USER_init_module", (CUSTOM_EXIT_ftn_t)PLM_execute_callbacks1);
		iStatus = CUSTOM_register_exit("DLL", "USER_exit_module", (CUSTOM_EXIT_ftn_t)PLM_execute_callbacks2);
		cout << "\n  Exit DLL_register_callbacks \n ";
		return iStatus;
	}

	extern DLLAPI int PLM_execute_callbacks1(int *decision, va_list argv)
	{
		cout << "\n  Entered PLM_execute_callbacks1 \n ";
		*decision = ALL_CUSTOMIZATIONS;
		cout << "\n Welcome to DLL Registration process \n";
		cout << "\n  Login Successfull \n ";

		iStatus =  METHOD_find_method("A2ParameterRevision", "ITEM_create_rev", &method_id);

		iStatus = METHOD_add_action(method_id, METHOD_post_action_type, (METHOD_function_t)add_macro_and_constraint, NULL);
		cout << "\n  Exit PLM_execute_callbacks1 \n ";
		return iStatus;
	}
	extern DLLAPI int add_macro_and_constraint(int *decision, va_list argv)
	{
		cout << "\n  Entered add_macro_and_constraint \n ";
		tag_t tParam= va_arg(argv, tag_t);
		const char* rev_id = va_arg(argv, const char*);
		tag_t* new_rev = va_arg(argv, tag_t*);
		tag_t tObjRev1 = NULLTAG, tObjRev2 = NULLTAG, tRev = NULLTAG;

		iStatus = ITEM_ask_latest_rev(tParam, &tRev);
		
		tObjRev1 = create_objects("A2Macro");
		tObjRev2 = create_objects("A2Constraint");
		
		iStatus = create_relation(tRev, tObjRev1);
		iStatus = create_relation(tRev, tObjRev2);
		cout << "\n  Exit add_macro_and_constraint \n ";
		
		return iStatus;
	}
	extern DLLAPI int create_objects(const char* type)
	{
		tag_t tType = NULLTAG, tCreateIput = NULLTAG, tObj = NULLTAG, tObjRev = NULLTAG;
		iStatus = TCTYPE_ask_type(type, &tType);
		iStatus = TCTYPE_construct_create_input(tType, &tCreateIput);
		iStatus = TCTYPE_create_object(tCreateIput, &tObj);
		iStatus = AOM_save_with_extensions(tObj);
		iStatus = ITEM_ask_latest_rev(tObj, &tObjRev);
		return tObjRev;
	}
	extern DLLAPI int create_relation(tag_t ParentRev, tag_t ChildRev)
	{
		tag_t  tRelationType = NULLTAG, tRelation=NULLTAG;
		iStatus = (GRM_find_relation_type("IMAN_specification", &tRelationType));
		iStatus = (GRM_create_relation(ParentRev, ChildRev, tRelationType, NULLTAG, &tRelation));
		iStatus = (AOM_save_with_extensions(tRelation));
		return iStatus;
	}

	extern DLLAPI int PLM_execute_callbacks2(int *decision, va_list argv)
	{
		cout << "\n  Entered PLM_execute_callbacks2 \n ";
		*decision = ALL_CUSTOMIZATIONS;
		cout << "\n Logout Sucessfull \n ";
		cout << "\n  Exit PLM_execute_callbacks2 \n ";
		return iStatus;
	}
}