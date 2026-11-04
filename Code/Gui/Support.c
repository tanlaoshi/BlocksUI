/*
 * Support.c — 刷新画布 / 状态 / 撤销压栈 / 应用语言
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
    snprintf(Message, sizeof(Message),
             BlocksUILocaleText(Widgets->Language, BLOCKS_UI_TEXT_UNDO_STATUS),
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

static void BlocksUIGuiSetButtonLabel(GtkButton *Button, BlocksUILanguage Language,
                                      BlocksUILocaleTextId Id) {
    if (Button == 0) {
        return;
    }
    gtk_button_set_label(Button, BlocksUILocaleText(Language, Id));
}

void BlocksUIGuiApplyLocale(BlocksUIGuiWidgets *Widgets) {
    BlocksUILanguage Language;

    if (Widgets == 0) {
        return;
    }
    Language = Widgets->Language;
    if (Widgets->PaletteTitleLabel != 0) {
        gtk_label_set_text(Widgets->PaletteTitleLabel,
                           BlocksUILocaleText(Language, BLOCKS_UI_TEXT_WIDGETS));
    }
    BlocksUIGuiSetButtonLabel(Widgets->KindButtonButton, Language,
                              BLOCKS_UI_TEXT_BUTTON);
    BlocksUIGuiSetButtonLabel(Widgets->KindLabelButton, Language, BLOCKS_UI_TEXT_LABEL);
    BlocksUIGuiSetButtonLabel(Widgets->KindCheckBoxButton, Language,
                              BLOCKS_UI_TEXT_CHECKBOX);
    BlocksUIGuiSetButtonLabel(Widgets->KindTextBoxButton, Language,
                              BLOCKS_UI_TEXT_TEXTBOX);
    BlocksUIGuiSetButtonLabel(Widgets->DeleteButton, Language, BLOCKS_UI_TEXT_DELETE);
    BlocksUIGuiSetButtonLabel(Widgets->UndoButton, Language, BLOCKS_UI_TEXT_UNDO);
    BlocksUIGuiSetButtonLabel(Widgets->RedoButton, Language, BLOCKS_UI_TEXT_REDO);
    BlocksUIGuiSetButtonLabel(Widgets->OpenButton, Language, BLOCKS_UI_TEXT_OPEN);
    BlocksUIGuiSetButtonLabel(Widgets->SaveButton, Language, BLOCKS_UI_TEXT_SAVE);
    BlocksUIGuiSetButtonLabel(Widgets->SaveAsButton, Language, BLOCKS_UI_TEXT_SAVE_AS);
    BlocksUIGuiSetButtonLabel(Widgets->LanguageButton, Language,
                              BLOCKS_UI_TEXT_LANGUAGE_TOGGLE);
    if (Widgets->PropertiesTitleLabel != 0) {
        gtk_label_set_text(Widgets->PropertiesTitleLabel,
                           BlocksUILocaleText(Language, BLOCKS_UI_TEXT_PROPERTIES));
    }
    if (Widgets->PageTitleLabel != 0) {
        gtk_label_set_text(Widgets->PageTitleLabel,
                           BlocksUILocaleText(Language, BLOCKS_UI_TEXT_PAGE));
    }
    if (Widgets->WindowFieldLabel != 0) {
        gtk_label_set_text(Widgets->WindowFieldLabel,
                           BlocksUILocaleText(Language, BLOCKS_UI_TEXT_WINDOW));
    }
    if (Widgets->CanvasFrame != 0) {
        gtk_frame_set_label(Widgets->CanvasFrame,
                            BlocksUILocaleText(Language, BLOCKS_UI_TEXT_CANVAS));
    }
    BlocksUIGuiRebuildPageNames(Widgets);
    BlocksUIGuiRefreshPageHint(Widgets);
    BlocksUIGuiRefreshHistoryStatus(Widgets);
}
