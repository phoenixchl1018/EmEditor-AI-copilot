// EmEditor AI Plugin - Configuration Manager
// Copyright (c) 2025 - AI Assistant Plugin for EmEditor

#include "EmEditorAIPlugin.h"
#include <shlobj.h>
#include <fstream>
#include <sstream>

// Default Configuration
void InitDefaultConfig() {
    g_config.windowWidth = DEFAULT_WINDOW_WIDTH;
    g_config.windowHeight = DEFAULT_WINDOW_HEIGHT;
    g_config.windowPosX = CW_USEDEFAULT;
    g_config.windowPosY = CW_USEDEFAULT;
    g_config.autoShowOnSelection = true;
    g_config.rememberWindowSize = true;
    g_config.topMost = false;
    
    g_config.inputFontSize = DEFAULT_FONT_SIZE;
    g_config.outputFontSize = DEFAULT_FONT_SIZE;
    g_config.fontName = L"Microsoft YaHei";
    g_config.fontBold = false;
    
    g_config.defaultProvider = AIProvider::DeepSeek;
    g_config.defaultTranslateLang = TranslateLang::ChineseSimplified;
    g_config.maxTokens = 2048;
    g_config.temperature = 0.7;
    g_config.streamResponse = false;
    
    g_config.saveHistory = true;
    g_config.maxHistoryDays = 30;
    
    g_config.showFunctionButtons = true;
    g_config.showProviderSelector = true;
    g_config.darkMode = false;
    g_config.theme = L"default";
}

// Get Config Path
std::wstring GetConfigPath() {
    wchar_t path[MAX_PATH];
    if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, 0, path))) {
        std::wstring configPath = path;
        configPath += L"\\EmEditorAIPlugin";
        CreateDirectory(configPath.c_str(), NULL);
        configPath += L"\\";
        configPath += CONFIG_FILE_NAME;
        return configPath;
    }
    return L"";
}

// Get History Path
std::wstring GetHistoryPath() {
    wchar_t path[MAX_PATH];
    if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, 0, path))) {
        std::wstring historyPath = path;
        historyPath += L"\\EmEditorAIPlugin";
        CreateDirectory(historyPath.c_str(), NULL);
        historyPath += L"\\";
        historyPath += HISTORY_FILE_NAME;
        return historyPath;
    }
    return L"";
}

// Load Configuration
void LoadConfig() {
    InitDefaultConfig();
    InitDefaultProviderConfigs();
    
    std::wstring configPath = GetConfigPath();
    if (configPath.empty()) return;
    
    // Read INI file
    wchar_t buffer[1024];
    
    // Window settings
    GetPrivateProfileString(L"Window", L"Width", L"400", buffer, 1024, configPath.c_str());
    g_config.windowWidth = _wtoi(buffer);
    
    GetPrivateProfileString(L"Window", L"Height", L"500", buffer, 1024, configPath.c_str());
    g_config.windowHeight = _wtoi(buffer);
    
    GetPrivateProfileString(L"Window", L"PosX", L"-1", buffer, 1024, configPath.c_str());
    g_config.windowPosX = _wtoi(buffer);
    
    GetPrivateProfileString(L"Window", L"PosY", L"-1", buffer, 1024, configPath.c_str());
    g_config.windowPosY = _wtoi(buffer);
    
    GetPrivateProfileString(L"Window", L"AutoShowOnSelection", L"1", buffer, 1024, configPath.c_str());
    g_config.autoShowOnSelection = (_wtoi(buffer) != 0);
    
    GetPrivateProfileString(L"Window", L"RememberWindowSize", L"1", buffer, 1024, configPath.c_str());
    g_config.rememberWindowSize = (_wtoi(buffer) != 0);
    
    GetPrivateProfileString(L"Window", L"TopMost", L"0", buffer, 1024, configPath.c_str());
    g_config.topMost = (_wtoi(buffer) != 0);
    
    // Font settings
    GetPrivateProfileString(L"Font", L"InputSize", L"10", buffer, 1024, configPath.c_str());
    g_config.inputFontSize = _wtoi(buffer);
    
    GetPrivateProfileString(L"Font", L"OutputSize", L"10", buffer, 1024, configPath.c_str());
    g_config.outputFontSize = _wtoi(buffer);
    
    GetPrivateProfileString(L"Font", L"Name", L"Microsoft YaHei", buffer, 1024, configPath.c_str());
    g_config.fontName = buffer;
    
    GetPrivateProfileString(L"Font", L"Bold", L"0", buffer, 1024, configPath.c_str());
    g_config.fontBold = (_wtoi(buffer) != 0);
    
    // AI settings
    GetPrivateProfileString(L"AI", L"DefaultProvider", L"0", buffer, 1024, configPath.c_str());
    int provider = _wtoi(buffer);
    if (provider >= 0 && provider < (int)AIProvider::ProviderCount) {
        g_config.defaultProvider = (AIProvider)provider;
    }
    
    GetPrivateProfileString(L"AI", L"DefaultTranslateLang", L"1", buffer, 1024, configPath.c_str());
    int lang = _wtoi(buffer);
    if (lang >= 0 && lang < (int)TranslateLang::LangCount) {
        g_config.defaultTranslateLang = (TranslateLang)lang;
    }
    
    GetPrivateProfileString(L"AI", L"MaxTokens", L"2048", buffer, 1024, configPath.c_str());
    g_config.maxTokens = _wtoi(buffer);
    
    GetPrivateProfileString(L"AI", L"Temperature", L"0.7", buffer, 1024, configPath.c_str());
    g_config.temperature = _wtof(buffer);
    
    GetPrivateProfileString(L"AI", L"StreamResponse", L"0", buffer, 1024, configPath.c_str());
    g_config.streamResponse = (_wtoi(buffer) != 0);
    
    // History settings
    GetPrivateProfileString(L"History", L"SaveHistory", L"1", buffer, 1024, configPath.c_str());
    g_config.saveHistory = (_wtoi(buffer) != 0);
    
    GetPrivateProfileString(L"History", L"MaxHistoryDays", L"30", buffer, 1024, configPath.c_str());
    g_config.maxHistoryDays = _wtoi(buffer);
    
    // UI settings
    GetPrivateProfileString(L"UI", L"ShowFunctionButtons", L"1", buffer, 1024, configPath.c_str());
    g_config.showFunctionButtons = (_wtoi(buffer) != 0);
    
    GetPrivateProfileString(L"UI", L"ShowProviderSelector", L"1", buffer, 1024, configPath.c_str());
    g_config.showProviderSelector = (_wtoi(buffer) != 0);
    
    GetPrivateProfileString(L"UI", L"DarkMode", L"0", buffer, 1024, configPath.c_str());
    g_config.darkMode = (_wtoi(buffer) != 0);
    
    GetPrivateProfileString(L"UI", L"Theme", L"default", buffer, 1024, configPath.c_str());
    g_config.theme = buffer;
    
    // Load provider configurations
    for (int i = 0; i < (int)AIProvider::ProviderCount; i++) {
        AIProvider provider = (AIProvider)i;
        std::wstring section = L"Provider_" + std::to_wstring(i);
        
        GetPrivateProfileString(section.c_str(), L"ApiUrl", g_config.providers[provider].apiUrl.c_str(), 
            buffer, 1024, configPath.c_str());
        g_config.providers[provider].apiUrl = buffer;
        
        GetPrivateProfileString(section.c_str(), L"ApiKey", L"", buffer, 1024, configPath.c_str());
        g_config.providers[provider].apiKey = buffer;
        
        GetPrivateProfileString(section.c_str(), L"Model", g_config.providers[provider].defaultModel.c_str(),
            buffer, 1024, configPath.c_str());
        g_config.providers[provider].model = buffer;
        
        GetPrivateProfileString(section.c_str(), L"Enabled", L"1", buffer, 1024, configPath.c_str());
        g_config.providers[provider].enabled = (_wtoi(buffer) != 0);
    }
}

// Save Configuration
void SaveConfig() {
    std::wstring configPath = GetConfigPath();
    if (configPath.empty()) return;
    
    // Window settings
    WritePrivateProfileString(L"Window", L"Width", std::to_wstring(g_config.windowWidth).c_str(), configPath.c_str());
    WritePrivateProfileString(L"Window", L"Height", std::to_wstring(g_config.windowHeight).c_str(), configPath.c_str());
    WritePrivateProfileString(L"Window", L"PosX", std::to_wstring(g_config.windowPosX).c_str(), configPath.c_str());
    WritePrivateProfileString(L"Window", L"PosY", std::to_wstring(g_config.windowPosY).c_str(), configPath.c_str());
    WritePrivateProfileString(L"Window", L"AutoShowOnSelection", 
        g_config.autoShowOnSelection ? L"1" : L"0", configPath.c_str());
    WritePrivateProfileString(L"Window", L"RememberWindowSize", 
        g_config.rememberWindowSize ? L"1" : L"0", configPath.c_str());
    WritePrivateProfileString(L"Window", L"TopMost", 
        g_config.topMost ? L"1" : L"0", configPath.c_str());
    
    // Font settings
    WritePrivateProfileString(L"Font", L"InputSize", std::to_wstring(g_config.inputFontSize).c_str(), configPath.c_str());
    WritePrivateProfileString(L"Font", L"OutputSize", std::to_wstring(g_config.outputFontSize).c_str(), configPath.c_str());
    WritePrivateProfileString(L"Font", L"Name", g_config.fontName.c_str(), configPath.c_str());
    WritePrivateProfileString(L"Font", L"Bold", g_config.fontBold ? L"1" : L"0", configPath.c_str());
    
    // AI settings
    WritePrivateProfileString(L"AI", L"DefaultProvider", std::to_wstring((int)g_config.defaultProvider).c_str(), configPath.c_str());
    WritePrivateProfileString(L"AI", L"DefaultTranslateLang", std::to_wstring((int)g_config.defaultTranslateLang).c_str(), configPath.c_str());
    WritePrivateProfileString(L"AI", L"MaxTokens", std::to_wstring(g_config.maxTokens).c_str(), configPath.c_str());
    WritePrivateProfileString(L"AI", L"Temperature", std::to_wstring(g_config.temperature).c_str(), configPath.c_str());
    WritePrivateProfileString(L"AI", L"StreamResponse", g_config.streamResponse ? L"1" : L"0", configPath.c_str());
    
    // History settings
    WritePrivateProfileString(L"History", L"SaveHistory", g_config.saveHistory ? L"1" : L"0", configPath.c_str());
    WritePrivateProfileString(L"History", L"MaxHistoryDays", std::to_wstring(g_config.maxHistoryDays).c_str(), configPath.c_str());
    
    // UI settings
    WritePrivateProfileString(L"UI", L"ShowFunctionButtons", g_config.showFunctionButtons ? L"1" : L"0", configPath.c_str());
    WritePrivateProfileString(L"UI", L"ShowProviderSelector", g_config.showProviderSelector ? L"1" : L"0", configPath.c_str());
    WritePrivateProfileString(L"UI", L"DarkMode", g_config.darkMode ? L"1" : L"0", configPath.c_str());
    WritePrivateProfileString(L"UI", L"Theme", g_config.theme.c_str(), configPath.c_str());
    
    // Save provider configurations
    for (int i = 0; i < (int)AIProvider::ProviderCount; i++) {
        AIProvider provider = (AIProvider)i;
        std::wstring section = L"Provider_" + std::to_wstring(i);
        
        WritePrivateProfileString(section.c_str(), L"ApiUrl", g_config.providers[provider].apiUrl.c_str(), configPath.c_str());
        WritePrivateProfileString(section.c_str(), L"ApiKey", g_config.providers[provider].apiKey.c_str(), configPath.c_str());
        WritePrivateProfileString(section.c_str(), L"Model", g_config.providers[provider].model.c_str(), configPath.c_str());
        WritePrivateProfileString(section.c_str(), L"Enabled", 
            g_config.providers[provider].enabled ? L"1" : L"0", configPath.c_str());
    }
}

// Load History
void LoadHistory() {
    if (!g_config.saveHistory) return;
    
    std::wstring historyPath = GetHistoryPath();
    if (historyPath.empty()) return;
    
    std::ifstream file(historyPath, std::ios::binary);
    if (!file.is_open()) return;
    
    // Read file content
    std::string content((std::istreambuf_iterator<char>(file)),
                         std::istreambuf_iterator<char>());
    file.close();
    
    if (content.empty()) return;
    
    // Simple JSON parsing for history
    // Format: [{"role":"user","content":"...","timestamp":"...","provider":"...","model":"..."},...]
    
    // Clear existing history
    g_chatHistory.clear();
    
    // Parse JSON array
    size_t pos = 0;
    while ((pos = content.find("{", pos)) != std::string::npos) {
        ChatMessage msg;
        
        // Parse role
        size_t rolePos = content.find("\"role\":\"", pos);
        if (rolePos != std::string::npos) {
            rolePos += 8;
            size_t roleEnd = content.find("\"", rolePos);
            if (roleEnd != std::string::npos) {
                msg.role = Utf8ToWide(content.substr(rolePos, roleEnd - rolePos));
            }
        }
        
        // Parse content
        size_t contentPos = content.find("\"content\":\"", pos);
        if (contentPos != std::string::npos) {
            contentPos += 12;
            size_t contentEnd = content.find("\"", contentPos);
            if (contentEnd != std::string::npos) {
                msg.content = Utf8ToWide(content.substr(contentPos, contentEnd - contentPos));
            }
        }
        
        // Parse timestamp
        size_t timePos = content.find("\"timestamp\":\"", pos);
        if (timePos != std::string::npos) {
            timePos += 14;
            size_t timeEnd = content.find("\"", timePos);
            if (timeEnd != std::string::npos) {
                std::wstring timeStr = Utf8ToWide(content.substr(timePos, timeEnd - timePos));
                // Parse timestamp
                swscanf_s(timeStr.c_str(), L"%hd-%hd-%hd %hd:%hd:%hd",
                    &msg.timestamp.wYear, &msg.timestamp.wMonth, &msg.timestamp.wDay,
                    &msg.timestamp.wHour, &msg.timestamp.wMinute, &msg.timestamp.wSecond);
            }
        }
        
        // Parse provider
        size_t providerPos = content.find("\"provider\":\"", pos);
        if (providerPos != std::string::npos) {
            providerPos += 13;
            size_t providerEnd = content.find("\"", providerPos);
            if (providerEnd != std::string::npos) {
                msg.provider = Utf8ToWide(content.substr(providerPos, providerEnd - providerPos));
            }
        }
        
        // Parse model
        size_t modelPos = content.find("\"model\":\"", pos);
        if (modelPos != std::string::npos) {
            modelPos += 10;
            size_t modelEnd = content.find("\"", modelPos);
            if (modelEnd != std::string::npos) {
                msg.model = Utf8ToWide(content.substr(modelPos, modelEnd - modelPos));
            }
        }
        
        if (!msg.content.empty()) {
            g_chatHistory.push_back(msg);
        }
        
        pos++;
    }
}

// Save History
void SaveHistory() {
    if (!g_config.saveHistory) return;
    
    std::wstring historyPath = GetHistoryPath();
    if (historyPath.empty()) return;
    
    std::ofstream file(historyPath, std::ios::binary);
    if (!file.is_open()) return;
    
    // Write JSON array
    file << "[";
    
    for (size_t i = 0; i < g_chatHistory.size(); i++) {
        if (i > 0) file << ",";
        
        const ChatMessage& msg = g_chatHistory[i];
        
        file << "{";
        file << "\"role\":\"" << WideToUtf8(msg.role) << "\",";
        file << "\"content\":\"" << WideToUtf8(EscapeJsonString(msg.content)) << "\",";
        file << "\"timestamp\":\"" << WideToUtf8(FormatTimestamp(msg.timestamp)) << "\",";
        file << "\"provider\":\"" << WideToUtf8(msg.provider) << "\",";
        file << "\"model\":\"" << WideToUtf8(msg.model) << "\"";
        file << "}";
    }
    
    file << "]";
    file.close();
}

// Clear History
void ClearHistory() {
    g_chatHistory.clear();
    
    std::wstring historyPath = GetHistoryPath();
    if (!historyPath.empty()) {
        DeleteFile(historyPath.c_str());
    }
}
