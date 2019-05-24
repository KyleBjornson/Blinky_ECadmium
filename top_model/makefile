CC=g++
CFLAGS=-std=c++17
COMPILE_TARGET=NUCLEO_F401RE
FLASH_TARGET=NODE_F401RE
EXECUTABLE_NAME=BLINKY_TOP

INCLUDECADMIUM=-I ../../cadmium/include

embedded: 
	mbed compile --target $(COMPILE_TARGET) --toolchain GCC_ARM --profile ../cadmium.json

flash: 
	sudo cp ../BUILD/$(COMPILE_TARGET)/GCC_ARM-CADMIUM/*.bin /media/$(USER)/$(FLASH_TARGET)/

all: main.o ../data_structures/message.o 
	$(CC) -g -o $(EXECUTABLE_NAME) main.o ../data_structures/message.o 

main.o: main.cpp
	$(CC) -g -c $(CFLAGS) $(INCLUDECADMIUM) main.cpp -o main.o

../data_structures/message.o: 
	$(CC) -g -c $(CFLAGS) $(INCLUDECADMIUM) ../data_structures/message.cpp -o ../data_structures/message.o

clean:
	rm -f $(EXECUTABLE_NAME) *.o *~
	-for d in ../data_structures; do (cd $$d; rm -f *.o *~ ); done

eclean: 
	rm -rf ../BUILD
