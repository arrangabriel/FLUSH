flush:
	cd src && gcc -o flush.o command.c list.c parsing.c utils.c flush.c -I. && mv flush.o ../

flush_gdb:
	cd src && gcc -o flush.o command.c list.c parsing.c utils.c flush.c -g -I. && mv flush.o ../
