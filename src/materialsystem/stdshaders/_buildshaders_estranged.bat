@echo off

call ../../../run/_baseinclude.bat

setlocal

rem == Setup path to nmake.exe, from vc common tools directory ==
if "%VS100COMNTOOLS%" == "" (
	call "%VS120COMNTOOLS%vsvars32.bat"
) else (
	call "%VS100COMNTOOLS%vsvars32.bat"
)

rem ================================
rem ==== MOD PATH CONFIGURATIONS ===

set GAMEDIR=%ESTRANGED%
set SDKBINDIR=%BASE%\bin
set SOURCEDIR=..\..

rem ==== MOD PATH CONFIGURATIONS END ===
rem ====================================

set TTEXE=..\..\devtools\bin\timeprecise.exe
if not exist %TTEXE% goto no_ttexe
goto no_ttexe_end

:no_ttexe
set TTEXE=time /t
:no_ttexe_end

%TTEXE% -cur-Q
set tt_all_start=%ERRORLEVEL%
set tt_all_chkpt=%tt_start%

set BUILD_SHADER=call buildshaders.bat
set ARG_EXTRA=

rem Only Estranged's own custom post-process shaders (see estranged_shaders_ps20.txt
rem and estranged_shaders_ps30.txt) - none of the stock SDK sample shaders.
%BUILD_SHADER% estranged_shaders_ps20 -game "%GAMEDIR%" -source "%SOURCEDIR%"
%BUILD_SHADER% estranged_shaders_ps30 -game "%GAMEDIR%" -source "%SOURCEDIR%" -dx9_30 -force30

pause
