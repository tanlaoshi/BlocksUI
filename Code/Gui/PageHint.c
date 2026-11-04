/*
 * PageHint.c — 多页 ID 区间下拉 + 提示（策略 A）
 */
#include "Widgets.h"

#include "../Model/PageGuide.h"

#include <stdio.h>
#include <string.h>

static BlocksUILocaleTextId BlocksUIGuiPageLocaleId(int PageIndex) {
    switch (PageIndex) {
    case 1:
        return BLOCKS_UI_TEXT_PAGE_SETTINGS;
    case 2:
        return BLOCKS_UI_TEXT_PAGE_ABOUT;
    case 3:
        return BLOCKS_UI_TEXT_PAGE_FILES;
    case 4:
        return BLOCKS_UI_TEXT_PAGE_NAV;
    default:
        return BLOCKS_UI_TEXT_PAGE_HOME;
    }
}

void BlocksUIGuiRebuildPageNames(BlocksUIGuiWidgets *Widgets) {
    GtkStringList *Names;
    int Index;
    guint Selected;

    if (Widgets == 0 || Widgets->PageDropDown == 0) {
        return;
    }
    Selected = gtk_drop_down_get_selected(Widgets->PageDropDown);
    Names = gtk_string_list_new(NULL);
    for (Index = 0; Index < BlocksUIPageGuideCount(); Index++) {
        gtk_string_list_append(
            Names, BlocksUILocaleText(Widgets->Language, BlocksUIGuiPageLocaleId(Index)));
    }
    gtk_drop_down_set_model(Widgets->PageDropDown, G_LIST_MODEL(Names));
    if (Selected >= (guint)BlocksUIPageGuideCount()) {
        Selected = 0;
    }
    gtk_drop_down_set_selected(Widgets->PageDropDown, Selected);
}

void BlocksUIGuiRefreshPageHint(BlocksUIGuiWidgets *Widgets) {
    const BlocksUIPageGuide *Guide;
    char Message[192];
    int Suggested;
    int LogicId;
    BlocksUIWidget *Selected;
    const char *PageName;

    if (Widgets == 0 || Widgets->PageHintLabel == 0) {
        return;
    }
    Guide = BlocksUIPageGuideAt(Widgets->PageIndex);
    Suggested = BlocksUIPageGuideSuggestLogicId(&Widgets->Document, Widgets->PageIndex);
    PageName =
        BlocksUILocaleText(Widgets->Language, BlocksUIGuiPageLocaleId(Widgets->PageIndex));
    snprintf(Message, sizeof(Message),
             BlocksUILocaleText(Widgets->Language, BLOCKS_UI_TEXT_PAGE_HINT), PageName,
             Guide->RangeBegin, Guide->RangeEnd, Suggested, Guide->IdPrefix);
    gtk_label_set_text(Widgets->PageHintLabel, Message);

    if (Widgets->LogicHintLabel == 0) {
        return;
    }
    Selected = BlocksUIDocumentSelected(&Widgets->Document);
    if (Selected == 0 || Widgets->Document.SelectedIndex < 0) {
        gtk_label_set_text(Widgets->LogicHintLabel,
                           BlocksUILocaleText(Widgets->Language, BLOCKS_UI_TEXT_LOGIC_NONE));
        return;
    }
    LogicId = BlocksUIPageGuideLogicIdForWidget(
        &Widgets->Document, Widgets->PageIndex, Widgets->Document.SelectedIndex);
    if (LogicId < 0) {
        snprintf(Message, sizeof(Message),
                 BlocksUILocaleText(Widgets->Language, BLOCKS_UI_TEXT_LOGIC_OTHER),
                 Guide->IdPrefix);
    } else {
        snprintf(Message, sizeof(Message),
                 BlocksUILocaleText(Widgets->Language, BLOCKS_UI_TEXT_LOGIC_VALUE),
                 LogicId);
    }
    gtk_label_set_text(Widgets->LogicHintLabel, Message);
}

static void BlocksUIGuiOnPageChanged(GtkDropDown *DropDown, GParamSpec *Parameter,
                                     gpointer UserData) {
    BlocksUIGuiWidgets *Widgets = (BlocksUIGuiWidgets *)UserData;
    const BlocksUIPageGuide *Guide;
    guint Selected;

    (void)Parameter;
    Selected = gtk_drop_down_get_selected(DropDown);
    if (Selected >= (guint)BlocksUIPageGuideCount()) {
        Selected = 0;
    }
    Widgets->PageIndex = (int)Selected;
    Guide = BlocksUIPageGuideAt(Widgets->PageIndex);
    snprintf(Widgets->Document.IdPrefix, sizeof(Widgets->Document.IdPrefix), "%s",
             Guide->IdPrefix);
    BlocksUIGuiRefreshPageHint(Widgets);
}

void BlocksUIGuiPageHintBind(BlocksUIGuiWidgets *Widgets, GtkBox *RightColumn) {
    GtkStringList *Names;
    int Index;
    const BlocksUIPageGuide *Guide;

    Widgets->PageTitleLabel = GTK_LABEL(
        gtk_label_new(BlocksUILocaleText(Widgets->Language, BLOCKS_UI_TEXT_PAGE)));
    gtk_box_append(RightColumn, GTK_WIDGET(Widgets->PageTitleLabel));
    Names = gtk_string_list_new(NULL);
    for (Index = 0; Index < BlocksUIPageGuideCount(); Index++) {
        gtk_string_list_append(
            Names, BlocksUILocaleText(Widgets->Language, BlocksUIGuiPageLocaleId(Index)));
    }
    Widgets->PageDropDown = GTK_DROP_DOWN(gtk_drop_down_new(G_LIST_MODEL(Names), NULL));
    gtk_drop_down_set_selected(Widgets->PageDropDown, 0);
    g_signal_connect(Widgets->PageDropDown, "notify::selected",
                     G_CALLBACK(BlocksUIGuiOnPageChanged), Widgets);
    gtk_box_append(RightColumn, GTK_WIDGET(Widgets->PageDropDown));

    Widgets->PageHintLabel = GTK_LABEL(gtk_label_new(""));
    gtk_label_set_xalign(Widgets->PageHintLabel, 0.0f);
    gtk_label_set_wrap(Widgets->PageHintLabel, TRUE);
    gtk_box_append(RightColumn, GTK_WIDGET(Widgets->PageHintLabel));

    Widgets->LogicHintLabel = GTK_LABEL(gtk_label_new(
        BlocksUILocaleText(Widgets->Language, BLOCKS_UI_TEXT_LOGIC_NONE)));
    gtk_label_set_xalign(Widgets->LogicHintLabel, 0.0f);
    gtk_label_set_wrap(Widgets->LogicHintLabel, TRUE);
    gtk_box_append(RightColumn, GTK_WIDGET(Widgets->LogicHintLabel));

    Widgets->PageIndex = 0;
    Guide = BlocksUIPageGuideAt(0);
    snprintf(Widgets->Document.IdPrefix, sizeof(Widgets->Document.IdPrefix), "%s",
             Guide->IdPrefix);
    BlocksUIGuiRefreshPageHint(Widgets);
}
