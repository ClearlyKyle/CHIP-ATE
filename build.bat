@echo off

REM ----------------------------------------------------------------------------------------------------------
if "%~1"=="" (
    REM dont forget the stupid carrot
    echo Usage: batch.bat ^<file_to_compile^> 
    exit /b 0
)

REM Get the base name of the C file without the extension
set OUTPUT_DIRECTORY=bin
set OUTPUT_FILE=%OUTPUT_DIRECTORY%\%~n1
set CFLAGS=/W4 /guard:cf /std:c11 /Zi
set DEPS_INCLUDE_FOLDER=/Ideps\include
set DEPS_LIB_FOLDER=/LIBPATH:deps\libs
set DEPS_LIBS=Shell32.lib SDL2main.lib SDL2.lib
set LIBS=%DEPS_INCLUDE_FOLDER% /link /SUBSYSTEM:CONSOLE %DEPS_LIB_FOLDER% %DEPS_LIBS%

REM Ensure the bin directory exists
if not exist %OUTPUT_DIRECTORY% (
    mkdir %OUTPUT_DIRECTORY%
)

if "%~1"=="clean" (
    if exist "%OUTPUT_DIRECTORY%" (
        echo Cleaning up bin directory...
        REM Delete all files except .dll files
        for %%I in ("%OUTPUT_DIRECTORY%\*") do (
            if /I not "%%~xI"==".dll" (
                del /Q "%%I"
            )
        )
    )
    echo Sqeaky clean now :D
    exit /b 0
)

echo [MSVC] Starting to build..

REM /Fe	    Renames the executable file.
REM /Fo     Creates an object file.
REM /Fd	    Renames program database file.
cl /nologo %CFLAGS% /Fo%OUTPUT_DIRECTORY%\ /Fd%OUTPUT_FILE% /Fe%OUTPUT_FILE% "%~1" %LIBS%
REM ----------------------------------------------------------------------------------------------------------

REM Check if compilation was successful
if %ERRORLEVEL% NEQ 0 (
    echo Compilation failed with error code %ERRORLEVEL%.
    exit /b %ERRORLEVEL%
)

echo Compilation successful. The executable is located in "%OUTPUT_FILE%.exe"