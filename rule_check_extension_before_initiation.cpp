//check the extension of the attached named reference to a dataset.
//for PDF dataset extension must be .pdf etc

#include"Headers.h"
#include<iostream>
#include<tccore/custom.h>
#include<epm/epm.h>
#include<tccore/aom_prop.h>
#include<tc/preferences.h>
#include<tccore/grm.h>
#include<ae/dataset.h>

using namespace std;
#define DLLAPI _declspec(dllexport)
#define error (EMH_USER_error_base+12)
int iTemp = 0;

extern"C"
{
	extern DLLAPI int DLL_register_callbacks();
	extern DLLAPI int PLM_execute_callbacks1(int* decision, va_list argv);
	extern DLLAPI int check_property_on_BOM_childs(EPM_rule_message_t msg);

	extern DLLAPI int DLL_register_callbacks()
	{
		iCheckError(CUSTOM_register_exit("DLL", "USER_init_module", (CUSTOM_EXIT_ftn_t)PLM_execute_callbacks1));
		return ITK_ok;
	}
	extern DLLAPI int PLM_execute_callbacks1(int *decision, va_list argv)
	{
		*decision = ALL_CUSTOMIZATIONS;
		EPM_register_rule_handler("CheckPropertyBomChilds", "handler from ITK", (EPM_rule_handler_t)check_property_on_BOM_childs);
		return ITK_ok;
	}
	extern DLLAPI int check_property_on_BOM_childs(EPM_rule_message_t msg)
	{
		TC_write_syslog("Rule Handler Started");
		int status = ITK_ok;
		EPM_decision_t decision = EPM_nogo;

		tag_t root_task, *attachments;
		int count = 0;
		char* sObjectType = NULL;
		int ref_count;
		tag_t *named_refs;
		char *cName;

		status = EPM_ask_root_task(msg.task, &root_task);
		if (status != ITK_ok) {
			TC_write_syslog("Root task not found");
			return decision;
		}
		TC_write_syslog("Root Task found!!\n");

		status = EPM_ask_attachments(root_task, EPM_target_attachment, &count, &attachments);
		TC_write_syslog("Root Task found!! %d\n", count);
		if (status != ITK_ok || count == 0) {
			TC_write_syslog("Error: No attachments found\n");
			return decision;
		}
		TC_write_syslog("attachments found!!\n");

		tag_t tClassId, *secondary_objects;
		char *class_name;
		int seccount;
		int pref_count = 0;
		char **dataset_preferences = NULL;

		status = PREF_ask_char_values("Dataset_NamedReference_Pairs", &pref_count, &dataset_preferences);
		if (status != ITK_ok || pref_count == 0) {
			TC_write_syslog("Error: No dataset-named reference pairs found in preference\n");
			return decision;
		}

		status = GRM_list_secondary_objects_only(attachments[0], NULLTAG, &seccount, &secondary_objects);
		if (status != ITK_ok) {
			TC_write_syslog("Error: No secondary objects found\n");
			return decision;
		}

		for (int j = 0; j < seccount; j++) {
			status = AOM_ask_value_string(secondary_objects[j], "object_type", &sObjectType);
			if (status != ITK_ok) {
				TC_write_syslog("Error: No object_type found\n");
				continue;
			}
			TC_write_syslog("object_type found\n");

			status = POM_class_of_instance(secondary_objects[j], &tClassId);
			if (status != ITK_ok) {
				TC_write_syslog("Error: No tClassId found\n");
				continue;
			}
			TC_write_syslog("tClassId found\n");

			status = POM_name_of_class(tClassId, &class_name);
			if (status != ITK_ok) {
				TC_write_syslog("Error: No class_name found\n");
				continue;
			}
			TC_write_syslog("class_name found\n");

			if (tc_strcmp("Dataset", class_name) == 0) {
				tag_t tempTag = secondary_objects[j];

				status = AE_ask_dataset_named_refs(tempTag, &ref_count, &named_refs);
				TC_write_syslog("count of named ref %d\n", ref_count);
				if (status != ITK_ok) {
					TC_write_syslog("Error: No named_refs found\n");
					continue;
				}
				TC_write_syslog("named_refs found\n");

				status = AOM_ask_value_string(named_refs[0], "object_string", &cName);
				if (status != ITK_ok) {
					TC_write_syslog("Error: finding name of the named reference\n");
					continue;
				}
				TC_write_syslog("Name of Named Reference: %s\n", cName);

				for (int iPref = 0; iPref < pref_count; iPref++) {
					char *dataset_type = NULL;
					char *named_ref_suffix = NULL;
					TC_write_syslog("Pref Value : %s\n", dataset_preferences[iPref]);

					char preference_copy[256];
					tc_strncpy(preference_copy, dataset_preferences[iPref], sizeof(preference_copy));
					preference_copy[sizeof(preference_copy) - 1] = '\0';

					dataset_type = tc_strtok(preference_copy, ":");
					named_ref_suffix = tc_strtok(NULL, ":");

					TC_write_syslog("token of preference value %s\n", dataset_type);
					TC_write_syslog("token of preference named reference value %s\n", named_ref_suffix);

					TC_write_syslog("cName %s\n", cName);
					TC_write_syslog("sObjectType %s\n", sObjectType);
					TC_write_syslog("dataset_type %s\n", dataset_type);

					if (tc_strstr(cName, named_ref_suffix) && tc_strcmp(sObjectType, dataset_type) == 0) {
						TC_write_syslog("Decision:Pass \n");
						decision = EPM_go;
						break;
					}
					else {
						TC_write_syslog("Decision:Failed \n");
						decision = EPM_nogo;
					}
				}
			}
		}

		return decision;
	}
}