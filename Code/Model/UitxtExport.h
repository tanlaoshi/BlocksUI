#ifndef BLOCKS_UI_UITXT_EXPORT_H
#define BLOCKS_UI_UITXT_EXPORT_H

#include "Document.h"

/* 成功返回 0；失败 -1（路径/写盘） */
int BlocksUIUitxtExportPath(const BlocksUIDocument *Document, const char *Path);

#endif
