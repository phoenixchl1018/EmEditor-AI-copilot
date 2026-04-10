@echo off
chcp 65001 >nul
title EmEditor AI插件 - 一键编译工具
color 0A
cls

echo ============================================
echo    EmEditor AI助手插件 - 一键编译工具
echo ============================================
echo.

:: 检查管理员权限
net session >nul 2>&1
if %errorLevel% neq 0 (
    echo [提示] 需要管理员权限来安装运行库
    echo 正在请求管理员权限...
    powershell -Command "Start-Process '%~f0' -Verb RunAs"
    exit /b
)

:: 查找 Visual Studio
echo [1/5] 正在查找 Visual Studio...
set "VS_FOUND=0"
set "MSBUILD="

for %%p in (
    "C:\Program Files\Microsoft Visual Studio\2022\Community"
    "C:\Program Files\Microsoft Visual Studio\2022\Professional"
    "C:\Program Files\Microsoft Visual Studio\2022\Enterprise"
    "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community"
    "C:\Program Files (x86)\Microsoft Visual Studio\2019\Professional"
    "C:\Program Files (x86)\Microsoft Visual Studio\2019\Enterprise"
) do (
    if exist "%%~p\MSBuild\Current\Bin\MSBuild.exe" (
        set "MSBUILD=%%~p\MSBuild\Current\Bin\MSBuild.exe"
        set "VS_PATH=%%~p"
        set "VS_FOUND=1"
        echo [成功] 找到 Visual Studio: %%~p
        goto :VS_FOUND
    )
)

if %VS_FOUND%==0 (
    echo [错误] 未找到 Visual Studio!
    echo.
    echo 请按以下步骤安装：
    echo 1. 访问 https://visualstudio.microsoft.com/zh-hans/downloads/
    echo 2. 下载 Visual Studio Community 2022（免费）
    echo 3. 安装时勾选"使用C++的桌面开发"
    echo.
    echo 按任意键打开下载页面...
    pause >nul
    start https://visualstudio.microsoft.com/zh-hans/downloads/
    exit /b 1
)

:VS_FOUND
echo.

:: 检查 Windows SDK
echo [2/5] 正在检查 Windows SDK...
set "SDK_FOUND=0"
for %%s in (
    "C:\Program Files (x86)\Windows Kits\10"
    "C:\Program Files\Windows Kits\10"
) do (
    if exist "%%~s\Include\" (
        set "SDK_FOUND=1"
        echo [成功] 找到 Windows SDK: %%~s
        goto :SDK_FOUND
    )
)

if %SDK_FOUND%==0 (
    echo [警告] 未找到 Windows SDK，编译可能会失败
    echo 建议通过 Visual Studio Installer 安装 Windows SDK
)

:SDK_FOUND
echo.

:: 设置环境
echo [3/5] 正在设置编译环境...
call "%VS_PATH%\VC\Auxiliary\Build\vcvarsall.bat" x64 >nul 2>&1
if errorlevel 1 (
    echo [错误] 无法设置编译环境
    pause
    exit /b 1
)
echo [成功] 环境设置完成
echo.

:: 清理旧文件
echo [4/5] 正在清理旧文件...
if exist "x64" rmdir /s /q "x64" 2>nul
if exist "Release" rmdir /s /q "Release" 2>nul
if exist "Debug" rmdir /s /q "Debug" 2>nul
echo [成功] 清理完成
echo.

:: 编译
echo [5/5] 正在编译项目...
echo 配置: Release x64
echo.

"%MSBUILD%" EmEditorAIPlugin.sln ^
    /t:Build ^
    /p:Configuration=Release ^
    /p:Platform=x64 ^
    /p:BuildProjectReferences=true ^
    /verbosity:minimal

if errorlevel 1 (
    echo.
    echo ============================================
    echo [失败] 编译出错！
    echo ============================================
    echo.
    echo 常见错误及解决方法：
    echo.
    echo 1. 找不到头文件 - 确保 Windows SDK 已安装
    echo 2. 链接错误 - 确保所有 .cpp 文件都在项目中
    echo 3. 语法错误 - 检查代码是否完整
    echo.
    echo 查看上方的错误信息了解详情。
    pause
    exit /b 1
)

echo.
echo ============================================
echo [成功] 编译完成！
echo ============================================
echo.

:: 检查输出文件
if exist "x64\Release\EmEditorAIPlugin.dll" (
    set "DLL_PATH=x64\Release\EmEditorAIPlugin.dll"
) else if exist "Release\EmEditorAIPlugin.dll" (
    set "DLL_PATH=Release\EmEditorAIPlugin.dll"
) else (
    echo [错误] 找不到编译输出的 DLL 文件
    pause
    exit /b 1
)

:: 显示文件信息
for %%F in ("%DLL_PATH%") do (
    echo DLL文件: %%~fF
    echo 文件大小: %%~zF 字节
    echo 修改时间: %%~tF
)
echo.

:: 安装选项
echo 是否安装到 EmEditor 插件目录？
echo [1] 是 - 安装到 Program Files
echo [2] 是 - 安装到用户目录
echo [3] 否 - 仅显示文件位置
echo.
set /p CHOICE="请选择 (1-3): "

if "%CHOICE%"=="1" (
    echo.
    echo 正在安装...
    if exist "C:\Program Files\EmEditor\PlugIns" (
        copy /y "%DLL_PATH%" "C:\Program Files\EmEditor\PlugIns\" >nul
        echo [成功] 已安装到: C:\Program Files\EmEditor\PlugIns\
    ) else (
        echo [错误] 未找到 EmEditor 安装目录
        echo 请手动复制文件到 EmEditor 插件目录
    )
) else if "%CHOICE%"=="2" (
    echo.
    echo 正在安装...
    set "USER_PLUGIN=%LOCALAPPDATA%\Emurasoft\EmEditor\PlugIns"
    if not exist "%USER_PLUGIN%" mkdir "%USER_PLUGIN%"
    copy /y "%DLL_PATH%" "%USER_PLUGIN%\" >nul
    echo [成功] 已安装到: %USER_PLUGIN%
) else (
    echo.
    echo DLL文件位置: %DLL_PATH%
)

echo.
echo ============================================
echo 编译和安装完成！
echo ============================================
echo.
echo 下一步：
echo 1. 启动 EmEditor
echo 2. 菜单: 工具 → 插件 → AI助手
echo 3. 点击"设置"配置 API 密钥
echo.
pause
