flush: command.c parsing.c flush.c
	gcc -o flush.o command.c parsing.c flush.c -I.

flush_gdb: command.c parsing.c flush.c
	gcc -o flush.o command.c parsing.c flush.c -g -I.
