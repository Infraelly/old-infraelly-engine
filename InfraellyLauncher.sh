echo off
:start
title Infraelly Launcher
cls
echo                          ____
echo       __                / __ \                                /\     /\
echo      /_/               / /  \/                               / /    / /
echo      __    ____       / /_    ____     ____       ____      / /    / /
echo     / /   / __ \     / ___\  / __ \   / __ \     / __ \    / /    / /   /\  /\
echo    / /   / /  \ \   / /     / /  \/  / /  \ \   / ____/   / /    / /   / / / /
echo   / /_  / /   / /  / /     / /       \ \__/ /_  \  \___  / /_   / /_   \ \/ /
echo   \__/  \/    \/   \/      \/         \______/   \____/  \__/   \__/    \  /
echo                                                                         / /
echo  ______________________________________________________________________/ /
echo /   ____________________________________________________________________/
echo \__/
echo                                                Copyright Tony Huynh 2007-2010
echo.

:mode_sel
echo Please select mode:
echo [1] Standard (client)
echo [2] Server
echo [3] Mapper
echo [4] Animator
echo [5] Item editor
echo [6] Test Bed
echo [7] Server + clientx2 (ie, connect to self)
echo [8] Two Clients
echo [9] Command Prompt
echo.

set /p MODE=Mode Number: 
if "%MODE%"=="1" goto client
if "%MODE%"=="2" goto server
if "%MODE%"=="3" goto mapper
if "%MODE%"=="4" goto animator
if "%MODE%"=="5" goto item
if "%MODE%"=="6" goto test
if "%MODE%"=="7" goto conSelf
if "%MODE%"=="8" goto double
if "%MODE%"=="9" cmd
if "%MODE%"=="666" goto crash
echo Unknown mode, please try again
pause
goto start



:client
cls
title Infraelly Client
echo Infraelly Client

set /p S_IP=Server IP: 
set /p S_PORT=Game port: 

echo Launching Infraelly: %S_IP%: %S_PORT%
title Infraelly Client: %S_IP%: %S_PORT%
Infraelly
goto logs


:double
cls
title Infraelly Client x2
echo Infraelly Client x2

set /p S_IP=Server IP: 
set /p S_PORT=Game port: 

echo Launching Infraelly: %S_IP%: %S_PORT% x2
title Infraelly Client: %S_IP%: %S_PORT% x2
start infraelly
start infraelly
exit


:server
cls
title Infraelly Nucleus (Server)
echo Infraelly Server

set /p S_PORT=Game port: 

title Infraelly Server: %S_PORT%
echo Launching Server at port %S_PORT%
infraelly.exe -server
goto logs




:mapper
cls
title Infraelly Excogitator (Infraelly Map Editor)
echo Starting Map Editor
infraelly.exe -mapper
goto logs



:animator
cls
title Infraelly Breath (Infraelly Animator)
echo Starting Animator
infraelly.exe -animator
goto logs



:item
cls
title Infraelly Forge (Infraelly Item Maker)
echo Starting Item Editor
infraelly.exe -item
goto logs



:test
cls
title Infraelly Client (Test Area)
echo Launching Test Area
infraelly.exe -test
goto logs




:conSelf
cls
title Infraelly Client/Server
start infraelly.exe -server
start infraelly.exe
start infraelly.exe
exit




:crash
cls
title ERROR
echo ERROR
infraelly.exe -crash
goto logs




:logs
echo.
type stdout.txt
type stderr.txt
echo Press any key to exit
pause>NUL
