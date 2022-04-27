#!/bin/bash

set -o errexit
set -o nounset
set -o pipefail
set -x

cd "$(dirname "${BASH_SOURCE[0]}")"

cd thirdparty
bash ./build.sh
cd ..

INSTALL_DIR=${PWD}/install

rm -rf build/
rm -rf install/

mkdir build
mkdir install

cd build
cmake -G Ninja -DCMAKE_BUILD_TYPE=Release ..
ninja
ninja install
cd ..
