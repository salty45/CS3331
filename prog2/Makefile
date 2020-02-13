G = gcc -g -Wall -Wextra -O0

all:
	${G} main.c -o main
	${G} qsort.c -o qsort
	${G} merge.c -o merge	

grade:
	gcc main.c -o main
	gcc qsort.c -o qsort
	gcc merge.c -o merge

.PHONY: clean
clean:
	rm main
	rm qsort
	rm merge
