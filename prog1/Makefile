all: prog1.c
	gcc -g -std=c89 -Wall -Wextra -O0 -lm prog1.c -o prog1

grade: prog1.c
	gcc prog1.c -lm -o prog1

.PHONY:clean
clean:
	rm prog1
