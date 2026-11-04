#ifndef BLOCKS_UI_GUI_WIDGETS_H
#define BLOCKS_UI_GUI_WIDGETS_H

#include "../Model/Document.h"

#include <gtk/gtk.h>

typedef struct BlocksUIGuiWidgets {
    GtkApplication *Application;
    GtkWindow *Window;
    GtkDrawingArea *Canvas;
    GtkEntry *TitleEntry;
    GtkEntry *IdEntry;
    GtkEntry *TextEntry;
    GtkSpinButton *XSpin;
    GtkSpinButton *YSpin;
    GtkSpinButton *WidthSpin;
    GtkSpinButton *HeightSpin;
    GtkLabel *StatusLabel;
    BlocksUIDocument Document;
    double DisplayScale;
    int Dragging;
    int DragOriginX;
    int DragOriginY;
    int DragWidgetStartX;
    int DragWidgetStartY;
} BlocksUIGuiWidgets;

GtkWidget *BlocksUIGuiBuildWindow(BlocksUIGuiWidgets *Widgets);
void BlocksUIGuiRefreshCanvas(BlocksUIGuiWidgets *Widgets);
void BlocksUIGuiRefreshPropertyPanel(BlocksUIGuiWidgets *Widgets);
void BlocksUIGuiSetStatus(BlocksUIGuiWidgets *Widgets, const char *Text);

void BlocksUIGuiCanvasBind(BlocksUIGuiWidgets *Widgets);
void BlocksUIGuiPaletteBind(BlocksUIGuiWidgets *Widgets, GtkBox *LeftColumn);
void BlocksUIGuiPropertyBind(BlocksUIGuiWidgets *Widgets, GtkBox *RightColumn);

#endif
