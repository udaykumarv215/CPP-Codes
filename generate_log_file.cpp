//#include <iostream>
//#include <fstream>  // For file handling
//#include <stdio.h>
//#include <stdlib.h>
//#include <tc/tc_startup.h>
//#include <tcinit/tcinit.h>
//#include <tc/emh.h>
//#include <tccore/item.h>
//#include <tccore/aom.h>
//#include <tccore/aom_prop.h>
//
//using namespace std;
//
//char *cmessage = NULL;
//
//int ITK_user_main(int argc, char *argv[]) {
//	int iFail = 0;
//	tag_t titem = NULLTAG;
//	char * cValue = NULL;
//	char * cDesc = NULL;
//
//	ofstream logFile("C:\\Users\\13335\\Downloads\\test\\logFile.txt");
//	if (!logFile.is_open()) 
//	{
//		cout << "Failed to open logFile \n";
//		return -1;
//	}
//	char *cuser_id = ITK_ask_cli_argument("-u=");
//	char *cpassword = ITK_ask_cli_argument("-p=");
//	char *cgroup = ITK_ask_cli_argument("-g=");
//	iFail = ITK_init_module(cuser_id, cpassword, cgroup);
//
//	if (iFail == ITK_ok) {
//		cout << "Login successful\n";
//		logFile << "Login successful " << endl;
//
//		char *citem_id = ITK_ask_cli_argument("-i=");
//		iFail = ITEM_find_item(citem_id, &titem);
//
//		if (iFail == ITK_ok && titem != NULLTAG) {
//			cout << "Item found successfully\n";
//
//			// Open CSV file for writing
//			logFile << "Item found successfully with citem_id :" << citem_id << endl;
//			cout << "Item found successfully with citem_id :" << citem_id << endl;
//			iFail = AOM_ask_value_string(titem, "object_name", &cValue);
//			if (iFail == ITK_ok && cValue != NULLTAG) 
//			{
//				logFile << "Object name found Sucessfully : " << cValue << endl;
//				cout << "Object name found Sucessfully : " << cValue << endl;
//
//				iFail = AOM_ask_value_string(titem, "object_desc", &cDesc);
//				if (iFail == ITK_ok && cDesc != NULLTAG) 
//				{
//					logFile << "Object desc found Sucessfully : " << cDesc << endl;
//					cout << "Object desc found Sucessfully: " << cDesc << endl;
//				}
//				else 
//				{
//					logFile << " Fail to find the object desc" << endl;
//					cout << "Fail to find the object desc" << endl;
//					EMH_ask_error_text(iFail, &cmessage);
//					cout << cmessage << endl;
//
//				}
//			}
//			else 
//			{
//				logFile << " Fail to find the object name" << endl;
//				cout << "Fail to find the object name" << endl;
//				EMH_ask_error_text(iFail, &cmessage);
//				cout << cmessage << endl;
//
//			}
//		}
//		else 
//		{
//			logFile << " Fail to find the object" << endl;
//			cout << "Fail to find the object" << endl;
//			EMH_ask_error_text(iFail, &cmessage);
//			cout << cmessage << endl;
//
//		}
//	}
//	else 
//	{
//		logFile << " Fail to log in" << endl;
//		cout << "Fail to log in" << endl;
//		EMH_ask_error_text(iFail, &cmessage);
//		cout << cmessage << endl;
//
//	}
//	logFile.close();
//	return 0;
//}