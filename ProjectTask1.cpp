// fetch table rows and add secondary objects based on the no.of rows.

#include"Headers.h"
#include<iostream>
#include<tccore/aom.h>
#include<tccore/aom_prop.h>
#include<tccore/tctype.h>
#include<tccore/grm.h>
#include<qry/qry.h>
#include<tccore/item.h>
#include <base_utils/IFail.hxx>
#include <base_utils/ScopedSmPtr.hxx>

using namespace std;
using namespace Teamcenter;

int iTablePropertyRows()
{
	tag_t tQuery = NULLTAG, tRev =NULLTAG, tItemTypeTag = NULLTAG, tCreateInputTag = NULLTAG, tSecondaryobject = NULLTAG, tSecondaryObjectRev=NULLTAG, tRelationType=NULLTAG, tRelation=NULLTAG;
	int iN_entries = 0, iN_found = 0, iNumRows=0, iStatus=0;
	const char* cQueryName = ITK_ask_cli_argument("-q=");
	char** cEntries = NULL;
	char** cValues = NULL;
	tag_t* tResults = NULL;
	char* type = NULL;
	tag_t* tTableRows = NULL;
	char *cTypeofWheel = NULL, *cBrand=NULL, *cHP=NULL, *iPrice=NULL, *cName=NULL;
	char* msg = NULL;
	
	try
	{
		iStatus = (QRY_find2(cQueryName, &tQuery));
		iStatus = (QRY_find_user_entries(tQuery, &iN_entries, &cEntries, &cValues));

		iStatus = (QRY_execute(tQuery, iN_entries, cEntries, cValues, &iN_found, &tResults));

		if (iN_found > 0)
		{
			for (int i = 0; i < iN_found; i++)
			{
				iStatus = ITEM_ask_latest_rev(tResults[i], &tRev);
				AOM_ask_value_string(tRev, "object_name", &cName);
				AOM_ask_table_rows(tRev, "a2TractorProperties", &iNumRows, &tTableRows);

				if (iNumRows > 0)
				{
					for (int j = 0; j < iNumRows; j++)
					{
					
						AOM_ask_value_string(tTableRows[j], "a24WheelDrive", &cTypeofWheel);
						AOM_ask_value_string(tTableRows[j], "a2Brand", &cBrand);
						AOM_ask_value_string(tTableRows[j], "a2HP", &cHP);
						AOM_ask_value_string(tTableRows[j], "a2Price", &iPrice);

						iStatus = TCTYPE_ask_type("A2SecondaryXYZ", &tItemTypeTag);
						iStatus = (TCTYPE_construct_create_input(tItemTypeTag, &tCreateInputTag));
						iStatus = (TCTYPE_create_object(tCreateInputTag, &tSecondaryobject));
						iStatus = (AOM_save_without_extensions(tSecondaryobject));

						iStatus = (ITEM_ask_latest_rev(tSecondaryobject, &tSecondaryObjectRev));
						iStatus = (AOM_refresh(tSecondaryObjectRev, true));
						iStatus = (AOM_set_value_string(tSecondaryObjectRev, "a24WheelDrive", cTypeofWheel));
						iStatus = (AOM_set_value_string(tSecondaryObjectRev, "a2Brand", cBrand));
						iStatus = (AOM_set_value_string(tSecondaryObjectRev, "a2HP", cHP));
						iStatus = (AOM_set_value_string(tSecondaryObjectRev, "a2Price", iPrice));
						iStatus = (AOM_save_without_extensions(tSecondaryObjectRev));
						iStatus = (AOM_refresh(tSecondaryObjectRev, false));

						iStatus = (GRM_find_relation_type("IMAN_specification", &tRelationType));
						iStatus = (GRM_create_relation(tRev, tSecondaryObjectRev, tRelationType, NULLTAG, &tRelation));
						iStatus = (AOM_save_without_extensions(tRelation));
					}

					iStatus = (AOM_refresh(tRev, true));
					iStatus = (AOM_delete_table_rows(tRev, "a2TractorProperties", 0, iNumRows));
					iStatus = (AOM_refresh(tRev, false));
					tRev = NULLTAG;
					iNumRows = 0;
					tTableRows = NULL;
				}
			}
		}
	}
	catch (IFail& ex)
	{
		iStatus = ex.ifail();
		scoped_smptr<char> msg;
		EMH_ask_error_text(iStatus, &msg);
		cout << "The error is :" << msg.getString();
		TC_write_syslog("Error while creating the Item: %s", msg.getString());
	}
	return iStatus;
}