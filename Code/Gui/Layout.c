/*
 * Layout.c — 三栏窗体 + 导出
 */
#include "Widgets.h"

#include "../Model/UitxtExport.h"

#include <stdio.h>

static void BlocksUIGuiExportSaveFinish(GObject *Source, GAsyncResult *Result,
                                        gpointer UserData) {
    BlocksUIGuiWidgets *Widgets = (BlocksUIGuiWidgets *)UserData;
    GError *Error = NULL;
    GFile *File = gtk_file_dialog_save_finish(GTK_FILE_DIALOG(Source), Result, &Error);
    char *Path;

    if (File == NULL) {
        if (Error != NULL) {
            g_error_free(Error);
        }
        return;
    }
    Path = g_file_get_path(File);
    if (Path != NULL) {
        if (BlocksUIUitxtExportPath(&Widgets->Document, Path) == 0) {
            char Message[256];

            snprintf(Message, sizeof(Message), "exported %s", Path);
            BlocksUIGuiSetStatus(Widgets, Message);
        } else {
            BlocksUIGuiSetStatus(Widgets, "export failed");
        }
        g_free(Path);
    }
    g_object_unref(File);
}

static void BlocksUIGuiOnExport(GtkButton *Button, gpointer UserData) {
    BlocksUIGuiWidgets *Widgets = (BlocksUIGuiWidgets *)UserData;
    GtkFileDialog *Dialog;
    GFile *Initial;

    (void)Button;
    Dialog = gtk_file_dialog_new();
    gtk_file_dialog_set_title(Dialog, "Export .uitxt");
    gtk_file_dialog_set_initial_name(Dialog, "MyApp.uitxt");
    Initial = g_file_new_for_path(".");
    gtk_file_dialog_set_initial_folder(Dialog, Initial);
    g_object_unref(Initial);
    gtk_file_dialog_save(Dialog, Widgets->Window, NULL, BlocksUIGuiExportSaveFinish,
                         Widgets);
}

GtkWidget *BlocksUIGuiBuildWindow(BlocksUIGuiWidgets *Widgets) {
    GtkWidget *Window;
    GtkWidget *Outer;
    GtkWidget *Body;
    GtkWidget *Left;
    GtkWidget *CenterScroll;
    GtkWidget *Right;
    GtkWidget *Bottom;
    GtkWidget *ExportButton;
    GtkWidget *CanvasFrame;

    Window = gtk_application_window_new(Widgets->Application);
    Widgets->Window = GTK_WINDOW(Window);
    gtk_window_set_title(Widgets->Window, "BlocksUI");
    gtk_window_set_default_size(Widgets->Window, 1400, 820);

    Outer = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_window_set_child(Widgets->Window, Outer);

    Body = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 8);
    gtk_widget_set_margin_start(Body, 8);
    gtk_widget_set_margin_end(Body, 8);
    gtk_widget_set_margin_top(Body, 8);
    gtk_box_append(GTK_BOX(Outer), Body);

    Left = gtk_box_new(GTK_ORIENTATION_VERTICAL, 6);
    gtk_widget_set_size_request(Left, 110, -1);
    gtk_box_append(GTK_BOX(Body), Left);
    BlocksUIGuiPaletteBind(Widgets, GTK_BOX(Left));

    CenterScroll = gtk_scrolled_window_new();
    gtk_widget_set_hexpand(CenterScroll, TRUE);
    gtk_widget_set_vexpand(CenterScroll, TRUE);
    CanvasFrame = gtk_frame_new("Canvas 1280×720");
    Widgets->Canvas = GTK_DRAWING_AREA(gtk_drawing_area_new());
    gtk_frame_set_child(GTK_FRAME(CanvasFrame), GTK_WIDGET(Widgets->Canvas));
    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(CenterScroll), CanvasFrame);
    gtk_box_append(GTK_BOX(Body), CenterScroll);
    BlocksUIGuiCanvasBind(Widgets);

    Right = gtk_box_new(GTK_ORIENTATION_VERTICAL, 6);
    gtk_widget_set_size_request(Right, 200, -1);
    gtk_box_append(GTK_BOX(Body), Right);
    BlocksUIGuiPropertyBind(Widgets, GTK_BOX(Right));

    Bottom = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 8);
    gtk_widget_set_margin_start(Bottom, 8);
    gtk_widget_set_margin_end(Bottom, 8);
    gtk_widget_set_margin_bottom(Bottom, 8);
    gtk_widget_set_margin_top(Bottom, 4);
    ExportButton = gtk_button_new_with_label("Export .uitxt");
    g_signal_connect(ExportButton, "clicked", G_CALLBACK(BlocksUIGuiOnExport), Widgets);
    gtk_box_append(GTK_BOX(Bottom), ExportButton);
    Widgets->StatusLabel = GTK_LABEL(gtk_label_new("ready"));
    gtk_widget_set_hexpand(GTK_WIDGET(Widgets->StatusLabel), TRUE);
    gtk_label_set_xalign(Widgets->StatusLabel, 0.0f);
    gtk_box_append(GTK_BOX(Bottom), GTK_WIDGET(Widgets->StatusLabel));
    gtk_box_append(GTK_BOX(Outer), Bottom);

    return Window;
}
