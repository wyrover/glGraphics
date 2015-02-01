#!/bin/bash

/bin/sh glGraphicsUnixDeconstruct.bash
cd ..
cp -r Unix/osGraphics.h osGraphics.h
cp -r Unix/glGraphicsCompile.bash glGraphicsCompile.bash
cp -r Unix/glOSFunctions.h glOSFunctions.h
cp -r Unix/glOSFunctions.cpp glOSFunctions.cpp
cd Unix
