#include"Headers.h"
#include<iostream>
#include<tc/tc_startup.h>
#include<tccore/aom.h>
#include<ps/ps.h>
#include<bom/bom.h>

using namespace std;

int iCreate_BOM()
{
	tag_t tParent, tParent_rev, tParent1, tParent_rev1,tChild1,tChild2,tChild3, tChild1_rev, tChild2_rev, tChild3_rev,tMain_rev, tNot_main_rev, tChild4, tChild4_rev, tNot_main_parent;
	int iFail = 0;

	iCheckError(ITEM_create_item("", "MainParent", "Item", "A", &tParent, &tParent_rev));

	AOM_save_with_extensions(tParent);

	iCheckError(ITEM_create_item("", "Child", "Item", "A", &tParent1, &tParent_rev1));

	AOM_save_with_extensions(tParent1);

	iCheckError(ITEM_create_item("", "subchild1", "Item", "A", &tChild1, &tChild1_rev));

	AOM_save_with_extensions(tChild1);

	iCheckError(ITEM_create_item("", "subchild2", "Item", "A", &tChild2, &tChild2_rev));

	AOM_save_with_extensions(tChild2);

	iCheckError(ITEM_create_item("", "subchild3", "Item", "A", &tChild3, &tChild3_rev));

	AOM_save_with_extensions(tChild3);

	iCheckError(ITEM_create_item("", "subchild4", "Item", "A", &tChild4, &tChild4_rev));

	AOM_save_with_extensions(tChild4);

	ITEM_ask_latest_rev(tParent, &tMain_rev);
	ITEM_ask_latest_rev(tParent1, &tNot_main_rev);
	ITEM_ask_latest_rev(tChild1, &tNot_main_parent);

	vCreate_BV(tParent, tMain_rev);
	vCreate_BV(tParent1, tNot_main_rev);
	vCreate_BV(tChild1, tNot_main_parent);

	tag_t tBom_window=NULLTAG;
	iCheckError(BOM_create_window(&tBom_window));

	AOM_save_with_extensions(tBom_window);

	tag_t tTop_line = NULLTAG;
	iCheckError(BOM_set_window_top_line(tBom_window, tParent, NULLTAG, NULLTAG, &tTop_line));

	AOM_save_with_extensions(tTop_line);

	tag_t tMain_child = NULLTAG;
	iCheckError(BOM_line_add(tTop_line, NULLTAG, tNot_main_rev, NULLTAG, &tMain_child));

	AOM_save_with_extensions(tMain_child);

	tag_t tU1_bom = NULLTAG, tU2_bom = NULLTAG, tU3_bom = NULLTAG, tU4_bom = NULLTAG;
	iCheckError(BOM_line_add(tMain_child, tChild1, NULLTAG, NULLTAG, &tU1_bom));

	AOM_save_with_extensions(tU1_bom);

	iCheckError(BOM_line_add(tMain_child, tChild2, NULLTAG, NULLTAG, &tU2_bom));

	AOM_save_with_extensions(tU2_bom);

	iCheckError(BOM_line_add(tMain_child, tChild3, NULLTAG, NULLTAG, &tU3_bom));

	AOM_save_with_extensions(tU3_bom);

	iCheckError(BOM_line_add(tU1_bom, tChild4, NULLTAG, NULLTAG, &tU4_bom));

	AOM_save_with_extensions(tU4_bom);

	iCheckError(BOM_save_window(tBom_window));

	iCheckError(BOM_close_window(tBom_window));

	return 0;
}

void vCreate_BV(const tag_t tItem_tag, const tag_t tItem_revison_tag)
{
	tag_t tBom=NULLTAG;
	int Fail = 0;

	iCheckError(PS_create_bom_view(NULLTAG, NULL, NULL, tItem_tag, &tBom));

	AOM_save_with_extensions(tBom);
	AOM_save_with_extensions(tItem_tag);

	tag_t tBom_view_revision = NULLTAG;
	iCheckError(PS_create_bvr(tBom, NULL, NULL, false, tItem_revison_tag, &tBom_view_revision));

	AOM_save_with_extensions(tBom_view_revision);
	AOM_save_with_extensions(tItem_revison_tag);
}