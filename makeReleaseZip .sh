@echo off
echo Making a Release zip file

rem Check 7zip
:check_7zip
set path=C:\program files\7-Zip;C:\Program Files (x86)\7-Zip;%path%
7z > nul
IF %errorlevel%==0 goto has_7zip
echo 7zip not found
echo Please downlaod it from http://www.7-zip.org/
echo.
goto exit



:has_7zip
set fn=Infraelly

IF NOT EXIST version.h goto make_zip

rem get revision  number from version.h
findstr "REVISION" version.h > inf_r.tmp
FOR /F "usebackq tokens=6" %%i IN (inf_r.tmp) DO set inf_r=%%i
echo %inf_r% > inf_r.tmp
FOR /F "usebackq delims=; tokens=1" %%i IN (inf_r.tmp) DO set inf_r=%%i
del /F /Q inf_r.tmp

set fn=%fn%_r%inf_r%
goto make_zip

:make_zip
7z u -aoa -t7z -y %fn% InfraellyLancher.bat docs Infraelly.exe *.isk *.dll *.ipf -mx9 -ms=on -mf=on -mmt=on -mtc=on
copy /D /V /Y %fn%.7z ..\
del /F /Q %fn%.7z
echo.
echo ..\%fn%.7z created
echo.
goto exit

:exit