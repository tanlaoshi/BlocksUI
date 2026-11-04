/*
 * PageGuide.c — 多页策略 A 区间提示（设计器轻量引导）
 */
#include "PageGuide.h"

#include <stdio.h>
#include <string.h>

static const BlocksUIPageGuide sBlocksUIPageGuides[BLOCKS_UI_PAGE_GUIDE_COUNT] = {
    {"Home", "home", 1000, 1099},
    {"Settings", "settings", 1100, 1199},
    {"About", "about", 1200, 1299},
    {"Files", "files", 1300, 1399},
    {"Nav", "nav", 9000, 9099},
};

const BlocksUIPageGuide *BlocksUIPageGuideAt(int PageIndex) {
    if (PageIndex < 0 || PageIndex >= BLOCKS_UI_PAGE_GUIDE_COUNT) {
        return &sBlocksUIPageGuides[0];
    }
    return &sBlocksUIPageGuides[PageIndex];
}

int BlocksUIPageGuideCount(void) {
    return BLOCKS_UI_PAGE_GUIDE_COUNT;
}

static int BlocksUIPageGuideWidgetBelongs(const BlocksUIWidget *Widget,
                                          const char *IdPrefix) {
    char Expected[BLOCKS_UI_ID_MAX];
    size_t PrefixLength;

    if (Widget == 0 || IdPrefix == 0 || IdPrefix[0] == 0) {
        return 0;
    }
    snprintf(Expected, sizeof(Expected), "%s_", IdPrefix);
    PrefixLength = strlen(Expected);
    return strncmp(Widget->Id, Expected, PrefixLength) == 0;
}

static int BlocksUIPageGuideCountPrefix(const BlocksUIDocument *Document,
                                        const char *IdPrefix) {
    int Index;
    int Count = 0;

    if (Document == 0) {
        return 0;
    }
    for (Index = 0; Index < Document->Count; Index++) {
        if (BlocksUIPageGuideWidgetBelongs(&Document->Widgets[Index], IdPrefix)) {
            Count++;
        }
    }
    return Count;
}

int BlocksUIPageGuideSuggestLogicId(const BlocksUIDocument *Document,
                                    int PageIndex) {
    const BlocksUIPageGuide *Guide = BlocksUIPageGuideAt(PageIndex);
    int Used = BlocksUIPageGuideCountPrefix(Document, Guide->IdPrefix);
    int Suggested = Guide->RangeBegin + Used;

    if (Suggested > Guide->RangeEnd) {
        return Guide->RangeEnd;
    }
    return Suggested;
}

int BlocksUIPageGuideLogicIdForWidget(const BlocksUIDocument *Document,
                                      int PageIndex, int WidgetIndex) {
    const BlocksUIPageGuide *Guide = BlocksUIPageGuideAt(PageIndex);
    int Index;
    int Ordinal = 0;

    if (Document == 0 || WidgetIndex < 0 || WidgetIndex >= Document->Count) {
        return -1;
    }
    if (!BlocksUIPageGuideWidgetBelongs(&Document->Widgets[WidgetIndex],
                                        Guide->IdPrefix)) {
        return -1;
    }
    for (Index = 0; Index < WidgetIndex; Index++) {
        if (BlocksUIPageGuideWidgetBelongs(&Document->Widgets[Index],
                                           Guide->IdPrefix)) {
            Ordinal++;
        }
    }
    return Guide->RangeBegin + Ordinal;
}
