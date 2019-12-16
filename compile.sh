#!/bin/bash

gcc -c ./src/lib/*.c && gcc -c ./src/nimps/nimps.c && gcc -o nimps *.o