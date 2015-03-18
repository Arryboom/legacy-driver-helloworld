@echo off

set _SYS_="%HELLOWORLD_HOME%\driver\obj%BUILD_ALT_DIR%\%_BUILDARCH%\%TARGETNAME%.sys"

sc.exe stop %TARGETNAME%
sc.exe delete %TARGETNAME%
copy /Y %_SYS_% %SystemRoot%\System32\drivers\
sc.exe create %TARGETNAME% type= kernel binPath= %_SYS_%
sc.exe start %TARGETNAME%
