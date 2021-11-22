#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>

#include "main.h"
#include "group.h"
#include "team.h"

void init(){
	srand(time(0));

	per_iter = 0;

	groups = (Group **) malloc(sizeof(Group *) * G);

	for(int i = 0; i < G; i++){
		initGroup(i);
	}

	scanf("%d", &C);

	teams = (Team **) malloc(sizeof(Team *) * 3);
	for(int i = 0; i < 3; i++){
		Team * t = (Team *) malloc(sizeof(Team));
		t->uid = i;

		if(i==0){
			t->name = 'H';
		}
		else if(i==1){
			t->name = 'A';
		}
		else if(i==2){
			t->name = 'N';
		}

		t->iter = 0;
		t->goals = 0;

		teams[i] = t;
	}

	for(int i = 0; i < C; i++){
		initTeam();
	}

	person_t = (pthread_t *) malloc(sizeof(pthread_t) * tot_persons);

	for(int i = 0; i < G; i++){
		createPersonThreads(i);
	}

	team_t = (pthread_t *) malloc(sizeof(pthread_t) * 3);

	for(int i = 0; i < 3; i++){
		createTeamThreads(i);
	}
}

int main(){

	tot_persons = 0;

	scanf("%d %d %d %d %d", &c_H, &c_A, &c_N, &X, &G);
	init();

	for(int i = 0; i < tot_persons; i++)
        pthread_join(person_t[i], 0);

    for(int i = 0; i < 3; i++)
        pthread_join(team_t[i], 0);

    return 0;

}
