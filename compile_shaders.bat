@echo off
set OUTPUT_DIR=%1
set SHADER_DIR=shaders
set FXC_PATH=fxc

echo === Compiling Shaders with FXC ===

for %%F in (%SHADER_DIR%\*.hlsl) do (
    echo Compiling %%~nxF...

    if /i "%%~nF"=="BasicVertexShader" (
        %FXC_PATH% /T vs_5_0 /E main /Fo "%OUTPUT_DIR%\%%~nF.cso" "%%F"
        if errorlevel 1 goto :shader_error
    ) else if /i "%%~nF"=="BasicPixelShader" (
        %FXC_PATH% /T ps_5_0 /E main /Fo "%OUTPUT_DIR%\%%~nF.cso" "%%F"
        if errorlevel 1 goto :shader_error
    )
)