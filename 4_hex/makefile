all: hex

hex: hex.o utility.o update.o input.o string.o ai.o
	gcc hex.o utility.o update.o input.o string.o ai.o -o hex

hex.o: hex.c
	gcc -c hex.c

utility.o: utility.c
	gcc -c utility.c

update.o: update.c
	gcc -c update.c

input.o: input.c
	gcc -c input.c

string.o: string.c
	gcc -c string.c

ai.o: ai.c
	gcc -c ai.c
	
clean:
	rm *o hex
