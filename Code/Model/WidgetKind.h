#ifndef BLOCKS_UI_WIDGET_KIND_H
#define BLOCKS_UI_WIDGET_KIND_H

typedef enum BlocksUIWidgetKind {
    BLOCKS_UI_WIDGET_LABEL = 0,
    BLOCKS_UI_WIDGET_BUTTON,
    BLOCKS_UI_WIDGET_CHECKBOX,
    BLOCKS_UI_WIDGET_TEXTBOX
} BlocksUIWidgetKind;

#define BLOCKS_UI_DESIGN_WIDTH  1280
#define BLOCKS_UI_DESIGN_HEIGHT 720
#define BLOCKS_UI_ID_MAX        32
#define BLOCKS_UI_TEXT_MAX      128
#define BLOCKS_UI_TITLE_MAX     128
#define BLOCKS_UI_WIDGET_MAX    64

typedef struct BlocksUIWidget {
    char Id[BLOCKS_UI_ID_MAX];
    char Text[BLOCKS_UI_TEXT_MAX];
    BlocksUIWidgetKind Kind;
    int X;
    int Y;
    int Width;
    int Height;
} BlocksUIWidget;

const char *BlocksUIWidgetKindName(BlocksUIWidgetKind Kind);
BlocksUIWidgetKind BlocksUIWidgetKindFromName(const char *Name);

#endif
