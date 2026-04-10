// EmEditor Plugin SDK Header
// Based on EmEditor Plugin Reference

#ifndef _EMEDITOR_PLUGIN_H_
#define _EMEDITOR_PLUGIN_H_

#ifdef __cplusplus
extern "C" {
#endif

// Messages
#define EE_GET_VERSION              (WM_USER + 1)
#define EE_GET_SEL_TEXT             (WM_USER + 2)
#define EE_GET_LINES                (WM_USER + 3)
#define EE_GET_LINE                 (WM_USER + 4)
#define EE_GET_CARET_POS            (WM_USER + 5)
#define EE_SET_CARET_POS            (WM_USER + 6)
#define EE_GET_SCROLL_POS           (WM_USER + 7)
#define EE_SET_SCROLL_POS           (WM_USER + 8)
#define EE_INSERT_TEXT              (WM_USER + 9)
#define EE_DELETE_TEXT              (WM_USER + 10)
#define EE_GET_SEL_START            (WM_USER + 11)
#define EE_GET_SEL_END              (WM_USER + 12)
#define EE_SET_SEL                  (WM_USER + 13)
#define EE_REPLACE_TEXT             (WM_USER + 14)
#define EE_GET_FILENAME             (WM_USER + 15)
#define EE_IS_MODIFIED              (WM_USER + 16)
#define EE_SET_MODIFIED             (WM_USER + 17)
#define EE_GET_CONFIG               (WM_USER + 18)
#define EE_SET_CONFIG               (WM_USER + 19)
#define EE_GET_DOC_COUNT            (WM_USER + 20)
#define EE_GET_DOC_NAME             (WM_USER + 21)
#define EE_SET_DOC_NAME             (WM_USER + 22)
#define EE_NEW_DOC                  (WM_USER + 23)
#define EE_OPEN_DOC                 (WM_USER + 24)
#define EE_CLOSE_DOC                (WM_USER + 25)
#define EE_ACTIVATE_DOC             (WM_USER + 26)
#define EE_SAVE_DOC                 (WM_USER + 27)
#define EE_EXEC_COMMAND             (WM_USER + 28)
#define EE_GET_ATTR                 (WM_USER + 29)
#define EE_SET_ATTR                 (WM_USER + 30)
#define EE_GET_COLOR                (WM_USER + 31)
#define EE_SET_COLOR                (WM_USER + 32)
#define EE_GET_STATUS               (WM_USER + 33)
#define EE_SET_STATUS               (WM_USER + 34)
#define EE_GET_LINE_COUNT           (WM_USER + 35)
#define EE_GET_COLUMN               (WM_USER + 36)
#define EE_CONVERT                  (WM_USER + 37)
#define EE_FIND_REPLACE             (WM_USER + 38)
#define EE_GET_CLIP_TEXT            (WM_USER + 39)
#define EE_SET_CLIP_TEXT            (WM_USER + 40)
#define EE_IS_READONLY              (WM_USER + 41)
#define EE_SET_READONLY             (WM_USER + 42)
#define EE_GET_TEXT                 (WM_USER + 43)
#define EE_SET_TEXT                 (WM_USER + 44)
#define EE_GET_SEL_TYPE             (WM_USER + 45)
#define EE_IS_CHAR_HALF_OR_FULL     (WM_USER + 46)
#define EE_GET_ACTIVE_STRING        (WM_USER + 47)
#define EE_GET_REDO_COUNT           (WM_USER + 48)
#define EE_GET_UNDO_COUNT           (WM_USER + 49)
#define EE_CAN_UNDO                 (WM_USER + 50)
#define EE_CAN_REDO                 (WM_USER + 51)
#define EE_UNDO                     (WM_USER + 52)
#define EE_REDO                     (WM_USER + 53)
#define EE_EMPTY_UNDO_BUFFER        (WM_USER + 54)
#define EE_GET_CODEPAGE             (WM_USER + 55)
#define EE_SET_CODEPAGE             (WM_USER + 56)
#define EE_GET_LINE_CODEPAGE        (WM_USER + 57)
#define EE_GET_OUTPUT_STRING        (WM_USER + 58)
#define EE_OUTPUT_STRING            (WM_USER + 59)
#define EE_CLEAR_OUTPUT             (WM_USER + 60)
#define EE_ENABLE_OUTPUT            (WM_USER + 61)
#define EE_GET_OUTPUT_DIR           (WM_USER + 62)
#define EE_SET_OUTPUT_DIR           (WM_USER + 63)
#define EE_GET_DPI                  (WM_USER + 64)
#define EE_GET_ZOOM                 (WM_USER + 65)
#define EE_SET_ZOOM                 (WM_USER + 66)
#define EE_GET_DIGIT                (WM_USER + 67)
#define EE_SET_DIGIT                (WM_USER + 68)
#define EE_GET_HILITE               (WM_USER + 69)
#define EE_SET_HILITE               (WM_USER + 70)
#define EE_GET_ANCHOR_POS           (WM_USER + 71)
#define EE_SET_ANCHOR_POS           (WM_USER + 72)
#define EE_GET_MARGIN               (WM_USER + 73)
#define EE_SET_MARGIN               (WM_USER + 74)
#define EE_GET_TAB_STOP             (WM_USER + 75)
#define EE_SET_TAB_STOP             (WM_USER + 76)
#define EE_GET_WRAP_MODE            (WM_USER + 77)
#define EE_SET_WRAP_MODE            (WM_USER + 78)
#define EE_GET_SEL_TYPE_EX          (WM_USER + 79)
#define EE_GET_WORD                 (WM_USER + 80)
#define EE_IS_DOC_MODIFIED          (WM_USER + 81)
#define EE_GET_MODIFIED_DOCS        (WM_USER + 82)
#define EE_GET_STRINGS              (WM_USER + 83)
#define EE_GET_MARKED_POS           (WM_USER + 84)
#define EE_SET_MARKED_POS           (WM_USER + 85)
#define EE_ENUM_CONFIG              (WM_USER + 86)
#define EE_SAVE_FILE                (WM_USER + 87)
#define EE_LOGICAL_TO_SERIAL        (WM_USER + 88)
#define EE_SERIAL_TO_LOGICAL        (WM_USER + 89)
#define EE_LOGICAL_TO_VIEW          (WM_USER + 90)
#define EE_VIEW_TO_LOGICAL          (WM_USER + 91)
#define EE_GET_CELL                 (WM_USER + 92)
#define EE_SET_CELL                 (WM_USER + 93)
#define EE_GET_COLUMN               (WM_USER + 94)
#define EE_SET_COLUMN               (WM_USER + 95)
#define EE_GET_TITLE                (WM_USER + 96)
#define EE_SET_TITLE                (WM_USER + 97)
#define EE_GET_IDLE_COUNT           (WM_USER + 98)
#define EE_GET_INFO                 (WM_USER + 99)
#define EE_GET_OUTPUT_ENCODING      (WM_USER + 100)
#define EE_SET_OUTPUT_ENCODING      (WM_USER + 101)
#define EE_GET_FIND_REPLACE_HISTORY (WM_USER + 102)
#define EE_GET_GREP_HISTORY         (WM_USER + 103)
#define EE_GET_BAR_TEXT             (WM_USER + 104)
#define EE_SET_BAR_TEXT             (WM_USER + 105)
#define EE_GET_LINE_TOP             (WM_USER + 106)
#define EE_GET_SHOW_LINE_NUMBERS    (WM_USER + 107)
#define EE_SET_SHOW_LINE_NUMBERS    (WM_USER + 108)
#define EE_GET_SHOW_BOOKMARKS       (WM_USER + 109)
#define EE_SET_SHOW_BOOKMARKS       (WM_USER + 110)
#define EE_GET_PAINT_TEXT           (WM_USER + 111)
#define EE_GET_CUSTOM_BAR           (WM_USER + 112)
#define EE_CUSTOM_BAR_CLOSE         (WM_USER + 113)
#define EE_CUSTOM_BAR_OPEN          (WM_USER + 114)
#define EE_GET_TEXT_RANGE           (WM_USER + 115)
#define EE_GET_TEXT_RANGE_W         (WM_USER + 116)
#define EE_GET_CURRENT_MACRO        (WM_USER + 117)
#define EE_GET_BOOKMARK_COUNT       (WM_USER + 118)
#define EE_GET_CMD_ID               (WM_USER + 119)
#define EE_SHOW_TOOLTIP             (WM_USER + 120)
#define EE_GET_NATIVE_LANG          (WM_USER + 121)
#define EE_GET_NATIVE_LANG_NAME     (WM_USER + 122)
#define EE_GET_SYNC_DIR             (WM_USER + 123)
#define EE_SET_SYNC_DIR             (WM_USER + 124)
#define EE_LOAD_CONFIG              (WM_USER + 125)
#define EE_SET_ENCRYPTION           (WM_USER + 126)
#define EE_GET_ENCRYPTION           (WM_USER + 127)
#define EE_GET_PANES                (WM_USER + 128)
#define EE_GET_ACTIVE_PANE          (WM_USER + 129)
#define EE_GET_FULL_TITLE           (WM_USER + 130)
#define EE_GET_SHADOW               (WM_USER + 131)
#define EE_SET_SHADOW               (WM_USER + 132)
#define EE_GET_BUFFER               (WM_USER + 133)
#define EE_GET_VIEW                 (WM_USER + 134)
#define EE_GET_LAST_ERROR           (WM_USER + 135)
#define EE_GET_FRAME                (WM_USER + 136)
#define EE_SET_SEL_VIEW             (WM_USER + 137)
#define EE_GET_REF                  (WM_USER + 138)
#define EE_ADD_REF                  (WM_USER + 139)
#define EE_RELEASE                  (WM_USER + 140)
#define EE_GET_VERSION_EX           (WM_USER + 141)
#define EE_GET_RETRIEVE_COUNT       (WM_USER + 142)
#define EE_GET_COMMAND_ID           (WM_USER + 143)
#define EE_GET_CHAR_COLOR           (WM_USER + 144)
#define EE_GET_ATTR_EX              (WM_USER + 145)
#define EE_GET_OUTPUT_FONT          (WM_USER + 146)
#define EE_SET_OUTPUT_FONT          (WM_USER + 147)
#define EE_GET_SHORTCUTS            (WM_USER + 148)
#define EE_GET_COMMAND_ICON         (WM_USER + 149)
#define EE_GET_DROPPED_FILE         (WM_USER + 150)
#define EE_GET_REGISTER_NAME        (WM_USER + 151)
#define EE_GET_WND_PPI              (WM_USER + 152)
#define EE_SET_WND_PPI              (WM_USER + 153)
#define EE_GET_LANGUAGE_NAME        (WM_USER + 154)
#define EE_GET_TAG_EXISTS           (WM_USER + 155)
#define EE_GET_TAG_STRING           (WM_USER + 156)
#define EE_GET_TAG_RANGE            (WM_USER + 157)
#define EE_GET_TAG_POS              (WM_USER + 158)
#define EE_GET_TAG_ATTR             (WM_USER + 159)
#define EE_GET_TAG_ATTR_RANGE       (WM_USER + 160)
#define EE_GET_TAG_ATTR_POS         (WM_USER + 161)
#define EE_GET_TAG_PARENT           (WM_USER + 162)
#define EE_GET_TAG_CHILD            (WM_USER + 163)
#define EE_GET_TAG_SIBLING          (WM_USER + 164)
#define EE_ENUM_TAG_ATTR            (WM_USER + 165)
#define EE_GET_TAG_NAME_RANGE       (WM_USER + 166)
#define EE_GET_TAG_NAME_POS         (WM_USER + 167)
#define EE_GET_TAG_TEXT_RANGE       (WM_USER + 168)
#define EE_GET_TAG_TEXT_POS         (WM_USER + 169)
#define EE_IS_TAG_VALID             (WM_USER + 170)
#define EE_GET_TAG_COUNT            (WM_USER + 171)
#define EE_GET_TAG_INDEX            (WM_USER + 172)
#define EE_GET_TAG_BY_INDEX         (WM_USER + 173)
#define EE_GET_TAG_BY_POS           (WM_USER + 174)
#define EE_GET_TAG_BY_NAME          (WM_USER + 175)
#define EE_GET_TAG_BY_ATTR          (WM_USER + 176)
#define EE_GET_TAG_BY_TEXT          (WM_USER + 177)
#define EE_GET_TAG_BY_LEVEL         (WM_USER + 178)
#define EE_GET_TAG_LEVEL            (WM_USER + 179)
#define EE_GET_TAG_TYPE             (WM_USER + 180)
#define EE_GET_TAG_TEXT             (WM_USER + 181)
#define EE_GET_TAG_ATTR_COUNT       (WM_USER + 182)
#define EE_GET_TAG_ATTR_BY_INDEX    (WM_USER + 183)
#define EE_GET_TAG_ATTR_BY_NAME     (WM_USER + 184)
#define EE_GET_TAG_ATTR_BY_VALUE    (WM_USER + 185)
#define EE_GET_TAG_ATTR_NAME        (WM_USER + 186)
#define EE_GET_TAG_ATTR_VALUE       (WM_USER + 187)
#define EE_GET_TAG_ATTR_VALUE_INT   (WM_USER + 188)
#define EE_GET_TAG_ATTR_VALUE_BOOL  (WM_USER + 189)
#define EE_GET_TAG_ATTR_VALUE_FLOAT (WM_USER + 190)
#define EE_GET_TAG_ATTR_VALUE_COLOR (WM_USER + 191)
#define EE_GET_TAG_ATTR_VALUE_RECT  (WM_USER + 192)
#define EE_GET_TAG_ATTR_VALUE_POINT (WM_USER + 193)
#define EE_GET_TAG_ATTR_VALUE_SIZE  (WM_USER + 194)
#define EE_GET_TAG_ATTR_VALUE_FONT  (WM_USER + 195)
#define EE_GET_TAG_ATTR_VALUE_PEN   (WM_USER + 196)
#define EE_GET_TAG_ATTR_VALUE_BRUSH (WM_USER + 197)
#define EE_GET_TAG_ATTR_VALUE_IMAGE (WM_USER + 198)
#define EE_GET_TAG_ATTR_VALUE_ICON  (WM_USER + 199)
#define EE_GET_TAG_ATTR_VALUE_CURSOR (WM_USER + 200)
#define EE_GET_TAG_ATTR_VALUE_MENU  (WM_USER + 201)
#define EE_GET_TAG_ATTR_VALUE_ACCEL (WM_USER + 202)
#define EE_GET_TAG_ATTR_VALUE_WND   (WM_USER + 203)
#define EE_GET_TAG_ATTR_VALUE_REGION (WM_USER + 204)
#define EE_GET_TAG_ATTR_VALUE_PATH  (WM_USER + 205)
#define EE_GET_TAG_ATTR_VALUE_STRING (WM_USER + 206)
#define EE_GET_TAG_ATTR_VALUE_BINARY (WM_USER + 207)
#define EE_GET_TAG_ATTR_VALUE_CUSTOM (WM_USER + 208)

// Events
#define EVENT_SEL_CHANGED           1
#define EVENT_CARET_MOVED           2
#define EVENT_SCROLL                3
#define EVENT_MODIFIED              4
#define EVENT_DOC_CHANGED           5
#define EVENT_DOC_MODIFIED          6
#define EVENT_DOC_SAVED             7
#define EVENT_DOC_CLOSED            8
#define EVENT_DOC_OPENED            9
#define EVENT_DOC_CREATED           10
#define EVENT_DOC_ACTIVATED         11
#define EVENT_DOC_DEACTIVATED       12
#define EVENT_DOC_RENAMED           13
#define EVENT_CONFIG_CHANGED        14
#define EVENT_COLOR_CHANGED         15
#define EVENT_FONT_CHANGED          16
#define EVENT_ZOOM_CHANGED          17
#define EVENT_TAB_STOP_CHANGED      18
#define EVENT_WRAP_MODE_CHANGED     19
#define EVENT_MARGIN_CHANGED        20
#define EVENT_SHOW_LINE_NUMBERS_CHANGED 21
#define EVENT_SHOW_BOOKMARKS_CHANGED    22
#define EVENT_SHOW_HILITE_CHANGED       23
#define EVENT_SHOW_DIGIT_CHANGED        24
#define EVENT_SHOW_ANCHOR_CHANGED       25
#define EVENT_SHOW_MARKED_CHANGED       26
#define EVENT_SHOW_STATUS_CHANGED       27
#define EVENT_SHOW_BAR_CHANGED          28
#define EVENT_SHOW_OUTPUT_CHANGED       29
#define EVENT_SHOW_CUSTOM_BAR_CHANGED   30
#define EVENT_CUSTOM_BAR_CLOSED         31
#define EVENT_CUSTOM_BAR_OPENED         32
#define EVENT_IDLE                      33
#define EVENT_PRE_SAVE                  34
#define EVENT_POST_SAVE                 35
#define EVENT_PRE_CLOSE                 36
#define EVENT_POST_CLOSE                37
#define EVENT_PRE_OPEN                  38
#define EVENT_POST_OPEN                 39
#define EVENT_PRE_NEW                   40
#define EVENT_POST_NEW                  41
#define EVENT_PRE_EXEC_COMMAND          42
#define EVENT_POST_EXEC_COMMAND         43
#define EVENT_PRE_FIND_REPLACE          44
#define EVENT_POST_FIND_REPLACE         45
#define EVENT_PRE_GREP                  46
#define EVENT_POST_GREP                 47
#define EVENT_PRE_CONVERT               48
#define EVENT_POST_CONVERT              49
#define EVENT_PRE_INSERT_TEXT           50
#define EVENT_POST_INSERT_TEXT          51
#define EVENT_PRE_DELETE_TEXT           52
#define EVENT_POST_DELETE_TEXT          53
#define EVENT_PRE_REPLACE_TEXT          54
#define EVENT_POST_REPLACE_TEXT         55
#define EVENT_PRE_SET_SEL               56
#define EVENT_POST_SET_SEL              57
#define EVENT_PRE_SET_CARET_POS         58
#define EVENT_POST_SET_CARET_POS        59
#define EVENT_PRE_SET_SCROLL_POS        60
#define EVENT_POST_SET_SCROLL_POS       61
#define EVENT_PRE_SET_ATTR              62
#define EVENT_POST_SET_ATTR             63
#define EVENT_PRE_SET_COLOR             64
#define EVENT_POST_SET_COLOR            65
#define EVENT_PRE_SET_STATUS            66
#define EVENT_POST_SET_STATUS           67
#define EVENT_PRE_SET_CONFIG            68
#define EVENT_POST_SET_CONFIG           69
#define EVENT_PRE_SET_CODEPAGE          70
#define EVENT_POST_SET_CODEPAGE         71
#define EVENT_PRE_SET_OUTPUT_ENCODING   72
#define EVENT_POST_SET_OUTPUT_ENCODING  73
#define EVENT_PRE_SET_OUTPUT_FONT       74
#define EVENT_POST_SET_OUTPUT_FONT      75
#define EVENT_PRE_SET_OUTPUT_DIR        76
#define EVENT_POST_SET_OUTPUT_DIR       77
#define EVENT_PRE_SET_ZOOM              78
#define EVENT_POST_SET_ZOOM             79
#define EVENT_PRE_SET_DIGIT             80
#define EVENT_POST_SET_DIGIT            81
#define EVENT_PRE_SET_HILITE            82
#define EVENT_POST_SET_HILITE           83
#define EVENT_PRE_SET_ANCHOR_POS        84
#define EVENT_POST_SET_ANCHOR_POS       85
#define EVENT_PRE_SET_MARKED_POS        86
#define EVENT_POST_SET_MARKED_POS       87
#define EVENT_PRE_SET_MARGIN            88
#define EVENT_POST_SET_MARGIN           89
#define EVENT_PRE_SET_TAB_STOP          90
#define EVENT_POST_SET_TAB_STOP         91
#define EVENT_PRE_SET_WRAP_MODE         92
#define EVENT_POST_SET_WRAP_MODE        93
#define EVENT_PRE_SET_READONLY          94
#define EVENT_POST_SET_READONLY         95
#define EVENT_PRE_SET_MODIFIED          96
#define EVENT_POST_SET_MODIFIED         97
#define EVENT_PRE_SET_TITLE             98
#define EVENT_POST_SET_TITLE            99
#define EVENT_PRE_SET_ENCRYPTION        100
#define EVENT_POST_SET_ENCRYPTION       101
#define EVENT_PRE_SET_SHADOW            102
#define EVENT_POST_SET_SHADOW           103
#define EVENT_PRE_SET_SYNC_DIR          104
#define EVENT_POST_SET_SYNC_DIR         105
#define EVENT_PRE_LOAD_CONFIG           106
#define EVENT_POST_LOAD_CONFIG          107
#define EVENT_PRE_SAVE_FILE             108
#define EVENT_POST_SAVE_FILE            109
#define EVENT_PRE_ENUM_CONFIG           110
#define EVENT_POST_ENUM_CONFIG          111
#define EVENT_PRE_GET_CMD_ID            112
#define EVENT_POST_GET_CMD_ID           113
#define EVENT_PRE_GET_COMMAND_ID        114
#define EVENT_POST_GET_COMMAND_ID       115
#define EVENT_PRE_GET_SHORTCUTS         116
#define EVENT_POST_GET_SHORTCUTS        117
#define EVENT_PRE_GET_COMMAND_ICON      118
#define EVENT_POST_GET_COMMAND_ICON     119
#define EVENT_PRE_GET_DROPPED_FILE      120
#define EVENT_POST_GET_DROPPED_FILE     121
#define EVENT_PRE_GET_REGISTER_NAME     122
#define EVENT_POST_GET_REGISTER_NAME    123
#define EVENT_PRE_GET_WND_PPI           124
#define EVENT_POST_GET_WND_PPI          125
#define EVENT_PRE_SET_WND_PPI           126
#define EVENT_POST_SET_WND_PPI          127
#define EVENT_PRE_GET_LANGUAGE_NAME     128
#define EVENT_POST_GET_LANGUAGE_NAME    129
#define EVENT_PRE_GET_TAG_EXISTS        130
#define EVENT_POST_GET_TAG_EXISTS       131

// Plugin Messages
#define EP_GET_MASK                 0
#define EP_GET_NAME                 1
#define EP_GET_VERSION              2
#define EP_GET_AUTHOR               3
#define EP_GET_DESCRIPTION          4
#define EP_GET_ICON                 5
#define EP_GET_MENU_TEXT            6
#define EP_GET_STATUS_MESSAGE       7
#define EP_GET_TOOLTIP              8
#define EP_GET_HELP_FILE            9
#define EP_GET_HOME_PAGE            10
#define EP_GET_LATEST_VERSION       11
#define EP_GET_LATEST_VERSION_URL   12
#define EP_GET_AUTO_UPDATE          13
#define EP_GET_AUTO_UPDATE_URL      14
#define EP_GET_AUTO_UPDATE_INTERVAL 15
#define EP_GET_AUTO_UPDATE_CHECK    16
#define EP_GET_AUTO_UPDATE_DOWNLOAD 17
#define EP_GET_AUTO_UPDATE_INSTALL  18
#define EP_GET_AUTO_UPDATE_RESTART  19
#define EP_GET_AUTO_UPDATE_SILENT   20
#define EP_GET_AUTO_UPDATE_PROMPT   21
#define EP_GET_AUTO_UPDATE_CONFIRM  22
#define EP_GET_AUTO_UPDATE_PROGRESS 23
#define EP_GET_AUTO_UPDATE_CANCEL   24
#define EP_GET_AUTO_UPDATE_ERROR    25
#define EP_GET_AUTO_UPDATE_SUCCESS  26
#define EP_GET_AUTO_UPDATE_COMPLETE 27
#define EP_GET_AUTO_UPDATE_STATUS   28
#define EP_GET_AUTO_UPDATE_INFO     29
#define EP_GET_AUTO_UPDATE_CONFIG   30
#define EP_SET_AUTO_UPDATE_CONFIG   31
#define EP_GET_AUTO_UPDATE_LOG      32
#define EP_CLEAR_AUTO_UPDATE_LOG    33
#define EP_GET_AUTO_UPDATE_HISTORY  34
#define EP_CLEAR_AUTO_UPDATE_HISTORY 35
#define EP_GET_AUTO_UPDATE_SCHEDULE 36
#define EP_SET_AUTO_UPDATE_SCHEDULE 37
#define EP_GET_AUTO_UPDATE_PROXY    38
#define EP_SET_AUTO_UPDATE_PROXY    39
#define EP_GET_AUTO_UPDATE_AUTH     40
#define EP_SET_AUTO_UPDATE_AUTH     41
#define EP_GET_AUTO_UPDATE_CERT     42
#define EP_SET_AUTO_UPDATE_CERT     43
#define EP_GET_AUTO_UPDATE_TIMEOUT  44
#define EP_SET_AUTO_UPDATE_TIMEOUT  45
#define EP_GET_AUTO_UPDATE_RETRY    46
#define EP_SET_AUTO_UPDATE_RETRY    47
#define EP_GET_AUTO_UPDATE_RETRY_DELAY 48
#define EP_SET_AUTO_UPDATE_RETRY_DELAY 49
#define EP_GET_AUTO_UPDATE_RETRY_MAX   50
#define EP_SET_AUTO_UPDATE_RETRY_MAX   51
#define EP_GET_AUTO_UPDATE_RETRY_COUNT 52
#define EP_GET_AUTO_UPDATE_RETRY_STATUS 53
#define EP_GET_AUTO_UPDATE_RETRY_ERROR  54
#define EP_GET_AUTO_UPDATE_RETRY_SUCCESS 55
#define EP_GET_AUTO_UPDATE_RETRY_COMPLETE 56
#define EP_GET_AUTO_UPDATE_RETRY_CANCEL  57
#define EP_GET_AUTO_UPDATE_RETRY_PROGRESS 58
#define EP_GET_AUTO_UPDATE_RETRY_INFO    59
#define EP_GET_AUTO_UPDATE_RETRY_CONFIG  60
#define EP_SET_AUTO_UPDATE_RETRY_CONFIG  61
#define EP_GET_AUTO_UPDATE_RETRY_LOG     62
#define EP_CLEAR_AUTO_UPDATE_RETRY_LOG   63
#define EP_GET_AUTO_UPDATE_RETRY_HISTORY 64
#define EP_CLEAR_AUTO_UPDATE_RETRY_HISTORY 65
#define EP_GET_AUTO_UPDATE_RETRY_SCHEDULE 66
#define EP_SET_AUTO_UPDATE_RETRY_SCHEDULE 67
#define EP_GET_AUTO_UPDATE_RETRY_PROXY    68
#define EP_SET_AUTO_UPDATE_RETRY_PROXY    69
#define EP_GET_AUTO_UPDATE_RETRY_AUTH     70
#define EP_SET_AUTO_UPDATE_RETRY_AUTH     71
#define EP_GET_AUTO_UPDATE_RETRY_CERT     72
#define EP_SET_AUTO_UPDATE_RETRY_CERT     73
#define EP_GET_AUTO_UPDATE_RETRY_TIMEOUT  74
#define EP_SET_AUTO_UPDATE_RETRY_TIMEOUT  75

// Structures
#pragma pack(push, 1)

typedef struct _SEL_INFO {
    INT nStart;
    INT nEnd;
    BOOL bSelected;
} SEL_INFO;

typedef struct _GET_LINE_INFO {
    INT nLine;
    LPWSTR pBuf;
    INT cchBuf;
} GET_LINE_INFO;

typedef struct _FIND_REPLACE_INFO {
    UINT nFlags;
    LPCWSTR pszFind;
    LPCWSTR pszReplace;
    INT nReplaceLen;
} FIND_REPLACE_INFO;

typedef struct _CUSTOM_BAR_INFO {
    HWND hwndCustomBar;
    HWND hwndPlugin;
    LPCWSTR pszTitle;
    UINT nID;
    UINT nFlags;
} CUSTOM_BAR_INFO;

typedef struct _CUSTOM_BAR_CLOSE_INFO {
    UINT nID;
    UINT nFlags;
} CUSTOM_BAR_CLOSE_INFO;

#pragma pack(pop)

// Inline Functions
inline UINT Editor_GetVersion(HWND hwnd) {
    return (UINT)SendMessage(hwnd, EE_GET_VERSION, 0, 0);
}

inline int Editor_GetSelStart(HWND hwnd) {
    return (int)SendMessage(hwnd, EE_GET_SEL_START, 0, 0);
}

inline int Editor_GetSelEnd(HWND hwnd) {
    return (int)SendMessage(hwnd, EE_GET_SEL_END, 0, 0);
}

inline void Editor_SetSel(HWND hwnd, int nStart, int nEnd) {
    SendMessage(hwnd, EE_SET_SEL, (WPARAM)nStart, (LPARAM)nEnd);
}

inline BOOL Editor_IsSelectionEmpty(HWND hwnd) {
    return Editor_GetSelStart(hwnd) == Editor_GetSelEnd(hwnd);
}

inline int Editor_GetSelText(HWND hwnd, LPWSTR pBuf, int cchBuf) {
    return (int)SendMessage(hwnd, EE_GET_SEL_TEXT, (WPARAM)cchBuf, (LPARAM)pBuf);
}

inline int Editor_GetLineCount(HWND hwnd) {
    return (int)SendMessage(hwnd, EE_GET_LINE_COUNT, 0, 0);
}

inline int Editor_GetLine(HWND hwnd, int nLine, LPWSTR pBuf, int cchBuf) {
    GET_LINE_INFO gli = { nLine, pBuf, cchBuf };
    return (int)SendMessage(hwnd, EE_GET_LINE, 0, (LPARAM)&gli);
}

inline void Editor_InsertText(HWND hwnd, LPCWSTR pszText) {
    SendMessage(hwnd, EE_INSERT_TEXT, 0, (LPARAM)pszText);
}

inline void Editor_ReplaceText(HWND hwnd, LPCWSTR pszFind, LPCWSTR pszReplace, UINT nFlags) {
    FIND_REPLACE_INFO fri = { nFlags, pszFind, pszReplace, 0 };
    SendMessage(hwnd, EE_REPLACE_TEXT, 0, (LPARAM)&fri);
}

inline int Editor_GetText(HWND hwnd, LPWSTR pBuf, int cchBuf) {
    return (int)SendMessage(hwnd, EE_GET_TEXT, (WPARAM)cchBuf, (LPARAM)pBuf);
}

inline void Editor_SetText(HWND hwnd, LPCWSTR pszText) {
    SendMessage(hwnd, EE_SET_TEXT, 0, (LPARAM)pszText);
}

inline BOOL Editor_CustomBarOpen(HWND hwnd, CUSTOM_BAR_INFO* pCustomBarInfo) {
    return (BOOL)SendMessage(hwnd, EE_CUSTOM_BAR_OPEN, 0, (LPARAM)pCustomBarInfo);
}

inline BOOL Editor_CustomBarClose(HWND hwnd, CUSTOM_BAR_CLOSE_INFO* pCustomBarCloseInfo) {
    return (BOOL)SendMessage(hwnd, EE_CUSTOM_BAR_CLOSE, 0, (LPARAM)pCustomBarCloseInfo);
}

inline HWND Editor_GetCustomBar(HWND hwnd, UINT nID) {
    return (HWND)SendMessage(hwnd, EE_GET_CUSTOM_BAR, (WPARAM)nID, 0);
}

#ifdef __cplusplus
}
#endif

#endif // _EMEDITOR_PLUGIN_H_
