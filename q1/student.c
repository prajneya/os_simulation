#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>

#include "main.h"
#include "student.h"

void wait_for_slot(Student * s) {
    while(s->curr_pref!=-1) {
        for(int i = 0; i < C; i++) {
            pthread_mutex_lock(&courses[i]->mutex);

            if(s->curr_alloc == -1 && courses[i]->d > 0 && courses[i]->ta_allocated >= 0 && courses[i]->uid == s->curr_pref) {

                pthread_mutex_lock(&s->mutex);
                s->curr_alloc = courses[i]->uid;
                pthread_mutex_unlock(&s->mutex);

                // EVENT 2
                printf("Student %d has been allocated a seat in "
                        "course %s\n",
                        s->uid, courses[i]->name);

                courses[i]->d--;

                pthread_mutex_unlock(&courses[i]->mutex);
                break;
            }

            pthread_mutex_unlock(&courses[i]->mutex);
        }
    }

    return;
}

void * studentRunner(void * a) {
    Student * s = (Student *)a;

    sleep(s->arr_time);
    // EVENT 1
    printf("Student %d has filled in preferences for course registration\n", s->uid);

	wait_for_slot(s);

    return 0;
}


void initStudent(int i) {
    Student * s = (Student *) malloc(sizeof(Student));

    s->uid = i;
    s->finalised = 0;
    s->curr_alloc = -1;

    scanf("%lf %d %d %d %d", &s->callibre, &s->pref_one, &s->pref_two, &s->pref_three, &s->arr_time);

    s->curr_pref = s->pref_one;

    pthread_mutex_init(&s->mutex, 0);

    students[i] = s;
}

void createStudentThreads(int i) {
	pthread_create(&student_t[i], 0, studentRunner, students[i]); 
}