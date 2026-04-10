// EmEditor AI Plugin - Main Source File
// Copyright (c) 2025 - AI Assistant Plugin for EmEditor

#include "EmEditorAIPlugin.h"
#include "resource.h"
#include <wininet.h>
#include <shlobj.h>

#pragma comment(lib, "wininet.lib")
#pragma comment(lib, "comctl32.lib")
#pragma comment(lib, "shell32.lib")

// Global Variables
HINSTANCE g_hInstance = NULL;
HWND g_hwndEmEditor = NULL;
HWND g_hwndCustomBar = NULL;
HWND g_hwndInput = NULL;
HWND g_hwndOutput = NULL;
HWND g_hwndStatus = NULL;
HWND g_hwndFunctionPanel = NULL;
HWND g_hwndProviderCombo = NULL;
HWND g_hwndSendButton = NULL;
HWND g_hwndFontSlider = NULL;

std::wstring g_selectedText;
PluginConfig g_config;
std::vector<ChatMessage> g_chatHistory;
std::mutex g_historyMutex;
std::mutex g_apiMutex;
bool g_isProcessing = false;
bool g_isCustomBarVisible = false;

static const wchar_t* CUSTOM_BAR_CLASS_NAME = L"EmEditorAICustomBar";
static const UINT CUSTOM_BAR_ID = 9001;

// Default Provider Configurations
void InitDefaultProviderConfigs() {
    // DeepSeek
    g_config.providers[AIProvider::DeepSeek] = {
        L"DeepSeek",
        L"https://api.deepseek.com/v1/chat/completions",
        L"",
        L"deepseek-chat",
        L"deepseek-chat",
        60000,
        true,
        {{L"Authorization", L"Bearer "}, {L"Content-Type", L"application/json"}},
        {}
    };
    
    // Doubao (Volcano Engine)
    g_config.providers[AIProvider::Doubao] = {
        L"豆包 (火山引擎)",
        L"https://ark.cn-beijing.volces.com/api/v3/chat/completions",
        L"",
        L"doubao-1.5-pro-32k",
        L"doubao-1.5-pro-32k",
        60000,
        true,
        {{L"Authorization", L"Bearer "}, {L"Content-Type", L"application/json"}},
        {}
    };
    
    // Kimi (Moonshot)
    g_config.providers[AIProvider::Kimi] = {
        L"Kimi",
        L"https://api.moonshot.cn/v1/chat/completions",
        L"",
        L"moonshot-v1-8k",
        L"moonshot-v1-8k",
        60000,
        true,
        {{L"Authorization", L"Bearer "}, {L"Content-Type", L"application/json"}},
        {}
    };
    
    // MiniMax
    g_config.providers[AIProvider::MiniMax] = {
        L"MiniMax",
        L"https://api.minimax.chat/v1/text/chatcompletion_v2",
        L"",
        L"abab6.5s-chat",
        L"abab6.5s-chat",
        60000,
        true,
        {{L"Authorization", L"Bearer "}, {L"Content-Type", L"application/json"}},
        {}
    };
    
    // Qianwen (Tongyi)
    g_config.providers[AIProvider::Qianwen] = {
        L"通义千问",
        L"https://dashscope.aliyuncs.com/api/v1/services/aigc/text-generation/generation",
        L"",
        L"qwen-turbo",
        L"qwen-turbo",
        60000,
        true,
        {{L"Authorization", L"Bearer "}, {L"Content-Type", L"application/json"}},
        {{L"X-DashScope-SSE", L"disable"}}
    };
    
    // OpenAI
    g_config.providers[AIProvider::OpenAI] = {
        L"OpenAI GPT",
        L"https://api.openai.com/v1/chat/completions",
        L"",
        L"gpt-3.5-turbo",
        L"gpt-3.5-turbo",
        60000,
        true,
        {{L"Authorization", L"Bearer "}, {L"Content-Type", L"application/json"}},
        {}
    };
    
    // Gemini
    g_config.providers[AIProvider::Gemini] = {
        L"Google Gemini",
        L"https://generativelanguage.googleapis.com/v1beta/models/gemini-pro:generateContent",
        L"",
        L"gemini-pro",
        L"gemini-pro",
        60000,
        true,
        {{L"Content-Type", L"application/json"}},
        {}
    };
    
    // Custom
    g_config.providers[AIProvider::Custom] = {
        L"自定义",
        L"",
        L"",
        L"",
        L"",
        60000,
        false,
        {{L"Content-Type", L"application/json"}},
        {}
    };
}

// DLL Entry Point
BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
    switch (ul_reason_for_call) {
    case DLL_PROCESS_ATTACH:
        g_hInstance = hModule;
        DisableThreadLibraryCalls(hModule);
        InitializePlugin();
        break;
    case DLL_PROCESS_DETACH:
        UninitializePlugin();
        break;
    }
    return TRUE;
}

// Initialize Plugin
BOOL InitializePlugin() {
    InitCommonControls();
    InitDefaultProviderConfigs();
    LoadConfig();
    LoadHistory();
    RegisterCustomBarClass();
    return TRUE;
}

// Uninitialize Plugin
void UninitializePlugin() {
    CloseAICustomBar();
    UnregisterCustomBarClass();
    SaveConfig();
    SaveHistory();
}

// Register Custom Bar Window Class
BOOL RegisterCustomBarClass() {
    WNDCLASSEX wcex = { 0 };
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.lpfnWndProc = CustomBarWndProc;
    wcex.hInstance = g_hInstance;
    wcex.hIcon = LoadIcon(g_hInstance, MAKEINTRESOURCE(IDI_PLUGIN_ICON));
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
    wcex.lpszClassName = CUSTOM_BAR_CLASS_NAME;
    wcex.hIconSm = LoadIcon(g_hInstance, MAKEINTRESOURCE(IDI_PLUGIN_ICON));
    return RegisterClassEx(&wcex) != 0;
}

// Unregister Custom Bar Window Class
void UnregisterCustomBarClass() {
    UnregisterClass(CUSTOM_BAR_CLASS_NAME, g_hInstance);
}

// Plugin Export Functions
extern "C" {

__declspec(dllexport) void OnCommand(HWND hwnd) {
    g_hwndEmEditor = hwnd;
    
    if (g_hwndCustomBar && IsWindow(g_hwndCustomBar)) {
        // Toggle visibility
        if (IsWindowVisible(g_hwndCustomBar)) {
            CloseAICustomBar();
        } else {
            ShowWindow(g_hwndCustomBar, SW_SHOW);
        }
    } else {
        CreateAICustomBar(hwnd);
    }
}

__declspec(dllexport) void QueryStatus(HWND hwnd, LPBOOL pbChecked) {
    *pbChecked = (g_hwndCustomBar && IsWindow(g_hwndCustomBar) && IsWindowVisible(g_hwndCustomBar));
}

__declspec(dllexport) void OnEvents(HWND hwnd, UINT nEvent, LPARAM lParam) {
    g_hwndEmEditor = hwnd;
    
    switch (nEvent) {
    case EVENT_SEL_CHANGED:
        if (g_config.autoShowOnSelection) {
            std::wstring selText = GetSelectedText(hwnd);
            if (!selText.empty() && selText != g_selectedText) {
                g_selectedText = selText;
                if (!g_hwndCustomBar || !IsWindow(g_hwndCustomBar)) {
                    CreateAICustomBar(hwnd);
                }
                // Auto-populate with selected text
                if (g_hwndInput) {
                    SetWindowText(g_hwndInput, (L"请分析以下内容：\n" + selText).c_str());
                }
            }
        }
        break;
        
    case EVENT_DOC_CHANGED:
    case EVENT_DOC_CLOSED:
        // Close custom bar when document changes
        if (g_hwndCustomBar && IsWindow(g_hwndCustomBar)) {
            CloseAICustomBar();
        }
        break;
        
    case EVENT_CUSTOM_BAR_CLOSED:
        if (lParam == CUSTOM_BAR_ID) {
            g_hwndCustomBar = NULL;
        }
        break;
    }
}

__declspec(dllexport) UINT GetMenuTextID() {
    return IDS_MENU_TEXT;
}

__declspec(dllexport) UINT GetStatusMessageID() {
    return IDS_STATUS_MESSAGE;
}

__declspec(dllexport) UINT GetBitmapID() {
    return IDB_PLUGIN_BITMAP;
}

__declspec(dllexport) LRESULT PlugInProc(HWND hwnd, UINT nMsg, WPARAM wParam, LPARAM lParam) {
    switch (nMsg) {
    case EP_GET_NAME:
        return (LRESULT)PLUGIN_NAME;
    case EP_GET_VERSION:
        return (LRESULT)PLUGIN_VERSION_STRING;
    case EP_GET_AUTHOR:
        return (LRESULT)PLUGIN_AUTHOR;
    case EP_GET_DESCRIPTION:
        return (LRESULT)L"AI Assistant Plugin for EmEditor - Chat with various AI providers";
    default:
        return 0;
    }
}

} // extern "C"

// Create AI Custom Bar
BOOL CreateAICustomBar(HWND hwndParent) {
    if (g_hwndCustomBar && IsWindow(g_hwndCustomBar)) {
        ShowWindow(g_hwndCustomBar, SW_SHOW);
        SetForegroundWindow(g_hwndCustomBar);
        return TRUE;
    }
    
    // Get selected text
    g_selectedText = GetSelectedText(hwndParent);
    
    // Create custom bar window
    CUSTOM_BAR_INFO cbi = { 0 };
    cbi.pszTitle = PLUGIN_NAME;
    cbi.nID = CUSTOM_BAR_ID;
    cbi.nFlags = 0;
    
    // Create the custom bar using EmEditor's API
    if (!Editor_CustomBarOpen(hwndParent, &cbi)) {
        // Fallback: Create as child window
        g_hwndCustomBar = CreateWindowEx(
            WS_EX_CLIENTEDGE | WS_EX_TOOLWINDOW,
            CUSTOM_BAR_CLASS_NAME,
            PLUGIN_NAME,
            WS_OVERLAPPEDWINDOW | WS_VISIBLE,
            g_config.windowPosX, g_config.windowPosY,
            g_config.windowWidth, g_config.windowHeight,
            hwndParent, NULL, g_hInstance, NULL
        );
    } else {
        g_hwndCustomBar = cbi.hwndCustomBar;
    }
    
    if (!g_hwndCustomBar) {
        return FALSE;
    }
    
    // Create controls
    CreateCustomBarControls(g_hwndCustomBar);
    
    // Apply theme
    ApplyTheme();
    
    // Show window
    ShowWindow(g_hwndCustomBar, SW_SHOW);
    UpdateWindow(g_hwndCustomBar);
    
    // Set focus to input
    if (g_hwndInput) {
        SetFocus(g_hwndInput);
    }
    
    return TRUE;
}

// Close AI Custom Bar
void CloseAICustomBar() {
    if (g_hwndCustomBar && IsWindow(g_hwndCustomBar)) {
        // Save window position and size
        RECT rc;
        GetWindowRect(g_hwndCustomBar, &rc);
        g_config.windowPosX = rc.left;
        g_config.windowPosY = rc.top;
        g_config.windowWidth = rc.right - rc.left;
        g_config.windowHeight = rc.bottom - rc.top;
        
        DestroyWindow(g_hwndCustomBar);
        g_hwndCustomBar = NULL;
    }
}

// Custom Bar Window Procedure
LRESULT CALLBACK CustomBarWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_CREATE:
        return 0;
        
    case WM_SIZE: {
        int width = LOWORD(lParam);
        int height = HIWORD(lParam);
        LayoutControls(width, height);
        return 0;
    }
    
    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        case IDC_BTN_SEND:
            if (g_hwndInput) {
                wchar_t buffer[8192] = { 0 };
                GetWindowText(g_hwndInput, buffer, 8192);
                if (wcslen(buffer) > 0) {
                    SendChatMessage(buffer);
                }
            }
            return 0;
            
        case IDC_BTN_SUMMARIZE:
            if (!g_selectedText.empty()) {
                SummarizeText(g_selectedText);
            } else {
                MessageBox(hwnd, L"请先选择要总结的文本。", L"提示", MB_OK | MB_ICONINFORMATION);
            }
            return 0;
            
        case IDC_BTN_EXPLAIN:
            if (!g_selectedText.empty()) {
                ExplainText(g_selectedText);
            } else {
                MessageBox(hwnd, L"请先选择要解释的文本。", L"提示", MB_OK | MB_ICONINFORMATION);
            }
            return 0;
            
        case IDC_BTN_TRANSLATE:
            if (!g_selectedText.empty()) {
                TranslateText(g_selectedText, g_config.defaultTranslateLang);
            } else {
                MessageBox(hwnd, L"请先选择要翻译的文本。", L"提示", MB_OK | MB_ICONINFORMATION);
            }
            return 0;
            
        case IDC_BTN_CHAT:
            // Switch to chat mode
            if (g_hwndInput) {
                SetFocus(g_hwndInput);
            }
            return 0;
            
        case IDC_BTN_SETTINGS:
            ShowSettingsDialog(hwnd);
            return 0;
            
        case IDC_BTN_HISTORY:
            ShowHistoryDialog(hwnd);
            return 0;
            
        case IDC_BTN_CLEAR:
            ClearOutput();
            return 0;
            
        case IDC_COMBO_AI_PROVIDER:
            if (HIWORD(wParam) == CBN_SELCHANGE) {
                int sel = (int)SendMessage((HWND)lParam, CB_GETCURSEL, 0, 0);
                if (sel >= 0 && sel < (int)AIProvider::ProviderCount) {
                    g_config.defaultProvider = (AIProvider)sel;
                }
            }
            return 0;
            
        case IDC_SLIDER_FONT_SIZE:
            if (HIWORD(wParam) == TB_THUMBPOSITION || HIWORD(wParam) == TB_THUMBTRACK) {
                int fontSize = LOWORD(wParam);
                g_config.outputFontSize = fontSize;
                UpdateFontSizes();
            }
            return 0;
        }
        break;
        
    case WM_NOTIFY: {
        LPNMHDR pnmh = (LPNMHDR)lParam;
        if (pnmh->code == TTN_GETDISPINFO) {
            // Tooltip handling
        }
        break;
    }
    
    case WM_AI_RESPONSE: {
        // AI response received
        wchar_t* response = (wchar_t*)lParam;
        if (response) {
            AppendToOutput(response, false);
            delete[] response;
        }
        g_isProcessing = false;
        UpdateStatusText(L"就绪");
        return 0;
    }
    
    case WM_AI_ERROR: {
        // AI error
        wchar_t* error = (wchar_t*)lParam;
        if (error) {
            AppendToOutput((std::wstring(L"[错误] ") + error).c_str(), false);
            delete[] error;
        }
        g_isProcessing = false;
        UpdateStatusText(L"错误", RGB(255, 0, 0));
        return 0;
    }
    
    case WM_CLOSE:
        CloseAICustomBar();
        return 0;
        
    case WM_DESTROY:
        g_hwndCustomBar = NULL;
        return 0;
    }
    
    return DefWindowProc(hwnd, msg, wParam, lParam);
}

// Create Custom Bar Controls
void CreateCustomBarControls(HWND hwndParent) {
    HINSTANCE hInst = g_hInstance;
    int x = 10, y = 10;
    int buttonWidth = 60, buttonHeight = 25;
    int comboWidth = 120, comboHeight = 100;
    
    // Function buttons panel
    g_hwndFunctionPanel = CreateWindow(
        L"BUTTON", L"功能",
        WS_CHILD | WS_VISIBLE | BS_GROUPBOX,
        5, 5, 390, 45,
        hwndParent, NULL, hInst, NULL
    );
    
    // Summarize button
    CreateWindow(
        L"BUTTON", L"总结",
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        15, 20, buttonWidth, buttonHeight,
        hwndParent, (HMENU)IDC_BTN_SUMMARIZE, hInst, NULL
    );
    
    // Explain button
    CreateWindow(
        L"BUTTON", L"解释",
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        15 + buttonWidth + 5, 20, buttonWidth, buttonHeight,
        hwndParent, (HMENU)IDC_BTN_EXPLAIN, hInst, NULL
    );
    
    // Translate button
    CreateWindow(
        L"BUTTON", L"翻译",
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        15 + (buttonWidth + 5) * 2, 20, buttonWidth, buttonHeight,
        hwndParent, (HMENU)IDC_BTN_TRANSLATE, hInst, NULL
    );
    
    // Chat button
    CreateWindow(
        L"BUTTON", L"对话",
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        15 + (buttonWidth + 5) * 3, 20, buttonWidth, buttonHeight,
        hwndParent, (HMENU)IDC_BTN_CHAT, hInst, NULL
    );
    
    // AI Provider Combo
    g_hwndProviderCombo = CreateWindow(
        L"COMBOBOX", L"",
        WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | WS_VSCROLL,
        280, 22, comboWidth, comboHeight,
        hwndParent, (HMENU)IDC_COMBO_AI_PROVIDER, hInst, NULL
    );
    
    // Add providers to combo
    for (int i = 0; i < (int)AIProvider::ProviderCount; i++) {
        SendMessage(g_hwndProviderCombo, CB_ADDSTRING, 0, 
            (LPARAM)GetProviderName((AIProvider)i).c_str());
    }
    SendMessage(g_hwndProviderCombo, CB_SETCURSEL, (int)g_config.defaultProvider, 0);
    
    // Output edit (Rich Edit)
    LoadLibrary(L"Msftedit.dll");
    g_hwndOutput = CreateWindow(
        MSFTEDIT_CLASS,
        L"",
        WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_HSCROLL | 
        ES_MULTILINE | ES_AUTOVSCROLL | ES_AUTOHSCROLL | ES_READONLY | WS_BORDER,
        10, 55, 380, 280,
        hwndParent, (HMENU)IDC_EDIT_OUTPUT, hInst, NULL
    );
    
    // Set output font
    UpdateFontSizes();
    
    // Input edit
    g_hwndInput = CreateWindow(
        MSFTEDIT_CLASS,
        L"",
        WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_BORDER |
        ES_MULTILINE | ES_AUTOVSCROLL,
        10, 340, 310, 80,
        hwndParent, (HMENU)IDC_EDIT_INPUT, hInst, NULL
    );
    
    // Send button
    g_hwndSendButton = CreateWindow(
        L"BUTTON", L"发送",
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_DEFPUSHBUTTON,
        325, 340, 65, 80,
        hwndParent, (HMENU)IDC_BTN_SEND, hInst, NULL
    );
    
    // Font size slider
    CreateWindow(
        L"STATIC", L"字体:",
        WS_CHILD | WS_VISIBLE | SS_LEFT,
        10, 425, 35, 20,
        hwndParent, NULL, hInst, NULL
    );
    
    g_hwndFontSlider = CreateWindow(
        TRACKBAR_CLASS,
        L"",
        WS_CHILD | WS_VISIBLE | TBS_HORZ | TBS_AUTOTICKS,
        45, 425, 100, 20,
        hwndParent, (HMENU)IDC_SLIDER_FONT_SIZE, hInst, NULL
    );
    SendMessage(g_hwndFontSlider, TBM_SETRANGE, TRUE, MAKELPARAM(MIN_FONT_SIZE, MAX_FONT_SIZE));
    SendMessage(g_hwndFontSlider, TBM_SETPOS, TRUE, g_config.outputFontSize);
    SendMessage(g_hwndFontSlider, TBM_SETTICFREQ, 2, 0);
    
    // Bottom buttons
    CreateWindow(
        L"BUTTON", L"设置",
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        160, 425, 50, 25,
        hwndParent, (HMENU)IDC_BTN_SETTINGS, hInst, NULL
    );
    
    CreateWindow(
        L"BUTTON", L"历史",
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        215, 425, 50, 25,
        hwndParent, (HMENU)IDC_BTN_HISTORY, hInst, NULL
    );
    
    CreateWindow(
        L"BUTTON", L"清空",
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        270, 425, 50, 25,
        hwndParent, (HMENU)IDC_BTN_CLEAR, hInst, NULL
    );
    
    // Status bar
    g_hwndStatus = CreateWindow(
        L"STATIC", L"就绪",
        WS_CHILD | WS_VISIBLE | SS_LEFT,
        10, 455, 380, 20,
        hwndParent, (HMENU)IDC_STATIC_STATUS, hInst, NULL
    );
    
    // Populate input with selected text if any
    if (!g_selectedText.empty()) {
        std::wstring prompt = L"请分析以下内容：\n" + g_selectedText;
        SetWindowText(g_hwndInput, prompt.c_str());
    }
}

// Layout Controls
void LayoutControls(int width, int height) {
    if (!g_hwndOutput || !g_hwndInput) return;
    
    int margin = 10;
    int panelHeight = 45;
    int inputHeight = 80;
    int statusHeight = 20;
    int bottomPanelHeight = 50;
    
    // Function panel
    SetWindowPos(g_hwndFunctionPanel, NULL, 
        margin, margin, 
        width - margin * 2, panelHeight, 
        SWP_NOZORDER);
    
    // Output edit
    int outputHeight = height - panelHeight - inputHeight - bottomPanelHeight - margin * 3 - statusHeight;
    SetWindowPos(g_hwndOutput, NULL,
        margin, margin + panelHeight + 5,
        width - margin * 2, outputHeight,
        SWP_NOZORDER);
    
    // Input edit
    SetWindowPos(g_hwndInput, NULL,
        margin, margin + panelHeight + outputHeight + 10,
        width - margin * 2 - 75, inputHeight,
        SWP_NOZORDER);
    
    // Send button
    SetWindowPos(g_hwndSendButton, NULL,
        width - margin - 70, margin + panelHeight + outputHeight + 10,
        70, inputHeight,
        SWP_NOZORDER);
    
    // Status bar
    SetWindowPos(g_hwndStatus, NULL,
        margin, height - statusHeight - margin,
        width - margin * 2, statusHeight,
        SWP_NOZORDER);
}

// Update Font Sizes
void UpdateFontSizes() {
    if (!g_hwndOutput) return;
    
    HFONT hFont = CreateFont(
        -MulDiv(g_config.outputFontSize, GetDeviceCaps(GetDC(g_hwndOutput), LOGPIXELSY), 72),
        0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS,
        g_config.fontName.empty() ? L"Microsoft YaHei" : g_config.fontName.c_str()
    );
    
    SendMessage(g_hwndOutput, WM_SETFONT, (WPARAM)hFont, TRUE);
    
    if (g_hwndInput) {
        HFONT hInputFont = CreateFont(
            -MulDiv(g_config.inputFontSize, GetDeviceCaps(GetDC(g_hwndInput), LOGPIXELSY), 72),
            0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
            DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
            DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS,
            g_config.fontName.empty() ? L"Microsoft YaHei" : g_config.fontName.c_str()
        );
        SendMessage(g_hwndInput, WM_SETFONT, (WPARAM)hInputFont, TRUE);
    }
}

// Apply Theme
void ApplyTheme() {
    if (g_config.darkMode) {
        // Dark theme colors
        if (g_hwndOutput) {
            SendMessage(g_hwndOutput, EM_SETBKGNDCOLOR, 0, RGB(30, 30, 30));
            CHARFORMAT2 cf = { 0 };
            cf.cbSize = sizeof(cf);
            cf.dwMask = CFM_COLOR;
            cf.crTextColor = RGB(220, 220, 220);
            SendMessage(g_hwndOutput, EM_SETCHARFORMAT, SCF_ALL, (LPARAM)&cf);
        }
        if (g_hwndInput) {
            SendMessage(g_hwndInput, EM_SETBKGNDCOLOR, 0, RGB(40, 40, 40));
        }
    } else {
        // Light theme (default)
        if (g_hwndOutput) {
            SendMessage(g_hwndOutput, EM_SETBKGNDCOLOR, 0, RGB(255, 255, 255));
        }
        if (g_hwndInput) {
            SendMessage(g_hwndInput, EM_SETBKGNDCOLOR, 0, RGB(255, 255, 255));
        }
    }
}

// Update Status Text
void UpdateStatusText(const wchar_t* text, COLORREF color) {
    if (g_hwndStatus) {
        SetWindowText(g_hwndStatus, text);
        InvalidateRect(g_hwndStatus, NULL, TRUE);
    }
}

// Get Selected Text
std::wstring GetSelectedText(HWND hwnd) {
    if (!hwnd) return L"";
    
    int selStart = Editor_GetSelStart(hwnd);
    int selEnd = Editor_GetSelEnd(hwnd);
    
    if (selStart == selEnd) return L"";
    
    int len = selEnd - selStart;
    std::vector<wchar_t> buffer(len + 1);
    
    Editor_SetSel(hwnd, selStart, selEnd);
    Editor_GetSelText(hwnd, buffer.data(), len + 1);
    
    return std::wstring(buffer.data());
}

// Append to Output
void AppendToOutput(const std::wstring& text, bool isUser) {
    if (!g_hwndOutput) return;
    
    // Get current selection
    CHARRANGE cr;
    SendMessage(g_hwndOutput, EM_EXGETSEL, 0, (LPARAM)&cr);
    
    // Move to end
    int len = GetWindowTextLength(g_hwndOutput);
    cr.cpMin = len;
    cr.cpMax = len;
    SendMessage(g_hwndOutput, EM_EXSETSEL, 0, (LPARAM)&cr);
    
    // Set text color
    CHARFORMAT2 cf = { 0 };
    cf.cbSize = sizeof(cf);
    cf.dwMask = CFM_COLOR | CFM_BOLD;
    cf.crTextColor = isUser ? RGB(0, 100, 200) : RGB(0, 128, 0);
    cf.dwEffects = isUser ? CFE_BOLD : 0;
    SendMessage(g_hwndOutput, EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM)&cf);
    
    // Add prefix
    std::wstring prefix = isUser ? L"\n[我] " : L"\n[AI] ";
    SendMessage(g_hwndOutput, EM_REPLACESEL, 0, (LPARAM)prefix.c_str());
    
    // Add message
    cf.crTextColor = g_config.darkMode ? RGB(220, 220, 220) : RGB(0, 0, 0);
    cf.dwEffects = 0;
    SendMessage(g_hwndOutput, EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM)&cf);
    SendMessage(g_hwndOutput, EM_REPLACESEL, 0, (LPARAM)text.c_str());
    
    // Scroll to end
    SendMessage(g_hwndOutput, WM_VSCROLL, SB_BOTTOM, 0);
}

// Clear Output
void ClearOutput() {
    if (g_hwndOutput) {
        SetWindowText(g_hwndOutput, L"");
    }
}

// Get Provider Name
std::wstring GetProviderName(AIProvider provider) {
    switch (provider) {
    case AIProvider::DeepSeek: return L"DeepSeek";
    case AIProvider::Doubao: return L"豆包 (火山引擎)";
    case AIProvider::Kimi: return L"Kimi";
    case AIProvider::MiniMax: return L"MiniMax";
    case AIProvider::Qianwen: return L"通义千问";
    case AIProvider::OpenAI: return L"OpenAI GPT";
    case AIProvider::Gemini: return L"Google Gemini";
    case AIProvider::Custom: return L"自定义";
    default: return L"Unknown";
    }
}

// Get Function Name
std::wstring GetFunctionName(AIFunction func) {
    switch (func) {
    case AIFunction::Summarize: return L"总结";
    case AIFunction::Explain: return L"解释";
    case AIFunction::Translate: return L"翻译";
    case AIFunction::Chat: return L"对话";
    case AIFunction::CodeReview: return L"代码审查";
    case AIFunction::GrammarCheck: return L"语法检查";
    case AIFunction::Rewrite: return L"改写";
    default: return L"自定义";
    }
}

// Get Language Name
std::wstring GetLanguageName(TranslateLang lang) {
    switch (lang) {
    case TranslateLang::Auto: return L"自动检测";
    case TranslateLang::ChineseSimplified: return L"简体中文";
    case TranslateLang::ChineseTraditional: return L"繁体中文";
    case TranslateLang::English: return L"英语";
    case TranslateLang::Japanese: return L"日语";
    case TranslateLang::Korean: return L"韩语";
    case TranslateLang::French: return L"法语";
    case TranslateLang::German: return L"德语";
    case TranslateLang::Spanish: return L"西班牙语";
    case TranslateLang::Russian: return L"俄语";
    default: return L"未知";
    }
}

// Get System Prompt
std::wstring GetSystemPrompt(AIFunction func) {
    switch (func) {
    case AIFunction::Summarize:
        return L"你是一个专业的文本总结助手。请对用户提供的内容进行简洁准确的总结，提取关键信息。";
    case AIFunction::Explain:
        return L"你是一个知识渊博的解释者。请用通俗易懂的语言解释用户提供的概念或内容，必要时可以举例说明。";
    case AIFunction::Translate:
        return L"你是一个专业的翻译助手。请准确翻译用户提供的内容，保持原文的语气和风格。";
    case AIFunction::CodeReview:
        return L"你是一个经验丰富的代码审查专家。请审查用户提供的代码，指出潜在问题、改进建议和最佳实践。";
    case AIFunction::GrammarCheck:
        return L"你是一个语法检查专家。请检查用户提供的文本中的语法错误，并给出修正建议。";
    case AIFunction::Rewrite:
        return L"你是一个写作专家。请改写用户提供的文本，使其更加流畅、清晰和专业。";
    default:
        return L"你是一个有帮助的AI助手。请根据用户的问题提供准确、有用的回答。";
    }
}

// UTF-8 to Wide String
std::wstring Utf8ToWide(const std::string& utf8) {
    if (utf8.empty()) return L"";
    
    int size = MultiByteToWideChar(CP_UTF8, 0, utf8.c_str(), -1, NULL, 0);
    std::vector<wchar_t> buffer(size);
    MultiByteToWideChar(CP_UTF8, 0, utf8.c_str(), -1, buffer.data(), size);
    return std::wstring(buffer.data());
}

// Wide String to UTF-8
std::string WideToUtf8(const std::wstring& wide) {
    if (wide.empty()) return "";
    
    int size = WideCharToMultiByte(CP_UTF8, 0, wide.c_str(), -1, NULL, 0, NULL, NULL);
    std::vector<char> buffer(size);
    WideCharToMultiByte(CP_UTF8, 0, wide.c_str(), -1, buffer.data(), size, NULL, NULL);
    return std::string(buffer.data());
}

// Format Timestamp
std::wstring FormatTimestamp(const SYSTEMTIME& st) {
    wchar_t buffer[64];
    swprintf_s(buffer, L"%04d-%02d-%02d %02d:%02d:%02d",
        st.wYear, st.wMonth, st.wDay,
        st.wHour, st.wMinute, st.wSecond);
    return std::wstring(buffer);
}

// Trim String
void TrimString(std::wstring& str) {
    size_t start = str.find_first_not_of(L" \t\n\r");
    if (start == std::wstring::npos) {
        str.clear();
        return;
    }
    size_t end = str.find_last_not_of(L" \t\n\r");
    str = str.substr(start, end - start + 1);
}

// Escape JSON String
std::wstring EscapeJsonString(const std::wstring& str) {
    std::wstring result;
    for (wchar_t c : str) {
        switch (c) {
        case L'"': result += L"\\\""; break;
        case L'\\': result += L"\\\\"; break;
        case L'\b': result += L"\\b"; break;
        case L'\f': result += L"\\f"; break;
        case L'\n': result += L"\\n"; break;
        case L'\r': result += L"\\r"; break;
        case L'\t': result += L"\\t"; break;
        default: result += c; break;
        }
    }
    return result;
}
