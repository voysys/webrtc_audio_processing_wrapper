setlocal

cd thirdparty
CALL build.bat
cd ..

SET WRAPPER_NAME=webrtc_audio_processing_wrapper
SET INSTALL_DIR=%cd%\install

rmdir /q /s build
rmdir /q /s install

mkdir build
mkdir install

cd build
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=%INSTALL_DIR% ..
cmake --build . --config Release
cmake --install .
cd ..

copy /Y %INSTALL_DIR%\lib\%WRAPPER_NAME%.lib ..\..\..\lib\windows\x86_64\
copy /Y %INSTALL_DIR%\bin\%WRAPPER_NAME%.dll ..\..\..\lib\windows\x86_64\
copy /Y %WRAPPER_NAME%.h ..\..\..\include\windows\
