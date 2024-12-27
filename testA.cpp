#include<iostream>
#include<stdio.h>
#include<tc/tc_startup.h>
#include<tcinit/tcinit.h>
#include<tccore/item.h>
#include <tccore/workspaceobject.h>
#include <tccore/aom_prop.h>
#include <fclasses/tc_string.h>
#include <iostream>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <epm/epm.h>

using namespace std;

#define WSO_where_ref_any_depth   -1;

int testA() 
{
	int ifail = ITK_ok;

	int* ilevels = NULLTAG;
	int iref = NULLTAG;

	tag_t * tn_ref = NULLTAG;
	tag_t titem = NULLTAG;
	char** crelations = NULL;
	int iReleased = 0;

	tag_t tClassId = NULLTAG;
	tag_t tSClassId = NULLTAG;
	char* cClassName = NULL;
	//	char* cClass = "ChabgeItem";
	logical answer;

	// Obtain command-line arguments
	char* user_id = ITK_ask_cli_argument("-u=");
	char* password = ITK_ask_cli_argument("-p=");
	char* group = ITK_ask_cli_argument("-g=");

	ifail = ITK_init_module(user_id, password, group);

	if (ifail == ITK_ok) {
		cout << "Login successful\n";
		char* citem = ITK_ask_cli_argument("-i=");
		ifail = ITEM_find_item(citem, &titem);
		if (ifail == ITK_ok) {
			ifail = WSOM_where_referenced2(titem, -1, &iref, &ilevels, &tn_ref, &crelations);
			if (ifail == ITK_ok) {
				cout << "The total number of referencers found: " << iref << endl;

				// Print the references
				for (int i = 0; i < iref; ++i) {
					ifail = POM_class_of_instance(tn_ref[i], &tClassId);
					if (ifail == ITK_ok) {
						POM_class_id_of_class("ChangeItem", &tSClassId);
						ifail = POM_is_descendant(tSClassId, tClassId, &answer);
						//ifail = POM_name_of_class(tClassId, &cClassName);
						//POM_is_descendant	
						if (answer) {
							ifail = EPM_ask_if_released(titem, &iReleased);
							if (iReleased != 0) {
								AOM_set_value_logical(titem, "a2ReviewProp", 1);
							}
							else {
								AOM_set_value_logical(titem, "a2ReviewProp", 0);
							}

						}
					}
				}
			}
			else {
				cout << "Failed to find the references" << endl;
			}
		}
		else {
			cout << "Failed to find the item" << endl;
		}
	}
	else {
		cout << "Failed to log in" << endl;
	}
	return 0;
}