:: Usage		rarMeWithPassword.bat		C:\Destination		D:\Source
:: Usage		rarMeWithPassword.bat		D:\Source
:: Usage		rarMeWithPassword.bat		

:: ------------------------BEGIN OF SCRIPT------------------------
@echo off
setlocal enabledelayedexpansion
chcp 65001 >nul

cls
set thisScriptFileDir=%~dp0
set thisScriptFileName=%~nx0
set thisScriptFilePath=%~df0
set initialPath=%~dp0
set currentPath=%initialPath%
set previousPath=%initialPath%
:: Geçerli dizini al
set currentDir=%cd%
set hataKodu=0
set ERRORLEVEL=0

set argCount=0
for %%x in (%*) do (
	set /a argCount+=1
	set argvec[!argCount!]=%%~x
)

set printArguments=1
if %printArguments% == 1 (
    echo.
	echo =============================================================================================
	echo Girilen Parametreler : [%thisScriptFileName%] 
	echo ArgCount : %argCount%
	for /L %%i in (1,1,%argCount%) do (
		echo        %%i : "!argVec[%%i]!"	
		REM set item=!argVec[%%i]
		REM if exist %item%\NUL (
			REM echo is directory
		REM ) else (
			REM echo is file
		REM )	
	)
	echo =============================================================================================
	echo.
)

:: ------------------------BEGIN OF SCRIPT------------------------
:: Sıkıştırıcıların yolları
set "RAR_PATH=C:\Program Files\WinRAR\Rar.exe"
set "UNRAR_PATH=C:\Program Files\WinRAR\UnRAR.exe"
set "WINRAR_PATH=C:\Program Files\WinRAR\WinRAR.exe"
set "SEVENZIP_PATH=C:\Program Files\7-Zip\7z.exe"
set "WINZIP_PATH=C:\Program Files\WinZip\wzzip.exe"

:: Sıkıştırıcılar mevcut mu kontrol et
if not exist "%RAR_PATH%" (
    echo Hata: RAR bulunamadı! Lütfen yolu kontrol edin.
    exit /b 1
)
if not exist "%UNRAR_PATH%" (
    echo Hata: UNRAR bulunamadı! Lütfen yolu kontrol edin.
    exit /b 1
)
if not exist "%WINRAR_PATH%" (
    echo Hata: WINRAR bulunamadı! Lütfen yolu kontrol edin.
    exit /b 1
)
REM if not exist "%SEVENZIP_PATH%" (
    REM echo Hata: 7ZIP bulunamadı! Lütfen yolu kontrol edin.
    REM exit /b 1
REM )
REM if not exist "%WINZIP_PATH%" (
    REM echo Hata: WINZIP bulunamadı! Lütfen yolu kontrol edin.
    REM exit /b 1
REM )

:: Degiskenleri tanimla
REM set srcPath=.
REM set dstPath=.
set srcPath=%cd%\
set dstPath=%cd%\
set password=1234
set fileName=backup
set comment=
set commentFile=message.tmp
set rarFileName=%fileName%.rar
set zipFileName=%fileName%.zip
set sevenZipFileName=%fileName%.7z
set rarFilePath=%dstPath%%rarFileName%
set zipFilePath=%dstPath%%zipFileName%
set sevenZipFilePath=%dstPath%%sevenZipFileName%

:: Degiskenleri editle
if %ArgCount% == 1 set srcPath=%argVec[1]%\
if %ArgCount% == 2 (
	set dstPath=%argVec[1]%\
	set srcPath=%argVec[2]%\
)

set rarFilePath=%dstPath%%rarFileName%
set zipFilePath=%dstPath%%zipFileName%
set sevenZipFilePath=%dstPath%%sevenZipFileName%

REM echo srcPath : %srcPath% 

if exist %rarFilePath% (
    del %rarFilePath% 
		echo %rarFilePath% silindi...
			echo.	
)

if exist %zipFilePath% (
    del %zipFilePath% 
		echo %zipFilePath% silindi...
			echo.	
)

call :extractDateTimeSageConfig
	call :createComment
		echo comment olusturuldu... %comment%
			echo.

set srcPath="%srcPath%"

echo %comment% >> %commentFile%

"%WINRAR_PATH%" a -p%password% -r -ep1 %rarFilePath% 	%srcPath%  -z%commentFile% -x%commentFile% -x%thisScriptFilePath%
if %errorlevel% neq 0 echo RAR sıkıştırma işlemi başarısız oldu! 
if %errorlevel% == 0 echo %rarFilePath%  olusturuldu...

"%WINRAR_PATH%" a -p%password% -r -ep1 %zipFilePath% 	%srcPath%  -z%commentFile% -x%commentFile% -x%thisScriptFilePath% -x%rarFilePath%
if %errorlevel% neq 0 echo ZIP sıkıştırma işlemi başarısız oldu! 		
if %errorlevel% == 0 echo %zipFilePath%  olusturuldu...

del %commentFile%

echo.

		

call :printSuccesMsg
echo.
goto end
:: ---------------------------------------------------------------
:: ---------------------------------------------------------------
:: ---------------------------------------------------------------
:: ---------------------------------------------------------------
:: ------------------------END OF SCRIPT--------------------------
:end
exit /b 0
:: ------------------------END OF SCRIPT--------------------------
:: ---------------------------------------------------------------
:: ---------------------------------------------------------------
:: ---------------------------------------------------------------
:: ---------------------------------------------------------------




















REM :: ---------------------------------------------------------------
REM echo [1]....

REM ::Extract Date Time Information
REM call :extractDateTimeSageConfig
REM echo        Computer is : %ComputerName%
REM echo        DateTime is : %dateTimeString2%
REM echo.

REM echo [2]....

REM if exist %rarFileName% (
    REM del %rarFileName% 
	REM echo        %rarFileName% silindi...
	REM echo.	
REM )

REM echo [3]....

REM call :createComment
REM if not %ERRORLEVEL% == 0 set hataKodu=1 & goto :HandleErrors
REM echo        comment olusturuldu... %comment%
REM echo.

REM echo [4]....

REM :: Sıkıştır
	REM echo        %rarFileName% olusturuluyor...
	REM echo.
	
	REM ::echo DateTime : %dateTimeString2% > %commentFile%
	REM echo %comment% >> %commentFile%
	REM "%WINRAR_PATH%" a -p%password% -r -ep1 %dstPath%\%rarFileName% 	%srcPath%  -z%commentFile% -x%commentFile% -x%thisScriptFileName%
	REM del %commentFile% 
	
	REM echo        %rarFileName% olusturuldu...
	REM echo.

REM echo [5]....

REM :: Print Result
REM echo.
REM echo.
REM call :printSuccesMsg
REM echo.

REM goto end
REM :: ---------------------------------------------------------------










:: ---------------------------------------------------------------
:: ---------------------------------------------------------------
:: ---------------------------------------------------------------
:: ---------------------------------------------------------------
:: ------------------------FUNCTIONS------------------------------
:: ---------------------------------------------------------------
:: ---------------------------------------------------------------
:: ---------------------------------------------------------------
:: ---------------------------------------------------------------
:createComment
	set comment=Password : %password%
	:: Antivirus should be turned off while using KeyPatch
	set ERRORLEVEL=0
	exit /b %ERRORLEVEL%

:printSuccesMsg
echo =============================================================================================
echo İşlem tamamlandı, NoError !
echo =============================================================================================
echo.
set ERRORLEVEL=0
exit /b %ERRORLEVEL%

:printFailMsg
echo =============================================================================================
echo İşlem tamamlandı, WithError !
echo =============================================================================================
echo.
set ERRORLEVEL=0
exit /b %ERRORLEVEL%

:HandleErrors
echo        Hata olustu : %hataKodu%
echo.
set ERRORLEVEL=0
exit /b %ERRORLEVEL%

:extractDateTimeSageConfig
	set ERRORLEVEL=0
	set _time_=%time:~0,8%

	set CUR_YYYY=%date:~10,4%
	set CUR_MM=%date:~7,2%
	set CUR_DD=%date:~4,2%
	set CUR_HH=%time:~0,2%
	set CUR_NN=%time:~3,2%
	set CUR_SS=%time:~6,2%
	set CUR_MS=%time:~9,2%
	if %CUR_HH% lss 10 (set CUR_HH=0%time:~1,1%)
	set dateString=%CUR_YYYY%_%CUR_MM%_%CUR_DD%
	set timeString=%CUR_HH%_%CUR_NN%_%CUR_SS%
	set dateTimeString=%dateString%_%timeString%
	set dateString2=%CUR_YYYY%.%CUR_MM%.%CUR_DD%
	set timeString2=%CUR_HH%:%CUR_NN%:%CUR_SS%
	set dateTimeString2=%dateString2% %timeString2%
	exit /b %ERRORLEVEL%

:extractDateTimeHomeConfig
	set ERRORLEVEL=0
	set _time_=%time:~0,8%

	set CUR_YYYY=%date:~10,4%
	set CUR_MM=%date:~7,2%
	set CUR_DD=%date:~4,2%
	set CUR_HH=%time:~0,2%
	set CUR_NN=%time:~3,2%
	set CUR_SS=%time:~6,2%
	set CUR_MS=%time:~9,2%
	if %CUR_HH% lss 10 (set CUR_HH=0%time:~1,1%)
	set dateString=%CUR_YYYY%_%CUR_MM%_%CUR_DD%
	set timeString=%CUR_HH%_%CUR_NN%_%CUR_SS%
	set dateTimeString=%dateString%_%timeString%
	set dateString2=%CUR_YYYY%.%CUR_MM%.%CUR_DD%
	set timeString2=%CUR_HH%:%CUR_NN%:%CUR_SS%
	set dateTimeString2=%dateString2% %timeString2%
	exit /b %ERRORLEVEL%


:checkArguments
exit /b 0

:hasNoArguments
exit /b 0

:hasArguments
exit /b 0




::UnRar
if EXIST %dstPath2% (
    RMDIR /S /Q  %dstPath2%
)
mkdir %dstPath2%

"%WINRAR_PATH%" x -p%rarPassword% %rarFileName%   *.* %dstPath2%



:: Kullanıcıdan şifre alma
set /p password="Lütfen bir şifre girin: "