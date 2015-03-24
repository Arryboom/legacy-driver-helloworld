@echo off

REM Password used for PFX file
set PFX_PWD=TODO

REM Set up a 64-bit Windows Server 2003 checked build environment
set BUILD_ENV=chk WNET x64

REM Root directory of the Windows 7 DDK
set DDK_HOME=C:\WINDDK\7600.16385.1

REM Name used for binaries and service
set TARGETNAME=HelloWorld

set HELLOWORLD_HOME=%CD%

REM Set the compiler warning level to strict
set MSC_WARNING_LEVEL=/W3 /WX

REM Configure DDK build environment
REM setenv.bat Directory [fre|chk] [x86|64|x64] [hal] [WIN7|WLH|WXP|WNET] [no_prefast] [bscmake] [separate_object_root][oacr|no_oacr]
pushd %HELLOWORLD_HOME%
call %DDK_HOME%\bin\setenv.bat %DDK_HOME% %BUILD_ENV%
popd
