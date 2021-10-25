#!/bin/bash

#log: tail -f var/log/kern.log
#g++ main.cpp -lpthread && ./a.out

rmmod kmtx.ko;
make &&
insmod kmtx.ko
