/*
 * PageHint.c — 多页 ID 区间下拉 + 提示（策略 A）
 */
#include "Widgets.h"

#include "../Model/PageGuide.h"

#include <stdio.h>
#include <string.h>

void BlocksUIGuiRefreshPageHint(BlocksUIGuiWidgets *Widgets) {
    const BlocksUIPageGuide *Guide;
    char Message[160];
    int Suggested;
    int LogicId;
    BlocksUIWidget *Selected;

    if (Widgets == 0 || Widgets->PageHintLabel == 0) {
        return;
    }
    Guide = BlocksUIPageGuideAt(Widgets->PageIndex);
    Suggested = BlocksUIPageGuideSuggestLogicId(&Widgets->Document, Widgets->PageIndex);
    snprintf(Message, sizeof(Message),
             "%s  %d–%d\nnext logic %d\nid prefix %s_", Guide->Name,
             Guide->RangeBegin, Guide->RangeEnd, Suggested, Guide->IdPrefix);
    gtk_label_set_text(Widgets->PageHintLabel, Message);

    if (Widgets->LogicHintLabel == 0) {
        return;
    }
    Selected = BlocksUIDocumentSelected(&Widgets->Document);
    if (Selected == 0 || Widgets->Document.SelectedIndex < 0) {
        gtk_label_set_text(Widgets->LogicHintLabel, "logic —");
        return;
    }
    LogicId = BlocksUIPageGuideLogicIdForWidget(
        &Widgets->Document, Widgets->PageIndex, Widgets->Document.SelectedIndex);
    if (LogicId < 0) {
        snprintf(Message, sizeof(Message), "logic — (id not %s_*)", Guide->IdPrefix);
    } else {
        snprintf(Message, sizeof(Message), "logic %d  (enum hint)", LogicId);
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

    gtk_box_append(RightColumn, gtk_label_new("Page (ID ranges)"));
    Names = gtk_string_list_new(NULL);
    for (Index = 0; Index < BlocksUIPageGuideCount(); Index++) {
        Guide = BlocksUIPageGuideAt(Index);
        gtk_string_list_append(Names, Guide->Name);
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

    Widgets->LogicHintLabel = GTK_LABEL(gtk_label_new("logic —"));
    gtk_label_set_xalign(Widgets->LogicHintLabel, 0.0f);
    gtk_label_set_wrap(Widgets->LogicHintLabel, TRUE);
    gtk_box_append(RightColumn, GTK_WIDGET(Widgets->LogicHintLabel));

    Widgets->PageIndex = 0;
    Guide = BlocksUIPageGuideAt(0);
    snprintf(Widgets->Document.IdPrefix, sizeof(Widgets->Document.IdPrefix), "%s",
             Guide->IdPrefix);
    BlocksUIGuiRefreshPageHint(Widgets);
}
