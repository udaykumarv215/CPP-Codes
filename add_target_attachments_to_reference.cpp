// add all the attachments from target folder to reference folder

#include"Headers.h"
#include<iostream>
#include<tccore/aom_prop.h>
#include<tccore/custom.h>
#include<tc/emh.h>
#include<epm/epm.h>
#include<tc/tc_arguments.h>
#include<tccore/grm.h>
#include<vector>

using namespace std;

#define DLLAPI _declspec(dllexport)
int iStatus = 0;

extern"C"
{
	extern DLLAPI int DLL_register_callbacks();
	extern DLLAPI int PLM_execute_callbacks1(int *decision, va_list argv);
	extern DLLAPI int action_handler2(EPM_action_message_t msg);

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

		EPM_register_action_handler("ITKhandlermultipleargs", "This handler is created using ITK", (EPM_action_handler_t)action_handler2);
		return iStatus;
	}

	extern DLLAPI int action_handler2(EPM_action_message_t msg)
	{
		int ifail = ITK_ok;
		TC_write_syslog("Action Handler Started");

		tag_t root_task, *attachments, *primary_list;
		int count, count1;

		ifail = EPM_ask_root_task(msg.task, &root_task);
		if (ifail != ITK_ok)
		{
			TC_write_syslog("Root task not found");
		}
		else
		{
			TC_write_syslog("Root Task found!!");
		}
		TC_write_syslog("EPM_ask_attachments start\n");
		ifail = EPM_ask_attachments(root_task, EPM_target_attachment, &count, &attachments);
		TC_write_syslog("EPM_ask_attachments ended\n");
		if (ifail != ITK_ok)
		{
			TC_write_syslog("attachments not found\n");
		}
		else
		{
			TC_write_syslog("attachments found!!\n");
		}
		TC_write_syslog("number of attachment\n", count);
		for (int i = 0; i < count; i++)
		{
			TC_write_syslog("GRM_list_primary_objects started\n");
			ifail = GRM_list_primary_objects_only(attachments[i], NULLTAG, &count1, &primary_list);
			TC_write_syslog("GRM_list_primary_objects ended\n");
			if (ifail != ITK_ok)
			{
				TC_write_syslog("primary_list not found\n");
			}
			else
			{
				TC_write_syslog("primary_list found!!\n");
			}
			TC_write_syslog("primary_list number\n", count1);

			//const tag_t* constTargetObjects = static_cast<const tag_t*>(attachments);

			//const int* constAttachmentTypes = EPM_reference_attachment;
		   //const tag_t* attachment1 = attachments[0];
			if (count1 > 0) {
				for (int j = 0; j < count1; j++)
				{
					TC_write_syslog("EPM_add_attachments start\n");
					ifail = EPM_add_attachments(root_task, count1, &primary_list[j], std::vector<int>(count1, EPM_reference_attachment).data());
					TC_write_syslog("EPM_add_attachments ended\n");
					if (ifail != ITK_ok)
					{
						TC_write_syslog("attachments not added\n");
					}
					else
					{
						TC_write_syslog("attachments added!!\n");
					}

				}
				
				

			}

		}
		
		TC_write_syslog("EPM_remove_attachments_from_att_type start\n");
		ifail = EPM_remove_attachments_from_att_type(root_task, count1, attachments, std::vector<int>(count1, EPM_target_attachment).data());
		TC_write_syslog("EPM_remove_attachments_from_att_type ended\n");
		if (ifail != ITK_ok)
		{
			TC_write_syslog("attachments not removed\n", msg);
		}
		else
		{
			TC_write_syslog("attachments removed!!\n");
		}
		return ifail;
	}
}