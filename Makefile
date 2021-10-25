obj-m += kmtx.o

all: mod a.out

mod:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

main: main.cpp Makefile
	clang++ -lpthread main.cpp 

run: main 
	./a.out
	


clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
