
vpath %.h includes
vpath %.c src
vpath %.o bin

CC = gcc
SRC = $(wildcard src/*.c)
OBJ = $(patsubst src/%.c, bin/%.o, $(SRC))

default: $(OBJ) 

clean: # delete all .o files
	rm bin/*.o
	rm *.out

run:
	$(CC) $(OBJ) -o threadPool.out -lpthread
	./threadPool.out

#rules	
bin/%.o: %.c
	$(CC) -c -o $@ $< -lpthread
