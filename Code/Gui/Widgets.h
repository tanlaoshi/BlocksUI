#ifndef BLOCKS_UI_GUI_WIDGETS_H
#define BLOCKS_UI_GUI_WIDGETS_H

#include "../Model/Document.h"
#include "../Model/History.h"
#include "../Model/PageGuide.h"
#include "Locale.h"

#include <gtk/gtk.h>

#define BLOCKS_UI_GRID_SIZE 40
#define BLOCKS_UI_DOCUMENT_PATH_MAX 512

typedef struct BlocksUIGuiWidgets {
    GtkApplication *Application;
    GtkWindow *Window;
    GtkDrawingArea *Canvas;
    GtkFrame *CanvasFrame;
    GtkEntry *TitleEntry;
    GtkEntry *IdEntry;
    GtkEntry *TextEntry;
    GtkSpinButton *XSpin;
    GtkSpinButton *YSpin;
    GtkSpinButton *WidthSpin;
    GtkSpinButton *HeightSpin;
    GtkLabel *StatusLabel;
    GtkDropDown *PageDropDown;
    GtkLabel *PageTitleLabel;
    GtkLabel *PageHintLabel;
    GtkLabel *LogicHintLabel;
    GtkLabel *PaletteTitleLabel;
    GtkLabel *PropertiesTitleLabel;
    GtkLabel *WindowFieldLabel;
    GtkButton *KindButtonButton;
    GtkButton *KindLabelButton;
    GtkButton *KindCheckBoxButton;
    GtkButton *KindTextBoxButton;
    GtkButton *DeleteButton;
    GtkButton *UndoButton;
    GtkButton *RedoButton;
    GtkButton *OpenButton;
    GtkButton *SaveButton;
    GtkButton *SaveAsButton;
    GtkButton *LanguageButton;
    BlocksUIDocument Document;
    BlocksUIHistory History;
    char DocumentPath[BLOCKS_UI_DOCUMENT_PATH_MAX];
    double DisplayScale;
    BlocksUILanguage Language;
    int PageIndex;
    int Dragging;
    int DragOriginX;
    int DragOriginY;
    int DragWidgetStartX;
    int DragWidgetStartY;
    int DragUndoPushed;
} BlocksUIGuiWidgets;

GtkWidget *BlocksUIGuiBuildWindow(BlocksUIGuiWidgets *Widgets);
void BlocksUIGuiRefreshCanvas(BlocksUIGuiWidgets *Widgets);
void BlocksUIGuiRefreshPropertyPanel(BlocksUIGuiWidgets *Widgets);
void BlocksUIGuiRefreshPageHint(BlocksUIGuiWidgets *Widgets);
void BlocksUIGuiRebuildPageNames(BlocksUIGuiWidgets *Widgets);
void BlocksUIGuiApplyLocale(BlocksUIGuiWidgets *Widgets);
void BlocksUIGuiSetStatus(BlocksUIGuiWidgets *Widgets, const char *Text);
void BlocksUIGuiPushUndo(BlocksUIGuiWidgets *Widgets);
void BlocksUIGuiRefreshHistoryStatus(BlocksUIGuiWidgets *Widgets);
void BlocksUIGuiCanvasPlaceKind(BlocksUIGuiWidgets *Widgets, BlocksUIWidgetKind Kind,
                                int LogicX, int LogicY);

void BlocksUIGuiCanvasBind(BlocksUIGuiWidgets *Widgets);
void BlocksUIGuiPaletteBind(BlocksUIGuiWidgets *Widgets, GtkBox *LeftColumn);
void BlocksUIGuiPageHintBind(BlocksUIGuiWidgets *Widgets, GtkBox *RightColumn);
void BlocksUIGuiPropertyBind(BlocksUIGuiWidgets *Widgets, GtkBox *RightColumn);
void BlocksUIGuiFileBind(BlocksUIGuiWidgets *Widgets, GtkBox *BottomBar);

#endif
