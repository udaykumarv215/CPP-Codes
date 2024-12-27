#include "Headers.h"
#include<iostream>
#include<stdio.h>
#include<tc/tc_startup.h>
#include<tcinit/tcinit.h>
#include<tccore/item.h>
#include<tccore/aom.h>
#include<tc/envelope.h>
#include<tc/emh.h>
#include<sa/user.h>

using namespace std;

int iSend_mail()
{
	char* cDesc = nullptr;
	char* cName = nullptr;
	tag_t tEnvelope, tItem, tRuser, tCcuser;

	// Obtain command-line arguments
	char* cItem = ITK_ask_cli_argument("-i=");
	char* cUser = ITK_ask_cli_argument("-cu=");
	char* cCuser = ITK_ask_cli_argument("-ccu=");

	iCheckError(ITEM_find_item(cItem, &tItem));

	iCheckError(WSOM_ask_name2(tItem, &cName));

	iCheckError(WSOM_ask_description2(tItem, &cDesc));

	iCheckError(MAIL_create_envelope(cName, cDesc, &tEnvelope));

	iCheckError(SA_find_user2(cUser, &tRuser));

	iCheckError(SA_find_user2(cCuser, &tCcuser));

	iCheckError(MAIL_add_envelope_receiver(tEnvelope, tRuser));

	iCheckError(MAIL_add_envelope_cc_receiver(tEnvelope, tCcuser));

	iCheckError(MAIL_send_envelope(tEnvelope));

	cout << "Mail sent successfully\n";

	return 0;

}