#!/bin/bash

/bin/sh glGraphicsUnixDeconstruct.bash
cd ..
cp -r Unix/osGraphics.h osGraphics.h
cp -r Unix/glGraphicsCompile.bash glGraphicsCompile.bash
cd Unix
