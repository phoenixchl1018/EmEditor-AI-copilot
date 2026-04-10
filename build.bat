@echo off
chcp 65001 >nul
echo ==========================================
echo EmEditor AI Plugin 构建脚本
echo ==========================================
echo.

:: Check for Visual Studio installation
set "VS_PATH="
set "MSBUILD_PATH="

:: Try to find Visual Studio 2022
if exist "%ProgramFiles%\Microsoft Visual Studio\2022\Community\MSBuild\Current\Bin\MSBuild.exe" (
    set "MSBUILD_PATH=%ProgramFiles%\Microsoft Visual Studio\2022\Community\MSBuild\Current\Bin\MSBuild.exe"
    set "VS_PATH=%ProgramFiles%\Microsoft Visual Studio\2022\Community"
) else if exist "%ProgramFiles%\Microsoft Visual Studio\2022\Professional\MSBuild\Current\Bin\MSBuild.exe" (
    set "MSBUILD_PATH=%ProgramFiles%\Microsoft Visual Studio\2022\Professional\MSBuild\Current\Bin\MSBuild.exe"
    set "VS_PATH=%ProgramFiles%\Microsoft Visual Studio\2022\Professional"
) else if exist "%ProgramFiles%\Microsoft Visual Studio\2022\Enterprise\MSBuild\Current\Bin\MSBuild.exe" (
    set "MSBUILD_PATH=%ProgramFiles%\Microsoft Visual Studio\2022\Enterprise\MSBuild\Current\Bin\MSBuild.exe"
    set "VS_PATH=%ProgramFiles%\Microsoft Visual Studio\2022\Enterprise"
) else if exist "%ProgramFiles(x86)%\Microsoft Visual Studio\2019\Community\MSBuild\Current\Bin\MSBuild.exe" (
    set "MSBUILD_PATH=%ProgramFiles(x86)%\Microsoft Visual Studio\2019\Community\MSBuild\Current\Bin\MSBuild.exe"
    set "VS_PATH=%ProgramFiles(x86)%\Microsoft Visual Studio\2019\Community"
)

if not defined MSBUILD_PATH (
    echo [错误] 未找到 Visual Studio MSBuild。
    echo 请确保已安装 Visual Studio 2019 或更高版本。
    pause
    exit /b 1
)

echo [信息] 找到 Visual Studio: %VS_PATH%
echo [信息] MSBuild 路径: %MSBUILD_PATH%
echo.

:: Parse command line arguments
set "CONFIG=Release"
set "PLATFORM=x64"

if /i "%1"=="Debug" set "CONFIG=Debug"
if /i "%1"=="Release" set "CONFIG=Release"
if /i "%2"=="x86" set "PLATFORM=Win32"
if /i "%2"=="Win32" set "PLATFORM=Win32"
if /i "%2"=="x64" set "PLATFORM=x64"

echo [信息] 构建配置: %CONFIG%
echo [信息] 目标平台: %PLATFORM%
echo.

:: Setup environment
echo [信息] 设置构建环境...
call "%VS_PATH%\VC\Auxiliary\Build\vcvarsall.bat" %PLATFORM% >nul 2>&1
if errorlevel 1 (
    echo [错误] 无法设置 Visual Studio 环境。
    pause
    exit /b 1
)

:: Clean previous build
echo [信息] 清理之前的构建...
if exist "%PLATFORM%\%CONFIG%" rmdir /s /q "%PLATFORM%\%CONFIG%" 2>nul
if exist "%CONFIG%" rmdir /s /q "%CONFIG%" 2>nul

:: Build the project
echo [信息] 开始构建...
echo.
"%MSBUILD_PATH%" EmEditorAIPlugin.sln /t:Build /p:Configuration=%CONFIG% /p:Platform=%PLATFORM% /m

if errorlevel 1 (
    echo.
    echo [错误] 构建失败！
    pause
    exit /b 1
)

echo.
echo ==========================================
echo [成功] 构建完成！
echo ==========================================
echo.

:: Show output location
if "%PLATFORM%"=="Win32" (
    echo 输出文件: %CONFIG%\EmEditorAIPlugin.dll
) else (
    echo 输出文件: %PLATFORM%\%CONFIG%\EmEditorAIPlugin.dll
)
echo.

:: Copy to EmEditor plugins folder (optional)
echo 是否安装到 EmEditor 插件目录？(Y/N)
set /p INSTALL=
if /i "%INSTALL%"=="Y" (
    echo [信息] 正在安装插件...
    
    :: Try common EmEditor installation paths
    if exist "%ProgramFiles%\EmEditor\PlugIns" (
        if "%PLATFORM%"=="Win32" (
            copy /y "%CONFIG%\EmEditorAIPlugin.dll" "%ProgramFiles%\EmEditor\PlugIns\" >nul
        ) else (
            copy /y "%PLATFORM%\%CONFIG%\EmEditorAIPlugin.dll" "%ProgramFiles%\EmEditor\PlugIns\" >nul
        )
        echo [成功] 插件已安装到: %ProgramFiles%\EmEditor\PlugIns\
    ) else if exist "%ProgramFiles(x86)%\EmEditor\PlugIns" (
        if "%PLATFORM%"=="Win32" (
            copy /y "%CONFIG%\EmEditorAIPlugin.dll" "%ProgramFiles(x86)%\EmEditor\PlugIns\" >nul
        ) else (
            copy /y "%PLATFORM%\%CONFIG%\EmEditorAIPlugin.dll" "%ProgramFiles(x86)%\EmEditor\PlugIns\" >nul
        )
        echo [成功] 插件已安装到: %ProgramFiles(x86)%\EmEditor\PlugIns\
    ) else (
        echo [警告] 未找到 EmEditor 安装目录。
        echo 请手动复制 DLL 文件到 EmEditor 插件目录。
    )
)

echo.
pause
