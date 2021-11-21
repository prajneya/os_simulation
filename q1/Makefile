CFLAGS = -g -Wall -Wextra -pthread

all: main.o student.o course.o lab.o
	gcc $(CFLAGS) -o q1 main.o student.o course.o lab.o

main.o: main.h main.c student.h course.h lab.h
	gcc $(CFLAGS) -c main.c

student.o: student.h main.h student.c
	gcc $(CFLAGS) -c student.c

course.o: course.h main.h course.c
	gcc $(CFLAGS) -c course.c

lab.o: lab.h main.h lab.c
	gcc $(CFLAGS) -c lab.c

clean:
	rm *.o q1