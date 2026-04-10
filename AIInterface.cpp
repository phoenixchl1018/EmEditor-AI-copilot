// EmEditor AI Plugin - AI API Interface
// Copyright (c) 2025 - AI Assistant Plugin for EmEditor

#include "EmEditorAIPlugin.h"
#include "resource.h"
#include <wininet.h>
#include <sstream>

#pragma comment(lib, "wininet.lib")

// Send Chat Message
void SendChatMessage(const std::wstring& message) {
    if (message.empty() || g_isProcessing) return;
    
    // Add to output
    AppendToOutput(message, true);
    
    // Clear input
    if (g_hwndInput) {
        SetWindowText(g_hwndInput, L"");
    }
    
    // Update status
    g_isProcessing = true;
    std::wstring statusText = LoadStringRes(IDS_STATUS_PROCESSING);
    statusText += L" " + GetProviderName(g_config.defaultProvider) + L"...";
    UpdateStatusText(statusText.c_str());
    
    // Process in background thread
    std::thread([message]() {
        std::wstring systemPrompt = GetSystemPrompt(AIFunction::Chat);
        std::wstring response = CallAIAPI(message, systemPrompt);
        
        if (!response.empty()) {
            // Add to history
            {
                std::lock_guard<std::mutex> lock(g_historyMutex);
                SYSTEMTIME st;
                GetLocalTime(&st);
                
                ChatMessage userMsg = { L"user", message, st, 
                    GetProviderName(g_config.defaultProvider),
                    g_config.providers[g_config.defaultProvider].model };
                ChatMessage aiMsg = { L"assistant", response, st,
                    GetProviderName(g_config.defaultProvider),
                    g_config.providers[g_config.defaultProvider].model };
                
                g_chatHistory.push_back(userMsg);
                g_chatHistory.push_back(aiMsg);
                
                // Limit history size
                while (g_chatHistory.size() > MAX_HISTORY_ITEMS * 2) {
                    g_chatHistory.erase(g_chatHistory.begin());
                }
            }
            
            // Send response to UI
            wchar_t* responseCopy = new wchar_t[response.length() + 1];
            wcscpy_s(responseCopy, response.length() + 1, response.c_str());
            PostMessage(g_hwndCustomBar, WM_AI_RESPONSE, 0, (LPARAM)responseCopy);
        } else {
            wchar_t* errorCopy = new wchar_t[100];
            wcscpy_s(errorCopy, 100, L"无法获取响应，请检查API配置。");
            PostMessage(g_hwndCustomBar, WM_AI_ERROR, 0, (LPARAM)errorCopy);
        }
    }).detach();
}

// Summarize Text
void SummarizeText(const std::wstring& text) {
    if (text.empty()) return;
    
    std::wstring prompt = L"请对以下内容进行总结：\n\n" + text;
    SendChatMessage(prompt);
}

// Explain Text
void ExplainText(const std::wstring& text) {
    if (text.empty()) return;
    
    std::wstring prompt = L"请解释以下内容：\n\n" + text;
    SendChatMessage(prompt);
}

// Translate Text
void TranslateText(const std::wstring& text, TranslateLang targetLang) {
    if (text.empty()) return;
    
    std::wstring targetLangName = GetLanguageName(targetLang);
    std::wstring prompt;
    
    if (targetLang == TranslateLang::Auto) {
        prompt = L"请翻译以下内容（自动检测源语言）：\n\n" + text;
    } else {
        prompt = L"请将以下内容翻译成" + targetLangName + L"：\n\n" + text;
    }
    
    SendChatMessage(prompt);
}

// Code Review
void CodeReview(const std::wstring& code) {
    if (code.empty()) return;
    
    std::wstring prompt = L"请审查以下代码：\n\n```\n" + code + L"\n```";
    SendChatMessage(prompt);
}

// Grammar Check
void GrammarCheck(const std::wstring& text) {
    if (text.empty()) return;
    
    std::wstring prompt = L"请检查以下文本的语法错误：\n\n" + text;
    SendChatMessage(prompt);
}

// Rewrite Text
void RewriteText(const std::wstring& text) {
    if (text.empty()) return;
    
    std::wstring prompt = L"请改写以下内容，使其更加流畅和专业：\n\n" + text;
    SendChatMessage(prompt);
}

// Call AI API
std::wstring CallAIAPI(const std::wstring& prompt, const std::wstring& systemPrompt) {
    std::lock_guard<std::mutex> lock(g_apiMutex);
    
    AIProvider provider = g_config.defaultProvider;
    AIProviderConfig& config = g_config.providers[provider];
    
    if (config.apiUrl.empty() || config.apiKey.empty()) {
        return L"错误：API URL 或 API Key 未配置。请在设置中配置API信息。";
    }
    
    // Build request body
    std::vector<ChatMessage> messages;
    if (!systemPrompt.empty()) {
        ChatMessage sysMsg;
        sysMsg.role = L"system";
        sysMsg.content = systemPrompt;
        messages.push_back(sysMsg);
    }
    ChatMessage userMsg;
    userMsg.role = L"user";
    userMsg.content = prompt;
    messages.push_back(userMsg);
    
    std::wstring requestBodyW = BuildRequestBody(provider, messages);
    std::string requestBody = WideToUtf8(requestBodyW);
    
    // Build headers
    std::map<std::wstring, std::wstring> headers = config.headers;
    auto authIt = headers.find(L"Authorization");
    if (authIt != headers.end() && authIt->second.find(L"Bearer ") != std::wstring::npos) {
        authIt->second += config.apiKey;
    }
    
    // For Gemini, append key to URL
    std::wstring url = config.apiUrl;
    if (provider == AIProvider::Gemini) {
        url += L"?key=" + config.apiKey;
        headers.erase(L"Authorization");
    }
    
    // Make HTTP request
    std::string response = HttpPostWithWinInet(url, requestBody, headers, config.timeoutMs);
    
    if (response.empty()) {
        return L"错误：无法连接到AI服务，请检查网络连接和API配置。";
    }
    
    // Parse response
    std::wstring result;
    if (ParseAIResponse(provider, response, result)) {
        return result;
    } else {
        return L"错误：无法解析AI响应。";
    }
}

// Build Request Body
std::wstring BuildRequestBody(AIProvider provider, const std::vector<ChatMessage>& messages) {
    std::wstringstream json;
    AIProviderConfig& config = g_config.providers[provider];
    
    switch (provider) {
    case AIProvider::DeepSeek:
    case AIProvider::OpenAI:
    case AIProvider::Kimi:
    case AIProvider::Doubao:
        // OpenAI-compatible format
        json << L"{";
        json << L"\"model\":\"" << config.model << L"\",";
        json << L"\"messages\":[";
        for (size_t i = 0; i < messages.size(); i++) {
            if (i > 0) json << L",";
            json << L"{\"role\":\"" << messages[i].role << L"\",";
            json << L"\"content\":\"" << EscapeJsonString(messages[i].content) << L"";
            json << L"}";
        }
        json << L"],";
        json << L"\"max_tokens\":" << g_config.maxTokens << L",";
        json << L"\"temperature\":" << g_config.temperature;
        json << L"}";
        break;
        
    case AIProvider::MiniMax:
        // MiniMax format
        json << L"{";
        json << L"\"model\":\"" << config.model << L"\",";
        json << L"\"messages\":[";
        for (size_t i = 0; i < messages.size(); i++) {
            if (i > 0) json << L",";
            json << L"{\"role\":\"" << messages[i].role << L"\",";
            json << L"\"content\":\"" << EscapeJsonString(messages[i].content) << L"";
            json << L"}";
        }
        json << L"]";
        json << L"}";
        break;
        
    case AIProvider::Qianwen:
        // Tongyi Qianwen format
        json << L"{";
        json << L"\"model\":\"" << config.model << L"\",";
        json << L"\"input\":{";
        json << L"\"messages\":[";
        for (size_t i = 0; i < messages.size(); i++) {
            if (i > 0) json << L",";
            json << L"{\"role\":\"" << messages[i].role << L"\",";
            json << L"\"content\":\"" << EscapeJsonString(messages[i].content) << L"";
            json << L"}";
        }
        json << L"]";
        json << L"},";
        json << L"\"parameters\":{";
        json << L"\"max_tokens\":" << g_config.maxTokens << L",";
        json << L"\"temperature\":" << g_config.temperature;
        json << L"}";
        json << L"}";
        break;
        
    case AIProvider::Gemini:
        // Gemini format
        json << L"{";
        json << L"\"contents\":[";
        for (size_t i = 0; i < messages.size(); i++) {
            if (i > 0) json << L",";
            std::wstring role = (messages[i].role == L"user") ? L"user" : L"model";
            json << L"{\"role\":\"" << role << L"\",";
            json << L"\"parts\":[{\"text\":\"" << EscapeJsonString(messages[i].content) << L"";
            json << L"";
            json << L"}]}";
        }
        json << L"],";
        json << L"\"generationConfig\":{";
        json << L"\"maxOutputTokens\":" << g_config.maxTokens << L",";
        json << L"\"temperature\":" << g_config.temperature;
        json << L"}";
        json << L"}";
        break;
        
    default:
        // Custom - use OpenAI-compatible format
        json << L"{";
        json << L"\"model\":\"" << config.model << L"\",";
        json << L"\"messages\":[";
        for (size_t i = 0; i < messages.size(); i++) {
            if (i > 0) json << L",";
            json << L"{\"role\":\"" << messages[i].role << L"\",";
            json << L"\"content\":\"" << EscapeJsonString(messages[i].content) << L"";
            json << L"}";
        }
        json << L"]";
        json << L"}";
        break;
    }
    
    return json.str();
}

// Parse AI Response
bool ParseAIResponse(AIProvider provider, const std::string& response, std::wstring& outText) {
    try {
        // Parse JSON response
        std::string jsonStr = response;
        
        // Simple JSON parsing (in production, use a proper JSON library)
        switch (provider) {
        case AIProvider::DeepSeek:
        case AIProvider::OpenAI:
        case AIProvider::Kimi:
        case AIProvider::Doubao:
        case AIProvider::MiniMax:
            // OpenAI-compatible format
            {
                // Find "content":"
                size_t contentPos = jsonStr.find("\"content\":\"");
                if (contentPos != std::string::npos) {
                    contentPos += 11;
                    size_t endPos = jsonStr.find("\"", contentPos);
                    if (endPos != std::string::npos) {
                        std::string content = jsonStr.substr(contentPos, endPos - contentPos);
                        // Unescape
                        std::wstring wideContent = Utf8ToWide(content);
                        // Replace \\n with \n, etc.
                        size_t pos = 0;
                        while ((pos = wideContent.find(L"\\n", pos)) != std::wstring::npos) {
                            wideContent.replace(pos, 2, L"\n");
                            pos++;
                        }
                        pos = 0;
                        while ((pos = wideContent.find(L"\\\"", pos)) != std::wstring::npos) {
                            wideContent.replace(pos, 2, L"\"");
                            pos++;
                        }
                        outText = wideContent;
                        return true;
                    }
                }
            }
            break;
            
        case AIProvider::Qianwen:
            // Tongyi Qianwen format
            {
                size_t contentPos = jsonStr.find("\"text\":\"");
                if (contentPos != std::string::npos) {
                    contentPos += 9;
                    size_t endPos = jsonStr.find("\"", contentPos);
                    if (endPos != std::string::npos) {
                        std::string content = jsonStr.substr(contentPos, endPos - contentPos);
                        outText = Utf8ToWide(content);
                        return true;
                    }
                }
            }
            break;
            
        case AIProvider::Gemini:
            // Gemini format
            {
                size_t textPos = jsonStr.find("\"text\":\"");
                if (textPos != std::string::npos) {
                    textPos += 9;
                    size_t endPos = jsonStr.find("\"", textPos);
                    if (endPos != std::string::npos) {
                        std::string content = jsonStr.substr(textPos, endPos - textPos);
                        outText = Utf8ToWide(content);
                        return true;
                    }
                }
            }
            break;
            
        default:
            // Try generic parsing
            {
                size_t contentPos = jsonStr.find("\"content\":\"");
                if (contentPos != std::string::npos) {
                    contentPos += 11;
                    size_t endPos = jsonStr.find("\"", contentPos);
                    if (endPos != std::string::npos) {
                        std::string content = jsonStr.substr(contentPos, endPos - contentPos);
                        outText = Utf8ToWide(content);
                        return true;
                    }
                }
            }
            break;
        }
        
        return false;
    } catch (...) {
        return false;
    }
}

// HTTP POST with WinINet
std::string HttpPostWithWinInet(const std::wstring& url, const std::string& body,
                                 const std::map<std::wstring, std::wstring>& headers, int timeoutMs) {
    std::string response;
    HINTERNET hInternet = NULL;
    HINTERNET hConnect = NULL;
    HINTERNET hRequest = NULL;
    
    // Parse URL
    URL_COMPONENTS urlComp = { 0 };
    urlComp.dwStructSize = sizeof(urlComp);
    
    wchar_t hostName[256] = { 0 };
    wchar_t urlPath[1024] = { 0 };
    wchar_t scheme[32] = { 0 };
    
    urlComp.lpszHostName = hostName;
    urlComp.dwHostNameLength = 256;
    urlComp.lpszUrlPath = urlPath;
    urlComp.dwUrlPathLength = 1024;
    urlComp.lpszScheme = scheme;
    urlComp.dwSchemeLength = 32;
    
    if (!InternetCrackUrl(url.c_str(), 0, 0, &urlComp)) {
        return "";
    }
    
    // Open Internet
    hInternet = InternetOpen(L"EmEditorAIPlugin/1.0", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
    if (!hInternet) {
        return "";
    }
    
    // Set timeout
    InternetSetOption(hInternet, INTERNET_OPTION_CONNECT_TIMEOUT, &timeoutMs, sizeof(timeoutMs));
    InternetSetOption(hInternet, INTERNET_OPTION_SEND_TIMEOUT, &timeoutMs, sizeof(timeoutMs));
    InternetSetOption(hInternet, INTERNET_OPTION_RECEIVE_TIMEOUT, &timeoutMs, sizeof(timeoutMs));
    
    // Connect
    INTERNET_PORT port = (urlComp.nScheme == INTERNET_SCHEME_HTTPS) ? INTERNET_DEFAULT_HTTPS_PORT : INTERNET_DEFAULT_HTTP_PORT;
    hConnect = InternetConnect(hInternet, hostName, port, NULL, NULL, INTERNET_SERVICE_HTTP, 0, 0);
    if (!hConnect) {
        InternetCloseHandle(hInternet);
        return "";
    }
    
    // Create request
    DWORD flags = (urlComp.nScheme == INTERNET_SCHEME_HTTPS) ? 
        INTERNET_FLAG_SECURE | INTERNET_FLAG_IGNORE_CERT_CN_INVALID | INTERNET_FLAG_IGNORE_CERT_DATE_INVALID : 0;
    
    hRequest = HttpOpenRequest(hConnect, L"POST", urlPath, NULL, NULL, NULL, flags, 0);
    if (!hRequest) {
        InternetCloseHandle(hConnect);
        InternetCloseHandle(hInternet);
        return "";
    }
    
    // Add headers
    std::wstring headerStr;
    for (const auto& header : headers) {
        headerStr += header.first + L": " + header.second + L"\r\n";
    }
    
    // Send request
    BOOL result = HttpSendRequest(hRequest, headerStr.c_str(), (DWORD)headerStr.length(),
                                   (LPVOID)body.c_str(), (DWORD)body.length());
    
    if (!result) {
        InternetCloseHandle(hRequest);
        InternetCloseHandle(hConnect);
        InternetCloseHandle(hInternet);
        return "";
    }
    
    // Read response
    char buffer[4096];
    DWORD bytesRead;
    while (InternetReadFile(hRequest, buffer, sizeof(buffer), &bytesRead) && bytesRead > 0) {
        response.append(buffer, bytesRead);
    }
    
    // Cleanup
    InternetCloseHandle(hRequest);
    InternetCloseHandle(hConnect);
    InternetCloseHandle(hInternet);
    
    return response;
}
