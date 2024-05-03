@echo off

set isMake=0
set isBuild=0

if "%1" equ "make"  set isMake=1 &                 goto MAKING
if "%1" equ "build" set isBuild=1 &                goto MAKING
if "%1" equ "full"  set isMake=1 & set isBuild=1 & goto MAKING
goto HINT

:MAKING
if %isMake%==0 goto BUILDING
echo Making Visual Studio version...
cd ../..
rmdir generated\Keil /s /q
cd scripts/vs_keil
@echo on
cmake ../../CMakeLists.txt -B../../generated/Keil -G "Visual Studio 17 2022" -A Win32 -DCMAKE_BUILD_TYPE=Debug
@echo off

:BUILDING
if %isBuild%==0 goto EXIT
@echo on
MSBuild.exe ../../generated/Keil/RES-Network.sln -clp:ErrorsOnly;WarningsOnly -nologo /m
@echo off
goto EXIT

:HINT
echo.
echo Using assembly.bat:
echo                    assembly.bat [make^|build^|full]
echo.
goto EXIT

:EXIT
