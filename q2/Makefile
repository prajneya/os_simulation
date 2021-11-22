CFLAGS = -g -Wall -Wextra -pthread

all: main.o group.o team.o
	gcc $(CFLAGS) -o q2 main.o group.o team.o

main.o: main.h main.c group.h team.h
	gcc $(CFLAGS) -c main.c

group.o: group.h main.h group.c
	gcc $(CFLAGS) -c group.c

team.o: team.h main.h team.c
	gcc $(CFLAGS) -c team.c

clean:
	rm *.o q2