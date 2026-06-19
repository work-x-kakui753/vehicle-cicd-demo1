@echo off
setlocal

set "SCRIPT_DIR=%~dp0"
set "REPO_ROOT=%SCRIPT_DIR%.."
pushd "%REPO_ROOT%" >nul

set "BUILD_DIR=build"
set "LOG_DIR=%BUILD_DIR%\logs"
set "REPORT_DIR=%BUILD_DIR%\reports"
set "BUILD_LOG=%LOG_DIR%\build.log"
set "TEST_LOG=%LOG_DIR%\test.log"
set "DEMO_LOG=%LOG_DIR%\run_demo.log"
set "SUMMARY_FILE=%REPORT_DIR%\ci_summary.md"

if "%~1"=="" goto run_all
if /I "%~1"=="prepare" goto prepare
if /I "%~1"=="build" goto build
if /I "%~1"=="test" goto test
if /I "%~1"=="demo" goto demo
if /I "%~1"=="summary" goto summary

echo [ERROR] Unknown command: %~1
popd >nul
exit /b 1

:run_all
call "%~f0" prepare || goto fail
call "%~f0" build || goto fail
call "%~f0" test || goto fail
call "%~f0" demo || goto fail
call "%~f0" summary || goto fail
popd >nul
exit /b 0

:prepare
echo [INFO] Preparing build directories
if not exist "%BUILD_DIR%" mkdir "%BUILD_DIR%"
if not exist "%LOG_DIR%" mkdir "%LOG_DIR%"
if not exist "%REPORT_DIR%" mkdir "%REPORT_DIR%"
del /q "%BUILD_LOG%" "%TEST_LOG%" "%DEMO_LOG%" "%SUMMARY_FILE%" 2>nul
echo [INFO] Build directory: %BUILD_DIR%
echo [INFO] Log directory: %LOG_DIR%
echo [INFO] Report directory: %REPORT_DIR%
popd >nul
exit /b 0

:build
if not exist "%LOG_DIR%" call "%~f0" prepare || goto fail
where cl >nul 2>&1
if errorlevel 1 (
    echo [ERROR] cl.exe was not found. Run this script from a Visual Studio Developer Command Prompt.
    >> "%BUILD_LOG%" echo [ERROR] cl.exe was not found. Run this script from a Visual Studio Developer Command Prompt.
    goto fail
)

echo [INFO] Building vehicle_demo.exe and vehicle_tests.exe
> "%BUILD_LOG%" echo [INFO] Starting MSVC build
>> "%BUILD_LOG%" echo [INFO] Compiling demo executable
cl /nologo /W4 /TC /Fe:%BUILD_DIR%\vehicle_demo.exe src\main.c src\vehicle_control.c >> "%BUILD_LOG%" 2>&1
if errorlevel 1 (
    type "%BUILD_LOG%"
    goto fail
)
>> "%BUILD_LOG%" echo [INFO] Compiling unit test executable
cl /nologo /W4 /TC /Fe:%BUILD_DIR%\vehicle_tests.exe tests\test_vehicle_control.c src\vehicle_control.c >> "%BUILD_LOG%" 2>&1
if errorlevel 1 (
    type "%BUILD_LOG%"
    goto fail
)
>> "%BUILD_LOG%" echo [INFO] Build completed successfully
type "%BUILD_LOG%"
popd >nul
exit /b 0

:test
if not exist "%BUILD_DIR%\vehicle_tests.exe" (
    echo [ERROR] Missing build\vehicle_tests.exe. Run build phase first.
    goto fail
)
echo [INFO] Running vehicle scenario tests
"%BUILD_DIR%\vehicle_tests.exe" > "%TEST_LOG%" 2>&1
set "TEST_EXIT=%ERRORLEVEL%"
type "%TEST_LOG%"
if not "%TEST_EXIT%"=="0" (
    set "EXIT_CODE=%TEST_EXIT%"
    goto fail
)
popd >nul
exit /b 0

:demo
if not exist "%BUILD_DIR%\vehicle_demo.exe" (
    echo [ERROR] Missing build\vehicle_demo.exe. Run build phase first.
    goto fail
)
echo [INFO] Running vehicle demo executable
"%BUILD_DIR%\vehicle_demo.exe" > "%DEMO_LOG%" 2>&1
set "DEMO_EXIT=%ERRORLEVEL%"
type "%DEMO_LOG%"
if not "%DEMO_EXIT%"=="0" (
    set "EXIT_CODE=%DEMO_EXIT%"
    goto fail
)
popd >nul
exit /b 0

:summary
if not exist "%LOG_DIR%" call "%~f0" prepare || goto fail
(
    echo # CI Summary
    echo.
    echo - Build platform: Windows with MSVC
    echo - Build script: `scripts\ci_windows.bat`
    echo - Demo executable: `build\vehicle_demo.exe`
    echo - Test executable: `build\vehicle_tests.exe`
    echo.
    echo ## Generated artifacts
    echo.
    echo - `build\logs\build.log`
    echo - `build\logs\test.log`
    echo - `build\logs\run_demo.log`
    echo - `build\reports\ci_summary.md`
    echo.
    echo ## Test scope
    echo.
    echo - Temperature status decision
    echo - Headlamp ON/OFF decision
    echo - CAN frame validation
    echo.
    echo ## Result
    echo.
    if exist "%BUILD_DIR%\vehicle_demo.exe" (echo - Demo build: SUCCESS) else (echo - Demo build: NOT FOUND)
    if exist "%BUILD_DIR%\vehicle_tests.exe" (echo - Test build: SUCCESS) else (echo - Test build: NOT FOUND)
    if exist "%TEST_LOG%" (echo - Test log: GENERATED) else (echo - Test log: NOT FOUND)
    if exist "%DEMO_LOG%" (echo - Demo log: GENERATED) else (echo - Demo log: NOT FOUND)
) > "%SUMMARY_FILE%"
popd >nul
exit /b 0

:fail
if not defined EXIT_CODE set "EXIT_CODE=%ERRORLEVEL%"
if "%EXIT_CODE%"=="0" set "EXIT_CODE=1"
popd >nul
exit /b %EXIT_CODE%
