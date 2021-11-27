#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>

#include "main.h"
#include "team.h"

void * teamRunner(void * a) {
    Team * t = (Team *)a;

    int wait_time = 0;

    for(int i = 0; i < t->iter; i++){
    	sleep(t->goal_time[i] - wait_time);

        wait_time += t->goal_time[i];

    	bool scores = (rand() % 100) < (t->chance[i] * 100);

    	if(scores){
    		t->goals++;
    		// TEAM EVENT 1
    		if(t->goals==1){
    			printf(ANSI_COLOR_GREEN "t=%d: Team %c have scored their %dst goal\n" ANSI_COLOR_RESET, t->goal_time[i], t->name, t->goals);
    		}
    		else if(t->goals==2){
    			printf(ANSI_COLOR_GREEN "t=%d: Team %c have scored their %dnd goal\n" ANSI_COLOR_RESET, t->goal_time[i], t->name, t->goals);
    		}
    		else if(t->goals==3){
    			printf(ANSI_COLOR_GREEN "t=%d: Team %c have scored their %drd goal\n" ANSI_COLOR_RESET, t->goal_time[i], t->name, t->goals);
    		}
    		else if(t->goals>3){
    			printf(ANSI_COLOR_GREEN "t=%d: Team %c have scored their %dth goal\n" ANSI_COLOR_RESET, t->goal_time[i], t->name, t->goals);
    		}
    	}
    	else{
    		// TEAM EVENT 2
    		if(t->goals+1==1){
    			printf(ANSI_COLOR_GREEN "t=%d: Team %c missed the chance to score their %dst goal\n" ANSI_COLOR_RESET, t->goal_time[i], t->name, t->goals+1);
    		}
    		else if(t->goals+1==2){
    			printf(ANSI_COLOR_GREEN "t=%d: Team %c missed the chance to score their %dnd goal\n" ANSI_COLOR_RESET, t->goal_time[i], t->name, t->goals+1);
    		}
    		else if(t->goals+1==3){
    			printf(ANSI_COLOR_GREEN "t=%d: Team %c missed the chance to score their %drd goal\n" ANSI_COLOR_RESET, t->goal_time[i], t->name, t->goals+1);
    		}
    		else if(t->goals+1>3){
    			printf(ANSI_COLOR_GREEN "t=%d: Team %c missed the chance to score their %dth goal\n" ANSI_COLOR_RESET, t->goal_time[i], t->name, t->goals+1);
    		}
    	}
    }
    

    return 0;
}

void initTeam() {

	char t_name;
	int t_time;
	double t_prob;

    scanf(" %c %d %lf", &t_name, &t_time, &t_prob);

    if(t_name=='H'){
    	teams[0]->goal_time[teams[0]->iter] = t_time;
    	teams[0]->chance[teams[0]->iter] = t_prob;
    	teams[0]->iter++;
    }
    else if(t_name=='A'){
    	teams[1]->goal_time[teams[1]->iter] = t_time;
    	teams[1]->chance[teams[1]->iter] = t_prob;
    	teams[1]->iter++;
    }
}

void createTeamThreads(int i) {
    pthread_create(&team_t[i], 0, teamRunner, teams[i]);
}