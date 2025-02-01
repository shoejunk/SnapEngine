@echo off
echo === Generating Project Files ===
call generate.bat
if errorlevel 1 (
    echo === Generation Failed ===
    exit /b 1
)

echo === Building Project ===
call build.bat
if errorlevel 1 (
    echo === Build Failed ===
    exit /b 1
)

echo === Build Completed Successfully ===
