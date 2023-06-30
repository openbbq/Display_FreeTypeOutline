TARGET=gen

CC=gcc
CFLAGS=`pkg-config --cflags freetype2`
LIBS=`pkg-config --libs freetype2` 

.PHONY: default all
default: $(TARGET)
all: default

gen.o: gen.cpp
	$(CC) $(CFLAGS) -c $< -o $@

gen: gen.o
	$(CC) gen.o $(LIBS) -lstdc++ -o $@
