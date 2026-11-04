/*
 * ImportSmoke.c — .uitxt 导入再导出 round-trip
 */
#include "Document.h"
#include "UitxtExport.h"
#include "UitxtImport.h"

#include <stdio.h>
#include <string.h>

int main(void) {
    BlocksUIDocument Document;
    const char *Sample = "/tmp/blocksui-import-smoke.uitxt";
    const char *Out = "/tmp/blocksui-import-smoke-out.uitxt";
    FILE *File;

    File = fopen(Sample, "w");
    if (File == 0) {
        return 1;
    }
    fputs("# smoke\n", File);
    fputs("window \"Smoke\" 1280 720\n", File);
    fputs("button id=ok x=40 y=40 w=80 h=28 text=\"OK\"\n", File);
    fputs("label id=hello x=40 y=80 w=120 h=24 text=\"Hi\"\n", File);
    fclose(File);

    BlocksUIDocumentInitialize(&Document);
    if (BlocksUIUitxtImportPath(&Document, Sample) != 0) {
        fprintf(stderr, "import failed\n");
        return 2;
    }
    if (Document.Count != 2 || strcmp(Document.Title, "Smoke") != 0) {
        fprintf(stderr, "bad document after import\n");
        return 3;
    }
    if (BlocksUIUitxtExportPath(&Document, Out) != 0) {
        fprintf(stderr, "export failed\n");
        return 4;
    }
    printf("import-smoke ok count=%d title=%s\n", Document.Count, Document.Title);
    return 0;
}
