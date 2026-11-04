#ifndef BLOCKS_UI_DOCUMENT_H
#define BLOCKS_UI_DOCUMENT_H

#include "WidgetKind.h"

typedef struct BlocksUIDocument {
    char Title[BLOCKS_UI_TITLE_MAX];
    char IdPrefix[16];
    int Width;
    int Height;
    BlocksUIWidget Widgets[BLOCKS_UI_WIDGET_MAX];
    int Count;
    int SelectedIndex;
    BlocksUIWidgetKind PendingKind;
    int HasPendingPlace;
} BlocksUIDocument;

void BlocksUIDocumentInitialize(BlocksUIDocument *Document);
int BlocksUIDocumentAddWidget(BlocksUIDocument *Document, BlocksUIWidgetKind Kind,
                              int X, int Y);
void BlocksUIDocumentRemoveSelected(BlocksUIDocument *Document);
void BlocksUIDocumentSelectAt(BlocksUIDocument *Document, int X, int Y);
BlocksUIWidget *BlocksUIDocumentSelected(BlocksUIDocument *Document);
void BlocksUIDocumentNudgeSelected(BlocksUIDocument *Document, int DeltaX,
                                   int DeltaY);
void BlocksUIDocumentSetPendingKind(BlocksUIDocument *Document,
                                    BlocksUIWidgetKind Kind);

#endif
