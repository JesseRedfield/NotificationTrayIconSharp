@echo off

:: Setup DevEnvDir
if not defined DevEnvDir (
    call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Professional\VC\Auxiliary\Build\vcvarsall.bat" x64
)

:: Setup paths to project paths X64
set "BUILD_ROOT=%~dp0\_build\win"
set "BUILD_X64=%BUILD_ROOT%\x64"
if EXIST "%BUILD_X64%" rmdir /s /q "%BUILD_X64%"
md "%BUILD_X64%"

:: Setup paths to project outputs X64
set "BIN_ROOT=%~dp0\bin\win"
set "BIN_X64=%BIN_ROOT%\x64"
if EXIST "%BIN_X64%" rmdir /s /q "%BIN_X64%"
md "%BIN_X64%"

:: Setup paths to project paths
set "BUILD_ROOT=%~dp0\_build\win"
set "BUILD_X86=%BUILD_ROOT%\x86"
if EXIST "%BUILD_X86%" rmdir /s /q "%BUILD_X86%"
md "%BUILD_X86%"

:: Setup paths to project outputs X86
set "BIN_ROOT=%~dp0\bin\win"
set "BIN_X86=%BIN_ROOT%\x86"
if EXIST "%BIN_X86%" rmdir /s /q "%BIN_X86%"
md "%BIN_X86%"

:: Setup paths to project outputs X86
set "BIN_PREBUILD=%~dp0\..\prebuilt\win"
if EXIST "%BIN_PREBUILD%" rmdir /s /q "%BIN_PREBUILD%"
md "%BIN_PREBUILD%"


:: Generate x86 build files
pushd "%BUILD_X86%"
cmake ..\..\..\ ^
-G "Visual Studio 16 2019" ^
-A Win32 ^
-DBUILD_ROOT="%BUILD_X86%" ^
-DBIN_ROOT="%BIN_X86%"
popd

:: Generate x64 build files
pushd "%BUILD_X64%"
cmake ..\..\..\ ^
-G "Visual Studio 16 2019" ^
-A x64 ^
-DBUILD_ROOT="%BUILD_X64%" ^
-DBIN_ROOT="%BIN_X64%"
popd

:: x86 build
pushd "%BUILD_X86%"
MSBuild -m /p:Configuration=Debug ALL_BUILD.vcxproj
if %errorlevel% neq 0 goto :eof
MSBuild -m /p:Configuration=Release ALL_BUILD.vcxproj
if %errorlevel% neq 0 goto :eof
popd

:: x64 build
pushd "%BUILD_X64%"
MSBuild -m /p:Configuration=Debug ALL_BUILD.vcxproj
if %errorlevel% neq 0 goto :eof
MSBuild -m /p:Configuration=Release ALL_BUILD.vcxproj
if %errorlevel% neq 0 goto :eof
popd

:: Copy artifacts
set "PREBUILT_ROOT=%~dp0"
xcopy /Y "%BIN_X64%\Debug" "%BIN_PREBUILD%\x64\Debug\"
xcopy /Y "%BIN_X64%\Release" "%BIN_PREBUILD%\x64\Release\"
xcopy /Y "%BIN_X86%\Debug" "%BIN_PREBUILD%\x86\Debug\"
xcopy /Y "%BIN_X86%\Release" "%BIN_PREBUILD%\x86\Release\"
goto :eof