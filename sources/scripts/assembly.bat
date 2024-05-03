@echo off

if "%1" equ "build" cd keil & call assembly build all & cd ../vs_keil & call assembly build & cd ../vs_win & call assembly build & cd .. & goto EXIT
if "%1" equ "full"  cd keil & call assembly rebuild all & cd ../vs_keil & call assembly full & cd ../vs_win & call assembly full & cd .. & goto EXIT

:HINT
echo.
echo Using assembly.bat:
echo                    assembly.bat [build^|full]
goto EXIT

:EXIT
