@echo off
setlocal EnableExtensions EnableDelayedExpansion
pushd "%~dp0"
set "Build_Root=%~dp0"
set version=7z2200
set "_7z=C:\Program Files\7-Zip\7z.exe"

:Build_x64
pushd CPP\7zip
nmake NEW_COMPILER=1 MY_STATIC_LINK=1 CPU=AMD64 PLATFORM=x64
rem nmake NEW_COMPILER=1 MY_STATIC_LINK=1 PLATFORM=x86
popd

pushd C\Util\7z
nmake NEW_COMPILER=1 MY_STATIC_LINK=1 CPU=AMD64 PLATFORM=x64
popd

pushd C\Util\7zipInstall
nmake NEW_COMPILER=1 MY_STATIC_LINK=1 CPU=AMD64 PLATFORM=x64
popd

pushd C\Util\7zipUninstall
nmake NEW_COMPILER=1 MY_STATIC_LINK=1 CPU=AMD64 PLATFORM=x64
popd

pushd C\Util\SfxSetup
nmake NEW_COMPILER=1 MY_STATIC_LINK=1 CPU=AMD64 PLATFORM=x64
nmake /F makefile_con NEW_COMPILER=1 MY_STATIC_LINK=1 CPU=AMD64 PLATFORM=x64
popd

pushd CPP\7zip\UI\Explorer
nmake NEW_COMPILER=1 MY_STATIC_LINK=1
popd

:Package

REM 7-zip
pushd CPP\7zip
mkdir ..\..\7-zip-x64
copy UI\Console\x64\7z.exe ..\..\7-zip-x64\7z.exe
copy UI\Explorer\x64\7-zip.dll ..\..\7-zip-x64\7-zip.dll
copy UI\FileManager\x64\7zFM.exe ..\..\7-zip-x64\7zFM.exe
copy UI\GUI\x64\7zG.exe ..\..\7-zip-x64\7zG.exe
copy Bundles\Format7zF\x64\7z.dll ..\..\7-zip-x64\7z.dll
copy Bundles\SFXCon\x64\7zCon.sfx ..\..\7-zip-x64\7zCon.sfx
copy Bundles\SFXWin\x64\7z.sfx ..\..\7-zip-x64\7z.sfx
copy ..\..\C\Util\7zipUninstall\x64\7zipUninstall.exe ..\..\7-zip-x64\Uninstall.exe
copy UI\Explorer\x86\7-zip.dll ..\..\7-zip-x64\7-zip32.dll
popd

"%_7z%" a -m0=lzma -mx9 %version%-x64.7z .\7-zip-x64\*
copy /b .\C\Util\7zipInstall\x64\7zipInstall.exe /b + %version%-x64.7z /b %version%-x64.exe
