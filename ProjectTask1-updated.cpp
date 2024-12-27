//// fetch table rows and add secondary objects based on the no.of rows.
//
//#include"Headers.h"
//#include<iostream>
//#include<tccore/aom.h>
//#include<tccore/aom_prop.h>
//#include<tccore/tctype.h>
//#include<tccore/grm.h>
//#include<qry/qry.h>
//#include<tccore/item.h>
//
//using namespace std;
//
//int iTablePropertyRows()
//{
//	tag_t tQuery = NULLTAG, tRev = NULLTAG, tItemTypeTag = NULLTAG, tCreateInputTag = NULLTAG, tSecondaryobject = NULLTAG, tSecondaryObjectRev = NULLTAG, tRelationType = NULLTAG, tRelation = NULLTAG;
//	int iN_entries = 0, iN_found = 0, iNumRows = 0;
//	const char* cQueryName = ITK_ask_cli_argument("-q=");
//	char** cEntries = NULL;
//	char** cValues = NULL;
//	tag_t* tResults = NULL;
//	char* type = NULL;
//	tag_t* tTableRows = NULL;
//	char *cTypeofWheel = NULL, *cBrand = NULL, *cHP = NULL, *iPrice = NULL, *cName = NULL;
//
//	iCheckError(QRY_find2(cQueryName, &tQuery));
//
//	iCheckError(QRY_find_user_entries(tQuery, &iN_entries, &cEntries, &cValues));
//
//	iCheckError(QRY_execute(tQuery, iN_entries, cEntries, cValues, &iN_found, &tResults));
//
//	if (iN_found > 0)
//	{
//		for (int i = 0; i < iN_found; i++)
//		{
//			ITEM_ask_latest_rev(tResults[i], &tRev);
//			AOM_ask_value_string(tRev, "object_name", &cName);
//			AOM_ask_table_rows(tRev, "a2TractorProperties", &iNumRows, &tTableRows);
//
//			if (iNumRows > 0)
//			{
//				for (int j = 0; j < iNumRows; j++)
//				{
//					int count = 0;
//					char** propnames = NULL;
//					AOM_ask_prop_names(tTableRows[j], &count, &propnames);
//					AOM_ask_value_string(tTableRows[j], "a24WheelDrive", &cTypeofWheel);
//					AOM_ask_value_string(tTableRows[j], "a2Brand", &cBrand);
//					AOM_ask_value_string(tTableRows[j], "a2HP", &cHP);
//					AOM_ask_value_string(tTableRows[j], "a2Price", &iPrice);
//
//					iCheckError(TCTYPE_ask_type("A2SecondaryXYZ", &tItemTypeTag));
//					iCheckError(TCTYPE_construct_create_input(tItemTypeTag, &tCreateInputTag));
//					iCheckError(TCTYPE_create_object(tCreateInputTag, &tSecondaryobject));
//					iCheckError(AOM_save_without_extensions(tSecondaryobject));
//
//					iCheckError(ITEM_ask_latest_rev(tSecondaryobject, &tSecondaryObjectRev));
//					iCheckError(AOM_refresh(tSecondaryObjectRev, true));
//					iCheckError(AOM_set_value_string(tSecondaryObjectRev, "a24WheelDrive", cTypeofWheel));
//					iCheckError(AOM_set_value_string(tSecondaryObjectRev, "a2Brand", cBrand));
//					iCheckError(AOM_set_value_string(tSecondaryObjectRev, "a2HP", cHP));
//					iCheckError(AOM_set_value_string(tSecondaryObjectRev, "a2Price", iPrice));
//					iCheckError(AOM_save_without_extensions(tSecondaryObjectRev));
//					iCheckError(AOM_refresh(tSecondaryObjectRev, false));
//
//					iCheckError(GRM_find_relation_type("IMAN_specification", &tRelationType));
//					iCheckError(GRM_create_relation(tRev, tSecondaryObjectRev, tRelationType, NULLTAG, &tRelation));
//					iCheckError(AOM_save_without_extensions(tRelation));
//				}
//
//				iCheckError(AOM_refresh(tRev, true));
//				iCheckError(AOM_delete_table_rows(tRev, "a2TractorProperties", 0, iNumRows));
//				iCheckError(AOM_refresh(tRev, false));
//				tRev = NULLTAG;
//				iNumRows = 0;
//				tTableRows = NULL;
//			}
//			else
//				cout << "Please add data to the table." << endl;
//		}
//	}
//	else
//		cout << "Cannot find the referenced instances." << endl;
//	return 0;
//}