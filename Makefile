all: mandel mandelmovie

mandel: mandel.o bitmap.o
	gcc mandel.o bitmap.o -o mandel -lpthread
mandelmovie: mandelmovie.c
	gcc -lm mandelmovie.c -o mandelmovie -Wall
mandel.o: mandel.c
	gcc -Wall -g -c mandel.c -o mandel.o

bitmap.o: bitmap.c
	gcc -Wall -g -c bitmap.c -o bitmap.o


clean:
	rm -f mandel.o bitmap.o mandelmovie.o mandel mandelmovie