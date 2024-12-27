#include"Headers.h"
#include <tccore/project.h>
#include <tccore/item.h>
#include <tccore/aom.h>
#include <iostream>

using namespace std;

int projAssignObject() {
	tag_t tProject;
	tag_t tItem;
	int status;

	status = PROJ_find("1111", &tProject);
	if (status != ITK_ok) {
		cout << "Error in PROJ_find: " << status << endl;
		return status;
	}
	cout << "Project tag: " << tProject << endl;

	status = ITEM_find_rev("000600", "A", &tItem);
	if (status != ITK_ok) {
		cout << "Error in ITEM_find_rev: " << status << endl;
		return status;
	}
	cout << "Item tag: " << tItem << endl;

	status = AOM_refresh(tItem, 1);
	if (status != ITK_ok) {
		cout << "Error in AOM_refresh: " << status << endl;
		return status;
	}

	status = PROJ_assign_objects(1, &tProject, 1, &tItem);
	if (status != ITK_ok) {
		cout << "Error in PROJ_assign_objects: " << status << endl;
		return status;
	}
	cout << "Assigned item to project." << endl;

	status = AOM_save_without_extensions(tItem);
	if (status != ITK_ok) {
		cout << "Error in AOM_save_without_extensions: " << status << endl;
		return status;
	}

	status = AOM_refresh(tItem, 0);
	if (status != ITK_ok) {
		cout << "Error in AOM_refresh: " << status << endl;
		return status;
	}

	return ITK_ok;
}