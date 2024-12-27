#include"Headers.h"
#include<iostream>
#include<tccore/custom.h>
#include<tccore/method.h>
#include<tccore/aom.h>
#include<tccore/aom_prop.h>
#include<tc/preferences.h>
#include<epm/epm.h>
#include<tccore/grm.h>
#include<ae/dataset.h>

#define DLLAPI _declspec(dllexport)
#define error (EMH_USER_error_base+12)
using namespace std;

int iStatus = 0;

extern"C"
{
	extern DLLAPI int DLL_register_callbacks();
	extern DLLAPI int PLM_execute_callbacks1(int *decision, va_list argv);
	extern DLLAPI int checkpropertyinpreference(EPM_rule_message_t msg);

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

		iCheckError(EPM_register_rule_handler("checkpropertyinpreference", "", (EPM_rule_handler_t)checkpropertyinpreference));
		return 0;
	}

	extern DLLAPI int checkpropertyinpreference(EPM_rule_message_t msg)
	{
		//EPM_decision_t rule_handler3(EPM_rule_message_t msg) {
		TC_write_syslog("Rule Handler Started");
		int status = ITK_ok;
		EPM_decision_t decision = EPM_nogo;

		tag_t root_task, *attachments, *secondary_objects;
		int count, count1;
		char *object_type;

		status = EPM_ask_root_task(msg.task, &root_task);
		if (status != ITK_ok) {
			TC_write_syslog("Root task not found");
		}
		else {
			TC_write_syslog("Root Task found!!");
		}

		EPM_ask_attachments(root_task, EPM_target_attachment, &count, &attachments);
		TC_write_syslog("Root Task found!! %d\n", count);
		if (status != ITK_ok || count == 0) {
			TC_write_syslog("Error: No attachments found\n");
			return decision;
		}
		else {
			TC_write_syslog("attachments found!!");
		}

		// Get the dataset-valid named reference pairs from the custom preference

		/*int pref_count = 0;
		char **dataset_preferences = NULL;
		status = PREF_ask_char_values("Dataset_NamedReference_Pairs", &pref_count,
			&dataset_preferences);
		if (status != ITK_ok || pref_count == 0) {
			TC_write_syslog(
				"Error: No dataset-named reference pairs found in preference\n");
			return decision;
		}*/

		// Iterate over each attachment

		for (int i = 0; i < count; i++) {

			tag_t tClassId, *secondary_objects;
			char *class_name;
			int seccount;
			char* cName = NULL;

			status = GRM_list_secondary_objects_only(attachments[i], NULLTAG, &seccount, &secondary_objects);
			for (int j = 0; j < seccount; j++)
			{
				status = POM_class_of_instance(secondary_objects[j], &tClassId);
				status = POM_name_of_class(tClassId, &class_name);
				if (tc_strcmp("Dataset", class_name) == 0)
				{
					int ref_count = 0;
					tag_t *named_refs = NULL;

					status = AE_ask_dataset_named_refs(secondary_objects[j], &ref_count, &named_refs);
					for (int k = 0; k < ref_count; k++)
					{
						AOM_ask_value_string(named_refs[k], "object_string", &cName);
						cout << cName << endl;
					}
					TC_write_syslog("All named refs\n", named_refs);
					TC_write_syslog("Fisrt named ref\n", secondary_objects[0], named_refs);

				}

			}
		}
	}
}

