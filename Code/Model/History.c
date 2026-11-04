/*
 * History.c — 文档快照撤销 / 重做
 */
#include "History.h"

#include <string.h>

void BlocksUIHistoryInitialize(BlocksUIHistory *History) {
    if (History == 0) {
        return;
    }
    memset(History, 0, sizeof(*History));
}

static int BlocksUIHistoryDocumentsEqual(const BlocksUIDocument *Left,
                                         const BlocksUIDocument *Right) {
    return memcmp(Left, Right, sizeof(*Left)) == 0;
}

void BlocksUIHistoryPushUndo(BlocksUIHistory *History,
                             const BlocksUIDocument *Document) {
    if (History == 0 || Document == 0) {
        return;
    }
    if (History->UndoCount > 0 &&
        BlocksUIHistoryDocumentsEqual(&History->UndoStack[History->UndoCount - 1],
                                      Document)) {
        return;
    }
    if (History->UndoCount >= BLOCKS_UI_HISTORY_MAX) {
        memmove(&History->UndoStack[0], &History->UndoStack[1],
                sizeof(BlocksUIDocument) * (BLOCKS_UI_HISTORY_MAX - 1));
        History->UndoCount = BLOCKS_UI_HISTORY_MAX - 1;
    }
    History->UndoStack[History->UndoCount] = *Document;
    History->UndoCount++;
    History->RedoCount = 0;
}

int BlocksUIHistoryUndo(BlocksUIHistory *History, BlocksUIDocument *Document) {
    if (History == 0 || Document == 0 || History->UndoCount <= 0) {
        return 0;
    }
    if (History->RedoCount >= BLOCKS_UI_HISTORY_MAX) {
        memmove(&History->RedoStack[0], &History->RedoStack[1],
                sizeof(BlocksUIDocument) * (BLOCKS_UI_HISTORY_MAX - 1));
        History->RedoCount = BLOCKS_UI_HISTORY_MAX - 1;
    }
    History->RedoStack[History->RedoCount] = *Document;
    History->RedoCount++;
    History->UndoCount--;
    *Document = History->UndoStack[History->UndoCount];
    return 1;
}

int BlocksUIHistoryRedo(BlocksUIHistory *History, BlocksUIDocument *Document) {
    if (History == 0 || Document == 0 || History->RedoCount <= 0) {
        return 0;
    }
    if (History->UndoCount >= BLOCKS_UI_HISTORY_MAX) {
        memmove(&History->UndoStack[0], &History->UndoStack[1],
                sizeof(BlocksUIDocument) * (BLOCKS_UI_HISTORY_MAX - 1));
        History->UndoCount = BLOCKS_UI_HISTORY_MAX - 1;
    }
    History->UndoStack[History->UndoCount] = *Document;
    History->UndoCount++;
    History->RedoCount--;
    *Document = History->RedoStack[History->RedoCount];
    return 1;
}

int BlocksUIHistoryUndoCount(const BlocksUIHistory *History) {
    return History != 0 ? History->UndoCount : 0;
}

int BlocksUIHistoryRedoCount(const BlocksUIHistory *History) {
    return History != 0 ? History->RedoCount : 0;
}

int BlocksUISnapToGrid(int Value, int Grid) {
    int Half;

    if (Grid <= 1) {
        return Value;
    }
    Half = Grid / 2;
    if (Value >= 0) {
        return ((Value + Half) / Grid) * Grid;
    }
    return -(((-Value) + Half) / Grid) * Grid;
}
