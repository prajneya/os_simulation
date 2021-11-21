#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>

#include "main.h"
#include "student.h"

Course * wait_for_slot(Student * s) {
    bool gotSlot = false;
    Course * c;

    while(!gotSlot) {
        for(int i = 0; i < C; i++) {
            pthread_mutex_lock(&courses[i]->mutex);

            if(courses[i]->d > 0) {
                gotSlot = true;

                printf("Student %d has been allocated a seat in "
                        "course %s\n",
                        s->uid, courses[i]->name);

                courses[i]->d--;
                c = courses[i];

                pthread_mutex_unlock(&courses[i]->mutex);
                break;
            }

            pthread_mutex_unlock(&courses[i]->mutex);
        }
    }

    return c;
}

void * studentRunner(void * a) {
    Student * s = (Student *)a;

    sleep(s->arr_time);
    printf("Student %d has filled in preferences for course registration\n", s->uid);
    printf("Student %d is waiting to be allocated "
            "a course\n", s->uid);

	Course * c = wait_for_slot(s);

    return 0;
}


void initStudent(int i) {
    Student * s = (Student *) malloc(sizeof(Student));

    s->uid = i;

    scanf("%lf %d %d %d %d", &s->callibre, &s->pref_one, &s->pref_two, &s->pref_three, &s->arr_time);

    students[i] = s;
}

void createStudentThreads(int i) {
	pthread_create(&student_t[i], 0, studentRunner, students[i]); 
}