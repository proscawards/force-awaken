@echo off
set "params=%*"
cd /d "%~dp0" && ( if exist "%temp%\getadmin.vbs" del "%temp%\getadmin.vbs" ) && fsutil dirty query %systemdrive% 1>nul 2>nul || (  echo Set UAC = CreateObject^("Shell.Application"^) : UAC.ShellExecute "cmd.exe", "/k cd ""%~sdp0"" && %~s0 %params%", "", "runas", 1 >> "%temp%\getadmin.vbs" && "%temp%\getadmin.vbs" && exit /B )

curl -o fa.zip -L --silent --output /dev/null https:/github.com/proscawards/force-awaken/releases/latest/download/Force-Awaken.Setup.zip
Taskkill /f /im Force_Awaken.exe
for %%i in (*.*) do if not "%%i"=="fa.zip" if not "%%i"=="settings/*" if not "%%i"=="update.bat" powershell -Command "remove-item -Force -Recurse "%%i 
powershell -Command "Expand-Archive -Path fa.zip -Force -DestinationPath "%CD%
DEL fa.zip
start Force_Awaken.exe
exit