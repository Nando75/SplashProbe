@echo off
setlocal EnableDelayedExpansion

title CubeCell Firmware Flasher

set "DIR=%~dp0"
set "FLASHER=%DIR%CubeCellflash.exe"
set "FIRMWARE=%DIR%firmware_it.cyacd"

echo.
echo ==========================================
echo          CubeCell Firmware Flasher
echo ==========================================
echo.

if not exist "%FLASHER%" (
    echo ERRORE: manca CubeCellflash.exe
    echo.
    pause
    exit /b 1
)

if not exist "%FIRMWARE%" (
    echo ERRORE: manca firmware_it.cyacd
    echo.
    pause
    exit /b 1
)

echo Ricerca CubeCell...
echo.

set "PORT="

for /f "tokens=1,2 delims=;" %%A in ('powershell -NoProfile -Command "foreach ($p in Get-CimInstance Win32_SerialPort) { Write-Output ($p.DeviceID + [char]59 + $p.Caption) }"') do (
    echo Trovata: %%A - %%B

    echo %%B | findstr /I "CP210x" >nul
    if not errorlevel 1 (
        set "PORT=%%A"
    )
)

echo.

if not defined PORT (
    echo ==========================================
    echo ERRORE: CubeCell non trovata!
    echo ==========================================
    echo.
    echo Assicurati che:
    echo   - la CubeCell sia collegata
    echo   - il driver USB sia installato
    echo.
    pause
    exit /b 1
)

echo ==========================================
echo CubeCell trovata sulla porta: %PORT%
echo ==========================================
echo.
echo Firmware: firmware_it.cyacd
echo.

set /p "CONFIRM=Procedere con il flashing? [S/N]: "

if /i not "%CONFIRM%"=="S" (
    echo.
    echo Operazione annullata.
    pause
    exit /b 0
)

echo.
echo Avvio flashing...
echo.

"%FLASHER%" -serial "%PORT%" "%FIRMWARE%"

if errorlevel 1 (
    echo.
    echo ==========================================
    echo             FLASH FALLITO
    echo ==========================================
) else (
    echo.
    echo ==========================================
    echo          FLASH COMPLETATO!
    echo ==========================================
)

echo.
pause