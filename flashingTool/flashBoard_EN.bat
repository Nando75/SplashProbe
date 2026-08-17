@echo off
setlocal EnableDelayedExpansion

title CubeCell Firmware Flasher

set "DIR=%~dp0"
set "FLASHER=%DIR%CubeCellflash.exe"
set "FIRMWARE=%DIR%firmware_en.cyacd"

echo.
echo ==========================================
echo          CubeCell Firmware Flasher
echo ==========================================
echo.

if not exist "%FLASHER%" (
    echo ERROR: CubeCellflash.exe not found!
    echo.
    pause
    exit /b 1
)

if not exist "%FIRMWARE%" (
    echo ERROR: firmware_en.cyacd not found!
    echo.
    pause
    exit /b 1
)

echo Searching for CubeCell...
echo.

set "PORT="

for /f "tokens=1,2 delims=;" %%A in ('powershell -NoProfile -Command "foreach ($p in Get-CimInstance Win32_SerialPort) { Write-Output ($p.DeviceID + [char]59 + $p.Caption) }"') do (
    echo Found: %%A - %%B

    echo %%B | findstr /I "CP210x" >nul
    if not errorlevel 1 (
        set "PORT=%%A"
    )
)

echo.

if not defined PORT (
    echo ==========================================
    echo ERROR: CubeCell not found!
    echo ==========================================
    echo.
    echo Please make sure that:
    echo   - the CubeCell is connected
    echo   - the USB driver is installed
    echo.
    pause
    exit /b 1
)

echo ==========================================
echo CubeCell found on port: %PORT%
echo ==========================================
echo.
echo Firmware: firmware_en.cyacd
echo.

set /p "CONFIRM=Proceed with flashing? [Y/N]: "

if /i not "%CONFIRM%"=="Y" (
    echo.
    echo Operation cancelled.
    pause
    exit /b 0
)

echo.
echo Starting flashing...
echo.

"%FLASHER%" -serial "%PORT%" "%FIRMWARE%"

if errorlevel 1 (
    echo.
    echo ==========================================
    echo             FLASH FAILED
    echo ==========================================
) else (
    echo.
    echo ==========================================
    echo          FLASH COMPLETED!
    echo ==========================================
)

echo.
pause