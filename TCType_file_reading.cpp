#include"Headers.h"
#include<iostream>
#include<stdio.h>
#include<fstream>
#include<string>
#include<tc/tc_startup.h>
#include<tcinit/tcinit.h>
#include<tccore/tctype.h>
#include<tccore/aom_prop.h>
#include<tccore/aom.h>

using namespace std;

int iTctype_fileread() 
{
	char* cMessage = nullptr;
	string sLine;
	tag_t tItemTypeTag=NULLTAG, tCreateInputTag = NULLTAG, tItem = NULLTAG;

	char* cFile_name = ITK_ask_cli_argument("-file=");


	if (cFile_name != nullptr) 
	{
		ifstream fin(cFile_name); //Creates an input file stream object named fin to read from the specified file.

		if (fin.is_open()) {
			cout << "Processing file: " << cFile_name << endl;

			while (getline(fin, sLine)) { // gets each line from the file and stores them in sLine
				// Expecting format: item_id|item_name
				size_t pos = sLine.find('|');
				if (pos != string::npos) // If pos is not equal to string::npos, it means the | character was found.
				{
					string sItem_id = sLine.substr(0, pos); // Extracts the substring from the start of the line up to (but not including) the | character, giving you the item_id.
					string sItem_name = sLine.substr(pos + 1); // Extracts the substring from just after the | to the end of the line, giving you the item_name.

					const char* sItem_id1 = sItem_id.c_str(); // converting string to const char.
					const char* sItem_name1 = sItem_name.c_str();

					iCheckError(TCTYPE_ask_type("Item", &tItemTypeTag));//to get the object type

					iCheckError(TCTYPE_construct_create_input(tItemTypeTag, &tCreateInputTag));//to create input of object type 

					iCheckError(AOM_set_value_string(tCreateInputTag, "item_id", sItem_id1));

					iCheckError(AOM_set_value_string(tCreateInputTag, "object_name", sItem_name1));

					iCheckError(TCTYPE_create_object(tCreateInputTag, &tItem));

					iCheckError(AOM_save_with_extensions(tItem));

				}
				else {
					cout << "Malformed line: " << sLine << endl;
				}
			}

			fin.close();
			cout << "File processed successfully.\n";
		}
		else {
			cout << "Failed to open file.\n";
		}
	}
	else {
		cout << "No file specified.\n";
	}
	return 0;
}