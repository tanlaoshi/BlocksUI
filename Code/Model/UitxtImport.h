#ifndef BLOCKS_UI_UITXT_IMPORT_H
#define BLOCKS_UI_UITXT_IMPORT_H

#include "Document.h"

/* 成功 0；失败 -1（缺文件 / 格式错）。失败时 Document 不变。 */
int BlocksUIUitxtImportPath(BlocksUIDocument *Document, const char *Path);

#endif
