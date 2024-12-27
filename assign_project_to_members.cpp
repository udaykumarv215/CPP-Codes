// Assign project to the members in the mentioned group

#include"Headers.h"
#include<iostream>
#include<tccore/project.h>
#include<sa/groupmember.h>
#include<sa/group.h>

using namespace std;

int iAssignProject()
{
	tag_t tGroupTag=NULLTAG;
	tag_t tProject = NULLTAG;
	int iNumMem = 0;
	tag_t* tMembertags = NULL;
	const char* cGroupName = ITK_ask_cli_argument("-gid=");
	const char* cProjectId = ITK_ask_cli_argument("-pid=");

	iCheckError(SA_find_group(cGroupName, &tGroupTag));
	iCheckError(SA_find_groupmembers_by_group(tGroupTag, &iNumMem, &tMembertags));
	iCheckError(PROJ_find(cProjectId, &tProject));
	iCheckError(PROJ_add_members(tProject, iNumMem, tMembertags));

	return 0;
}