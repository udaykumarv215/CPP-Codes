#pragma once
#include<iostream>
#include<stdio.h>
#include<tc/tc_startup.h>
#include<tcinit/tcinit.h>
#include<tc/emh.h>
#include<string>
#include<string.h>

using namespace std;

int iCheckError(int status);
int iItem_create();
int iCreate_item_cli();
int iCreate_folder();
int iCreate_dataset();
int iSet_value_property();
int iUpdate_named_reference();
int iTctype_fileread();
int iCreate_BOM();
void vCreate_BV(const tag_t item_tag, const tag_t item_revison_tag);
void vImport_named_reference();
int iFetch_child();
int iReturn_children1(tag_t line);
int iWhere_referenced();
int iSet_property_preference();
int iSend_mail();
int iCurrentDateTime();
int iDelete_datsets();
int iSingleLevel_bom_from_file();
void vCreate_BV(const tag_t parent);
int iDate_property();
date_t dCalc30Days(date_t date1);
int iUpdate_latest_revision();
int	iFind_latest_revision(tag_t item_tag);
time_t convert_date_to_time_t(string DateString);
int iPrint_csv();
int iWhere_used();
int iQueryExcecute();
int iUpdate_property_with_lov_value();
int iUpdatePropertiesWip();
void iSetProperty(tag_t tRevList);
int iAssignProject();
int iCheckAndSetRevisionEffectivity();
int iSetUserStatus();
int iAddImmediateParent();
int create_user_attach_to_group();
int projAssignObject();
int iFreeze();
int testA();
int iTablePropertyRows();
int iCheckConsumedLicense();
int iAskChildLines(char* cUserId);
string iAttachNamedReferenceToFolder(tag_t tRev);
int iReturn_children(tag_t tLine, string tFolder);

