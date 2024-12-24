#!/bin/bash

#TODO: proper version management
VERSION="0.1"
OUTPUT_DIR="release"
ZIP_DIR="publish"
EXEC_NAME=escape-room.exe
ZIP_NAME="escape-room-unseen-wilderness-v$VERSION.zip"

rm -r $OUTPUT_DIR
mkdir $OUTPUT_DIR

if [ ! -d $ZIP_DIR ]; then
    mkdir $ZIP_DIR
fi

./build.sh $OUTPUT_DIR $EXEC_NAME

cp -r res $OUTPUT_DIR
cp -r configs $OUTPUT_DIR

zip -r $ZIP_DIR/$ZIP_NAME $OUTPUT_DIR/*
