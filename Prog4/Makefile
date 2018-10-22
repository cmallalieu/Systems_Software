OPTIONS = -DNDEBUG -g -Wall -Werror
OPTIONS = -g -Wall -Werror

Allocation.o: Allocation.h Allocation.c
	gcc -g -Wall -Werror -c Allocation.c

Allocator.o: Allocator.h Allocator.c
	gcc -g -Wall -Werror -c Allocator.c

newprog4: Allocation.o Allocator.o prog4.o
	gcc -g -Wall -Werror -o newprog4 prog4.c Allocation.o Allocator.o

test: newprog4
	./newprog4 20 99 1
	rm Allocation.o Allocator.o
