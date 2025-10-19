CC = gcc

# final executable
app: main.o commands.o csv.o event.o node.o utils.o
	$(CC) main.o commands.o csv.o event.o node.o utils.o -o app
	

main.o: src/main.c include/commands.h include/csv.h include/event.h include/node.h include/utils.h
	$(CC) -Iinclude -c src/main.c -o main.o

commands.o: src/commands.c include/commands.h include/csv.h include/event.h include/node.h include/utils.h
	$(CC) -Iinclude -c src/commands.c -o commands.o

csv.o: src/csv.c include/commands.h include/csv.h include/event.h include/node.h include/utils.h
	$(CC) -Iinclude -c src/csv.c -o csv.o

event.o: src/event.c include/commands.h include/csv.h include/event.h include/node.h include/utils.h
	$(CC) -Iinclude -c src/event.c -o event.o

node.o: src/node.c include/commands.h include/csv.h include/event.h include/node.h include/utils.h
	$(CC) -Iinclude -c src/node.c -o node.o

utils.o: src/utils.c include/commands.h include/csv.h include/event.h include/node.h include/utils.h
	$(CC) -Iinclude -c src/utils.c -o utils.o

run: app
	./app
