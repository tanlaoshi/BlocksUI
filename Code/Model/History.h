#ifndef BLOCKS_UI_HISTORY_H
#define BLOCKS_UI_HISTORY_H

#include "Document.h"

#define BLOCKS_UI_HISTORY_MAX 100

typedef struct BlocksUIHistory {
    BlocksUIDocument UndoStack[BLOCKS_UI_HISTORY_MAX];
    int UndoCount;
    BlocksUIDocument RedoStack[BLOCKS_UI_HISTORY_MAX];
    int RedoCount;
} BlocksUIHistory;

void BlocksUIHistoryInitialize(BlocksUIHistory *History);
void BlocksUIHistoryPushUndo(BlocksUIHistory *History,
                             const BlocksUIDocument *Document);
int BlocksUIHistoryUndo(BlocksUIHistory *History, BlocksUIDocument *Document);
int BlocksUIHistoryRedo(BlocksUIHistory *History, BlocksUIDocument *Document);
int BlocksUIHistoryUndoCount(const BlocksUIHistory *History);
int BlocksUIHistoryRedoCount(const BlocksUIHistory *History);

/* 坐标吸到 Grid（设计器网格 40） */
int BlocksUISnapToGrid(int Value, int Grid);

#endif
