@echo off
set /p commandName=<"%~dp0\commandName.txt"
start /min "" "%~dp0\..\x64\Release\DynamicMacro.exe" "%commandName%"
echo program launched.