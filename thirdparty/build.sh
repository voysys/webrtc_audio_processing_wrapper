#!/bin/bash

set -o errexit
set -o nounset
set -o pipefail
set -x

cd "$(dirname "${BASH_SOURCE[0]}")"

VERSION=master
WEBRTC_ARTIFACTS_DIR=${PWD}/artifacts/webrtc
ABSL_ARTIFACTS_DIR=${PWD}/artifacts/absl

rm -rf artifacts/
rm -rf build/

mkdir build
mkdir -p "${WEBRTC_ARTIFACTS_DIR}"
mkdir -p "${ABSL_ARTIFACTS_DIR}"

# Build webrtc-audio-processing
cd build
git clone --depth 1 https://github.com/voysys/webrtc-audio-processing.git -b "${VERSION}"
cd webrtc-audio-processing

mkdir build
meson . build --buildtype=release -Dprefix="${WEBRTC_ARTIFACTS_DIR}"
ninja -C build
ninja -C build install

# # Build abseil only for the headers...
cd subprojects/abseil-cpp-20211102.0

mkdir build
cd build

cmake -G Ninja -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX="${ABSL_ARTIFACTS_DIR}" ..
cmake --build . --config Release
cmake --install .
