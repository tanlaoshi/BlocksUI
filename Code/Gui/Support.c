/*
 * Support.c — 刷新画布 / 状态 / 撤销压栈
 */
#include "Widgets.h"

#include <stdio.h>

void BlocksUIGuiRefreshCanvas(BlocksUIGuiWidgets *Widgets) {
    if (Widgets == 0 || Widgets->Canvas == 0) {
        return;
    }
    gtk_widget_queue_draw(GTK_WIDGET(Widgets->Canvas));
}

void BlocksUIGuiSetStatus(BlocksUIGuiWidgets *Widgets, const char *Text) {
    if (Widgets == 0 || Widgets->StatusLabel == 0) {
        return;
    }
    gtk_label_set_text(Widgets->StatusLabel, Text != 0 ? Text : "");
}

void BlocksUIGuiRefreshHistoryStatus(BlocksUIGuiWidgets *Widgets) {
    char Message[80];

    if (Widgets == 0) {
        return;
    }
    snprintf(Message, sizeof(Message), "undo %d  redo %d",
             BlocksUIHistoryUndoCount(&Widgets->History),
             BlocksUIHistoryRedoCount(&Widgets->History));
    BlocksUIGuiSetStatus(Widgets, Message);
}

void BlocksUIGuiPushUndo(BlocksUIGuiWidgets *Widgets) {
    if (Widgets == 0) {
        return;
    }
    BlocksUIHistoryPushUndo(&Widgets->History, &Widgets->Document);
    BlocksUIGuiRefreshHistoryStatus(Widgets);
}
