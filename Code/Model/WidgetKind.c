/*
 * WidgetKind.c — 控件类型名 ↔ 枚举
 */
#include "WidgetKind.h"

#include <string.h>

const char *BlocksUIWidgetKindName(BlocksUIWidgetKind Kind) {
    switch (Kind) {
    case BLOCKS_UI_WIDGET_LABEL:
        return "label";
    case BLOCKS_UI_WIDGET_BUTTON:
        return "button";
    case BLOCKS_UI_WIDGET_CHECKBOX:
        return "checkbox";
    case BLOCKS_UI_WIDGET_TEXTBOX:
        return "textbox";
    default:
        return "label";
    }
}

BlocksUIWidgetKind BlocksUIWidgetKindFromName(const char *Name) {
    if (Name == 0) {
        return BLOCKS_UI_WIDGET_LABEL;
    }
    if (strcmp(Name, "button") == 0) {
        return BLOCKS_UI_WIDGET_BUTTON;
    }
    if (strcmp(Name, "checkbox") == 0) {
        return BLOCKS_UI_WIDGET_CHECKBOX;
    }
    if (strcmp(Name, "textbox") == 0) {
        return BLOCKS_UI_WIDGET_TEXTBOX;
    }
    return BLOCKS_UI_WIDGET_LABEL;
}
