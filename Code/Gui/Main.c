/*
 * Main.c — GTK4 入口
 */
#include "Widgets.h"

#include <string.h>

static void BlocksUIGuiOnActivate(GtkApplication *Application, gpointer UserData) {
    BlocksUIGuiWidgets *Widgets = (BlocksUIGuiWidgets *)UserData;
    GtkWidget *Window;

    Widgets->Application = Application;
    Window = BlocksUIGuiBuildWindow(Widgets);
    gtk_window_present(GTK_WINDOW(Window));
}

int main(int ArgumentCount, char **ArgumentVector) {
    BlocksUIGuiWidgets Widgets;
    GtkApplication *Application;
    int Status;

    memset(&Widgets, 0, sizeof(Widgets));
    BlocksUIDocumentInitialize(&Widgets.Document);
    BlocksUIHistoryInitialize(&Widgets.History);
    Widgets.DisplayScale = 0.72;
    Application = gtk_application_new("com.blocks.blocksui", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(Application, "activate", G_CALLBACK(BlocksUIGuiOnActivate),
                     &Widgets);
    Status = g_application_run(G_APPLICATION(Application), ArgumentCount,
                               ArgumentVector);
    g_object_unref(Application);
    return Status;
}
