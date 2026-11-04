/*
 * UitxtImport.c — 读入 Bingo 约定 .uitxt
 */
#include "UitxtImport.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void BlocksUIUitxtSkipSpaces(const char **Cursor) {
    while (**Cursor != 0 && isspace((unsigned char)**Cursor)) {
        (*Cursor)++;
    }
}

static int BlocksUIUitxtParseQuoted(const char **Cursor, char *Out, int OutMax) {
    int Length = 0;

    if (**Cursor != '"') {
        return -1;
    }
    (*Cursor)++;
    while (**Cursor != 0 && **Cursor != '"') {
        char Character = **Cursor;

        if (Character == '\\' && (*Cursor)[1] != 0) {
            (*Cursor)++;
            Character = **Cursor;
        }
        if (Length + 1 >= OutMax) {
            return -1;
        }
        Out[Length++] = Character;
        (*Cursor)++;
    }
    if (**Cursor != '"') {
        return -1;
    }
    (*Cursor)++;
    Out[Length] = 0;
    return 0;
}

static int BlocksUIUitxtParseWindow(const char *Line, BlocksUIDocument *Document) {
    const char *Cursor = Line;
    char Title[BLOCKS_UI_TITLE_MAX];
    int Width = 0;
    int Height = 0;

    if (strncmp(Cursor, "window", 6) != 0) {
        return -1;
    }
    Cursor += 6;
    BlocksUIUitxtSkipSpaces(&Cursor);
    if (BlocksUIUitxtParseQuoted(&Cursor, Title, (int)sizeof(Title)) != 0) {
        return -1;
    }
    BlocksUIUitxtSkipSpaces(&Cursor);
    if (sscanf(Cursor, "%d %d", &Width, &Height) != 2 || Width <= 0 || Height <= 0) {
        return -1;
    }
    snprintf(Document->Title, sizeof(Document->Title), "%s", Title);
    Document->Width = Width;
    Document->Height = Height;
    return 0;
}

static int BlocksUIUitxtParseFieldToken(const char **Cursor, char *Key, int KeyMax,
                                        char *Value, int ValueMax) {
    int KeyLength = 0;

    BlocksUIUitxtSkipSpaces(Cursor);
    if (**Cursor == 0) {
        return 1;
    }
    while (**Cursor != 0 && **Cursor != '=' && !isspace((unsigned char)**Cursor)) {
        if (KeyLength + 1 >= KeyMax) {
            return -1;
        }
        Key[KeyLength++] = **Cursor;
        (*Cursor)++;
    }
    Key[KeyLength] = 0;
    BlocksUIUitxtSkipSpaces(Cursor);
    if (**Cursor != '=') {
        return -1;
    }
    (*Cursor)++;
    BlocksUIUitxtSkipSpaces(Cursor);
    if (**Cursor == '"') {
        return BlocksUIUitxtParseQuoted(Cursor, Value, ValueMax);
    }
    {
        int ValueLength = 0;

        while (**Cursor != 0 && !isspace((unsigned char)**Cursor)) {
            if (ValueLength + 1 >= ValueMax) {
                return -1;
            }
            Value[ValueLength++] = **Cursor;
            (*Cursor)++;
        }
        Value[ValueLength] = 0;
    }
    return 0;
}

static int BlocksUIUitxtParseWidgetLine(const char *Line, BlocksUIDocument *Document) {
    char KindName[32];
    char Key[32];
    char Value[BLOCKS_UI_TEXT_MAX];
    const char *Cursor = Line;
    BlocksUIWidget Widget;
    int HaveId = 0;
    int HaveX = 0;
    int HaveY = 0;
    int HaveWidth = 0;
    int HaveHeight = 0;
    int Index;
    int KindLength = 0;

    memset(&Widget, 0, sizeof(Widget));
    while (*Cursor != 0 && !isspace((unsigned char)*Cursor)) {
        if (KindLength + 1 >= (int)sizeof(KindName)) {
            return -1;
        }
        KindName[KindLength++] = (char)tolower((unsigned char)*Cursor);
        Cursor++;
    }
    KindName[KindLength] = 0;
    if (strcmp(KindName, "label") != 0 && strcmp(KindName, "button") != 0 &&
        strcmp(KindName, "checkbox") != 0 && strcmp(KindName, "textbox") != 0) {
        return -1;
    }
    Widget.Kind = BlocksUIWidgetKindFromName(KindName);
    for (;;) {
        int FieldStatus =
            BlocksUIUitxtParseFieldToken(&Cursor, Key, (int)sizeof(Key), Value,
                                        (int)sizeof(Value));

        if (FieldStatus > 0) {
            break;
        }
        if (FieldStatus < 0) {
            return -1;
        }
        if (strcmp(Key, "id") == 0) {
            if (strlen(Value) >= sizeof(Widget.Id)) {
                return -1;
            }
            memcpy(Widget.Id, Value, strlen(Value) + 1);
            HaveId = 1;
        } else if (strcmp(Key, "x") == 0) {
            Widget.X = atoi(Value);
            HaveX = 1;
        } else if (strcmp(Key, "y") == 0) {
            Widget.Y = atoi(Value);
            HaveY = 1;
        } else if (strcmp(Key, "w") == 0) {
            Widget.Width = atoi(Value);
            HaveWidth = 1;
        } else if (strcmp(Key, "h") == 0) {
            Widget.Height = atoi(Value);
            HaveHeight = 1;
        } else if (strcmp(Key, "text") == 0) {
            snprintf(Widget.Text, sizeof(Widget.Text), "%s", Value);
        }
    }
    if (!HaveId || !HaveX || !HaveY || !HaveWidth || !HaveHeight) {
        return -1;
    }
    if (Document->Count >= BLOCKS_UI_WIDGET_MAX) {
        return -1;
    }
    for (Index = 0; Index < Document->Count; Index++) {
        if (strcmp(Document->Widgets[Index].Id, Widget.Id) == 0) {
            return -1;
        }
    }
    Document->Widgets[Document->Count] = Widget;
    Document->Count++;
    return 0;
}

int BlocksUIUitxtImportPath(BlocksUIDocument *Document, const char *Path) {
    FILE *File;
    char Line[512];
    BlocksUIDocument Loaded;
    int SawWindow = 0;

    if (Document == 0 || Path == 0 || Path[0] == 0) {
        return -1;
    }
    File = fopen(Path, "r");
    if (File == 0) {
        return -1;
    }
    BlocksUIDocumentInitialize(&Loaded);
    Loaded.Count = 0;
    while (fgets(Line, (int)sizeof(Line), File) != 0) {
        char *Start = Line;
        char *End;

        while (*Start != 0 && isspace((unsigned char)*Start)) {
            Start++;
        }
        End = Start + strlen(Start);
        while (End > Start && isspace((unsigned char)End[-1])) {
            End--;
        }
        *End = 0;
        if (Start[0] == 0 || Start[0] == '#') {
            continue;
        }
        if (!SawWindow) {
            if (BlocksUIUitxtParseWindow(Start, &Loaded) != 0) {
                fclose(File);
                return -1;
            }
            SawWindow = 1;
            continue;
        }
        if (BlocksUIUitxtParseWidgetLine(Start, &Loaded) != 0) {
            fclose(File);
            return -1;
        }
    }
    fclose(File);
    if (!SawWindow) {
        return -1;
    }
    Loaded.SelectedIndex = Loaded.Count > 0 ? 0 : -1;
    Loaded.HasPendingPlace = 0;
    *Document = Loaded;
    return 0;
}
