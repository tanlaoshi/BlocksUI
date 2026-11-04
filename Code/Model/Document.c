/*
 * Document.c — 设计文档：增删选中 / 放置
 */
#include "Document.h"

#include <stdio.h>
#include <string.h>

void BlocksUIDocumentInitialize(BlocksUIDocument *Document) {
    if (Document == 0) {
        return;
    }
    memset(Document, 0, sizeof(*Document));
    snprintf(Document->Title, sizeof(Document->Title), "MyApp");
    Document->Width = BLOCKS_UI_DESIGN_WIDTH;
    Document->Height = BLOCKS_UI_DESIGN_HEIGHT;
    Document->SelectedIndex = -1;
    Document->HasPendingPlace = 0;
}

static void BlocksUIDocumentMakeId(BlocksUIDocument *Document,
                                   BlocksUIWidgetKind Kind, char *Out,
                                   int OutMax) {
    const char *Prefix;
    int Serial;
    int i;
    char Candidate[BLOCKS_UI_ID_MAX];

    switch (Kind) {
    case BLOCKS_UI_WIDGET_BUTTON:
        Prefix = "btn";
        break;
    case BLOCKS_UI_WIDGET_CHECKBOX:
        Prefix = "chk";
        break;
    case BLOCKS_UI_WIDGET_TEXTBOX:
        Prefix = "txt";
        break;
    default:
        Prefix = "lbl";
        break;
    }
    for (Serial = 1; Serial < 1000; Serial++) {
        snprintf(Candidate, sizeof(Candidate), "%s%d", Prefix, Serial);
        for (i = 0; i < Document->Count; i++) {
            if (strcmp(Document->Widgets[i].Id, Candidate) == 0) {
                break;
            }
        }
        if (i >= Document->Count) {
            snprintf(Out, OutMax, "%s", Candidate);
            return;
        }
    }
    snprintf(Out, OutMax, "%s9", Prefix);
}

int BlocksUIDocumentAddWidget(BlocksUIDocument *Document, BlocksUIWidgetKind Kind,
                              int X, int Y) {
    BlocksUIWidget *Widget;
    int DefaultWidth = 80;
    int DefaultHeight = 28;

    if (Document == 0 || Document->Count >= BLOCKS_UI_WIDGET_MAX) {
        return -1;
    }
    if (Kind == BLOCKS_UI_WIDGET_LABEL) {
        DefaultWidth = 120;
        DefaultHeight = 24;
    } else if (Kind == BLOCKS_UI_WIDGET_CHECKBOX) {
        DefaultWidth = 100;
        DefaultHeight = 24;
    } else if (Kind == BLOCKS_UI_WIDGET_TEXTBOX) {
        DefaultWidth = 160;
        DefaultHeight = 28;
    }
    if (X < 0) {
        X = 0;
    }
    if (Y < 0) {
        Y = 0;
    }
    if (X + DefaultWidth > Document->Width) {
        X = Document->Width - DefaultWidth;
    }
    if (Y + DefaultHeight > Document->Height) {
        Y = Document->Height - DefaultHeight;
    }
    Widget = &Document->Widgets[Document->Count];
    memset(Widget, 0, sizeof(*Widget));
    Widget->Kind = Kind;
    Widget->X = X;
    Widget->Y = Y;
    Widget->Width = DefaultWidth;
    Widget->Height = DefaultHeight;
    BlocksUIDocumentMakeId(Document, Kind, Widget->Id, (int)sizeof(Widget->Id));
    {
        char TextCopy[BLOCKS_UI_ID_MAX];

        snprintf(TextCopy, sizeof(TextCopy), "%s", Widget->Id);
        snprintf(Widget->Text, sizeof(Widget->Text), "%s", TextCopy);
    }
    Document->SelectedIndex = Document->Count;
    Document->Count++;
    Document->HasPendingPlace = 0;
    return Document->SelectedIndex;
}

void BlocksUIDocumentRemoveSelected(BlocksUIDocument *Document) {
    int i;

    if (Document == 0 || Document->SelectedIndex < 0 ||
        Document->SelectedIndex >= Document->Count) {
        return;
    }
    for (i = Document->SelectedIndex; i + 1 < Document->Count; i++) {
        Document->Widgets[i] = Document->Widgets[i + 1];
    }
    Document->Count--;
    if (Document->SelectedIndex >= Document->Count) {
        Document->SelectedIndex = Document->Count - 1;
    }
}

void BlocksUIDocumentSelectAt(BlocksUIDocument *Document, int X, int Y) {
    int i;

    if (Document == 0) {
        return;
    }
    Document->SelectedIndex = -1;
    for (i = Document->Count - 1; i >= 0; i--) {
        BlocksUIWidget *Widget = &Document->Widgets[i];

        if (X >= Widget->X && X < Widget->X + Widget->Width && Y >= Widget->Y &&
            Y < Widget->Y + Widget->Height) {
            Document->SelectedIndex = i;
            return;
        }
    }
}

BlocksUIWidget *BlocksUIDocumentSelected(BlocksUIDocument *Document) {
    if (Document == 0 || Document->SelectedIndex < 0 ||
        Document->SelectedIndex >= Document->Count) {
        return 0;
    }
    return &Document->Widgets[Document->SelectedIndex];
}

void BlocksUIDocumentNudgeSelected(BlocksUIDocument *Document, int DeltaX,
                                   int DeltaY) {
    BlocksUIWidget *Widget = BlocksUIDocumentSelected(Document);

    if (Widget == 0) {
        return;
    }
    Widget->X += DeltaX;
    Widget->Y += DeltaY;
    if (Widget->X < 0) {
        Widget->X = 0;
    }
    if (Widget->Y < 0) {
        Widget->Y = 0;
    }
}

void BlocksUIDocumentSetPendingKind(BlocksUIDocument *Document,
                                    BlocksUIWidgetKind Kind) {
    if (Document == 0) {
        return;
    }
    Document->PendingKind = Kind;
    Document->HasPendingPlace = 1;
}
