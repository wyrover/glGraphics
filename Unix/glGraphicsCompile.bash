#!/bin/bash

g++ -o gl_main_void -I -pthread gl_main_void.cpp glElements.cpp glForm.cpp -lGL -lglut -lGLU
