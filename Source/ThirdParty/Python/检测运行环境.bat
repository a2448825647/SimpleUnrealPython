:: Check for Python Installation
echo --------------------------
py -3 --version
if %errorlevel%==0 (goto successPython) else (goto errorNoPython)
pause
:: Reaching here means Python is installed.
:: Execute stuff...

:: Once done, exit the batch file -- skips executing the errorNoPython section
goto:eof

:errorNoPython
echo --------------------------
echo Python not installed
echo install Python
set curdir=%~dp0
%curdir%/BiliBiliTest/Plugins/SimpleUnrealPython/Source/ThirdParty/Python/python-3.9.6-amd64.exe /quiet InstallAllUsers=1 PrependPath=1
pause

:successPython
echo --------------------------
echo HasPython
pause
