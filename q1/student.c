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
                printf(ANSI_COLOR_GREEN "Student %d has been allocated a seat in course %s\n" ANSI_COLOR_RESET, s->uid, courses[i]->name);

                courses[i]->d--;

                pthread_mutex_unlock(&courses[i]->mutex);
                break;
            }

            pthread_mutex_unlock(&courses[i]->mutex);
        }

        if(s->curr_pref != -1 && courses[s->curr_pref]->courseValid <= 0){
        	// EVENT 3
		    printf(ANSI_COLOR_YELLOW "Student %d has withdrawn from course %s\n" ANSI_COLOR_RESET, s->uid, courses[s->curr_pref]->name);
		    pthread_mutex_lock(&s->mutex);
			if(s->curr_pref==s->pref_one){
    			s->curr_alloc = -1;
    			s->curr_pref = s->pref_two;
    			// EVENT 4
    			printf(ANSI_COLOR_BLUE "Student %d has changed current preference from %s (priority 1) to %s (priority 2)\n" ANSI_COLOR_RESET, s->uid, courses[s->pref_one]->name, courses[s->pref_two]->name);
    		}
    		else if(s->curr_pref==s->pref_two){
    			s->curr_alloc = -1;
    			s->curr_pref = s->pref_three;
    			// EVENT 4
    			printf(ANSI_COLOR_BLUE "Student %d has changed current preference from %s (priority 2) to %s (priority 3)\n" ANSI_COLOR_RESET, s->uid, courses[s->pref_two]->name, courses[s->pref_three]->name);
    		}
    		else{
    			// EVENT 6
    			printf(ANSI_COLOR_CYAN "Student %d couldn't get any of his preferred courses\n" ANSI_COLOR_RESET, s->uid);
    			s->curr_pref = -1;
    		}
    		pthread_mutex_unlock(&s->mutex);
        }
        
    }

    return;
}

void * studentRunner(void * a) {
    Student * s = (Student *)a;

    sleep(s->arr_time);
    // EVENT 1
    printf(ANSI_COLOR_RED "Student %d has filled in preferences for course registration\n" ANSI_COLOR_RESET, s->uid);

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