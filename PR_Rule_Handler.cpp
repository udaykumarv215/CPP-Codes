#include<epm/epm.h>
#include<iostream>

#include<tccore/custom.h>
#include<tccore/aom_prop.h>
#include<tccore/grm.h>
#include<tccore/aom.h>
#include<tccore/item.h>
#include<ae/datasettype.h>
#include<fclasses/tc_string.h>
#include<ps/ps.h>

using namespace std;

#define DLLAPI _declspec(dllexport)
#define va_arg   __crt_va_arg
#define error (EMH_USER_error_base+3)
int iStatus = 0;
char* cerror = NULL;


void check(int ifail)
{

	if (ifail != ITK_ok)
	{
		EMH_ask_error_text(ifail, &cerror);
		cout << "the error is : " << cerror;
		exit(0);
	}
}

extern "C"
{

	extern DLLAPI int DLL_register_callbacks();
	extern DLLAPI int PLM_execute_callbacks1(int* decisison, va_list argv);
	extern DLLAPI int PLM_execute_callbacks2(int* decisison, va_list argv);
	extern DLLAPI int toCheckProblemItems(EPM_rule_message_t msg);

	extern DLLAPI int DLL_register_callbacks()
	{
		iStatus = CUSTOM_register_exit("DLL", "USER_init_module", (CUSTOM_EXIT_ftn_t)PLM_execute_callbacks1);
		iStatus = CUSTOM_register_exit("DLL", "USER_exit_module", (CUSTOM_EXIT_ftn_t)PLM_execute_callbacks2);
		return iStatus;
	}

	extern DLLAPI int PLM_execute_callbacks1(int* decision, va_list argv)
	{
		*decision = ALL_CUSTOMIZATIONS;

		EPM_register_rule_handler("ToCheckProblemItems", "To Check Problem item parents in Problem item folder", (EPM_rule_handler_t)toCheckProblemItems);
		cout << "\n Welcome to DLL Regestration process \n";
		cout << "\n  Login Sucessfull \n ";
		return iStatus;
	}

	extern DLLAPI int PLM_execute_callbacks2(int* decision, va_list argv)
	{
		*decision = ALL_CUSTOMIZATIONS;
		printf("\n **************** Logout Sucessfull **************\n ");
		return iStatus;
	}

	extern DLLAPI int toCheckProblemItems(EPM_rule_message_t msg)
	{
		int iNumberOfArg = 0, iCount = 0, iNum = 0, iRef = 0, *iLevels = 0, iCheck = 0;
		char *cValue = NULL, *cAttachmentValue = NULL;
		string cMainValue = "";
		tag_t tTask = msg.task;
		tag_t tRootTask = NULLTAG, tRelationType = NULLTAG, tClassId = NULLTAG;
		char *cClassName = NULL;
		tag_t *tSecoundaryObjects = NULL, *tErrorArray = NULL, *tAttachments = NULL, *tParents = NULL;
		logical lFound = false;

		ITKCALL(EPM_ask_root_task(tTask, &tRootTask));
		check(EPM_ask_attachments(tRootTask, EPM_target_attachment, &iCount, &tAttachments));
		if (iCount > 0)
		{
			for (int i = 0; i < iCount; i++)
			{
				iStatus = POM_class_of_instance(tAttachments[i], &tClassId);
				if (tAttachments[i] != NULLTAG)
				{
					iStatus = POM_name_of_class(tClassId, &cClassName);
					check(iStatus);
					if (tc_strcmp(cClassName, "ProblemReportRevision") == 0)
					{
						GRM_find_relation_type("CMHasProblemItem", &tRelationType);
						GRM_list_secondary_objects_only(tAttachments[i], tRelationType, &iNum, &tSecoundaryObjects);
						string attachmentValue = "";
						if (iNum > 0) {
							for (int j = 0; j < iNum; j++)
							{
								PS_where_used_all(tSecoundaryObjects[j], -1, &iRef, &iLevels, &tParents);
								for (int k = 0; k < iRef; k++)
								{
									for (int r = 0; r < iNum; r++)
									{
										lFound = false;
										if (tSecoundaryObjects[r] == tParents[k])
										{
											lFound = true;
											break;
										}
									}
									if (!lFound)
									{
										AOM_ask_value_string(tParents[k], "object_name", &cValue);
										if (!attachmentValue.empty()) {
											attachmentValue += ','; 
										}
										attachmentValue += cValue;
										iCheck++;
										cout << "Missing in Secondary Objects: " << cValue << endl;
									}
								}
							}
							if (!attachmentValue.empty()) {
								if (!cMainValue.empty()) {
									cMainValue += " ";
								}
								char* attachmentName = NULL;
								AOM_ask_value_string(tAttachments[i], "object_name", &attachmentName);
								cMainValue += attachmentName;
								cMainValue += ": " + attachmentValue;
							}
						}
					}
				}
			}
			if (iCheck != 0)
			{
				EMH_store_error_s1(EMH_severity_error, error, cMainValue.c_str());
				return error;
			}
			else
				return EPM_go;
		}
		return iStatus;
	}

}