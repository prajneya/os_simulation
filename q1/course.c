#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "main.h"
#include "course.h"

void * courseRunner(void * a) {
    Course * t = (Course *)a;

    while(1) {
    	pthread_mutex_lock(&t->mutex);
        if(t->ta_allocated >= 0){
        	t->d = randRange(1, t->course_max_slots);
        	printf("Course %s has been allotted %d seats\n", t->name, t->d);
        }
        pthread_mutex_unlock(&t->mutex);
    }

    return 0;
}

void initCourse(int i) {
    Course * t = (Course *) malloc(sizeof(Course));

    t->uid = i;
    t->ta_allocated = -1;
    t->d = 0;

   	scanf("%s %lf %d %d", t->name, &t->interest, &t->course_max_slots, &t->p);

   	for(int j = 0; j < t->p; j++){
   		scanf("%d", &t->course_labs[j]);
   	}

    pthread_mutex_init(&t->mutex, 0);

    courses[i] = t;
}

void createCourseThreads(int i) {
	pthread_create(&course_t[i], 0, courseRunner, courses[i]); 
}