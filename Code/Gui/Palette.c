/*
 * Palette.c — 左侧控件：点击待放 + 拖到画布
 */
#include "Widgets.h"

static void BlocksUIGuiOnPickKind(GtkButton *Button, gpointer UserData) {
    BlocksUIGuiWidgets *Widgets = (BlocksUIGuiWidgets *)UserData;
    const char *Name = gtk_widget_get_name(GTK_WIDGET(Button));

    BlocksUIDocumentSetPendingKind(&Widgets->Document,
                                   BlocksUIWidgetKindFromName(Name));
    BlocksUIGuiSetStatus(
        Widgets, BlocksUILocaleText(Widgets->Language, BLOCKS_UI_TEXT_DRAG_HINT));
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

static GdkContentProvider *BlocksUIGuiOnDragPrepare(GtkDragSource *Source, double X,
                                                    double Y, gpointer UserData) {
    GtkWidget *Button = gtk_event_controller_get_widget(GTK_EVENT_CONTROLLER(Source));
    const char *KindName;

    (void)X;
    (void)Y;
    (void)UserData;
    KindName = gtk_widget_get_name(Button);
    if (KindName == 0) {
        return NULL;
    }
    return gdk_content_provider_new_typed(G_TYPE_STRING, KindName);
}

static void BlocksUIGuiAttachKindDrag(GtkWidget *Button) {
    GtkDragSource *Source = gtk_drag_source_new();

    gtk_drag_source_set_actions(Source, GDK_ACTION_COPY);
    g_signal_connect(Source, "prepare", G_CALLBACK(BlocksUIGuiOnDragPrepare), NULL);
    gtk_widget_add_controller(Button, GTK_EVENT_CONTROLLER(Source));
}

static GtkButton *BlocksUIGuiMakeKindButton(BlocksUIGuiWidgets *Widgets,
                                           BlocksUILocaleTextId LabelId,
                                           const char *KindName) {
    GtkButton *Button = GTK_BUTTON(gtk_button_new_with_label(
        BlocksUILocaleText(Widgets->Language, LabelId)));

    gtk_widget_set_name(GTK_WIDGET(Button), KindName);
    g_signal_connect(Button, "clicked", G_CALLBACK(BlocksUIGuiOnPickKind), Widgets);
    BlocksUIGuiAttachKindDrag(GTK_WIDGET(Button));
    return Button;
}

void BlocksUIGuiPaletteBind(BlocksUIGuiWidgets *Widgets, GtkBox *LeftColumn) {
    Widgets->PaletteTitleLabel =
        GTK_LABEL(gtk_label_new(BlocksUILocaleText(Widgets->Language, BLOCKS_UI_TEXT_WIDGETS)));
    gtk_box_append(LeftColumn, GTK_WIDGET(Widgets->PaletteTitleLabel));

    Widgets->KindButtonButton =
        BlocksUIGuiMakeKindButton(Widgets, BLOCKS_UI_TEXT_BUTTON, "button");
    Widgets->KindLabelButton =
        BlocksUIGuiMakeKindButton(Widgets, BLOCKS_UI_TEXT_LABEL, "label");
    Widgets->KindCheckBoxButton =
        BlocksUIGuiMakeKindButton(Widgets, BLOCKS_UI_TEXT_CHECKBOX, "checkbox");
    Widgets->KindTextBoxButton =
        BlocksUIGuiMakeKindButton(Widgets, BLOCKS_UI_TEXT_TEXTBOX, "textbox");
    gtk_box_append(LeftColumn, GTK_WIDGET(Widgets->KindButtonButton));
    gtk_box_append(LeftColumn, GTK_WIDGET(Widgets->KindLabelButton));
    gtk_box_append(LeftColumn, GTK_WIDGET(Widgets->KindCheckBoxButton));
    gtk_box_append(LeftColumn, GTK_WIDGET(Widgets->KindTextBoxButton));

    Widgets->DeleteButton = GTK_BUTTON(gtk_button_new_with_label(
        BlocksUILocaleText(Widgets->Language, BLOCKS_UI_TEXT_DELETE)));
    g_signal_connect(Widgets->DeleteButton, "clicked",
                     G_CALLBACK(BlocksUIGuiOnDeleteSelected), Widgets);
    gtk_box_append(LeftColumn, GTK_WIDGET(Widgets->DeleteButton));
}
