//Workflow should be initiated on particular object_type , only when that object having secondary objects with particular relation, Else should throw an error. The include_type and relation name should be taken from arguments.

#include"Headers.h"
#include<iostream>
#include<epm/epm.h>
#include<tc/emh.h>
#include<tccore/custom.h>
#include<tccore/grm.h>

using namespace std;
#define DLLAPI _declspec(dllexport)
#define error (EMH_USER_error_base+12)

extern"C"
{
	extern DLLAPI int DLL_register_callbacks();
	extern DLLAPI int PLM_execute_callbacks1(int* decision, va_list argv);
	extern DLLAPI int check_secondary_objects(EPM_rule_message_t msg);

	extern DLLAPI int DLL_register_callbacks()
	{
		iCheckError(CUSTOM_register_exit("DLL", "USER_init_module", (CUSTOM_EXIT_ftn_t)PLM_execute_callbacks1));
		return ITK_ok;
	}
	extern DLLAPI int PLM_execute_callbacks1(int *decision, va_list argv)
	{
		*decision = ALL_CUSTOMIZATIONS;
		EPM_register_rule_handler("CheckSecondaryObjects", "handler from ITK", (EPM_rule_handler_t)check_secondary_objects);
		return ITK_ok;
	}
	extern DLLAPI int check_secondary_objects(EPM_rule_message_t msg)
	{
		tag_t tTask = msg.task;
		tag_t tRootTask = NULLTAG, tRelationType =NULLTAG;
		int iCount = 0, iAttachmentType = 0, iNum=0, iValue=0;
		tag_t* tAttachments = NULL;
		char* cArgument = NULL;
		char* cValue = NULL;
		tag_t* tSecondaryObjects = NULLTAG;

		int iNumOfArgs = TC_number_of_arguments(msg.arguments);
		for (int i = 0; i < iNumOfArgs; i++)
		{
			iCheckError(ITK_ask_argument_named_value(TC_next_argument(msg.arguments), &cArgument, &cValue));
			if (tc_strcmp(cArgument, "AttachmentType") == 0)
			{
				char* end;
				iValue = strtol(cValue, &end, 10);
				if (iValue == 1)
					iAttachmentType = EPM_target_attachment;
				else
					iAttachmentType = EPM_reference_attachment;
			}
			else
				iCheckError(GRM_find_relation_type(cValue, &tRelationType));
		}

		iCheckError(EPM_ask_root_task(tTask, &tRootTask));
		iCheckError(EPM_ask_attachments(tRootTask, EPM_target_attachment, &iCount, &tAttachments));
		for (int i = 0; i < iCount; i++)
		{
			iCheckError(GRM_list_secondary_objects_only(tAttachments[i], tRelationType, &iNum, &tSecondaryObjects));
			if (iNum > 0)
				EPM_go;
			else
			{
				EMH_store_error(EMH_severity_error, error);
				return error;
			}
		}
	}
}