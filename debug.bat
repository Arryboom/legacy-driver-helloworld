set _NT_EXECUTABLE_IMAGE_PATH="%HELLOWORLD_HOME%\driver\obj%BUILD_ALT_DIR%\%_BUILDARCH%"
set _NT_SOURCE_PATH="%HELLOWORLD_HOME%\driver"
%DDK_HOME%\Debuggers\windbg.exe -b -k com:port=com2,resets=0,reconnect