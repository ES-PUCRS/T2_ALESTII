@ECHO OFF

if NOT exist ./Cases/ start /wait cmd /k CALL ./Downloader.bat

gcc -Wall main.c
REM gcc -fmudflap -Wall -Wextra main.c -lmudflap

if ["%ERRORLEVEL%"]==["0"] ( a.exe )

PAUSE
REM EXIT
