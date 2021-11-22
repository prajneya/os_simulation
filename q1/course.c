#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>

#include "main.h"
#include "course.h"

void * courseRunner(void * a) {
    Course * t = (Course *)a;

    while(1){
    	pthread_mutex_lock(&t->mutex);

    	for(int k = 0; k < t->p; k++){
    		Lab * l = iiit_labs[t->course_labs[k]];
    		pthread_mutex_lock(&l->mutex);
    		for(int i = 0; i < l->n_tas; i++){
	          // Check if TAs are available
	          if(l->availability[i] == 1 && l->curr_times[i] < l->max_times){
	            // if TA is not allocated
	            if(t->ta_allocated == -1){


	              // BONUS
	              bool equalOpp = true;
	              for(int j = 0; j < l->n_tas; j++){
	              	if(j!=i){
	              		if(l->curr_times[i]-l->curr_times[j]>0){
	              			equalOpp = false;
	              		}
	              	}
	              }

	              if(!equalOpp){
	              	pthread_mutex_lock(&l->mutex);
	              	break;
	              }

	              t->ta_allocated = i;
	              t->lab_allocated = l->uid;

	              //suffix for TASHIP, EVENT 11
	              if(l->curr_times[i] == 0){
	                printf("TA %d from lab %s has been allocated to course %s for his %dst TA ship\n", i, l->name, t->name, l->curr_times[i]+1);
	              }
	              else if(l->curr_times[i] == 1){
	                printf("TA %d from lab %s has been allocated to course %s for his %dnd TA ship\n", i, l->name, t->name, l->curr_times[i]+1);
	              }
	              else if(l->curr_times[i] == 2){
	                printf("TA %d from lab %s has been allocated to course %s for his %drd TA ship\n", i, l->name, t->name, l->curr_times[i]+1);
	              }
	              else{
	                printf("TA %d from lab %s has been allocated to course %s for his %dth TA ship\n", i, l->name, t->name, l->curr_times[i]+1);
	              }
	              l->availability[i] = 0;
	              l->curr_times[i]++;
	              break;
	            }
	          }
	        }
    		pthread_mutex_unlock(&l->mutex);
    	}


        if(t->ta_allocated >= 0 && t->tutorial == 0){
        	t->d = randRange(1, t->course_max_slots);
        	t->tutorial = 1;
        	t->tut_seats = t->d;
        	//EVENT 7
        	printf("Course %s has been allotted %d seats\n", t->name, t->d);
        }

        if(t->tutorial){
        	int w = 0;
	        for(int i = 0; i < S; i++){
	        	if(students[i]->curr_pref == t->uid){
	        		w++;
	        	}
	        }

	        if(w!=0 && (w+t->d == t->tut_seats || t->d == 0)){
	        	// EVENT 8
	        	printf("Tutorial has started for %s with %d seats filled out of %d\n", t->name, t->tut_seats - t->d, t->tut_seats);
	        	// Sleep for 2 seconds for tutorial
		        sleep(2);

		        // end tutorial
		        t->tutorial = 0;
		        t->d = 0;
		        t->tut_seats = 0;

		        // TA exits
		        // EVENT 9
	        	printf("TA %d from lab %s has completed the tutorial and left the course %s\n", t->ta_allocated, iiit_labs[t->lab_allocated]->name, t->name);
	    		t->ta_allocated = -1;
		        t->lab_allocated = -1;

	        	for(int i = 0; i < S; i++){
		        	if(students[i]->curr_pref >=0 && students[i]->curr_alloc == t->uid){
		        		Student * s = students[i];

		        		pthread_mutex_lock(&s->mutex);
						double probability = s->callibre * t->interest;

						bool finalises = (rand() % 100) < (probability * 100);

						if(finalises){
							// EVENT 5
				    		printf("Student %d has selected %s permanently\n", s->uid, t->name);
				    		s->curr_pref = -1;
						}
						else{
							// EVENT 3
				    		printf("Student %d has withdrawn from course %s\n", s->uid, t->name);

				    		if(s->curr_pref==s->pref_one){
				    			s->curr_alloc = -1;
				    			s->curr_pref = s->pref_two;
				    			// EVENT 4
				    			printf("Student %d has changed current preference from %s (priority 1) to %s (priority 2)\n", s->uid, courses[s->pref_one]->name, courses[s->pref_two]->name);
				    		}
				    		else if(s->curr_pref==s->pref_two){
				    			s->curr_alloc = -1;
				    			s->curr_pref = s->pref_three;
				    			// EVENT 4
				    			printf("Student %d has changed current preference from %s (priority 2) to %s (priority 3)\n", s->uid, courses[s->pref_two]->name, courses[s->pref_three]->name);
				    		}
				    		else{
				    			// EVENT 6
				    			printf("Student %d couldn't get any of his preferred courses\n", s->uid);
				    			s->curr_pref = -1;
				    		}
						}

		        		pthread_mutex_unlock(&s->mutex);
		        	}
		        }

	        } 
        }

        if(t->courseValid>0){
	        t->courseValid = 0;

	        for(int k = 0; k < t->p; k++){
	          if(iiit_labs[t->course_labs[k]]->ta_worthy!=2){
	          	t->courseValid = 1;
	          }
	        }
	    }

        if(t->courseValid <= 0){
        	if(t->courseValid == 0){
        		t->courseValid = -1;
		    	//EVENT 10
		    	printf("Course %s doesn't have any TA's eligible and is removed from course offerings\n", t->name);
			}
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
    t->tutorial = 0;
    t->tut_seats = 0;
    t->lab_allocated = -1;
    t->courseValid = 1;

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