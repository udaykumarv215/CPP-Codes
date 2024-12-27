// Working on project related task on ITK, To add 2 custom business objects on creation of one custom business object and add a custom status and attaching naming rules for all.

#include"Headers.h"
#include<iostream>
#include<tccore/custom.h>
#include<tccore/tctype.h>
#include<tccore/grm.h>
#include<tccore/item.h>
#include<tccore/aom.h>
#include<tccore/aom_prop.h>
#include<epm/epm.h>
#include<epm/epm_toolkit_tc_utils.h>
#include<epm/epm_task_template_itk.h>

using namespace std;

#define DLLAPI _declspec(dllexport)
#define error (EMH_USER_error_base +14)
int iStatus = ITK_ok;
int iCount = 0;
METHOD_id_t method_id;
METHOD_id_t method_id1;
METHOD_id_t method_id2;

extern"C"
{
	extern DLLAPI int DLL_register_callbacks();
	extern DLLAPI int PLM_execute_callbacks1(int *decision, va_list argv);
	extern DLLAPI int PLM_execute_callbacks2(int *decisison, va_list argv);
	extern DLLAPI int iAddMacroAndConstraint(int *decision, va_list argv);
	extern DLLAPI int iReviseMacroAndParam(int *decision, va_list argv);
	extern DLLAPI int iReviseConstraintAndParam(int *decision, va_list argv);
	extern DLLAPI int iCreateObjects(const char* type, tag_t ParentRev);
	extern DLLAPI int iReviseOperation(tag_t tSourceRev, tag_t tNew_rev);
	extern DLLAPI int iActionHndlr(EPM_action_message_t msg);
	extern DLLAPI int iCalculateSpeed(int iDistance, int iTime);
	extern DLLAPI int iCalculateForce(int iMass, int iAcc);
	extern DLLAPI int iCalculateDistance(int iSpeed, int iTime);

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
		cout << "\n Entered PLM_execute_callbacks1 \n ";
		*decision = ALL_CUSTOMIZATIONS;
		cout << "\n Welcome to DLL Registration process \n";
		cout << "\n Login Successfull \n ";

		iStatus = METHOD_find_method("A2ParameterRevision", "ITEM_create_rev", &method_id);
		iStatus = METHOD_find_method("A2MacroRevision", "ITEM_copy_rev", &method_id1);
		iStatus = METHOD_find_method("A2ConstraintRevision", "ITEM_copy_rev", &method_id2);

		iStatus = METHOD_add_action(method_id, METHOD_post_action_type, (METHOD_function_t)iAddMacroAndConstraint, NULL);
		iStatus = METHOD_add_action(method_id1, METHOD_post_action_type, (METHOD_function_t)iReviseMacroAndParam, NULL);
		iStatus = METHOD_add_action(method_id2, METHOD_post_action_type, (METHOD_function_t)iReviseConstraintAndParam, NULL);

		iStatus = EPM_register_action_handler("Parameter Value Execution", "This handler is created using ITK", (EPM_action_handler_t)iActionHndlr);

		cout << "\n Exit PLM_execute_callbacks1 \n ";
		return iStatus;
	}

	extern DLLAPI int iAddMacroAndConstraint(int *decision, va_list argv)
	{
		if (iCount == 0)
		{
			cout << "\n Entered add_macro_and_constraint \n ";
			tag_t tParam = va_arg(argv, tag_t);
			const char* rev_id = va_arg(argv, const char*);
			tag_t* new_rev = va_arg(argv, tag_t*);
			tag_t tObjRev1 = NULLTAG, tObjRev2 = NULLTAG, tRev = NULLTAG, wfProcess = NULLTAG, attachments[1], newProcess = NULLTAG;
			int attachment_types[1];
			logical lBool = true;

			(AOM_ask_value_logical(tParam, "a2MacroAndConstraint", &lBool));
			if (lBool)
			{
				ITKCALL(ITEM_ask_latest_rev(tParam, &tRev));

				iStatus = iCreateObjects("A2Macro", tRev);
				iStatus = iCreateObjects("A2Constraint", tRev);

				attachments[0] = tRev;
				attachment_types[0] = EPM_target_attachment;
				ITKCALL(EPM_find_process_template("In Work release status", &wfProcess));
				ITKCALL(EPM_create_process("In Work release status", "In Work release status", wfProcess, 1, attachments, attachment_types, &newProcess));//initiating the workflow
				AOM_save_with_extensions(newProcess);
				ITK_set_bypass(true);

				iCount++;
				return iStatus;
			}
			else
			{
				ITKCALL(ITEM_ask_latest_rev(tParam, &tRev));
				attachments[0] = tRev;
				attachment_types[0] = EPM_target_attachment;

				ITKCALL(EPM_find_process_template("In Work release status", &wfProcess));
				ITKCALL(EPM_create_process("In Work release status", "In Work release status", wfProcess, 1, attachments, attachment_types, &newProcess));//initiating the workflow
				AOM_save_with_extensions(newProcess);

				iCount++;
				return ITK_ok;
			}
		}
		else
			iCount = 0;
			return iStatus;
	}

	extern DLLAPI int iCreateObjects(const char* type, tag_t ParentRev)
	{
		tag_t tType = NULLTAG, tCreateIput = NULLTAG, tObj = NULLTAG, tObjRev = NULLTAG;
		tag_t  tRelationType = NULLTAG, tRelation = NULLTAG;
		ITKCALL(TCTYPE_ask_type(type, &tType));
		ITKCALL(TCTYPE_construct_create_input(tType, &tCreateIput));
		ITKCALL(TCTYPE_create_object(tCreateIput, &tObj));
		iStatus = AOM_save_with_extensions(tObj);
		ITKCALL(ITEM_ask_latest_rev(tObj, &tObjRev));

		ITKCALL(GRM_find_relation_type("IMAN_specification", &tRelationType));
		ITKCALL(GRM_create_relation(ParentRev, tObjRev, tRelationType, NULLTAG, &tRelation));
		iStatus = (AOM_save_with_extensions(tRelation));
		return iStatus;
	}

	extern DLLAPI int iReviseMacroAndParam(int *decision, va_list argv)
	{
		tag_t source_rev = va_arg(argv, tag_t);
		const char* rev_id = va_arg(argv, const char*);
		tag_t* new_rev = va_arg(argv, tag_t*);

		iStatus = iReviseOperation(source_rev, *new_rev);
		return iStatus;
	}

	extern DLLAPI int iReviseConstraintAndParam(int *decision, va_list argv)
	{
		tag_t source_rev = va_arg(argv, tag_t);
		const char* rev_id = va_arg(argv, const char*);
		tag_t* new_rev = va_arg(argv, tag_t*);

		iStatus = iReviseOperation(source_rev, *new_rev);
		return iStatus;
	}

	extern DLLAPI int iReviseOperation(tag_t tSourceRev, tag_t tNew_rev)
	{
		int iNum = 0;
		tag_t tNewParamRev = NULLTAG, Status=NULLTAG;
		tag_t* tPrimaryObjects = NULLTAG;
		char* cValue = NULL;

		ITKCALL(GRM_list_primary_objects_only(tSourceRev, NULLTAG, &iNum, &tPrimaryObjects));

		for (int i = 0; i < iNum; i++)
		{
			ITKCALL(AOM_ask_value_string(tPrimaryObjects[i], "object_type", &cValue));
			if (tc_strcmp(cValue, "A2ParameterRevision") == 0)
			{
				ITKCALL(ITEM_copy_rev(tPrimaryObjects[i], NULL, &tNewParamRev));
				tag_t attachments[2] = { tNewParamRev,tNew_rev };

				int iSize = sizeof(attachments) / sizeof(attachments[0]);
				ITKCALL (RELSTAT_create_release_status("A2INWORK", &Status));
				ITKCALL (RELSTAT_add_release_status(Status, iSize, attachments, false));

				SAFE_SM_FREE(tPrimaryObjects);
				return ITK_ok;
			}
		}
	}

	extern DLLAPI int iActionHndlr(EPM_action_message_t msg)
	{
		tag_t tTask = msg.task;
		tag_t tRootTask = NULLTAG, tLatestRev = NULLTAG, tParamRevision=NULLTAG, tParamItem=NULL, tTag =NULLTAG;
		int iCount = 0, iNum = 0;
		tag_t* tAttachments = NULLTAG;
		char* tType = NULL;
		char** cValues = NULL;
		logical lLogical = NULL;
		char *cName = NULL, *cValue = NULL, *cLOVValue = NULL, *cDistance = NULL, *cTime = NULL, *cSpeed = NULL, *cMass = NULL, *cAcc = NULL, *cType = NULL;
		int  iDistanceValue = NULL, iTimeValue = NULL,iSpeedValue = NULL, iMassValue = NULL, iAccValue = NULL, iMainValue = NULL, iAcc=NULL, iDis = NULL, iMass = NULL, iSpe = NULL, iTime = NULL;

		ITKCALL(EPM_ask_root_task(tTask, &tRootTask));
		ITKCALL(EPM_ask_attachments(tRootTask, EPM_target_attachment, &iCount, &tAttachments));
		if (iCount > 0)
		{
			for (int i = 0; i < iCount; i++)
			{
				ITKCALL(AOM_ask_value_string(tAttachments[i], "object_type", &tType));

				if (tc_strcmp(tType, "A2ParameterRevision") == 0)
					tParamRevision = tAttachments[i];

				if (tc_strcmp(tType, "A2MacroRevision") == 0)
				{
					ITKCALL(AOM_ask_value_logical(tAttachments[i], "a2ReferencedParameters", &lLogical));
					if (lLogical)
					{
						ITKCALL(AOM_ask_value_tag(tAttachments[i], "a2ReferenceParameters", &tTag));
						ITKCALL(AOM_ask_value_int(tTag, "a2Accelaration", &iAcc));
						ITKCALL(AOM_ask_value_int(tTag, "a2Distance", &iDis));
						ITKCALL(AOM_ask_value_int(tTag, "a2Mass", &iMass));
						ITKCALL(AOM_ask_value_int(tTag, "a2Speed", &iSpe));
						ITKCALL(AOM_ask_value_int(tTag, "a2Time", &iTime));
						ITKCALL(AOM_ask_value_string(tAttachments[i], "a2SelectLOV", &cLOVValue));

						if (tc_strcmp((&*cLOVValue), "Speed") == 0)
							iMainValue = iCalculateSpeed(iDis, iTime);

						if (tc_strcmp((&*cLOVValue), "Force") == 0)
							iMainValue = iCalculateForce(iMass, iAcc);

						if (tc_strcmp((&*cLOVValue), "Distance") == 0)
							iMainValue = iCalculateDistance(iSpe, iTime);
					}
					else
					{
						ITKCALL(AOM_ask_displayable_values(tAttachments[i], "a2ConstantParameters", &iNum, &cValues));
						for (int j = 0; j < iNum; j++)
						{
							cName = tc_strtok(cValues[j], ":");
							cValue = tc_strtok(NULL, ":");

							int iValue = atoi(cValue);
							if (tc_strcmp("Distance ", cName) == 0)
							{
								cDistance = cName;
								iDistanceValue = iValue;
								continue;
							}
							if (tc_strcmp("Time ", cName) == 0)
							{
								cTime = cName;
								iTimeValue = iValue;
								continue;
							}
							if (tc_strcmp("Speed ", cName) == 0)
							{
								cSpeed = cName;
								iSpeedValue = iValue;
								continue;
							}
							if (tc_strcmp("Mass ", cName) == 0)
							{
								cMass = cName;
								iMassValue = iValue;
								continue;
							}
							if (tc_strcmp("Accelaration ", cName) == 0)
							{
								cAcc = cName;
								iAccValue = iValue;
								continue;
							}
						}
						ITKCALL(AOM_ask_value_string(tAttachments[i], "a2SelectLOV", &cLOVValue));
						if (tc_strcmp((&*cLOVValue), "Speed") == 0)
							iMainValue = iCalculateSpeed(iDistanceValue, iTimeValue);

						if (tc_strcmp((&*cLOVValue), "Force") == 0)
							iMainValue = iCalculateForce(iMassValue, iAccValue);

						if (tc_strcmp((&*cLOVValue), "Distance") == 0)
							iMainValue = iCalculateDistance(iSpeedValue, iTimeValue);
					}
				}
			}
			ITKCALL(ITEM_ask_item_of_rev(tParamRevision, &tParamItem));
			ITKCALL(AOM_refresh(tParamRevision, true));
			ITKCALL(AOM_set_value_int(tParamRevision, "a2Value", iMainValue));
			ITKCALL(AOM_save_without_extensions(tParamRevision));
			ITKCALL(AOM_refresh(tParamRevision, false));
		}
		return 0;
	}

	extern DLLAPI int iCalculateSpeed(int iDistance, int iTime)
	{
		if (iDistance != 0 && iTime != 0)
			return iDistance / iTime;
		else
		{
			EMH_store_error_s1(EMH_severity_error, error, "Speed");
			return error;
		}
	}
	extern DLLAPI int iCalculateForce(int iMass, int iAcc)
	{
		if (iMass != 0 && iAcc != 0)
			return iMass * iAcc;
		else
		{
			EMH_store_error_s1(EMH_severity_error, error, "Mass");
			return error;
		}
	}
	extern DLLAPI int iCalculateDistance(int iSpeed, int iTime)
	{
		if (iSpeed != 0 && iTime != 0)
			return iSpeed * iTime;
		else
		{
			EMH_store_error_s1(EMH_severity_error, error, "Distance");
			return error;
		}
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