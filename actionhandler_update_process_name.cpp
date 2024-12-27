#include"Headers.h"
#include<iostream>
#include<tc/emh.h>
#include<epm/epm.h>
#include<tccore/custom.h>
#include<tc/tc_arguments.h>
#include<tccore/aom_prop.h>
#include<tccore/workspaceobject.h>
#include<tccore/aom.h>

using namespace std;

#define DLLAPI _declspec(dllexport)
int iStatus = 0;

extern"C"
{
	extern DLLAPI int DLL_register_callbacks();
	extern DLLAPI int PLM_execute_callbacks1(int *decision, va_list argv);
	extern DLLAPI int PLM_execute_callbacks2(int *decision, va_list argv);
	extern DLLAPI int Handler_add_custom_Handler(EPM_action_message_t msg);

	extern DLLAPI int DLL_register_callbacks()
	{
		iStatus = CUSTOM_register_exit("DLL", "USER_init_module", (CUSTOM_EXIT_ftn_t)PLM_execute_callbacks1);
		iStatus = CUSTOM_register_exit("DLL", "USER_exit_module", (CUSTOM_EXIT_ftn_t)PLM_execute_callbacks2);
		return iStatus;
	}
	extern DLLAPI int PLM_execute_callbacks1(int *decision, va_list argv)
	{
		*decision = ALL_CUSTOMIZATIONS;
		cout << "\n Welcome to DLL Registration process \n";
		cout << "\n  Login Successfull \n ";

		EPM_register_action_handler("update_name_process", "This handler is created using ITK", (EPM_action_handler_t)Handler_add_custom_Handler);
		return iStatus;
	}

	extern DLLAPI int Handler_add_custom_Handler(EPM_action_message_t msg)
	{
		tag_t tItem = msg.task;
		tag_t Roottask = NULLTAG, NewStatus=NULLTAG;
		int iNum = 0, count = 0;
		tag_t* tValues = NULL;
		tag_t* attachments = NULL;
		char* cName = NULL;
		int iEffs = 0;
		tag_t* tEffectivities = NULL;
		char* cDesc = NULL;
		tag_t Enditem = NULLTAG, NewEffectivity =NULLTAG;
		char* oldrange = NULL;
		date_t set_date = NULLDATE;
		tag_t date_released_attr = NULLTAG;
		tag_t t_status = NULLTAG;
		int* state_token = 0;
		date_t dOldDate = NULLDATE;
		tag_t tAttrid = NULLTAG;

		EPM_ask_root_task(msg.task, &Roottask);
		EPM_ask_attachments(Roottask, EPM_target_attachment, &count, &attachments);
		tag_t tWSO[1] = { attachments[0] };

		iCheckError( AOM_ask_value_tags(attachments[0], "release_status_list", &iNum, &tValues));

		//ITK_string_to_date("10-Apr-2000 08:00", &set_date);

		for (int i = 0; i < iNum; i++)
		{
			AOM_ask_value_string(tValues[i], "object_name", &cName);
			if (tc_strcmp(cName, "TCM Released") == 0)
			{	
				AOM_ask_value_date(tValues[i], "date_released", &dOldDate);

				RELSTAT_create_release_status("updated TCM Release", &NewStatus);
				RELSTAT_add_release_status(NewStatus, 1, tWSO, true);

				POM_attr_id_of_attr("date_released", "ReleaseStatus", &date_released_attr);
				POM_ask_instances_states(1, &NewStatus, &state_token);

				if (*state_token != POM_inst_is_loaded_for_modify)
				{

					POM_unload_instances(1, &NewStatus);

					POM_load_instances_any_class(1, &NewStatus, POM_modify_lock);
				}

				POM_set_attr_date(1, &NewStatus, date_released_attr, dOldDate);

				POM_save_instances(1, &NewStatus, true);

				WSOM_status_ask_effectivities(tValues[i], &iEffs, &tEffectivities);
				for (int j = 0; j < iEffs; j++)
				{
					WSOM_eff_ask_unit_range(tValues[i], tEffectivities[j], &oldrange);
					WSOM_eff_ask_end_item(tValues[i], tEffectivities[j], &Enditem);
					WSOM_effectivity_create(NewStatus, Enditem, &NewEffectivity);
					WSOM_eff_set_unit_range(NewStatus, NewEffectivity, oldrange, false);
					AOM_save_without_extensions(NewStatus);
				}
			}
			POM_attr_id_of_attr("release_status_list", "WorkspaceObject", &tAttrid);
			POM_remove_from_attr(1, tWSO, tAttrid, i, 1);
			POM_load_instances_any_class(1, &NewStatus, POM_no_lock);
		}
		return 0;
	}
	extern DLLAPI int PLM_execute_callbacks2(int *decision, va_list argv)
	{
		*decision = ALL_CUSTOMIZATIONS;
		printf("\n **************** Logout Sucessfull **************\n ");
		return iStatus;
	}
}