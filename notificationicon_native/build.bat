@echo off

:: Setup paths to project paths
set "BUILD_ROOT=%~dp0\_build\windows"
set "BUILD_X64=%BUILD_ROOT%\x64"
if EXIST "%BUILD_X64%" rmdir /s /q "%BUILD_X64%"
md "%BUILD_X64%"

:: Setup paths to project outputs
set "BIN_ROOT=%~dp0\bin\windows"
set "BIN_X64=%BIN_ROOT%\x64"
if EXIST "%BIN_X64%" rmdir /s /q "%BIN_X64%"
md "%BIN_X64%"

:: Generate x64 build files
pushd "%BUILD_X64%"
cmake ..\..\..\ ^
-G "Visual Studio 16 2019" ^
-Tv110 ^
-DBUILD_ROOT="%BUILD_X64%" ^
-DBIN_ROOT="%BIN_X64%"
popd

:: Run the build
if not defined DevEnvDir (
    call "C:\Program Files (x86)\Microsoft Visual Studio\2017\Professional\VC\Auxiliary\Build\vcvarsall.bat" x64
)

:: x64 build
pushd "%BUILD_X64%"
MSBuild -m /p:Configuration=Debug ALL_BUILD.vcxproj
if %errorlevel% neq 0 goto :eof
MSBuild -m /p:Configuration=Release ALL_BUILD.vcxproj
if %errorlevel% neq 0 goto :eof
popd

:: Copy artifacts
set "PREBUILT_ROOT=%~dp0\..\..\prebuilt\win64"
copy /Y "%BIN_X64%\Debug\native_tts.dll" "%PREBUILT_ROOT%\Debug\"
copy /Y "%BIN_X64%\Debug\native_tts.pdb" "%PREBUILT_ROOT%\Debug\"
copy /Y "%BIN_X64%\Release\native_tts.dll" "%PREBUILT_ROOT%\Release\"
goto :eof