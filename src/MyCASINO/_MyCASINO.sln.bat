@echo off
setlocal EnableDelayedExpansion
setlocal EnableExtensions

echo Starting Visual Studio 2015 with some more environmental variables (run as admin)

REM set solution name and remove leading underline
set "solutionName=%~n0"
set "solutionName=%solutionName:~1%

set "USERDATA=%~dp0_data\"

echo Set environmental variables:
echo USERDATA %USERDATA%

set "MYCASINO_USERDATA=%USERPROFILE%\_MyCasinoData\"
echo MYCASINO_USERDATA %MYCASINO_USERDATA%

if not exist %MYCASINO_USERDATA% (
	mkdir %MYCASINO_USERDATA%
)

echo Copy data to %MYCASINO_USERDATA%
copy "%USERDATA%" %MYCASINO_USERDATA%

REM add RPC registry file
echo regedit.exe /S SP2RpcNoRestriction.reg
regedit.exe /S SP2RpcNoRestriction.reg


echo Start solution:
echo "%VS140COMNTOOLS%\..\IDE\devenv.exe" "%~dp0%solutionName%"
"%VS140COMNTOOLS%\..\IDE\devenv.exe" "%~dp0%solutionName%"