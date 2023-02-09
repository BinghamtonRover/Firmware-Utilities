@echo off

rem Run this script on Windows to sync the libraries in this repo to your Arduino sketchbook. 

rem This allows variables to be evaluated after declared.
rem Why this is not the default baffles me.
setlocal ENABLEDELAYEDEXPANSION

rem Parse user's sketchbook as the first argument
if [%1] == [] (
	set sketchbook=%userprofile%\Documents\Arduino
	echo Sketchbook not provided. Using Windows default of !sketchbook!
	echo To override this, pass your sketchbook's libraries folder as the first argument.
) else (
	echo Using user-provided sketchbook location: %1
	set sketchbook=%1
)

rem Verify sketchbook directory exists
if NOT exist !sketchbook! (
	echo "Error: Cannot find sketchbook directory: !sketchbook!"
	pause`
	exit /b 1
)

rem Sync all libraries to the sketchbook
echo.
if exist !sketchbook!\libraries\BURT_utils (
	echo Folder already exists
) else (
	mklink /j !sketchbook!\libraries\BURT_utils .
	if ERRORLEVEL 1 (echo Operation failed) else (echo Done)
) 

pause 
