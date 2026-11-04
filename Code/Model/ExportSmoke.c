/*
 * ExportSmoke.c — 无 GUI 冒烟：写 /tmp/blocksui-smoke.uitxt
 */
#include "Document.h"
#include "UitxtExport.h"

#include <stdio.h>

int main(void) {
    BlocksUIDocument Document;

    BlocksUIDocumentInitialize(&Document);
    BlocksUIDocumentAddWidget(&Document, BLOCKS_UI_WIDGET_LABEL, 20, 20);
    BlocksUIDocumentAddWidget(&Document, BLOCKS_UI_WIDGET_BUTTON, 20, 60);
    if (BlocksUIUitxtExportPath(&Document, "/tmp/blocksui-smoke.uitxt") != 0) {
        fprintf(stderr, "export failed\n");
        return 1;
    }
    printf("wrote /tmp/blocksui-smoke.uitxt count=%d\n", Document.Count);
    return 0;
}
