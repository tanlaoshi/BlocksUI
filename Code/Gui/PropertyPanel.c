/*
 * PropertyPanel.c — 右侧属性栏
 */
#include "Widgets.h"

#include <string.h>

static int sBlocksUIGuiPropertyQuiet;

void BlocksUIGuiRefreshPropertyPanel(BlocksUIGuiWidgets *Widgets) {
    BlocksUIWidget *Selected;

    if (Widgets == 0) {
        return;
    }
    sBlocksUIGuiPropertyQuiet = 1;
    Selected = BlocksUIDocumentSelected(&Widgets->Document);
    gtk_editable_set_text(GTK_EDITABLE(Widgets->TitleEntry), Widgets->Document.Title);
    if (Selected == 0) {
        gtk_editable_set_text(GTK_EDITABLE(Widgets->IdEntry), "");
        gtk_editable_set_text(GTK_EDITABLE(Widgets->TextEntry), "");
        gtk_spin_button_set_value(Widgets->XSpin, 0);
        gtk_spin_button_set_value(Widgets->YSpin, 0);
        gtk_spin_button_set_value(Widgets->WidthSpin, 0);
        gtk_spin_button_set_value(Widgets->HeightSpin, 0);
        gtk_widget_set_sensitive(GTK_WIDGET(Widgets->IdEntry), FALSE);
        gtk_widget_set_sensitive(GTK_WIDGET(Widgets->TextEntry), FALSE);
        gtk_widget_set_sensitive(GTK_WIDGET(Widgets->XSpin), FALSE);
        gtk_widget_set_sensitive(GTK_WIDGET(Widgets->YSpin), FALSE);
        gtk_widget_set_sensitive(GTK_WIDGET(Widgets->WidthSpin), FALSE);
        gtk_widget_set_sensitive(GTK_WIDGET(Widgets->HeightSpin), FALSE);
    } else {
        gtk_editable_set_text(GTK_EDITABLE(Widgets->IdEntry), Selected->Id);
        gtk_editable_set_text(GTK_EDITABLE(Widgets->TextEntry), Selected->Text);
        gtk_spin_button_set_value(Widgets->XSpin, Selected->X);
        gtk_spin_button_set_value(Widgets->YSpin, Selected->Y);
        gtk_spin_button_set_value(Widgets->WidthSpin, Selected->Width);
        gtk_spin_button_set_value(Widgets->HeightSpin, Selected->Height);
        gtk_widget_set_sensitive(GTK_WIDGET(Widgets->IdEntry), TRUE);
        gtk_widget_set_sensitive(GTK_WIDGET(Widgets->TextEntry), TRUE);
        gtk_widget_set_sensitive(GTK_WIDGET(Widgets->XSpin), TRUE);
        gtk_widget_set_sensitive(GTK_WIDGET(Widgets->YSpin), TRUE);
        gtk_widget_set_sensitive(GTK_WIDGET(Widgets->WidthSpin), TRUE);
        gtk_widget_set_sensitive(GTK_WIDGET(Widgets->HeightSpin), TRUE);
    }
    sBlocksUIGuiPropertyQuiet = 0;
    BlocksUIGuiRefreshPageHint(Widgets);
}

static void BlocksUIGuiOnTitleChanged(GtkEditable *Editable, gpointer UserData) {
    BlocksUIGuiWidgets *Widgets = (BlocksUIGuiWidgets *)UserData;
    const char *Text;

    if (sBlocksUIGuiPropertyQuiet) {
        return;
    }
    BlocksUIGuiPushUndo(Widgets);
    Text = gtk_editable_get_text(Editable);
    snprintf(Widgets->Document.Title, sizeof(Widgets->Document.Title), "%s",
             Text != 0 ? Text : "MyApp");
}

static void BlocksUIGuiOnIdChanged(GtkEditable *Editable, gpointer UserData) {
    BlocksUIGuiWidgets *Widgets = (BlocksUIGuiWidgets *)UserData;
    BlocksUIWidget *Selected;
    const char *Text;

    if (sBlocksUIGuiPropertyQuiet) {
        return;
    }
    Selected = BlocksUIDocumentSelected(&Widgets->Document);
    if (Selected == 0) {
        return;
    }
    BlocksUIGuiPushUndo(Widgets);
    Text = gtk_editable_get_text(Editable);
    snprintf(Selected->Id, sizeof(Selected->Id), "%s", Text != 0 ? Text : "id");
}

static void BlocksUIGuiOnTextChanged(GtkEditable *Editable, gpointer UserData) {
    BlocksUIGuiWidgets *Widgets = (BlocksUIGuiWidgets *)UserData;
    BlocksUIWidget *Selected;
    const char *Text;

    if (sBlocksUIGuiPropertyQuiet) {
        return;
    }
    Selected = BlocksUIDocumentSelected(&Widgets->Document);
    if (Selected == 0) {
        return;
    }
    BlocksUIGuiPushUndo(Widgets);
    Text = gtk_editable_get_text(Editable);
    snprintf(Selected->Text, sizeof(Selected->Text), "%s", Text != 0 ? Text : "");
    BlocksUIGuiRefreshCanvas(Widgets);
}

static void BlocksUIGuiOnGeometryChanged(GtkSpinButton *Spin, gpointer UserData) {
    BlocksUIGuiWidgets *Widgets = (BlocksUIGuiWidgets *)UserData;
    BlocksUIWidget *Selected;

    (void)Spin;
    if (sBlocksUIGuiPropertyQuiet) {
        return;
    }
    Selected = BlocksUIDocumentSelected(&Widgets->Document);
    if (Selected == 0) {
        return;
    }
    BlocksUIGuiPushUndo(Widgets);
    Selected->X = gtk_spin_button_get_value_as_int(Widgets->XSpin);
    Selected->Y = gtk_spin_button_get_value_as_int(Widgets->YSpin);
    Selected->Width = gtk_spin_button_get_value_as_int(Widgets->WidthSpin);
    Selected->Height = gtk_spin_button_get_value_as_int(Widgets->HeightSpin);
    if (Selected->Width < 8) {
        Selected->Width = 8;
    }
    if (Selected->Height < 8) {
        Selected->Height = 8;
    }
    BlocksUIGuiRefreshCanvas(Widgets);
}

static GtkWidget *BlocksUIGuiLabeledSpin(const char *LabelText, GtkSpinButton **OutSpin,
                                         int MaxValue, BlocksUIGuiWidgets *Widgets) {
    GtkWidget *Row = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 6);
    GtkAdjustment *Adjustment =
        gtk_adjustment_new(0, 0, MaxValue, 1, 10, 0);

    gtk_box_append(GTK_BOX(Row), gtk_label_new(LabelText));
    *OutSpin = GTK_SPIN_BUTTON(gtk_spin_button_new(Adjustment, 1, 0));
    gtk_widget_set_hexpand(GTK_WIDGET(*OutSpin), TRUE);
    g_signal_connect(*OutSpin, "value-changed", G_CALLBACK(BlocksUIGuiOnGeometryChanged),
                     Widgets);
    gtk_box_append(GTK_BOX(Row), GTK_WIDGET(*OutSpin));
    return Row;
}

void BlocksUIGuiPropertyBind(BlocksUIGuiWidgets *Widgets, GtkBox *RightColumn) {
    GtkWidget *TitleRow;
    GtkWidget *IdRow;
    GtkWidget *TextRow;

    gtk_box_append(RightColumn, gtk_label_new("Properties"));
    TitleRow = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 6);
    gtk_box_append(GTK_BOX(TitleRow), gtk_label_new("window"));
    Widgets->TitleEntry = GTK_ENTRY(gtk_entry_new());
    gtk_widget_set_hexpand(GTK_WIDGET(Widgets->TitleEntry), TRUE);
    g_signal_connect(Widgets->TitleEntry, "changed", G_CALLBACK(BlocksUIGuiOnTitleChanged),
                     Widgets);
    gtk_box_append(GTK_BOX(TitleRow), GTK_WIDGET(Widgets->TitleEntry));
    gtk_box_append(RightColumn, TitleRow);

    IdRow = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 6);
    gtk_box_append(GTK_BOX(IdRow), gtk_label_new("id"));
    Widgets->IdEntry = GTK_ENTRY(gtk_entry_new());
    gtk_widget_set_hexpand(GTK_WIDGET(Widgets->IdEntry), TRUE);
    g_signal_connect(Widgets->IdEntry, "changed", G_CALLBACK(BlocksUIGuiOnIdChanged),
                     Widgets);
    gtk_box_append(GTK_BOX(IdRow), GTK_WIDGET(Widgets->IdEntry));
    gtk_box_append(RightColumn, IdRow);

    TextRow = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 6);
    gtk_box_append(GTK_BOX(TextRow), gtk_label_new("text"));
    Widgets->TextEntry = GTK_ENTRY(gtk_entry_new());
    gtk_widget_set_hexpand(GTK_WIDGET(Widgets->TextEntry), TRUE);
    g_signal_connect(Widgets->TextEntry, "changed", G_CALLBACK(BlocksUIGuiOnTextChanged),
                     Widgets);
    gtk_box_append(GTK_BOX(TextRow), GTK_WIDGET(Widgets->TextEntry));
    gtk_box_append(RightColumn, TextRow);

    gtk_box_append(RightColumn,
                   BlocksUIGuiLabeledSpin("x", &Widgets->XSpin, 4000, Widgets));
    gtk_box_append(RightColumn,
                   BlocksUIGuiLabeledSpin("y", &Widgets->YSpin, 4000, Widgets));
    gtk_box_append(RightColumn,
                   BlocksUIGuiLabeledSpin("w", &Widgets->WidthSpin, 4000, Widgets));
    gtk_box_append(RightColumn,
                   BlocksUIGuiLabeledSpin("h", &Widgets->HeightSpin, 4000, Widgets));
    BlocksUIGuiRefreshPropertyPanel(Widgets);
}
