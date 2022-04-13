flush: command.c list.c parsing.c flush.c
	gcc -o flush.o command.c list.c parsing.c flush.c -I.

flush_gdb: command.c list.c parsing.c flush.c
	gcc -o flush.o command.c list.c parsing.c flush.c -g -I.
