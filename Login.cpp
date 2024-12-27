#include"Headers.h"
#include "error_check.h"
#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<tc/tc_startup.h>
#include<tcinit/tcinit.h>
#include<tc/emh.h>
#include<tcinit/tcinit.h>
#include<tccore/item.h>
#include<tccore/aom.h>
#include<fclasses/tc_date.h>
#include<tccore/aom_prop.h>
#include<tccore/grm.h>
#include<ae/dataset.h>
#include<sa/tcfile.h>
#include <base_utils/IFail.hxx>
#include <base_utils/ScopedSmPtr.hxx>

using namespace std;
using namespace Teamcenter;

int ITK_user_main(int argc, char*argv[]) 
{
	date_t dCreation_date=NULLDATE, dModified_date = NULLDATE;
	tag_t tItem=NULLTAG;
	char* cMessage=nullptr;
	char* cDate_string = nullptr;
	char* cModified_string = nullptr;
	int iStatus = 0;
	char* msg = NULL;

	char* cUser_id = ITK_ask_cli_argument("-u=");
	char* cPassword = ITK_ask_cli_argument("-p=");
	char* cGroup = ITK_ask_cli_argument("-g=");
	try
	{
		iStatus = (ITK_init_module(cUser_id, cPassword, cGroup));
		//iCheckError(ITK_auto_login());

		cout << "login Successful" << endl;
		//item_create();
		//create_item_cli();
		//create_folder();
		//create_dataset();
		//set_value_property();
		//update_named_reference();
		//tctype_fileread();
		//create_BOM();
		//import_named_reference();
		//iFetch_child();
		//where_referenced();
		//iWhere_used();
		//set_property_preference();
		//send_mail();
		//currentDateTime();
		//delete_datsets();
		//singlelevel_bom_from_file();
		//date_property();
		//update_latest_revision();
		//print_csv();
		//iQueryExcecute();
		//iUpdate_property_with_lov_value();
		//iUpdatePropertiesWip();
		//iAssignProject();
		//iCheckAndSetRevisionEffectivity();
		//iSetUserStatus();
		//iDelete_datsets();
		//iAddImmediateParent();
		//create_user_attach_to_group();
		//projAssignObject();
		//iFreeze();
		//testA();
		//iTablePropertyRows();
		//iCheckConsumedLicense();
		iAskChildLines(cUser_id);

		/*ITEM_find_item(item_id, &item);
		AOM_ask_creation_date(item, &creation_date);
		AOM_ask_modified_date(item, &modified_date);
		int iFail = NULLTAG, iFail1=NULL;

		iFail = DATE_date_to_string(creation_date, "%d-%b-%y %H:%M", &date_string);
		if (iFail == ITK_ok)
			cout << "creation date conversion successful" << endl;
		else
			cout << "conversion unsuccessful" << endl;

		DATE_date_to_string(modified_date, "%d-%b-%y %H:%M", &modified_string);
		if (iFail == ITK_ok)
			cout << "modified date conversion successful" << endl;
		else
			cout << "conversion unsuccessful" << endl;

		cout << date_string << endl;
		cout << modified_string << endl;*/
	}
	catch (IFail& ex)
	{
		iStatus = ex.ifail();
		scoped_smptr<char> msg;
		EMH_ask_error_text(iStatus, &msg);
		cout << "The error is :" << msg.getString();
		TC_write_syslog("Error while creating the Item: %s", msg.getString());

	}
	return ITK_ok;
}
