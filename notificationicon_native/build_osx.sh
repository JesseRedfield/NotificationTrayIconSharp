#! /bin/zsh

SCRIPTDIR=$(pwd)

## Setup paths to build outputs
BUILD_ROOT=$SCRIPTDIR/_build/osx
if [[ -d "$BUILD_ROOT" ]]; then
    rm -rf $BUILD_ROOT
fi
mkdir -p $BUILD_ROOT

## Setup paths to project outputs
BIN_ROOT=$SCRIPTDIR/bin/osx
if [[ -d "$BIN_ROOT" ]]; then
    rm -rf $BIN_ROOT
fi

## Generate build files
cd $BUILD_ROOT
cmake ../../ \
-G "Xcode" \
-DBUILD_ROOT="$BUILD_ROOT" \
-DBIN_ROOT="$BIN_ROOT"
cd $SCRIPTDIR

## Run the build
declare -a archs=("x86_64")
declare -a configs=("Debug" "Release")

BIN_PREBUILT=$SCRIPTDIR/../prebuilt/osx
if [[ ! -d "$BIN_PREBUILT" ]]; then
    mkdir -p $BIN_PREBUILT
fi

for arch in "${archs[@]}"
do
    ARCH_ROOT=$BIN_ROOT/$arch
    mkdir -p $ARCH_ROOT

    for config in "${configs[@]}"
    do
        cd $BUILD_ROOT
        xcodebuild ONLY_ACTIVE_ARCH=NO -arch $arch -sdk macosx -configuration $config -parallelizeTargets
        mv $BIN_ROOT/$config $ARCH_ROOT/$config
    done

    ## Copy artifacts
    cp -r $ARCH_ROOT $BIN_PREBUILT
    cp -r $ARCH_ROOT $BIN_PREBUILT
done