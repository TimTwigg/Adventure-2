@ECHO OFF

IF "%~1" == "" GOTO MAIN
IF "%~1" == "test" GOTO TEST

:MAIN
build\bin\Adventure.exe
goto END

:TEST
build\bin\Tests.exe
goto END

:END
ENDLOCAL