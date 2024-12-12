#!/bin/bash

C_FILES="src/compile_extern.c"
C_FILES_OUTPUT=".obj/compile_extern.o"

SOURCE_FILE="src/compile.cpp"
PRECOMPILE_FILE="src/imports.h"

DEFAULT_BUILD_DIR=".build"
PRECOMPILE_OUTPUT=".obj/imports.h.pch"

if [[ "$1" =~ ^- ]] || [ -z "$1" ]; then
    BUILD_DIR=$DEFAULT_BUILD_DIR
else 
    BUILD_DIR=$1
fi

if [[ "$2" =~ ^- ]] || [ -z "$2" ]; then
    echo expecting the executable name as 2nd argument
    exit -1;
else
    EXEC_NAME=$2
fi

OUTPUT_FILE=$BUILD_DIR/$EXEC_NAME

# v for verbose
if [[ "$*" == *"-v"* ]]; then
    DEBUG="-v"
fi

# p for profile
if [[ "$*" == *"-p"* ]]; then
    PROFILE="-ftime-trace -c"
fi

WIN_SDK="$HOME/03-resources/libs/WindowsKits/10/Include/10.0.26100.0"
WIN_LIB="$HOME/03-resources/libs/WindowsKits/10/Lib/10.0.26100.0"
MSVC="$HOME/03-resources/libs/MSVC/14.40.33807"

# isystem supresses all the warning in the header files
# which windows seems to have a ton of
# NOTE: the windows sdk has a bunch of lower/upper case naming issues in it, that need to be fixed
# - an easy way to do that, is just to create a symbolic link to those "misspelled" files
SDK_INCLUDES="-isystem $WIN_SDK/um -isystem $WIN_SDK/shared -isystem $WIN_SDK/ucrt -isystem $WIN_SDK/winrt -isystem $WIN_SDK/cppwinrt"
MSVC_INCLUDES="-isystem $MSVC/include -isystem $MSVC/atlmfc/include"

# libs have the same case insensitive search problem
# It seems a better approach would be setting up a virtual filesystem, that all works out of the box
# Else i need to adjust it every single time
SDK_LIBS="-L$WIN_LIB/crt/x64 -L$WIN_LIB/um/x64 -L$WIN_LIB/ucrt/x64 -L$MSVC/lib/x64 -L$MSVC/atlmfc/lib/x64"

# including std 20 adds 600ms extra compile time for compiling with pre-compiled headers (1250ms vs 700ms)
# and for precompiling the headers its 2s (3500ms vs 1500ms)
# But 14 is the minimum required one so that the windows headers still work
# Even thou, it seems, this adds a whole bunch of compile time (maybe this is the major thing that introduces it?)
CPP_STD="-std=c++14"
INCLUDES="$MSVC_INCLUDES $SDK_INCLUDES"
LIB_PATHS="$SDK_LIBS"
INCLUDE_PRECOMPILED="-include-pch $PRECOMPILE_OUTPUT"
TARGET="-target x86_64-pc-win32 -fuse-ld=lld"
LINKED_LIBS="-lgdi32 -luser32 -lole32 -lshell32 -lwinmm -lmfreadwrite -lmfplat -lmfuuid -lxaudio2"
OPTIONS="$DEBUG -Wall -Wpedantic -Wno-c99-extensions -Wno-c++17-extensions $PROFILE -DDEBUG" 

# -D_AMD64_ or -DX86_ = 1 is required for some windows headers
MSVC_DEFAULTS="$TARGET $INCLUDES -D_AMD64_=1"

PRE_COMPILE_START=$(date +%s%N)

# h for headers
if [[ "$*" == *"-h"* ]]; then
    clang -c $C_FILES -o $C_FILES_OUTPUT $MSVC_DEFAULTS -w #-msse $LIB_PATHS $LINKED_LIBS
    clang++ -x c++-header $PRECOMPILE_FILE -o $PRECOMPILE_OUTPUT $MSVC_DEFAULTS $CPP_STD $DEBUG
fi

PRE_COMPILE_END=$(date +%s%N)
START_TIME=$(date +%s%N)

#TODO: cleaner setup for this

clang++ $SOURCE_FILE $C_FILES_OUTPUT -o $OUTPUT_FILE $INCLUDE_PRECOMPILED $MSVC_DEFAULTS $CPP_STD $LIB_PATHS $LINKED_LIBS $OPTIONS
COMPILE_STATUS=$?

END_TIME=$(date +%s%N)

# NOTE: this is not necessary for running, because we're starting the game from the root dir
# and then the root dir will be the executing directory for the game on linux
#cp -r ./res $BUILD_DIR/res

NANO_TO_MS=1000000
COMPILE_TIME=$(((END_TIME - START_TIME) / $NANO_TO_MS))
PRE_COMPILE_TIME=$(((PRE_COMPILE_END - PRE_COMPILE_START)/ $NANO_TO_MS))

# Check for errors
if [ $COMPILE_STATUS -eq 0 ]; then
    echo "Compilation successful." 
fi

PC_CHARS=${#PRE_COMPILE_TIME}
C_CHARS=${#COMPILE_TIME}
if (( $C_CHARS > $PC_CHARS )); then
    MAX_CHARS=$C_CHARS
else
    MAX_CHARS=$PC_CHARS
fi

# since seq / printf doesn't support 0, we just increment it by 1
# and then truncate one later
PAD_PC=$(printf "%.s " $(seq 1 $(($MAX_CHARS-$PC_CHARS+1))))
PAD_C=$(printf "%.s " $(seq 1 $(($MAX_CHARS-$C_CHARS+1))))

echo  "| Pre-Compile :: ${PAD_PC:1}$PRE_COMPILE_TIME ms"
echo  "| Compile     :: ${PAD_C:1}$COMPILE_TIME ms"

exit $COMPILE_STATUS
