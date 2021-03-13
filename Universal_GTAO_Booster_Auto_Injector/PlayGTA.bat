@echo off

REM Uses https://github.com/nefarius/Injector and https://github.com/QuickNET-Tech/GTAO_Booster_PoC/

REM Start GTAV through Steam
REM start steam://rungameid/271590

ECHO Simple Universal GTAO Booster Auto-Injector
ECHO Waiting for GTA5.exe...
ECHO _______________________

REM Search for GTA5.exe through tasklist. If not found, wait a few seconds and rescan
:search
tasklist|find "GTA5.exe" >nul 2>&1
IF %ERRORLEVEL% == 0 GOTO :inject
TIMEOUT /t 1 >nul 2>&1
GOTO :search

REM Wait for GTA5.exe to exit
:wait
tasklist|find "GTA5.exe" >nul 2>&1
IF %ERRORLEVEL% == 1 GOTO :search
TIMEOUT /T 5 >nul 2>&1
GOTO :wait

REM Run the injector
:inject
ECHO Injecting into GTA5.exe
start Injector.exe --process-name GTA5.exe --inject Universal.GTAO_Booster.dll
GOTO :wait
