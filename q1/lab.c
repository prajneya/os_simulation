#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "main.h"
#include "lab.h"

void * labRunner(void * a) {
    Lab * l = (Lab *)a;
    while(l->ta_worthy!=2) {
      pthread_mutex_lock(&l->mutex);
      l->ta_worthy = 0;
      
      for(int i = 0; i < l->n_tas; i++){
        if(l->curr_times[i] < l->max_times){
          l->ta_worthy = 1;
        }
      }

      // EVENT 12
      if(!l->ta_worthy){
        l->ta_worthy = 2;
        printf(ANSI_COLOR_BBLUE "Lab %s no longer has students available for TA ship\n" ANSI_COLOR_RESET, l->name);
      }
      pthread_mutex_unlock(&l->mutex);
    }

    return 0;
}

void initLab(int i) {
    Lab * l = (Lab *) malloc(sizeof(Lab));

    l->uid = i;
    l->ta_worthy = 0;

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