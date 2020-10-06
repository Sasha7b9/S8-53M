@echo off

if "%1" EQU "" goto HINT
if %1==debug   ( call :CLEAN_DEBUG & goto EXIT )
if %1==release ( call :CLEAN_RELEASE & goto EXIT )
if %1==all     ( call clean.bat debug & call clean.bat release & goto EXIT )

goto HINT

:CLEAN_DEBUG
    echo Clean Debug Keil
    %COMPILER% -c..\..\Device\Device-Debug.uvprojx -j0
    %COMPILER% -c..\..\DLoader\DLoader-Debug.uvprojx -j0
    %COMPILER% -c..\..\Panel\Panel-Debug.uvprojx -j0

    exit /b

:CLEAN_RELEASE
    echo Clean Release Keil
    %COMPILER% -c..\..\Device\Device.uvprojx -j0
    %COMPILER% -c..\..\DLoader\DLoader.uvprojx -j0
    %COMPILER% -c..\..\Panel\Panel.uvprojx -j0

    exit /b

:HINT

    echo.
    echo Usage:
    echo       clean [debug^|release^|all]
    echo.
    goto EXIT

:EXIT
