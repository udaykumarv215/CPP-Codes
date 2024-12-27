#include <iostream>
#include <fstream>
#include<tccore/custom.h>
#include<tccore/aom_prop.h>
#include<fclasses/tc_string.h>
#include<tc/emh.h>
#include<tc/tc_startup.h>
#include<epm\epm.h>
#include<tccore\aom.h>
#include<tccore/item.h>
#include<bom/bom.h>
#include<tc/preferences.h>
#include<vector>
#include<string>

using namespace std;

#define DLLAPI __declspec(dllexport)  //Load the dll
#define tc_error (EMH_USER_error_base+14)

int ifail = ITK_ok;

void check(int ifail)
{
	char* cerror = NULL;
	if (ifail != ITK_ok)
	{
		EMH_ask_error_text(ifail, &cerror);
		std::cout << "the error is : " << cerror;
		TC_write_syslog("The error is : %s \n", cerror);
		exit(0);
	}
}

void nullcheck(tag_t tag)
{
	//int ifail = 0;
	char* cerror = NULL;
	FILE* fp = NULL;
	if (tag == NULL || tag == NULLTAG)
	{
		EMH_ask_error_text(ifail, &cerror);
		std::cout << "the error is : " << cerror;
		TC_write_syslog("The error is : %s \n", cerror);
		exit(0);
	}
}

std::ofstream MyFile("cPrefFileLoc");

extern "C"
{
	extern DLLAPI int DLL_register_callbacks();
	extern DLLAPI int functiontoinitiate(int* decision, va_list args);
	extern DLLAPI int checkreport(EPM_action_message_t* msg);
	extern DLLAPI void SubChild(tag_t tChildLines);

	int nLevel = NULL;
	char* cItemId = NULL;
	char* cChildRevId = NULL;
	char* cUomTag = NULL;
	char* cQuantity = NULL;

	fstream fout;

	extern DLLAPI int DLL_register_callbacks()
	{
		int status = ITK_ok;
		status = CUSTOM_register_exit("DLL", "USER_init_module", (CUSTOM_EXIT_ftn_t)functiontoinitiate);
		TC_write_syslog("Callback function is registered check report");

		return ifail;
	}

	extern DLLAPI int functiontoinitiate(int* decision, va_list args)
	{
		*decision = ALL_CUSTOMIZATIONS;
		cout << "Welcome to Intelizign services" << endl;
		TC_write_syslog("\n BOM report login function entered");

		check(EPM_register_action_handler("Custom-check-report", "Create BOM Report", (EPM_action_handler_t)checkreport));
		TC_write_syslog("\n BOM report login function end!!!!");
		return ifail;
	}

	extern DLLAPI int checkreport(EPM_action_message_t* msg)
	{
		TC_write_syslog("\n BOM report Action Handler function entered");
		char* cArgumentName = NULL;
		char* cArgumentValue = NULL;
		char* cIncludeType = (char*)MEM_alloc(100);
		char* cPreference = (char*)MEM_alloc(100);
		int nAttachCount = 0;
		int* nAttachType = 0;
		tag_t* tAttachments = NULL;
		tag_t tClassId = NULLTAG;
		char* cClassName = NULL;
		int nBvrcount = 0;
		tag_t* tBvrs = NULL;
		tag_t tWindow = NULLTAG;
		tag_t tTop_bom_line = NULLTAG;
		char* cPrefFileLoc = NULL;
		int nBOMChildLineCount = 0;
		tag_t tRootTask = NULLTAG;
		tag_t *tChildLines = NULL;

		int number_of_args = TC_number_of_arguments(msg->arguments);

		TC_write_syslog("\n BOM report login function entered %d", number_of_args);

		if (number_of_args == 2)
		{
			for (int i = 0; i < number_of_args; i++)
			{
				ITK_ask_argument_named_value(TC_next_argument(msg->arguments), &cArgumentName, &cArgumentValue);
				if (tc_strcmp(cArgumentName, "includetype") == 0)
				{

					tc_strcpy(cIncludeType, cArgumentValue);
					TC_write_syslog("Include Type found %s\n", cIncludeType);
				}
				if (tc_strcmp(cArgumentName, "preference") == 0)
				{
					tc_strcpy(cPreference, cArgumentValue);
					TC_write_syslog("Preference found %s\n", cPreference);
				}
			}
		}

		//getting the file location from a preference
		check(PREF_ask_char_value(cPreference, 0, &cPrefFileLoc));
		TC_write_syslog("file location found %s\n", cPrefFileLoc);

		fout.open(cPrefFileLoc);
		if (!fout.is_open())
		{
			std::cerr << "Failed to open file: " << cPrefFileLoc << std::endl;
			TC_write_syslog("Failed to open file: %s\n", cPrefFileLoc);
			EMH_store_error(EMH_severity_error, tc_error);
			return tc_error;
		}

		check(EPM_ask_root_task(msg->task, &tRootTask));
		if (tRootTask == NULLTAG)
		{
			std::cerr << "tRootTask is NULLTAG" << std::endl;
			TC_write_syslog("tRootTask is NULLTAG\n");
			EMH_store_error(EMH_severity_error, tc_error);
			return tc_error;
		}
		else
		{
			TC_write_syslog("Root task found! %s\n");
		}
		check(EPM_ask_all_attachments(tRootTask, &nAttachCount, &tAttachments, &nAttachType));
		if (tAttachments == NULL)
		{
			std::cerr << "tAttachments is NULLTAG" << std::endl;
			TC_write_syslog("tAttachments is NULLTAG\n");
			EMH_store_error(EMH_severity_error, tc_error);
			return tc_error;
		}
		else
		{
			TC_write_syslog("Attachment found! %s\n");
		}
		TC_write_syslog("No. of attachment %d\n", nAttachCount);
		if (nAttachCount != 0)
		{
			for (int i = 0; i < nAttachCount; i++)
			{
				check(POM_class_of_instance(tAttachments[i], &tClassId));

				check(POM_name_of_class(tClassId, &cClassName));

				if (tc_strcmp(cClassName, cIncludeType) == 0)
				{
					check(ITEM_rev_list_all_bom_view_revs(tAttachments[i], &nBvrcount, &tBvrs));
					if (nBvrcount != 0)
					{
						check(BOM_create_window(&tWindow));

						check(BOM_set_window_top_line_bvr(tWindow, tBvrs[i], &tTop_bom_line));
						// Check if tTop_bom_line is valid
						if (tTop_bom_line == NULLTAG)
						{
							std::cerr << "tTop_bom_line is NULLTAG" << std::endl;
							TC_write_syslog("tTop_bom_line is NULLTAG\n");
							EMH_store_error(EMH_severity_error, tc_error);
							return tc_error;
						}
						else
						{
							TC_write_syslog("Top BOM Line found! %d\n", tTop_bom_line);
						}

						check(BOM_line_ask_all_child_lines(tTop_bom_line, &nBOMChildLineCount, &tChildLines));
						// Log the count of child lines
						TC_write_syslog("Number of child lines: %d\n", nBOMChildLineCount);

						if (nBOMChildLineCount > 0 && tChildLines != NULL)
						{
							// Check if fout is in a good state before writing
							if (fout.good())
							{
								fout << "Level" << "," << "ITEM_Id" << "," << "Rev_Id" << "," << "Quantity" << "," << "UOM" << "\n";
							}
							else
							{
								std::cerr << "File stream is not in a good state" << std::endl;
								TC_write_syslog("File stream is not in a good state\n");
								exit(0);
							}

							for (int i = 0; i < nBOMChildLineCount; i++)
							{
								check(AOM_ask_value_int(tChildLines[i], "bl_level_starting_0", &nLevel));
								check(AOM_ask_value_string(tChildLines[i], "bl_item_item_id", &cItemId));
								check(AOM_ask_value_string(tChildLines[i], "bl_rev_item_revision_id", &cChildRevId));
								check(AOM_ask_value_string(tChildLines[i], "bl_quantity", &cQuantity));
								check(AOM_ask_value_string(tChildLines[i], "bl_item_uom_tag", &cUomTag));

								fout << to_string(nLevel) << ", " << cItemId << "," << cChildRevId << "," << cQuantity << "," << cUomTag << "\n";

								SubChild(tChildLines[i]);
							}
						}
						else
						{
							std::cerr << "No child lines found or tChildLines is NULL" << std::endl;
							TC_write_syslog("No child lines found or tChildLines is NULL\n");
						}
					}
					else
					{
						std::cout << "No bom revisions exists for the selected item revision";
					}
				}
			}
		}
		fout.close();
		if (fout.fail())
		{
			std::cerr << "Failed to close file: " << cPrefFileLoc << std::endl;
			TC_write_syslog("Failed to close file: %s\n", cPrefFileLoc);
			exit(0);
		}
		MEM_free(cIncludeType);
		return ifail;
	}

	extern DLLAPI void SubChild(tag_t tChildLines)
	{
		char* msg = NULL;
		tag_t* subchild1 = NULL;
		int count1 = 0;

		check(BOM_line_ask_all_child_lines(tChildLines, &count1, &subchild1));
		for (int j = 0; j < count1; j++)
		{
			check(AOM_ask_value_int(subchild1[j], "bl_level_starting_0", &nLevel));
			check(AOM_ask_value_string(subchild1[j], "bl_item_item_id", &cItemId));
			check(AOM_ask_value_string(subchild1[j], "bl_rev_item_revision_id", &cChildRevId));
			check(AOM_ask_value_string(subchild1[j], "bl_quantity", &cQuantity));
			check(AOM_ask_value_string(subchild1[j], "bl_item_uom_tag", &cUomTag));
			fout << to_string(nLevel) << ", " << cItemId << "," << cChildRevId << "," << cQuantity << "," << cUomTag << "\n";
			SubChild(subchild1[j]);

		}
	}
}
