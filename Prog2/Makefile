all	: main qsort merge

main	: main.c
	gcc main.c -o main

qsort	: qsort.c
	gcc qsort.c -o qsort

merge	: merge.c
	gcc merge.c -o merge

clean	:
	rm -f main qsort merge

run	:
	./main < input
