# EmEditor AI Assistant Plugin

A powerful AI assistant plugin for EmEditor text editor, supporting multiple AI platforms for text summarization, explanation, translation, and chat.

## Features

- **Multiple AI Platforms**: DeepSeek, Doubao (Volcano), Kimi, MiniMax, Qianwen, OpenAI GPT, Google Gemini
- **Smart Text Selection**: Auto-popup when text is selected
- **AI Functions**: Summarize, Explain, Translate, Chat, Code Review, Grammar Check, Rewrite
- **History Management**: Save and export chat history
- **Customizable UI**: Adjustable window size, font size, themes
- **Multi-language Support**: Interface in Chinese

## Project Structure

```
EmEditorAIPlugin/
├── EmEditorAIPlugin.h      # Main header file
├── EmEditorAIPlugin.cpp    # Main source file
├── AIInterface.cpp         # AI API interface
├── ConfigManager.cpp       # Configuration management
├── Dialogs.cpp             # Settings and history dialogs
├── plugin.h                # EmEditor plugin SDK
├── resource.h              # Resource definitions
├── EmEditorAIPlugin.rc     # Resource script
├── EmEditorAIPlugin.def    # Module definition file
├── EmEditorAIPlugin.vcxproj # Visual Studio project
├── EmEditorAIPlugin.sln    # Visual Studio solution
├── plugin.ico              # Plugin icon
├── plugin.bmp              # Toolbar bitmap
├── 使用手册.md              # User manual (Chinese)
└── README.md               # This file
```

## Building

### Prerequisites

- Visual Studio 2022 or later
- Windows SDK 10.0 or later
- C++17 support

### Build Steps

1. Open `EmEditorAIPlugin.sln` in Visual Studio
2. Select Release configuration
3. Build the solution (Ctrl+Shift+B)
4. The output DLL will be in `Release\EmEditorAIPlugin.dll`

### Build from Command Line

```bash
# Using MSBuild
msbuild EmEditorAIPlugin.sln /p:Configuration=Release /p:Platform=x64

# Or using Visual Studio Developer Command Prompt
devenv EmEditorAIPlugin.sln /build "Release|x64"
```

## Installation

1. Close EmEditor
2. Copy `EmEditorAIPlugin.dll` to EmEditor plugins folder:
   - `C:\Program Files\EmEditor\PlugIns\`
   - Or `%APPDATA%\Emurasoft\EmEditor\PlugIns\`
3. Start EmEditor
4. Enable the plugin from Tools → Plugins → AI Assistant

## Configuration

1. Click the **Settings** button in the AI assistant window
2. Select your preferred AI platform
3. Enter your API key and model name
4. Click **Test Connection** to verify
5. Click **OK** to save settings

### API Key Sources

- **DeepSeek**: https://platform.deepseek.com/
- **Doubao**: https://console.volcengine.com/
- **Kimi**: https://platform.moonshot.cn/
- **MiniMax**: https://api.minimax.chat/
- **Qianwen**: https://dashscope.aliyun.com/
- **OpenAI**: https://platform.openai.com/
- **Gemini**: https://ai.google.dev/

## Usage

1. Select text in EmEditor
2. The AI assistant window will auto-popup (if enabled)
3. Click function buttons (Summarize, Explain, Translate, Chat)
4. Or type your question in the input box and press Enter
5. View AI responses in the output area

## Configuration File

Settings are stored in:
```
%APPDATA%\EmEditorAIPlugin\EmEditorAIPlugin.ini
```

## History File

Chat history is stored in:
```
%APPDATA%\EmEditorAIPlugin\AIChatHistory.json
```

## License

MIT License

Copyright (c) 2025 EmEditor AI Plugin Developer

## Acknowledgments

- EmEditor team for the excellent plugin SDK
- All AI platform providers for their APIs

---

For detailed usage instructions in Chinese, see [使用手册.md](使用手册.md)
