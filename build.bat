@echo off

REM c: Delete all object files. 
REM e: Produce log files. 
REM f: Rescan source and header files. 
REM Z: Inhibit dependency checking. 
build.exe -cefz

if %errorlevel% neq 0 goto :end

set _SYS_="%HELLOWORLD_HOME%\driver\obj%BUILD_ALT_DIR%\%_BUILDARCH%\%TARGETNAME%.sys"
set _PFX_="%HELLOWORLD_HOME%\certs\certificate.pfx"
set _CROSSCERT_="%HELLOWORLD_HOME%\certs\cross-certificate.crt"

"%DDK_HOME%\bin\x86\signtool.exe" sign /f %_PFX_% /p %PFX_PWD% /ac %_CROSSCERT_% /t http://timestamp.verisign.com/scripts/timestamp.dll %_SYS_% 
"%DDK_HOME%\bin\x86\signtool.exe" verify /v /kp %_SYS_%

:end