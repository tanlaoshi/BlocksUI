/*
 * Locale.c — 中/英界面文案表
 */
#include "Locale.h"

static const char *sBlocksUILocaleEnglish[BLOCKS_UI_TEXT_COUNT] = {
    "Undo",
    "Redo",
    "Open",
    "Save",
    "Save As",
    "Delete",
    "Widgets",
    "Button",
    "Label",
    "CheckBox",
    "TextBox",
    "Properties",
    "Page (ID ranges)",
    "window",
    "Canvas 1280×720",
    "中文",
    "ready",
    "drag widget onto canvas",
    "Open .uitxt",
    "Save .uitxt",
    "save failed",
    "open failed",
    "saved",
    "opened",
    "undo %d  redo %d",
    "logic —",
    "logic %d  (enum hint)",
    "logic — (id not %s_*)",
    "%s  %d–%d\nnext logic %d\nid prefix %s_",
    "Home",
    "Settings",
    "About",
    "Files",
    "Nav",
};

static const char *sBlocksUILocaleChinese[BLOCKS_UI_TEXT_COUNT] = {
    "撤销",
    "重做",
    "打开",
    "保存",
    "另存为",
    "删除",
    "控件",
    "按钮",
    "标签",
    "复选框",
    "文本框",
    "属性",
    "页面（ID 区间）",
    "窗口",
    "画布 1280×720",
    "EN",
    "就绪",
    "从边栏拖到画布",
    "打开 .uitxt",
    "保存 .uitxt",
    "保存失败",
    "打开失败",
    "已保存",
    "已打开",
    "撤销 %d  重做 %d",
    "逻辑号 —",
    "逻辑号 %d（枚举提示）",
    "逻辑号 —（id 非 %s_*）",
    "%s  %d–%d\n下一逻辑号 %d\nid 前缀 %s_",
    "首页",
    "设置",
    "关于",
    "文件",
    "导航",
};

const char *BlocksUILocaleText(BlocksUILanguage Language, BlocksUILocaleTextId Id) {
    if (Id < 0 || Id >= BLOCKS_UI_TEXT_COUNT) {
        return "";
    }
    if (Language == BLOCKS_UI_LANGUAGE_CHINESE) {
        return sBlocksUILocaleChinese[Id];
    }
    return sBlocksUILocaleEnglish[Id];
}
