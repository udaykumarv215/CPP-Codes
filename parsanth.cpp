#include"Headers.h"
extern "C"
{
	extern DLLAPI int ITK_TC_DLL_register_callbacks();
	extern DLLAPI int Action_execute_callback(int *decisison, va_list argv);
	extern DLLAPI int validateLOV(int *decisison, va_list argv);
	extern DLLAPI int ITK_TC_DLL_register_callbacks()
	{
		char *cErrMsg = NULL;
		int iStatus = CUSTOM_register_exit("ITK_TC_DLL", "USER_init_module", (CUSTOM_EXIT_ftn_t)Action_execute_callback);
		EMH_ask_error_text(iStatus, &cErrMsg);
		if (!iStatus)
		{
			cout << "Post action registration successful";
		}
		else
		{
			cout << "Post action registration not successful " << iStatus << endl;
			cout << cErrMsg << endl;
		}

		return iStatus;
	}
	extern DLLAPI int Action_execute_callback(int *decisison, va_list argv)
	{
		cout << "Entry into the registration grm attach of action";
		*decisison = ALL_CUSTOMIZATIONS;
		METHOD_id_t methodIdTag;
		METHOD_find_method("A2Chart", "GRM_create", &methodIdTag);
		//METHOD_add_pre_condition(methodIdTag, (METHOD_function_t)validateLOV, NULL);
		METHOD_add_action(methodIdTag, METHOD_post_action_type, (METHOD_function_t)validateLOV, NULL);
		return ITK_ok;
	}
	extern DLLAPI int validateLOV(int *decisison, va_list argv)
	{
		char *cObjectDesc = NULL;
		tag_t item = NULLTAG;
		char *cPropertyDispName = NULL;
		GRM_relation_t *relation;
		tag_t *tSecondaryObjects = NULLTAG;
		int iSecondaryCount = 0;
		tag_t tRealtionType = NULLTAG;
		//tag_t tRelationTag = NULLTAG;
		int iTest = 0;
		char *value = NULL;
		tag_t tRelationTagNew = NULLTAG;
		//tag_t tuserData = NULLTAG;
		//tag_t tRelationTag = NULLTAG;
		char *cNewValue = NULL;
		char *ctype_string = NULL;
		char *cobject_type = NULL;
		int iResult = ITK_ok;
		char *text = NULL;
		tag_t  primary_object = va_arg(argv, tag_t);
		tag_t  secondary_object = va_arg(argv, tag_t);
		tag_t  relation_type = va_arg(argv, tag_t);
		tag_t  tuserData = va_arg(argv, tag_t);
		tag_t*  tRelationTag = va_arg(argv, tag_t*);
		if (tRelationTag != NULL)
		{
			cout << "Entry into the validateLOV" << endl;
			//GRM_create_relation_type("A2Chart",&tRealtionType);
			GRM_list_secondary_objects_only(primary_object, relation_type, &iSecondaryCount, &tSecondaryObjects);

			//GRM_find_relation(primary_object, tSecondaryObjects[iSecondaryCount-1], tRealtionType, &tRelationTagNew);//New secondary object relation tag
			AOM_ask_value_string(tRelationTag[0], "a2Criticality", &cNewValue);//New secondary object criticality value
			cout << "THE VALUE OF THE NEW SEC OBJECT RELATION IS[" << cNewValue << endl;
			TC_write_syslog("THE VALUE OF THE NEW SEC OBJECT RELATION IS %S", cNewValue);

			cout << "iSecondaryCount [" << iSecondaryCount << "]" << endl;
			for (int i = 0; i < iSecondaryCount - 1; i++)
			{
				GRM_find_relation(primary_object, tSecondaryObjects[i], relation_type, &tRelationTagNew);
				AOM_ask_value_string(tSecondaryObjects[i], "object_type", &cobject_type);
				cout << "object_type" << cobject_type << endl;
				if (tRelationTag != NULLTAG)
				{
					AOM_ask_value_string(tRelationTagNew, "type_string", &ctype_string);
					cout << "ctype_string" << ctype_string << endl;
					AOM_ask_value_string(tRelationTagNew, "a2Criticality", &value);
					cout << "THE VALUE OF THE SEC OBJECT RELATION IS[" << value << endl;

					if (tc_strcmp(value, cNewValue) == 0)
					{
						cout << "Same value found" << endl;
						EMH_store_error(EMH_severity_error, Error7);
						//EMH_ask_error_text(Error7, &text);
						//cout << text << endl;
						return Error7;
						//break;
					}
				}

			}
		}
		return ITK_ok;
	}

}