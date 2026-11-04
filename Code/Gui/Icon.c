/*
 * Icon.c — 解析 Assets/icons 并设置窗口图标
 */
#include "Icon.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static int BlocksUIGuiDirectoryLooksLikeAssetsIcons(const char *Path) {
    char Probe[900];

    if (Path == 0 || Path[0] == 0) {
        return 0;
    }
    snprintf(Probe, sizeof(Probe),
             "%s/hicolor/128x128/apps/com.blocks.blocksui.png", Path);
    return access(Probe, R_OK) == 0;
}

static int BlocksUIGuiResolveIconsDirectory(char *Out, size_t Capacity) {
    const char *Home;
    const char *Root;
    char Candidate[512];
    char Executable[512];
    ssize_t Length;

    Root = getenv("BLOCKSUI_ROOT");
    if (Root != 0 && Root[0] != 0) {
        snprintf(Candidate, sizeof(Candidate), "%s/Assets/icons", Root);
        if (BlocksUIGuiDirectoryLooksLikeAssetsIcons(Candidate)) {
            snprintf(Out, Capacity, "%s", Candidate);
            return 0;
        }
    }
    Home = getenv("HOME");
    if (Home != 0) {
        snprintf(Candidate, sizeof(Candidate), "%s/BlocksUI/Assets/icons", Home);
        if (BlocksUIGuiDirectoryLooksLikeAssetsIcons(Candidate)) {
            snprintf(Out, Capacity, "%s", Candidate);
            return 0;
        }
    }
    Length = readlink("/proc/self/exe", Executable, sizeof(Executable) - 1);
    if (Length > 0) {
        char *Slash;

        Executable[Length] = 0;
        Slash = strrchr(Executable, '/');
        if (Slash != 0) {
            *Slash = 0;
        }
        snprintf(Candidate, sizeof(Candidate), "%s/../../../Assets/icons",
                 Executable);
        if (BlocksUIGuiDirectoryLooksLikeAssetsIcons(Candidate)) {
            snprintf(Out, Capacity, "%s", Candidate);
            return 0;
        }
    }
    if (BlocksUIGuiDirectoryLooksLikeAssetsIcons("Assets/icons")) {
        snprintf(Out, Capacity, "%s", "Assets/icons");
        return 0;
    }
    return -1;
}

void BlocksUIGuiApplyWindowIcon(GtkWindow *Window) {
    char IconsDirectory[512];
    GtkIconTheme *Theme;
    GdkDisplay *Display;

    if (Window == 0) {
        return;
    }
    gtk_window_set_icon_name(Window, "com.blocks.blocksui");
    gtk_window_set_default_icon_name("com.blocks.blocksui");
    if (BlocksUIGuiResolveIconsDirectory(IconsDirectory, sizeof(IconsDirectory)) !=
        0) {
        return;
    }
    Display = gtk_widget_get_display(GTK_WIDGET(Window));
    if (Display == 0) {
        Display = gdk_display_get_default();
    }
    if (Display == 0) {
        return;
    }
    Theme = gtk_icon_theme_get_for_display(Display);
    if (Theme == 0) {
        return;
    }
    gtk_icon_theme_add_search_path(Theme, IconsDirectory);
}
