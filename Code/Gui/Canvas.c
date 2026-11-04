/*
 * Canvas.c — 1280×720 逻辑画布（缩放显示）+ 放置/拖动
 */
#include "Widgets.h"

#include <math.h>

static void BlocksUIGuiCanvasMapPoint(BlocksUIGuiWidgets *Widgets, double EventX,
                                     double EventY, int *OutX, int *OutY) {
    double Scale = Widgets->DisplayScale > 0.05 ? Widgets->DisplayScale : 0.55;

    *OutX = (int)(EventX / Scale);
    *OutY = (int)(EventY / Scale);
}

static void BlocksUIGuiCanvasDrawWidget(cairo_t *Cairo, const BlocksUIWidget *Widget,
                                        int Selected) {
    double X = Widget->X;
    double Y = Widget->Y;
    double W = Widget->Width;
    double H = Widget->Height;

    if (Widget->Kind == BLOCKS_UI_WIDGET_BUTTON) {
        cairo_set_source_rgb(Cairo, 0.25, 0.45, 0.75);
        cairo_rectangle(Cairo, X, Y, W, H);
        cairo_fill(Cairo);
        cairo_set_source_rgb(Cairo, 1, 1, 1);
    } else if (Widget->Kind == BLOCKS_UI_WIDGET_TEXTBOX) {
        cairo_set_source_rgb(Cairo, 0.95, 0.93, 0.80);
        cairo_rectangle(Cairo, X, Y, W, H);
        cairo_fill(Cairo);
        cairo_set_source_rgb(Cairo, 0.2, 0.2, 0.2);
        cairo_rectangle(Cairo, X + 0.5, Y + 0.5, W - 1, H - 1);
        cairo_stroke(Cairo);
    } else if (Widget->Kind == BLOCKS_UI_WIDGET_CHECKBOX) {
        cairo_set_source_rgb(Cairo, 0.9, 0.9, 0.9);
        cairo_rectangle(Cairo, X, Y + H * 0.2, H * 0.6, H * 0.6);
        cairo_fill_preserve(Cairo);
        cairo_set_source_rgb(Cairo, 0.2, 0.2, 0.2);
        cairo_stroke(Cairo);
        cairo_move_to(Cairo, X + H * 0.75, Y + H * 0.65);
        cairo_set_font_size(Cairo, 12);
        cairo_show_text(Cairo, Widget->Text);
        if (Selected) {
            cairo_set_source_rgb(Cairo, 0.1, 0.4, 0.9);
            cairo_rectangle(Cairo, X - 2, Y - 2, W + 4, H + 4);
            cairo_stroke(Cairo);
        }
        return;
    } else {
        cairo_set_source_rgb(Cairo, 0.15, 0.15, 0.15);
    }
    cairo_move_to(Cairo, X + 6, Y + H * 0.7);
    cairo_set_font_size(Cairo, 13);
    cairo_show_text(Cairo, Widget->Text);
    if (Selected) {
        cairo_set_source_rgb(Cairo, 0.1, 0.4, 0.9);
        cairo_set_line_width(Cairo, 2);
        cairo_rectangle(Cairo, X - 2, Y - 2, W + 4, H + 4);
        cairo_stroke(Cairo);
    }
}

static void BlocksUIGuiCanvasOnDraw(GtkDrawingArea *Area, cairo_t *Cairo, int Width,
                                    int Height, gpointer UserData) {
    BlocksUIGuiWidgets *Widgets = (BlocksUIGuiWidgets *)UserData;
    double Scale;
    int i;
    int Grid;

    (void)Area;
    (void)Width;
    (void)Height;
    Scale = Widgets->DisplayScale > 0.05 ? Widgets->DisplayScale : 0.55;
    cairo_set_source_rgb(Cairo, 0.92, 0.92, 0.94);
    cairo_paint(Cairo);
    cairo_scale(Cairo, Scale, Scale);
    cairo_set_source_rgb(Cairo, 1, 1, 1);
    cairo_rectangle(Cairo, 0, 0, Widgets->Document.Width, Widgets->Document.Height);
    cairo_fill(Cairo);
    cairo_set_source_rgb(Cairo, 0.85, 0.85, 0.88);
    cairo_set_line_width(Cairo, 1.0 / Scale);
    for (Grid = 0; Grid <= Widgets->Document.Width; Grid += 40) {
        cairo_move_to(Cairo, Grid, 0);
        cairo_line_to(Cairo, Grid, Widgets->Document.Height);
        cairo_stroke(Cairo);
    }
    for (Grid = 0; Grid <= Widgets->Document.Height; Grid += 40) {
        cairo_move_to(Cairo, 0, Grid);
        cairo_line_to(Cairo, Widgets->Document.Width, Grid);
        cairo_stroke(Cairo);
    }
    cairo_set_source_rgb(Cairo, 0.5, 0.5, 0.55);
    cairo_rectangle(Cairo, 0, 0, Widgets->Document.Width, Widgets->Document.Height);
    cairo_stroke(Cairo);
    for (i = 0; i < Widgets->Document.Count; i++) {
        BlocksUIGuiCanvasDrawWidget(Cairo, &Widgets->Document.Widgets[i],
                                    i == Widgets->Document.SelectedIndex);
    }
}

static void BlocksUIGuiCanvasOnPressed(GtkGestureClick *Gesture, int NPress,
                                       double X, double Y, gpointer UserData) {
    BlocksUIGuiWidgets *Widgets = (BlocksUIGuiWidgets *)UserData;
    int LogicX;
    int LogicY;

    (void)Gesture;
    (void)NPress;
    BlocksUIGuiCanvasMapPoint(Widgets, X, Y, &LogicX, &LogicY);
    if (Widgets->Document.HasPendingPlace) {
        BlocksUIDocumentAddWidget(&Widgets->Document, Widgets->Document.PendingKind,
                                  LogicX, LogicY);
        BlocksUIGuiSetStatus(Widgets, "placed");
        BlocksUIGuiRefreshPropertyPanel(Widgets);
        BlocksUIGuiRefreshCanvas(Widgets);
        return;
    }
    BlocksUIDocumentSelectAt(&Widgets->Document, LogicX, LogicY);
    if (BlocksUIDocumentSelected(&Widgets->Document) != 0) {
        BlocksUIWidget *Selected = BlocksUIDocumentSelected(&Widgets->Document);

        Widgets->Dragging = 1;
        Widgets->DragOriginX = LogicX;
        Widgets->DragOriginY = LogicY;
        Widgets->DragWidgetStartX = Selected->X;
        Widgets->DragWidgetStartY = Selected->Y;
    }
    BlocksUIGuiRefreshPropertyPanel(Widgets);
    BlocksUIGuiRefreshCanvas(Widgets);
}

static void BlocksUIGuiCanvasOnReleased(GtkGestureClick *Gesture, int NPress,
                                        double X, double Y, gpointer UserData) {
    BlocksUIGuiWidgets *Widgets = (BlocksUIGuiWidgets *)UserData;

    (void)Gesture;
    (void)NPress;
    (void)X;
    (void)Y;
    Widgets->Dragging = 0;
}

static void BlocksUIGuiCanvasOnMotion(GtkEventControllerMotion *Controller,
                                      double X, double Y, gpointer UserData) {
    BlocksUIGuiWidgets *Widgets = (BlocksUIGuiWidgets *)UserData;
    BlocksUIWidget *Selected;
    int LogicX;
    int LogicY;

    (void)Controller;
    if (!Widgets->Dragging) {
        return;
    }
    Selected = BlocksUIDocumentSelected(&Widgets->Document);
    if (Selected == 0) {
        return;
    }
    BlocksUIGuiCanvasMapPoint(Widgets, X, Y, &LogicX, &LogicY);
    Selected->X = Widgets->DragWidgetStartX + (LogicX - Widgets->DragOriginX);
    Selected->Y = Widgets->DragWidgetStartY + (LogicY - Widgets->DragOriginY);
    if (Selected->X < 0) {
        Selected->X = 0;
    }
    if (Selected->Y < 0) {
        Selected->Y = 0;
    }
    BlocksUIGuiRefreshPropertyPanel(Widgets);
    BlocksUIGuiRefreshCanvas(Widgets);
}

void BlocksUIGuiCanvasBind(BlocksUIGuiWidgets *Widgets) {
    GtkGesture *Click;
    GtkEventController *Motion;
    int PixelWidth;
    int PixelHeight;

    PixelWidth = (int)(Widgets->Document.Width * Widgets->DisplayScale);
    PixelHeight = (int)(Widgets->Document.Height * Widgets->DisplayScale);
    gtk_drawing_area_set_content_width(Widgets->Canvas, PixelWidth);
    gtk_drawing_area_set_content_height(Widgets->Canvas, PixelHeight);
    gtk_drawing_area_set_draw_func(Widgets->Canvas, BlocksUIGuiCanvasOnDraw, Widgets,
                                   NULL);
    Click = gtk_gesture_click_new();
    g_signal_connect(Click, "pressed", G_CALLBACK(BlocksUIGuiCanvasOnPressed),
                     Widgets);
    g_signal_connect(Click, "released", G_CALLBACK(BlocksUIGuiCanvasOnReleased),
                     Widgets);
    gtk_widget_add_controller(GTK_WIDGET(Widgets->Canvas), GTK_EVENT_CONTROLLER(Click));
    Motion = gtk_event_controller_motion_new();
    g_signal_connect(Motion, "motion", G_CALLBACK(BlocksUIGuiCanvasOnMotion), Widgets);
    gtk_widget_add_controller(GTK_WIDGET(Widgets->Canvas), Motion);
}
