#!/bin/bash

OUTPUT_DIR=".build"
EXEC_NAME=hod24.exe

./build.sh $OUTPUT_DIR $EXEC_NAME

if [ $? -eq 0 ]; then
    echo -e "\n---- POWERED BY WINE ----"

    # For some reason using the native dll crashes on initalisation
    # No clue why, for ABAS it works without a problem, which is using the exact same setup
    #export WINEPREFIX="$HOME/.wine_xaudio"
    #export WINEDLLOVERRIDES="xaudio2_9=n"

    # NOTE: wine searches in the directory relative to the executable!
    # not the current directory from which it was called
    # this is different to how the game itself searches for resources from the directory it was called
    wine $OUTPUT_DIR/$EXEC_NAME debug
fi
