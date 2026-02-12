#!/bin/bash

set -o errexit
set -o nounset
set -o pipefail
set -x

cd "$(dirname "${BASH_SOURCE[0]}")"

VERSION=5a8709d00af2ed93ede71c0dc69bcc7cc990e0e6

WEBRTC_ARTIFACTS_DIR=${PWD}/artifacts/webrtc
ABSL_ARTIFACTS_DIR=${PWD}/artifacts/absl/include

# For newer clang versions that don't implicitly include cstdint
export CXXFLAGS="-include cstdint"

rm -rf artifacts/
rm -rf build/

mkdir build
mkdir -p "${WEBRTC_ARTIFACTS_DIR}"
mkdir -p "${ABSL_ARTIFACTS_DIR}"

# Build webrtc-audio-processing
cd build

git init webrtc-audio-processing
cd webrtc-audio-processing
git remote add origin https://github.com/voysys/webrtc-audio-processing.git
git fetch --depth 1 origin $VERSION
git checkout $VERSION

mkdir build
meson setup . build --buildtype=release --wrap-mode=forcefallback -Dprefix="${WEBRTC_ARTIFACTS_DIR}"
ninja -C build
ninja -C build install

# Copy abseil includes
cd subprojects/abseil-cpp-20250814.1

rsync -a --prune-empty-dirs --include '*/' --include '*.h' --exclude '*' absl "${ABSL_ARTIFACTS_DIR}"
