// EmEditor AI Plugin - Dialogs
// Copyright (c) 2025 - AI Assistant Plugin for EmEditor

#include "EmEditorAIPlugin.h"
#include "resource.h"
#include <commdlg.h>

// Settings Dialog Procedure
INT_PTR CALLBACK SettingsDlgProc(HWND hwndDlg, UINT msg, WPARAM wParam, LPARAM lParam) {
    static PluginConfig tempConfig;
    static AIProviderConfig tempProviderConfigs[(int)AIProvider::ProviderCount];
    
    switch (msg) {
    case WM_INITDIALOG: {
        // Copy current config to temp
        tempConfig = g_config;
        for (int i = 0; i < (int)AIProvider::ProviderCount; i++) {
            tempProviderConfigs[i] = g_config.providers[(AIProvider)i];
        }
        
        // Center dialog
        RECT rcDlg, rcParent;
        GetWindowRect(hwndDlg, &rcDlg);
        GetWindowRect(GetParent(hwndDlg), &rcParent);
        int x = rcParent.left + (rcParent.right - rcParent.left - (rcDlg.right - rcDlg.left)) / 2;
        int y = rcParent.top + (rcParent.bottom - rcParent.top - (rcDlg.bottom - rcDlg.top)) / 2;
        SetWindowPos(hwndDlg, NULL, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
        
        // Initialize controls
        // General tab
        CheckDlgButton(hwndDlg, IDC_CHK_AUTO_SHOW, tempConfig.autoShowOnSelection ? BST_CHECKED : BST_UNCHECKED);
        CheckDlgButton(hwndDlg, IDC_CHK_REMEMBER_SIZE, tempConfig.rememberWindowSize ? BST_CHECKED : BST_UNCHECKED);
        CheckDlgButton(hwndDlg, IDC_CHK_TOPMOST, tempConfig.topMost ? BST_CHECKED : BST_UNCHECKED);
        CheckDlgButton(hwndDlg, IDC_CHK_SAVE_HISTORY, tempConfig.saveHistory ? BST_CHECKED : BST_UNCHECKED);
        
        SetDlgItemInt(hwndDlg, IDC_EDIT_MAX_TOKENS, tempConfig.maxTokens, FALSE);
        SetDlgItemInt(hwndDlg, IDC_EDIT_HISTORY_DAYS, tempConfig.maxHistoryDays, FALSE);
        
        // Font settings
        SetDlgItemInt(hwndDlg, IDC_EDIT_FONT_SIZE, tempConfig.outputFontSize, FALSE);
        SetDlgItemText(hwndDlg, IDC_EDIT_FONT_NAME, tempConfig.fontName.c_str());
        
        // AI settings
        SetDlgItemInt(hwndDlg, IDC_EDIT_MAX_TOKENS, tempConfig.maxTokens, FALSE);
        
        wchar_t temp[32];
        swprintf_s(temp, L"%.1f", tempConfig.temperature);
        SetDlgItemText(hwndDlg, IDC_EDIT_TEMPERATURE, temp);
        
        // Initialize provider combo
        HWND hProviderCombo = GetDlgItem(hwndDlg, IDC_COMBO_SETTINGS_PROVIDER);
        for (int i = 0; i < (int)AIProvider::ProviderCount; i++) {
            SendMessage(hProviderCombo, CB_ADDSTRING, 0, 
                (LPARAM)GetProviderName((AIProvider)i).c_str());
        }
        SendMessage(hProviderCombo, CB_SETCURSEL, (int)tempConfig.defaultProvider, 0);
        
        // Load current provider settings
        AIProviderConfig& currentProvider = tempProviderConfigs[(int)tempConfig.defaultProvider];
        SetDlgItemText(hwndDlg, IDC_EDIT_API_URL, currentProvider.apiUrl.c_str());
        SetDlgItemText(hwndDlg, IDC_EDIT_API_KEY, currentProvider.apiKey.c_str());
        SetDlgItemText(hwndDlg, IDC_EDIT_MODEL, currentProvider.model.c_str());
        
        // Translation language combo
        HWND hLangCombo = GetDlgItem(hwndDlg, IDC_COMBO_TRANSLATE_LANG);
        for (int i = 0; i < (int)TranslateLang::LangCount; i++) {
            SendMessage(hLangCombo, CB_ADDSTRING, 0,
                (LPARAM)GetLanguageName((TranslateLang)i).c_str());
        }
        SendMessage(hLangCombo, CB_SETCURSEL, (int)tempConfig.defaultTranslateLang, 0);
        
        // Theme combo
        HWND hThemeCombo = GetDlgItem(hwndDlg, IDC_COMBO_THEME);
        SendMessage(hThemeCombo, CB_ADDSTRING, 0, (LPARAM)LoadStringRes(IDS_THEME_DEFAULT).c_str());
        SendMessage(hThemeCombo, CB_ADDSTRING, 0, (LPARAM)LoadStringRes(IDS_THEME_DARK).c_str());
        SendMessage(hThemeCombo, CB_SETCURSEL, tempConfig.darkMode ? 1 : 0, 0);
        
        return TRUE;
    }
    
    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        case IDC_COMBO_SETTINGS_PROVIDER:
            if (HIWORD(wParam) == CBN_SELCHANGE) {
                // Save current provider settings
                int currentSel = (int)SendMessage(GetDlgItem(hwndDlg, IDC_COMBO_SETTINGS_PROVIDER), 
                    CB_GETCURSEL, 0, 0);
                if (currentSel >= 0) {
                    wchar_t buffer[1024];
                    GetDlgItemText(hwndDlg, IDC_EDIT_API_URL, buffer, 1024);
                    tempProviderConfigs[currentSel].apiUrl = buffer;
                    GetDlgItemText(hwndDlg, IDC_EDIT_API_KEY, buffer, 1024);
                    tempProviderConfigs[currentSel].apiKey = buffer;
                    GetDlgItemText(hwndDlg, IDC_EDIT_MODEL, buffer, 1024);
                    tempProviderConfigs[currentSel].model = buffer;
                }
                
                // Load new provider settings
                int newSel = (int)SendMessage((HWND)lParam, CB_GETCURSEL, 0, 0);
                if (newSel >= 0 && newSel < (int)AIProvider::ProviderCount) {
                    SetDlgItemText(hwndDlg, IDC_EDIT_API_URL, tempProviderConfigs[newSel].apiUrl.c_str());
                    SetDlgItemText(hwndDlg, IDC_EDIT_API_KEY, tempProviderConfigs[newSel].apiKey.c_str());
                    SetDlgItemText(hwndDlg, IDC_EDIT_MODEL, tempProviderConfigs[newSel].model.c_str());
                }
            }
            return TRUE;
            
        case IDC_BTN_TEST_CONNECTION: {
            // Test connection to current provider
            int sel = (int)SendMessage(GetDlgItem(hwndDlg, IDC_COMBO_SETTINGS_PROVIDER), 
                CB_GETCURSEL, 0, 0);
            if (sel >= 0 && sel < (int)AIProvider::ProviderCount) {
                AIProvider provider = (AIProvider)sel;
                AIProviderConfig& config = tempProviderConfigs[sel];
                
                // Update config from dialog
                wchar_t buffer[1024];
                GetDlgItemText(hwndDlg, IDC_EDIT_API_URL, buffer, 1024);
                config.apiUrl = buffer;
                GetDlgItemText(hwndDlg, IDC_EDIT_API_KEY, buffer, 1024);
                config.apiKey = buffer;
                GetDlgItemText(hwndDlg, IDC_EDIT_MODEL, buffer, 1024);
                config.model = buffer;
                
                // Test connection
                SetDlgItemText(hwndDlg, IDC_STATIC_TEST_RESULT, LoadStringRes(IDS_TEST_CONNECTING).c_str());
                
                std::thread([hwndDlg, provider, config]() {
                    std::wstring result = CallAIAPI(L"Hello, this is a test message.", L"");
                    bool success = !result.empty() && result.find(L"错误") == std::wstring::npos;
                    
                    PostMessage(hwndDlg, WM_USER + 200, success, 0);
                }).detach();
            }
            return TRUE;
        }
        
        case IDC_BTN_CLEAR_HISTORY:
            if (MessageBox(hwndDlg, L"Clear all history records?", L"Confirm", 
                MB_YESNO | MB_ICONQUESTION) == IDYES) {
                ClearHistory();
                MessageBox(hwndDlg, L"History cleared.", L"Info", MB_OK | MB_ICONINFORMATION);
            }
            return TRUE;
            
        case IDOK: {
            // Save settings from dialog to temp config
            tempConfig.autoShowOnSelection = (IsDlgButtonChecked(hwndDlg, IDC_CHK_AUTO_SHOW) == BST_CHECKED);
            tempConfig.rememberWindowSize = (IsDlgButtonChecked(hwndDlg, IDC_CHK_REMEMBER_SIZE) == BST_CHECKED);
            tempConfig.topMost = (IsDlgButtonChecked(hwndDlg, IDC_CHK_TOPMOST) == BST_CHECKED);
            tempConfig.saveHistory = (IsDlgButtonChecked(hwndDlg, IDC_CHK_SAVE_HISTORY) == BST_CHECKED);
            
            // Font settings
            tempConfig.outputFontSize = GetDlgItemInt(hwndDlg, IDC_EDIT_FONT_SIZE, NULL, FALSE);
            tempConfig.inputFontSize = tempConfig.outputFontSize;
            
            wchar_t buffer[1024];
            GetDlgItemText(hwndDlg, IDC_EDIT_FONT_NAME, buffer, 1024);
            tempConfig.fontName = buffer;
            
            // AI settings
            tempConfig.maxTokens = GetDlgItemInt(hwndDlg, IDC_EDIT_MAX_TOKENS, NULL, FALSE);
            
            GetDlgItemText(hwndDlg, IDC_EDIT_TEMPERATURE, buffer, 1024);
            tempConfig.temperature = _wtof(buffer);
            
            tempConfig.maxHistoryDays = GetDlgItemInt(hwndDlg, IDC_EDIT_HISTORY_DAYS, NULL, FALSE);
            
            // Default provider
            int providerSel = (int)SendMessage(GetDlgItem(hwndDlg, IDC_COMBO_SETTINGS_PROVIDER), 
                CB_GETCURSEL, 0, 0);
            if (providerSel >= 0 && providerSel < (int)AIProvider::ProviderCount) {
                tempConfig.defaultProvider = (AIProvider)providerSel;
            }
            
            // Save current provider settings
            if (providerSel >= 0) {
                GetDlgItemText(hwndDlg, IDC_EDIT_API_URL, buffer, 1024);
                tempProviderConfigs[providerSel].apiUrl = buffer;
                GetDlgItemText(hwndDlg, IDC_EDIT_API_KEY, buffer, 1024);
                tempProviderConfigs[providerSel].apiKey = buffer;
                GetDlgItemText(hwndDlg, IDC_EDIT_MODEL, buffer, 1024);
                tempProviderConfigs[providerSel].model = buffer;
            }
            
            // Translation language
            int langSel = (int)SendMessage(GetDlgItem(hwndDlg, IDC_COMBO_TRANSLATE_LANG),
                CB_GETCURSEL, 0, 0);
            if (langSel >= 0 && langSel < (int)TranslateLang::LangCount) {
                tempConfig.defaultTranslateLang = (TranslateLang)langSel;
            }
            
            // Theme
            int themeSel = (int)SendMessage(GetDlgItem(hwndDlg, IDC_COMBO_THEME),
                CB_GETCURSEL, 0, 0);
            tempConfig.darkMode = (themeSel == 1);
            
            // Copy temp config to global
            g_config = tempConfig;
            for (int i = 0; i < (int)AIProvider::ProviderCount; i++) {
                g_config.providers[(AIProvider)i] = tempProviderConfigs[i];
            }
            
            // Save to file
            SaveConfig();
            
            // Apply changes
            UpdateFontSizes();
            ApplyTheme();
            
            EndDialog(hwndDlg, IDOK);
            return TRUE;
        }
        
        case IDCANCEL:
            EndDialog(hwndDlg, IDCANCEL);
            return TRUE;
        }
        break;
        
    case WM_USER + 200: {
        // Test connection result
        if (wParam) {
            SetDlgItemText(hwndDlg, IDC_STATIC_TEST_RESULT, LoadStringRes(IDS_TEST_SUCCESS).c_str());
        } else {
            SetDlgItemText(hwndDlg, IDC_STATIC_TEST_RESULT, LoadStringRes(IDS_TEST_FAILED).c_str());
        }
        return TRUE;
    }
    }
    
    return FALSE;
}

// Show Settings Dialog
void ShowSettingsDialog(HWND hwndParent) {
    DialogBox(g_hInstance, MAKEINTRESOURCE(IDD_SETTINGS), hwndParent, SettingsDlgProc);
}

// History Dialog Procedure
INT_PTR CALLBACK HistoryDlgProc(HWND hwndDlg, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_INITDIALOG: {
        // Center dialog
        RECT rcDlg, rcParent;
        GetWindowRect(hwndDlg, &rcDlg);
        GetWindowRect(GetParent(hwndDlg), &rcParent);
        int x = rcParent.left + (rcParent.right - rcParent.left - (rcDlg.right - rcDlg.left)) / 2;
        int y = rcParent.top + (rcParent.bottom - rcParent.top - (rcDlg.bottom - rcDlg.top)) / 2;
        SetWindowPos(hwndDlg, NULL, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
        
        // Populate history list
        HWND hList = GetDlgItem(hwndDlg, IDC_LIST_HISTORY);
        
        {
            std::lock_guard<std::mutex> lock(g_historyMutex);
            for (const auto& msg : g_chatHistory) {
                std::wstring display = L"[" + FormatTimestamp(msg.timestamp) + L"] ";
                display += (msg.role == L"user" ? L"[我] " : L"[AI] ");
                
                // Truncate content for display
                std::wstring content = msg.content;
                if (content.length() > 50) {
                    content = content.substr(0, 50) + L"...";
                }
                display += content;
                
                int index = (int)SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)display.c_str());
                SendMessage(hList, LB_SETITEMDATA, index, (LPARAM)&msg);
            }
        }
        
        // Set column headers if using list view
        // (For simplicity, using listbox here)
        
        return TRUE;
    }
    
    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        case IDC_LIST_HISTORY:
            if (HIWORD(wParam) == LBN_SELCHANGE || HIWORD(wParam) == LBN_DBLCLK) {
                HWND hList = (HWND)lParam;
                int sel = (int)SendMessage(hList, LB_GETCURSEL, 0, 0);
                if (sel >= 0) {
                    // Show full message in detail area
                    std::lock_guard<std::mutex> lock(g_historyMutex);
                    if (sel < (int)g_chatHistory.size()) {
                        const ChatMessage& msg = g_chatHistory[sel];
                        std::wstring detail;
                        detail += L"Time: ";
                        detail += FormatTimestamp(msg.timestamp);
                        detail += L"\r\nRole: ";
                        detail += (msg.role == L"user" ? L"User" : L"AI");
                        detail += L"\r\nProvider: ";
                        detail += msg.provider;
                        detail += L"\r\nModel: ";
                        detail += msg.model;
                        detail += L"\r\n\r\nContent:\r\n";
                        detail += msg.content;
                        
                        SetDlgItemText(hwndDlg, IDC_EDIT_HISTORY_DETAIL, detail.c_str());
                    }
                }
            }
            return TRUE;
            
        case IDC_BTN_HISTORY_USE:
            // Use selected message in chat
            {
                HWND hList = GetDlgItem(hwndDlg, IDC_LIST_HISTORY);
                int sel = (int)SendMessage(hList, LB_GETCURSEL, 0, 0);
                if (sel >= 0) {
                    std::lock_guard<std::mutex> lock(g_historyMutex);
                    if (sel < (int)g_chatHistory.size()) {
                        const ChatMessage& msg = g_chatHistory[sel];
                        if (g_hwndInput) {
                            SetWindowText(g_hwndInput, msg.content.c_str());
                        }
                    }
                }
                EndDialog(hwndDlg, IDOK);
            }
            return TRUE;
            
        case IDC_BTN_HISTORY_DELETE:
            // Delete selected message
            {
                HWND hList = GetDlgItem(hwndDlg, IDC_LIST_HISTORY);
                int sel = (int)SendMessage(hList, LB_GETCURSEL, 0, 0);
                if (sel >= 0) {
                    if (MessageBox(hwndDlg, L"Delete this record?", L"Confirm",
                        MB_YESNO | MB_ICONQUESTION) == IDYES) {
                        std::lock_guard<std::mutex> lock(g_historyMutex);
                        if (sel < (int)g_chatHistory.size()) {
                            g_chatHistory.erase(g_chatHistory.begin() + sel);
                            SendMessage(hList, LB_DELETESTRING, sel, 0);
                            SetDlgItemText(hwndDlg, IDC_EDIT_HISTORY_DETAIL, L"");
                            SaveHistory();
                        }
                    }
                }
            }
            return TRUE;
            
        case IDC_BTN_HISTORY_EXPORT:
            // Export history to file
            {
                wchar_t fileName[MAX_PATH] = { 0 };
                OPENFILENAME ofn = { 0 };
                ofn.lStructSize = sizeof(ofn);
                ofn.hwndOwner = hwndDlg;
                ofn.lpstrFilter = L"Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0";
                ofn.lpstrFile = fileName;
                ofn.nMaxFile = MAX_PATH;
                ofn.Flags = OFN_OVERWRITEPROMPT;
                ofn.lpstrDefExt = L"txt";
                
                if (GetSaveFileName(&ofn)) {
                    std::wofstream file(fileName);
                    if (file.is_open()) {
                        std::lock_guard<std::mutex> lock(g_historyMutex);
                        for (const auto& msg : g_chatHistory) {
                            file << L"[" << FormatTimestamp(msg.timestamp) << L"] ";
                            file << (msg.role == L"user" ? L"[User]" : L"[AI]") << L"\r\n";
                            file << L"Provider: " << msg.provider << L", Model: " << msg.model << L"\r\n";
                            file << msg.content << L"\r\n";
                            file << L"----------------------------------------\r\n";
                        }
                        file.close();
                        MessageBox(hwndDlg, L"历史记录已导出。", L"提示", MB_OK | MB_ICONINFORMATION);
                    }
                }
            }
            return TRUE;
            
        case IDCANCEL:
        case IDC_BTN_HISTORY_CLOSE:
            EndDialog(hwndDlg, IDCANCEL);
            return TRUE;
        }
        break;
    }
    
    return FALSE;
}

// Show History Dialog
void ShowHistoryDialog(HWND hwndParent) {
    DialogBox(g_hInstance, MAKEINTRESOURCE(IDD_HISTORY), hwndParent, HistoryDlgProc);
}
