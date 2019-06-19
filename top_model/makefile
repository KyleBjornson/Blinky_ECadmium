CC=g++
CFLAGS=-std=c++17
COMPILE_TARGET=NUCLEO_F401RE
FLASH_TARGET=NODE_F401RE
EXECUTABLE_NAME=BLINKY_TOP

INCLUDE_CADMIUM=-I ../../cadmium/include
INCLUDE_DESTIMES=-I ../../cadmium/DESTimes/include

embedded:
	mbed compile --target $(COMPILE_TARGET) --toolchain GCC_ARM --profile ../cadmium.json

flash:
	sudo cp ../BUILD/$(COMPILE_TARGET)/GCC_ARM-CADMIUM/*.bin /media/$(USER)/$(FLASH_TARGET)/
	$(info *** FLASH WILL TAKE ~15 Seconds! DO NOT RESET WHILE COM PORT LED IS FLASHING! ***)

all: main.o
	$(CC) -g -o $(EXECUTABLE_NAME) main.o 

main.o: main.cpp
	$(CC) -g -c $(CFLAGS) $(INCLUDE_CADMIUM) $(INCLUDE_DESTIMES) main.cpp -o main.o

rt_linux: 
	$(CC) -g -c $(CFLAGS) -D RT_LINUX $(INCLUDE_CADMIUM) $(INCLUDE_DESTIMES) main.cpp -o main.o
	$(CC) -g -o $(EXECUTABLE_NAME) main.o 

clean:
	rm -f $(EXECUTABLE_NAME) *.o *~

eclean:
	rm -rf ../BUILD
