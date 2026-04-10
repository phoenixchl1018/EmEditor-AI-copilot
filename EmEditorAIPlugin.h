#pragma once

// EmEditor AI Plugin - Main Header File
// Copyright (c) 2025 - AI Assistant Plugin for EmEditor

#ifndef _EMEDITOR_AI_PLUGIN_H_
#define _EMEDITOR_AI_PLUGIN_H_

// Windows Headers
#define WIN32_LEAN_AND_MEAN
#define _WIN32_WINNT 0x0A00
#include <windows.h>
#include <windowsx.h>
#include <commctrl.h>
#include <richedit.h>
#include <string>
#include <vector>
#include <map>
#include <memory>
#include <thread>
#include <mutex>
#include <fstream>
#include <sstream>
#include <json/json.h>

// EmEditor Plugin SDK
#include "plugin.h"

// Version Information
#define PLUGIN_VERSION_MAJOR    1
#define PLUGIN_VERSION_MINOR    0
#define PLUGIN_VERSION_BUILD    0
#define PLUGIN_VERSION_STRING   L"1.0.0"
#define PLUGIN_NAME             L"EmEditor AI Assistant"
#define PLUGIN_AUTHOR           L"AI Plugin Developer"

// Window Messages
#define WM_AI_RESPONSE          (WM_USER + 100)
#define WM_AI_ERROR             (WM_USER + 101)
#define WM_UPDATE_UI            (WM_USER + 102)
#define WM_RESIZE_PANEL         (WM_USER + 103)

// Control IDs
#define IDC_EDIT_INPUT          1001
#define IDC_EDIT_OUTPUT         1002
#define IDC_BTN_SEND            1003
#define IDC_BTN_SUMMARIZE       1004
#define IDC_BTN_EXPLAIN         1005
#define IDC_BTN_TRANSLATE       1006
#define IDC_BTN_CHAT            1007
#define IDC_BTN_SETTINGS        1008
#define IDC_BTN_HISTORY         1009
#define IDC_BTN_CLEAR           1010
#define IDC_COMBO_AI_PROVIDER   1011
#define IDC_COMBO_LANGUAGE      1012
#define IDC_SLIDER_FONT_SIZE    1013
#define IDC_STATIC_STATUS       1014
#define IDC_LIST_HISTORY        1015
#define IDC_TREE_FUNCTIONS      1016
#define IDC_EDIT_API_KEY        1017
#define IDC_EDIT_API_URL        1018
#define IDC_EDIT_MODEL          1019
#define IDC_SPIN_FONT_SIZE      1020

// Timer IDs
#define TIMER_CHECK_SELECTION   1
#define TIMER_AUTO_SAVE         2
#define TIMER_STATUS_CLEAR      3

// Configuration Constants
#define CONFIG_FILE_NAME        L"EmEditorAIPlugin.ini"
#define HISTORY_FILE_NAME       L"AIChatHistory.json"
#define MAX_HISTORY_ITEMS       1000
#define DEFAULT_FONT_SIZE       10
#define MIN_FONT_SIZE           8
#define MAX_FONT_SIZE           24
#define DEFAULT_WINDOW_WIDTH    400
#define DEFAULT_WINDOW_HEIGHT   500

// AI Provider Types
enum class AIProvider {
    DeepSeek = 0,
    Doubao,
    Kimi,
    MiniMax,
    Qianwen,
    OpenAI,
    Gemini,
    Custom,
    ProviderCount
};

// Function Types
enum class AIFunction {
    Summarize = 0,
    Explain,
    Translate,
    Chat,
    CodeReview,
    GrammarCheck,
    Rewrite,
    Custom
};

// Language Codes for Translation
enum class TranslateLang {
    Auto = 0,
    ChineseSimplified,
    ChineseTraditional,
    English,
    Japanese,
    Korean,
    French,
    German,
    Spanish,
    Russian,
    LangCount
};

// Chat Message Structure
struct ChatMessage {
    std::wstring role;      // "user", "assistant", "system"
    std::wstring content;
    SYSTEMTIME timestamp;
    std::wstring provider;
    std::wstring model;
};

// AI Provider Configuration
struct AIProviderConfig {
    std::wstring name;
    std::wstring apiUrl;
    std::wstring apiKey;
    std::wstring model;
    std::wstring defaultModel;
    int timeoutMs;
    bool enabled;
    std::map<std::wstring, std::wstring> headers;
    std::map<std::wstring, std::wstring> extraParams;
};

// Plugin Configuration
struct PluginConfig {
    // Window Settings
    int windowWidth;
    int windowHeight;
    int windowPosX;
    int windowPosY;
    bool autoShowOnSelection;
    bool rememberWindowSize;
    bool topMost;
    
    // Font Settings
    int inputFontSize;
    int outputFontSize;
    std::wstring fontName;
    bool fontBold;
    
    // AI Settings
    AIProvider defaultProvider;
    TranslateLang defaultTranslateLang;
    int maxTokens;
    double temperature;
    bool streamResponse;
    
    // Provider Configs
    std::map<AIProvider, AIProviderConfig> providers;
    
    // History Settings
    bool saveHistory;
    int maxHistoryDays;
    
    // UI Settings
    bool showFunctionButtons;
    bool showProviderSelector;
    bool darkMode;
    std::wstring theme;
};

// Global Variables
extern HINSTANCE g_hInstance;
extern HWND g_hwndEmEditor;
extern HWND g_hwndCustomBar;
extern HWND g_hwndInput;
extern HWND g_hwndOutput;
extern HWND g_hwndStatus;
extern std::wstring g_selectedText;
extern PluginConfig g_config;
extern std::vector<ChatMessage> g_chatHistory;
extern std::mutex g_historyMutex;
extern std::mutex g_apiMutex;
extern bool g_isProcessing;

// Function Declarations

// Plugin Export Functions
extern "C" {
    __declspec(dllexport) void OnCommand(HWND hwnd);
    __declspec(dllexport) void QueryStatus(HWND hwnd, LPBOOL pbChecked);
    __declspec(dllexport) void OnEvents(HWND hwnd, UINT nEvent, LPARAM lParam);
    __declspec(dllexport) UINT GetMenuTextID();
    __declspec(dllexport) UINT GetStatusMessageID();
    __declspec(dllexport) UINT GetBitmapID();
    __declspec(dllexport) LRESULT PlugInProc(HWND hwnd, UINT nMsg, WPARAM wParam, LPARAM lParam);
}

// Initialization
BOOL InitializePlugin();
void UninitializePlugin();
BOOL InitConfig();

// Custom Bar Window
BOOL CreateAICustomBar(HWND hwndParent);
void CloseAICustomBar();
LRESULT CALLBACK CustomBarWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL RegisterCustomBarClass();
void UnregisterCustomBarClass();

// UI Creation
void CreateCustomBarControls(HWND hwndParent);
void LayoutControls(int width, int height);
void UpdateFontSizes();
void ApplyTheme();
void UpdateStatusText(const wchar_t* text, COLORREF color = RGB(0, 0, 0));

// AI Functions
void SendToAI(const std::wstring& text, AIFunction func);
void SendChatMessage(const std::wstring& message);
void SummarizeText(const std::wstring& text);
void ExplainText(const std::wstring& text);
void TranslateText(const std::wstring& text, TranslateLang targetLang);
void CodeReview(const std::wstring& code);
void GrammarCheck(const std::wstring& text);
void RewriteText(const std::wstring& text);

// AI API Interface
std::wstring CallAIAPI(const std::wstring& prompt, const std::wstring& systemPrompt = L"");
std::wstring BuildRequestBody(AIProvider provider, const std::vector<ChatMessage>& messages);
bool ParseAIResponse(AIProvider provider, const std::string& response, std::wstring& outText);
std::wstring GetProviderName(AIProvider provider);
std::wstring GetFunctionName(AIFunction func);
std::wstring GetLanguageName(TranslateLang lang);

// HTTP Request
std::string HttpPost(const std::wstring& url, const std::string& body, 
                     const std::map<std::wstring, std::wstring>& headers, int timeoutMs);
std::string HttpPostWithWinInet(const std::wstring& url, const std::string& body,
                                 const std::map<std::wstring, std::wstring>& headers, int timeoutMs);

// Configuration Management
void LoadConfig();
void SaveConfig();
void LoadHistory();
void SaveHistory();
void ClearHistory();
std::wstring GetConfigPath();
std::wstring GetHistoryPath();

// Settings Dialog
INT_PTR CALLBACK SettingsDlgProc(HWND hwndDlg, UINT msg, WPARAM wParam, LPARAM lParam);
void ShowSettingsDialog(HWND hwndParent);

// History Dialog
INT_PTR CALLBACK HistoryDlgProc(HWND hwndDlg, UINT msg, WPARAM wParam, LPARAM lParam);
void ShowHistoryDialog(HWND hwndParent);

// Utility Functions
std::wstring Utf8ToWide(const std::string& utf8);
std::string WideToUtf8(const std::wstring& wide);
std::wstring GetSelectedText(HWND hwnd);
void InsertText(HWND hwnd, const std::wstring& text);
void AppendToOutput(const std::wstring& text, bool isUser = false);
void ClearOutput();
std::wstring GetSystemPrompt(AIFunction func);
std::wstring FormatTimestamp(const SYSTEMTIME& st);
void TrimString(std::wstring& str);
std::wstring EscapeJsonString(const std::wstring& str);

// Thread Functions
void ProcessAIRequestAsync(const std::wstring& prompt, const std::wstring& systemPrompt);

// Default Provider Configurations
void InitDefaultProviderConfigs();

#endif // _EMEDITOR_AI_PLUGIN_H_
