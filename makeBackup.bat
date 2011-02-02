@echo off
Title Making a Backup zip

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
FOR /F "usebackq tokens=2" %%i IN (`date /t`) DO set cdate=%%i
FOR /F "usebackq tokens=1 delims=/" %%A IN (`echo %cdate%`) DO SET dd=%%A
FOR /F "usebackq tokens=2 delims=/" %%A IN (`echo %cdate%`) DO SET mm=%%A
FOR /F "usebackq tokens=3 delims=/" %%A IN (`echo %cdate%`) DO SET yyyy=%%A
set fn=%yyyy%_%mm%_%dd%

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
rem cleqan temp files
echo Removing temp files
del /F /Q stderr.txt
del /F /Q stdout.txt
del /F /Q *.o
del /F /Q *.tmp
del /F /Q gmon.out
del /F /Q *.h*.save
del /F /Q *.c*.save
del /F /Q LibBuild\guichanbuild\src\*.o
del /F /Q LibBuild\guichanbuild\src\allegro\*.o
del /F /Q LibBuild\guichanbuild\src\contrib\allegro\*.o
del /F /Q LibBuild\guichanbuild\src\contrib\opengl\*.o
del /F /Q LibBuild\guichanbuild\src\contrib\sdl\*.o
del /F /Q LibBuild\guichanbuild\src\contrib\widgets\*.o
del /F /Q LibBuild\guichanbuild\src\contrib\*.o
del /F /Q LibBuild\guichanbuild\src\hge\*.o
del /F /Q LibBuild\guichanbuild\src\opengl\*.o
del /F /Q LibBuild\guichanbuild\src\openlayer\*.o
del /F /Q LibBuild\guichanbuild\src\sdl\*.o
del /F /Q LibBuild\guichanbuild\src\widgets\*.o

rem clean out dependency libraries' temp files
rmdir /S /Q .objs
rmdir /S /Q LibBuild\cryptoppbuild\Win32
rmdir /S /Q LibBuild\cryptoppbuild\Nix
rmdir /S /Q LibBuild\guichanbuild\.objs
rmdir /S /Q LibBuild\net2build\obj
rmdir /S /Q LibBuild\sdl_gfxbuild\.objs
rmdir /S /Q LibBuild\sdl_gfxbuild\Test\obj
rmdir /S /Q LibBuild\tinyxmlbuild\obj
rmdir /S /Q LibBuild\zlibbuild\contrib\vstudio\vc9\x86\ZlibDllReleaseWithoutAsm Win32\Tmp


7z u -aoa -t7z -y %fn% * -mx9 -ms=on -mf=on -mmt=on -mtc=on -o..\Backups -x!.git -x!*.ipf

IF NOT EXIST ..\Inf_Backups mkdir ..\Inf_Backups
copy /D /V /Y %fn%.7z ..\Inf_Backups\%fn%.7z
del /F /Q %fn%.7z
echo.
echo ..\Inf_Backups\%fn%.7z created
echo.

goto exit

:exit
pause
