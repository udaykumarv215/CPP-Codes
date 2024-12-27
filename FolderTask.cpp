#include "Headers.h"
#include <direct.h>
#include <string>
#include<sa/tcfile.h>
#include<pie/pie.h>
#include <cstdlib>
#include <filesystem>
#include <experimental/filesystem>
#include<bom/bom.h>
#include<tccore/aom_prop.h>
#include<tccore/grm.h>

namespace fs = std::experimental::filesystem;

void zipFolder(const std::string& folderPath, const std::string& zipFileName)
{
	std::string sevenZipPath = "\"C:\\Program Files\\7-Zip\\7z.exe\"";  // Full path to 7z.exe

	// Construct the 7z command to zip the folder
	std::string command = sevenZipPath + " a " + zipFileName + " " + folderPath + "\\*";

	// Call the system function to execute the command
	int result = system(command.c_str());

	if (result == 0) {
		std::cout << "Folder successfully zipped into: " << zipFileName << std::endl;
	}
	else {
		std::cerr << "Failed to zip the folder!" << std::endl;
	}
}

void askChildLines(tag_t subchilds, int level = 1) {
	tag_t* tChildLines = NULLTAG;
	tag_t tRelationType = NULLTAG;
	tag_t* tSecCount = NULLTAG;
	tag_t tItem = NULLTAG;
	tag_t tRev = NULLTAG;
	int iCountOfChildLines;
	int iSecCount = 0;
	int iCountOfInnerChildLines;

	BOM_line_ask_child_lines(subchilds, &iCountOfChildLines, &tChildLines);
	for (int i = 0; i < iCountOfChildLines; i++) 
	{
		char* objectString = NULL;
		AOM_ask_value_string(tChildLines[i], "bl_item_object_string", &objectString);
		for (int j = 0; j < level; j++) 
		{
			cout << "   ";
		}
		cout << "- " << objectString << endl;

		AOM_ask_value_tag(tChildLines[i], "bl_item", &tItem);
		ITEM_ask_latest_rev(tItem, &tRev);
		cout << tRev << endl;
		(GRM_find_relation_type("IMAN_specification", &tRelationType), "Found Relation Type");
		(GRM_list_secondary_objects_only(tRev, tRelationType, &iSecCount, &tSecCount), "Listing secondary objects of childs");
		cout << iSecCount << endl;
	}
}

int ITK_user_main(int charc, char* argv[]) {
	int iStatus = ITK_ok;
	char* cUser_name = NULL;
	char* cPassword = NULL;
	char* cUser_group = NULL;
	char *cError = NULL;
	tag_t tParent_Rev = NULLTAG;
	tag_t top_bom_line = NULLTAG;
	tag_t tWindow = NULLTAG;
	tag_t tPItem = NULLTAG;
	int iCountOfChildLines;
	tag_t* tChildLines = NULLTAG;
	int i;
	char* value = NULL;
	char* cItem_Id = NULL;
	char* cRevId = NULL;
	char* Pvalue = NULL;



	int iSecCount = 0;

	cUser_name = ITK_ask_cli_argument("-u=");
	cPassword = ITK_ask_cli_argument("-p=");
	cUser_group = ITK_ask_cli_argument("-g=");
	cItem_Id = ITK_ask_cli_argument("-i=");
	cRevId = ITK_ask_cli_argument("-rev=");
	iStatus = ITK_init_module(cUser_name, cPassword, cUser_group);

	if (iStatus == 0) {
		cout << "\nLogin Success" << endl;
	}
	else {
		EMH_ask_error_text(iStatus, &cError);
		printf("\n The error is: %s\n", cError);
	}
	//CREATING BOM WINDOW AND SETTING THE TOP LINE
	iFail1(BOM_create_window(&tWindow), "window created");
	iFail1(ITEM_find_item(cItem_Id, &tPItem), "Found Parent Item");
	iFail1(ITEM_find_rev(cItem_Id, cRevId, &tParent_Rev), "Found Parent ItemRevision");
	iFail1(BOM_set_window_top_line(tWindow, tPItem, tParent_Rev, NULLTAG, &top_bom_line), "top bom line is set successfully");
	AOM_ask_value_string(top_bom_line, "bl_item_object_string", &Pvalue);
	cout << Pvalue << endl;
	std::string path = "D:";
	std::string fullPath = path + "\\" + Pvalue;

	if (_mkdir(fullPath.c_str()) == 0) {
		std::cout << "Folder created successfully!" << std::endl;
	}
	else {
		std::perror("Error creating folder");
	}

	MEM_free(Pvalue);

	BOM_line_ask_child_lines(top_bom_line, &iCountOfChildLines, &tChildLines);
	cout << iCountOfChildLines << endl;

	for (i = 0; i < iCountOfChildLines; i++) {
		tag_t tRelationType = NULLTAG;
		tag_t* tSecCount = NULLTAG;
		tag_t tItem = NULLTAG;
		tag_t tSession = NULLTAG;
		tag_t* tTMode = NULLTAG;
		tag_t tRev = NULLTAG;
		int iNum = 0;
		int iNtm = 0;
		char* DatasetName = NULL;
		tag_t* tValues = NULL;
		AOM_ask_value_string(tChildLines[i], "bl_item_object_string", &value);
		cout << "-" << value << endl;
		AOM_ask_value_tag(tChildLines[i], "bl_item", &tItem);
		ITEM_ask_latest_rev(tItem, &tRev);
		cout << tRev << endl;
		iFail1(GRM_find_relation_type("IMAN_specification", &tRelationType), "Found Relation Type");
		iFail1(GRM_list_secondary_objects_only(tRev, tRelationType, &iSecCount, &tSecCount), "Listing secondary objects of childs");
		cout << iSecCount << endl;

		std::string path1 = fullPath;
		std::string fullPath1 = path1 + "\\" + value;

		// Create SubFolder1 inside ParentStruct
		if (_mkdir(fullPath1.c_str()) == 0) {
			std::cout << "SubFolder1 created successfully!" << std::endl;
			for (int j = 0; j < iSecCount; j++) {
				AOM_ask_value_tags(tSecCount[j], "ref_list", &iNum, &tValues);
				cout << iNum << endl;
				for (int k = 0; k < iNum; k++) {

					AOM_ask_value_string(tValues[k], "object_string", &DatasetName);
					std::string path2 = fullPath1;
					std::string fullPath2 = path2 + "\\" + DatasetName;
					iFail1(IMF_export_file(tValues[k], fullPath2.c_str()), "Exporting..");

				}
			}

		}
		else {
			std::perror("Error creating SubFolder1");
		}

		//askChildLines(tChildLines[i]);
	}

	std::string zipFileName = fullPath + ".zip";

	zipFolder(fullPath, zipFileName);

	try {
		//Check if the folder exists and then remove it
		if (fs::exists(fullPath) && fs::is_directory(fullPath)) {
			fs::remove_all(fullPath); // This will delete the parent folder and all its contents
			std::cout << "Parent folder removed successfully!" << std::endl;
		}
		else {
			std::cout << "The parent folder doesn't exist or is not a directory." << std::endl;
		}
	}
	catch (const fs::filesystem_error& e) {
		std::cerr << "Error removing parent folder: " << e.what() << std::endl;
	}
	MEM_free(value);
	MEM_free(tChildLines);
	return ITK_ok;
}