@echo off

call ..\..\..\run\_baseinclude

set CAPTIONCOMPILER=..\..\bin\captioncompiler.exe
set ESTRANGED=..\.

for %%f in (closecaption_*.txt) do (
	echo Compiling %%f
	"%CAPTIONCOMPILER%" -game "%ESTRANGED%" %%f
)

pause