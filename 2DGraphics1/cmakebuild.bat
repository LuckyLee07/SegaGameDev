@echo off
setlocal

set BUILD_DIR=build
set CONFIG=Debug
set PLATFORM=Win32
set GENERATOR="Visual Studio 16 2019"

if not exist %BUILD_DIR% (
	echo Creating build directory...
	mkdir %BUILD_DIR%
)

cd %BUILD_DIR%

echo Configuring the project with CMake...
cmake -G %GENERATOR% -A %PLATFORM% ..
cd ..

echo build completed
endlocal

rem pause