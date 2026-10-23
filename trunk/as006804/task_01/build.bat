@echo off
echo Starting build process...

:: Указываем путь к папке bin, где лежат и g++, и cmake, и make
set PATH=C:\mingw32\bin;%PATH%

if not exist build mkdir build
cd build

echo Running CMake...
cmake -G "MinGW Makefiles" ..
if %errorlevel% neq 0 (
    echo CMake failed!
    pause
    exit /b %errorlevel%
)

echo Building project...
cmake --build .
if %errorlevel% neq 0 (
    echo Build failed!
    pause
    exit /b %errorlevel%
)

echo Build successful!
cd ..
build\lab1_app.exe
pause