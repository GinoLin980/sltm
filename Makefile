all: build

build:
	gcc -D_GNU_SOURCE -I include/ src/* -o sltm -std=c11
