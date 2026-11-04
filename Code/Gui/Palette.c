/*
 * Palette.c — 左侧控件种类 + 删除
 */
#include "Widgets.h"

static void BlocksUIGuiOnPickKind(GtkButton *Button, gpointer UserData) {
    BlocksUIGuiWidgets *Widgets = (BlocksUIGuiWidgets *)UserData;
    const char *Name = gtk_widget_get_name(GTK_WIDGET(Button));

    BlocksUIDocumentSetPendingKind(&Widgets->Document,
                                   BlocksUIWidgetKindFromName(Name));
    BlocksUIGuiSetStatus(Widgets, "click canvas to place");
}

static void BlocksUIGuiOnDeleteSelected(GtkButton *Button, gpointer UserData) {
    BlocksUIGuiWidgets *Widgets = (BlocksUIGuiWidgets *)UserData;

    (void)Button;
    if (BlocksUIDocumentSelected(&Widgets->Document) == 0) {
        return;
    }
    BlocksUIGuiPushUndo(Widgets);
    BlocksUIDocumentRemoveSelected(&Widgets->Document);
    BlocksUIGuiRefreshPropertyPanel(Widgets);
    BlocksUIGuiRefreshCanvas(Widgets);
    BlocksUIGuiRefreshHistoryStatus(Widgets);
}

static GtkWidget *BlocksUIGuiMakeKindButton(BlocksUIGuiWidgets *Widgets,
                                           const char *Label, const char *KindName) {
    GtkWidget *Button = gtk_button_new_with_label(Label);

    gtk_widget_set_name(Button, KindName);
    g_signal_connect(Button, "clicked", G_CALLBACK(BlocksUIGuiOnPickKind), Widgets);
    return Button;
}

void BlocksUIGuiPaletteBind(BlocksUIGuiWidgets *Widgets, GtkBox *LeftColumn) {
    GtkWidget *DeleteButton;

    gtk_box_append(LeftColumn, gtk_label_new("Widgets"));
    gtk_box_append(LeftColumn,
                   BlocksUIGuiMakeKindButton(Widgets, "Button", "button"));
    gtk_box_append(LeftColumn, BlocksUIGuiMakeKindButton(Widgets, "Label", "label"));
    gtk_box_append(LeftColumn,
                   BlocksUIGuiMakeKindButton(Widgets, "CheckBox", "checkbox"));
    gtk_box_append(LeftColumn,
                   BlocksUIGuiMakeKindButton(Widgets, "TextBox", "textbox"));
    DeleteButton = gtk_button_new_with_label("Delete");
    g_signal_connect(DeleteButton, "clicked", G_CALLBACK(BlocksUIGuiOnDeleteSelected),
                     Widgets);
    gtk_box_append(LeftColumn, DeleteButton);
}
