#!/bin/bash

START_DIR=$PWD
SRC_DIR=$START_DIR/src

echo "building DaisyDrone . . ."
cd $SRC_DIR ; make clean ; make -j;
echo "done."
