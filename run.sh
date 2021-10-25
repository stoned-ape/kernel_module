#!/bin/bash

#log: tail -f /var/log/kern.log
#g++ main.cpp -lpthread && ./a.out
#sudo mknod /dev/mtx c 247 0

if [-f '/dev/mtx' ]; then
	echo 'creating node /dev/mtx'
	mknod /dev/mtx c 247 0
else
	echo 'file already exits'
fi


rmmod kmtx.ko;
make &&
insmod kmtx.ko
