#!/bin/bash

i686-w64-mingw32-gcc -c stomptime.c -o stomptime.x86.o
x86_64-w64-mingw32-gcc -c stomptime.c -o stomptime.x64.o