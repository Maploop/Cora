@echo off
REM Build Everything

ECHO Building everything...

PUSHD engine
CALL build.bat
POPD
IF %ERRORLEVEL% NEQ 0 (color C && echo Error:%ERRORLEVEL% && exit)

PUSHD testbed
CALL build.bat
POPD
IF %ERRORLEVEL% NEQ 0 (color C && echo Error:%ERRORLEVEL% && exit)

COLOR A
ECHO All assemblies built successfully!