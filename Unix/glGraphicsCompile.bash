#!/bin/bash

VAR_ARG="g++ -o gl_main_void -I -pthread gl_main_void.cpp glElements.cpp glForm.cpp glOSFunctions.cpp -w"

if [[ "$OSTYPE" == "darwin"* ]]; then
VAR_ARG="$VAR_ARG -framework OpenGL -framework GLUT"
else
VAR_ARG = "$VAR_ARG -lGL -lglut -lGLU"
fi
eval $VAR_ARG