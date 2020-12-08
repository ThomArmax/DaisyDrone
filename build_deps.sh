#!/bin/bash

START_DIR=$PWD
LIBDAISY_DIR=$PWD/libDaisy
DAISYSP_DIR=$PWD/DaisySP

echo "building libDaisy . . ."
cd $LIBDAISY_DIR ; make clean ; make -j;
echo "done."

echo "building DaisySP . . ."
cd $DAISYSP_DIR ; make clean ; make -j;
echo "done."
