#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>

#include "main.h"
#include "group.h"

void * personRunner(void * a) {
    Person * p = (Person *)a;
    sleep(p->arr_time);
    // SPEC EVENT 1
    printf("t= %d: %s has reached the stadium\n", p->arr_time, p->name);

    return 0;
}

void initGroup(int i) {
    Group * g = (Group *) malloc(sizeof(Group));

    g->uid = i;

    scanf("%d", &g->k);

    tot_persons += g->k;

    g->persons = (Person **) malloc(sizeof(Person *) * g->k);
    for(int j = 0; j < g->k; j++){
        g->persons[j] = (Person *) malloc(sizeof(Person));
        scanf("%s %c %d %d %d", g->persons[j]->name, &g->persons[j]->zone, &g->persons[j]->arr_time, &g->persons[j]->patience, &g->persons[j]->num_goals);
    }

    groups[i] = g;
}

void createPersonThreads(int i) {
    for(int j = 0; j<groups[i]->k; j++){
        pthread_create(&person_t[per_iter], 0, personRunner, groups[i]->persons[j]);
        per_iter++;
    }
}