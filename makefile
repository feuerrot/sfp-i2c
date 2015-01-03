all: main

main: main.o
	 gcc -o main main.o

main.o: main.c
	 gcc -c -Wall -Wextra -Werror --std c99 -O3 main.c
     
clean:
	 rm main
