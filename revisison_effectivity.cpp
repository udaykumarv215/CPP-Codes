#include"Headers.h"
#include<iostream>
#include<tccore/item.h>
#include<tccore/aom_prop.h>
#include<string.h>
#include<epm/epm.h>
#include<epm/epm_task_template_itk.h>
#include<tccore/releasestatus.h>
#include<fclasses/tc_date.h>

using namespace std;

int iCheckAndSetRevisionEffectivity()
{
	int iNum = 0, iNum1=0;
	tag_t tItemTag = NULLTAG, tTemplate=NULLTAG, tLatestRev =NULLTAG, tDateinfo=NULLTAG, tProcess = NULLTAG, tNewProcess=NULLTAG;
	tag_t* tEffs = NULLTAG;
	tag_t tWsos[10];
	tag_t* tValues = NULL;
	char* cName = NULL;
	logical lLogical , lLogical1;
	date_t dStartDate, dEndDate;
	int iAttachmentTpe[10] = { EPM_target_attachment };

	const char* start_date = "01-Jan-2000 09:09:56";
	char* finalStartDate = const_cast<char*>(start_date);
	const char* end_date = "31-Dec-2000 09:09:56";
	char* finalEndDate = const_cast<char*>(end_date);

	DATE_string_to_date_t(finalStartDate, &lLogical,&dStartDate);
	DATE_string_to_date_t(finalEndDate, &lLogical1, &dEndDate);

	char* cItemId = ITK_ask_cli_argument("-id=");
	iCheckError(ITEM_find_item(cItemId, &tItemTag));
	iCheckError(ITEM_ask_latest_rev(tItemTag, &tLatestRev));

	iCheckError(AOM_ask_value_tags(tLatestRev, "release_status_list", &iNum, &tValues));
	tWsos[0] = tLatestRev;
	if (iNum != 0)
	{
		for (int i = 0; i < iNum; i++)
		{

			AOM_ask_value_string(tValues[i], "object_name", &cName);
			if (tc_strcmp(cName, "TCM Released")==0)
			{
				WSOM_status_ask_effectivities(tValues[i], &iNum1, &tEffs);
				if (iNum1 == 0)
					//RELSTAT_set_date_effectivity(tValues[i], dStartDate, dEndDate);
					RELSTAT_set_unit_effectivity(tValues[i], 1, 5);
			}
		}
	}
	else
	{
		EPM_find_process_template("TCM Release Process", &tProcess);
		EPM_create_process("TCM Release Process", "", tProcess, 1, tWsos, iAttachmentTpe, &tNewProcess);
		//RELSTAT_add_release_status(tProcess, 1, tWsos, true);

		//RELSTAT_set_date_effectivity(tNewProcess, dStartDate, dEndDate);
		RELSTAT_set_unit_effectivity(tNewProcess, 1, 5);
	}
	return ITK_ok;
}