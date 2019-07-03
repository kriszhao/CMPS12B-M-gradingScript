Subset: clean
	gcc -c -std=c99 -Wall -Werror -pedantic Subset.c
	gcc -o Subset *.o

ModelSubsetTest: clean
	gcc -c -std=c99 -Wall -Werror -pedantic ModelSubsetTest.c
	gcc -o ModelSubsetTest *.o

clean:
	rm -f *.o Subset ModelSubsetTest

.PHONY: Subset ModelSubsetTest clean
