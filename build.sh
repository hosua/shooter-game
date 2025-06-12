#!/bin/bash

REBUILD=false
BUILD_TYPE="Release"

for arg in "$@"; do
    case "$arg" in
        --rebuild|-r)
            REBUILD=true
            ;;
        --debug|-d)
            BUILD_TYPE="Debug"
            ;;
        *)
            echo "Unknown option: $arg"
            exit 1
            ;;
    esac
done

if [ "$REBUILD" = true ] || [ ! -d build ]; then
    rm -rf build
    mkdir build
fi

cd build || exit

cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCMAKE_BUILD_TYPE="$BUILD_TYPE" ..
make
