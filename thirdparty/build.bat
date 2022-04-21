setlocal

REM Run using "x64 Native Tools Command Prompt for VS"
REM Prerequisites:
REM   Python3
REM   Meson
REM   Ninja

REM TODO: SET VERSION
set VERSION=master

rmdir /q /s artifacts
rmdir /q /s build

mkdir build
mkdir artifacts
mkdir artifacts\webrtc
mkdir artifacts\absl

SET WEBRTC_ARTIFACTS_DIR=%cd%\artifacts\webrtc
SET ABSL_ARTIFACTS_DIR=%cd%\artifacts\absl

REM Build webrtc-audio-processing

cd build

git clone --depth 1 https://github.com/voysys/webrtc-audio-processing.git -b %VERSION%

cd webrtc-audio-processing
mkdir build

meson . build --buildtype=release -Dprefix=%WEBRTC_ARTIFACTS_DIR%
ninja -C build
ninja -C build install

REM Build abseil only for the headers...

cd subprojects\abseil-cpp-20211102.0

mkdir build
cd build

cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=%ABSL_ARTIFACTS_DIR% ..
cmake --build . --config Release
cmake --install .
