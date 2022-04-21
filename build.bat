setlocal

cd thirdparty
CALL build.bat
cd ..

SET INSTALL_DIR=\\?\%cd%\install

rmdir /q /s build
rmdir /q /s install

mkdir build
mkdir install

cd build
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=%INSTALL_DIR% ..
cmake --build . --config Release
cmake --install .
cd ..
