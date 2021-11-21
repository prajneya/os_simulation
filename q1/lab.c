#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "main.h"
#include "lab.h"

void * labRunner(void * a) {
    Lab * l = (Lab *)a;

    while(1) {
      for(int i = 0; i < l->n_tas; i++){
        // Check if TAs are available
        if(l->availability[i] == 1 && l->curr_times[i] < l->max_times){
          for(int j = 0; j < C; j++) {
            pthread_mutex_lock(&courses[j]->mutex);

            bool accepted = false;

            for(int k = 0; k < courses[j]->p; k++){
              if(courses[j]->course_labs[k] == l->uid){
                accepted = true;
                break;
              }
            }

            if(!accepted){
              pthread_mutex_unlock(&courses[j]->mutex);
              break;
            }

            if(courses[j]->ta_allocated == -1){
              courses[j]->ta_allocated = i;

              //suffix for TASHIP
              if(l->curr_times[i] == 0){
                printf("TA %d from lab %s has been allocated to course %s for his %dst TA ship\n", i, l->name, courses[j]->name, l->curr_times[i]+1);
              }
              else if(l->curr_times[i] == 0){
                printf("TA %d from lab %s has been allocated to course %s for his %dnd TA ship\n", i, l->name, courses[j]->name, l->curr_times[i]+1);
              }
              else if(l->curr_times[i] == 0){
                printf("TA %d from lab %s has been allocated to course %s for his %drd TA ship\n", i, l->name, courses[j]->name, l->curr_times[i]+1);
              }
              else{
                printf("TA %d from lab %s has been allocated to course %s for his %dth TA ship\n", i, l->name, courses[j]->name, l->curr_times[i]+1);
              }
            }

            pthread_mutex_unlock(&courses[j]->mutex);
          }
        }
      }
    }

    return 0;
}

void initLab(int i) {
    Lab * l = (Lab *) malloc(sizeof(Lab));

    l->uid = i;

   	scanf("%s %d %d", l->name, &l->n_tas, &l->max_times);

    for(int j = 0; j < l->n_tas; j++){
      l->curr_times[j] = 0;
      l->availability[j] = 1;
    }

    pthread_mutex_init(&l->mutex, 0);

    iiit_labs[i] = l;
}

void createLabThreads(int i) {
	pthread_create(&lab_t[i], 0, labRunner, iiit_labs[i]); 
}