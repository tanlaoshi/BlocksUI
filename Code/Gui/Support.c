/*
 * Support.c — 刷新画布 / 状态
 */
#include "Widgets.h"

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
