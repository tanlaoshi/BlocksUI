#ifndef BLOCKS_UI_PAGE_GUIDE_H
#define BLOCKS_UI_PAGE_GUIDE_H

#include "Document.h"

/* 对表 Bingo/App/UI/多页面分发.md §3.1 策略 A */
typedef struct BlocksUIPageGuide {
    const char *Name;
    const char *IdPrefix;
    int RangeBegin;
    int RangeEnd;
} BlocksUIPageGuide;

#define BLOCKS_UI_PAGE_GUIDE_COUNT 5

const BlocksUIPageGuide *BlocksUIPageGuideAt(int PageIndex);
int BlocksUIPageGuideCount(void);
/* 本页已有前缀控件数 → 建议下一逻辑号（区间内） */
int BlocksUIPageGuideSuggestLogicId(const BlocksUIDocument *Document,
                                    int PageIndex);
/* 选中控件在本页区间内的建议逻辑号；不属于本页前缀则 -1 */
int BlocksUIPageGuideLogicIdForWidget(const BlocksUIDocument *Document,
                                      int PageIndex, int WidgetIndex);

#endif
