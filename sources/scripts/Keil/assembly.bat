@echo off

set _COMPILER_=c:\Keil_v5\UV4\UV4
set _PROJECT_DEVICE_=..\..\Network.uvmpw

rem 1, если требуется очистка
set isClean=0

rem 1, если требуется сборка
set isBuild=0

rem 1, если требуется загрузка
set isLoad=0

if "%1" equ "clear"   set isClean=1 &                  goto CLEANING
if "%1" equ "build"   set isBuild=1 &                  goto CHECK_ON_LOAD
if "%1" equ "rebuild" set isClean=1 & set isBuild=1 &  goto CHECK_ON_LOAD
if "%1" equ "load"    set isLoad=1 &                   goto LOADING
goto HINT

rem Проверка на то, нужно ли загружать (второй параметр - "load")
:CHECK_ON_LOAD
if "%2" equ "load"   set isLoad=1 &                    goto CLEANING

:CLEANING
if %isClean%==0 goto BUILDING
echo Cleaning Device ...
%_COMPILER_% -c %_PROJECT_DEVICE_% -j0

:BUILDING
if %isBuild%==0 goto LOADING
echo Building Device ...
%_COMPILER_% -b %_PROJECT_DEVICE_% -j0 -o Device.out
if %ERRORLEVEL%==0 goto LOADING
echo ERROR!!! Build failed !!!
type ..\..\Device.out

:LOADING
if %isLoad%==0 goto EXIT
echo Loading Device ...
%_COMPILER_% -f %_PROJECT_DEVICE_% -j0 -o Device.out
if %ERRORLEVEL%==0 goto EXIT
echo Error loading firmware to Device
type ..\..\GTC\Device.out

goto EXIT

:HINT
echo.
echo Using assembly.bat:
echo                    assembly.bat [clear^|build^|rebuild] [load]
goto EXIT

:EXIT
