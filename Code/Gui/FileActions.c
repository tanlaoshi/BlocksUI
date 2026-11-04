/*
 * FileActions.c — Open / Save / Save As .uitxt
 */
#include "Widgets.h"

#include "../Model/UitxtExport.h"
#include "../Model/UitxtImport.h"

#include <stdio.h>
#include <string.h>

static void BlocksUIGuiRememberDocumentPath(BlocksUIGuiWidgets *Widgets,
                                            const char *Path) {
    if (Widgets == 0 || Path == 0) {
        return;
    }
    snprintf(Widgets->DocumentPath, sizeof(Widgets->DocumentPath), "%s", Path);
}

static void BlocksUIGuiAfterDocumentLoad(BlocksUIGuiWidgets *Widgets,
                                         const char *Path) {
    BlocksUIHistoryInitialize(&Widgets->History);
    BlocksUIGuiRememberDocumentPath(Widgets, Path);
    BlocksUIGuiRefreshPropertyPanel(Widgets);
    BlocksUIGuiRefreshCanvas(Widgets);
    BlocksUIGuiRefreshHistoryStatus(Widgets);
}

static int BlocksUIGuiWriteDocumentPath(BlocksUIGuiWidgets *Widgets,
                                        const char *Path) {
    char Message[300];

    if (BlocksUIUitxtExportPath(&Widgets->Document, Path) != 0) {
        BlocksUIGuiSetStatus(
            Widgets, BlocksUILocaleText(Widgets->Language, BLOCKS_UI_TEXT_SAVE_FAILED));
        return -1;
    }
    BlocksUIGuiRememberDocumentPath(Widgets, Path);
    snprintf(Message, sizeof(Message), "%s %s",
             BlocksUILocaleText(Widgets->Language, BLOCKS_UI_TEXT_SAVED_PREFIX), Path);
    BlocksUIGuiSetStatus(Widgets, Message);
    return 0;
}

static void BlocksUIGuiOpenFinish(GObject *Source, GAsyncResult *Result,
                                  gpointer UserData) {
    BlocksUIGuiWidgets *Widgets = (BlocksUIGuiWidgets *)UserData;
    GError *Error = NULL;
    GFile *File = gtk_file_dialog_open_finish(GTK_FILE_DIALOG(Source), Result, &Error);
    char *Path;

    if (File == NULL) {
        if (Error != NULL) {
            g_error_free(Error);
        }
        return;
    }
    Path = g_file_get_path(File);
    if (Path != NULL) {
        if (BlocksUIUitxtImportPath(&Widgets->Document, Path) == 0) {
            char Message[300];

            BlocksUIGuiAfterDocumentLoad(Widgets, Path);
            snprintf(Message, sizeof(Message), "%s %s",
                     BlocksUILocaleText(Widgets->Language, BLOCKS_UI_TEXT_OPENED_PREFIX),
                     Path);
            BlocksUIGuiSetStatus(Widgets, Message);
        } else {
            BlocksUIGuiSetStatus(
                Widgets,
                BlocksUILocaleText(Widgets->Language, BLOCKS_UI_TEXT_OPEN_FAILED));
        }
        g_free(Path);
    }
    g_object_unref(File);
}

static void BlocksUIGuiSaveAsFinish(GObject *Source, GAsyncResult *Result,
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
        BlocksUIGuiWriteDocumentPath(Widgets, Path);
        g_free(Path);
    }
    g_object_unref(File);
}

static void BlocksUIGuiOnOpen(GtkButton *Button, gpointer UserData) {
    BlocksUIGuiWidgets *Widgets = (BlocksUIGuiWidgets *)UserData;
    GtkFileDialog *Dialog;
    GFile *Initial;

    (void)Button;
    Dialog = gtk_file_dialog_new();
    gtk_file_dialog_set_title(
        Dialog, BlocksUILocaleText(Widgets->Language, BLOCKS_UI_TEXT_OPEN_TITLE));
    Initial = g_file_new_for_path(".");
    gtk_file_dialog_set_initial_folder(Dialog, Initial);
    g_object_unref(Initial);
    gtk_file_dialog_open(Dialog, Widgets->Window, NULL, BlocksUIGuiOpenFinish,
                         Widgets);
}

static void BlocksUIGuiOnSaveAs(GtkButton *Button, gpointer UserData) {
    BlocksUIGuiWidgets *Widgets = (BlocksUIGuiWidgets *)UserData;
    GtkFileDialog *Dialog;
    GFile *Initial;
    const char *InitialName = "MyApp.uitxt";

    (void)Button;
    Dialog = gtk_file_dialog_new();
    gtk_file_dialog_set_title(
        Dialog, BlocksUILocaleText(Widgets->Language, BLOCKS_UI_TEXT_SAVE_TITLE));
    if (Widgets->DocumentPath[0] != 0) {
        const char *Slash = strrchr(Widgets->DocumentPath, '/');

        InitialName = Slash != 0 ? Slash + 1 : Widgets->DocumentPath;
    }
    gtk_file_dialog_set_initial_name(Dialog, InitialName);
    Initial = g_file_new_for_path(".");
    gtk_file_dialog_set_initial_folder(Dialog, Initial);
    g_object_unref(Initial);
    gtk_file_dialog_save(Dialog, Widgets->Window, NULL, BlocksUIGuiSaveAsFinish,
                         Widgets);
}

static void BlocksUIGuiOnSave(GtkButton *Button, gpointer UserData) {
    BlocksUIGuiWidgets *Widgets = (BlocksUIGuiWidgets *)UserData;

    (void)Button;
    if (Widgets->DocumentPath[0] == 0) {
        BlocksUIGuiOnSaveAs(Button, UserData);
        return;
    }
    BlocksUIGuiWriteDocumentPath(Widgets, Widgets->DocumentPath);
}

void BlocksUIGuiFileBind(BlocksUIGuiWidgets *Widgets, GtkBox *BottomBar) {
    Widgets->OpenButton = GTK_BUTTON(gtk_button_new_with_label(
        BlocksUILocaleText(Widgets->Language, BLOCKS_UI_TEXT_OPEN)));
    Widgets->SaveButton = GTK_BUTTON(gtk_button_new_with_label(
        BlocksUILocaleText(Widgets->Language, BLOCKS_UI_TEXT_SAVE)));
    Widgets->SaveAsButton = GTK_BUTTON(gtk_button_new_with_label(
        BlocksUILocaleText(Widgets->Language, BLOCKS_UI_TEXT_SAVE_AS)));

    g_signal_connect(Widgets->OpenButton, "clicked", G_CALLBACK(BlocksUIGuiOnOpen),
                     Widgets);
    g_signal_connect(Widgets->SaveButton, "clicked", G_CALLBACK(BlocksUIGuiOnSave),
                     Widgets);
    g_signal_connect(Widgets->SaveAsButton, "clicked", G_CALLBACK(BlocksUIGuiOnSaveAs),
                     Widgets);
    gtk_box_append(BottomBar, GTK_WIDGET(Widgets->OpenButton));
    gtk_box_append(BottomBar, GTK_WIDGET(Widgets->SaveButton));
    gtk_box_append(BottomBar, GTK_WIDGET(Widgets->SaveAsButton));
}
