#!/bin/bash

set -o errexit
set -o nounset
set -o pipefail
set -x

cd "$(dirname "${BASH_SOURCE[0]}")"

cd thirdparty
./build.sh
cd ..

INSTALL_DIR=${PWD}/install

rm -rf build/
rm -rf artifacts/

mkdir build
mkdir install

cd build
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX="${INSTALL_DIR}" ..
cmake --build . --config Release
cmake --install .
cd ..
