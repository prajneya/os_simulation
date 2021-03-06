#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>

#include "main.h"
#include "student.h"
#include "course.h"
#include "lab.h"

void init(){
	srand(time(0));

	courses = (Course **) malloc(sizeof(Course *) * C);
	course_t = (pthread_t *) malloc(sizeof(pthread_t) * C);

	for(int i = 0; i < C; i++){
		initCourse(i);
	}

	students = (Student **) malloc(sizeof(Student *) * S);
	student_t = (pthread_t *) malloc(sizeof(pthread_t) * S);

	for(int i = 0; i < S; i++){
		initStudent(i);
	}

	iiit_labs = (Lab **) malloc(sizeof(Lab *) * L);
	lab_t = (pthread_t *) malloc(sizeof(pthread_t) * L);

	for(int i = 0; i < L; i++){
		initLab(i);
	}

	for(int i = 0; i < S; i++){
		createStudentThreads(i);
	}

	for(int i = 0; i < C; i++){
		createCourseThreads(i);
	}

	for(int i = 0; i < L; i++){
		createLabThreads(i);
	}
}

int main(){
	scanf("%d %d %d", &S, &L, &C);
	init();

	for(int i = 0; i < S; i++)
        pthread_join(student_t[i], 0);


    bool simOver = true;

    for(int i = 0; i<S; i++){
    	if(students[i]->curr_pref != -1){
    		simOver = false;
    		break;
    	}
    }

    if(simOver){
    	printf("SIMULATION ENDING\n");
    }

    // printf("SIMULATION ENDING\n");

    return 0;

}

int randRange(int l, int r) {
    if(l > r)
        return l;

    return (rand() % (r - l + 1)) + l;
}