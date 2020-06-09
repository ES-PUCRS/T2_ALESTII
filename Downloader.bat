@ECHO off


	SET "CasesPath=./Cases/"

		MKDIR "%CasesPath%"

    :: Dependencies.txt download files
    :: %%f -> Extension
    :: %%g -> Download to path ()
    :: %%h -> Output name
    :: %%i -> URL
    ::for /f "tokens=1,2,3,4 skip=1" %%f in (%assetsPath%Dependencies.txt) do (
	SET "Extension=.tar"
	SET "DownloadToPath=%CasesPath%"
	SET "OutputName=CasesTest"
	SET "URL=https://inf.pucrs.br/oliveira/alestII/casos.tar"

	    @ECHO Downloading %OutputName%%Extension% on %DownloadToPath% of %URL%
        cd "%DownloadToPath%"
        curl %URL% --output %OutputName%%Extension%

        if "%Extension%" EQU ".tar" (tar -xvf %OutputName%%Extension%)
        if "%Extension%" EQU ".rar" (unrar e %OutputName%%Extension%)
        REM if "%%f" EQU ".zip" (wzunzip %OutputName%%Extension%)
        if NOT ["%ERRORLEVEL%"]==["0"] (
            PAUSE
            EXIT
        )
        del %OutputName%%Extension%

    CALL :extractCases
EXIT


:extractCases
    cd %testCasesPath%
    @ECHO Extracting cases
    for /f "tokens=*" %%j in ('dir /b "%testCasesPath%"') do (
        gzip -d %%j
    )
    GOTO :EOF
