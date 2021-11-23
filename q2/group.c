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
    printf(ANSI_COLOR_RED "t=%d: %s has reached the stadium\n" ANSI_COLOR_RESET, p->arr_time, p->name);

    int wait_time = 0;
    int spectate_time = 0;

    if(p->fan=='H'){
        // wait till zones are empty
        while(zones[0]->capacity == 0 && zones[2]->capacity == 0 && wait_time<p->patience){
            wait_time++;
            sleep(1);
        }
        wait_time++;
        sleep(1);
        if(wait_time > p->patience){
            // SPEC EVENT 3
            printf(ANSI_COLOR_YELLOW "t=%d: %s could not get a seat\n" ANSI_COLOR_RESET, p->arr_time+wait_time, p->name);
            sleep(1);
            p->atExit = 1;
            // SPEC EVENT 6
            printf(ANSI_COLOR_BLUE "t=%d: %s is waiting for their friends at the exit\n" ANSI_COLOR_RESET, p->arr_time+wait_time+1, p->name);
        }
        else{
            // SPEC EVENT 2
            if(zones[0]->capacity >0){
                zones[0]->capacity--;
                p->curr_zone = 0;
                printf(ANSI_COLOR_YELLOW "t=%d: %s has got a seat in zone H\n" ANSI_COLOR_RESET, p->arr_time+wait_time, p->name);
            }
            else if(zones[2]->capacity >0){
                zones[2]->capacity--;
                p->curr_zone = 2;
                printf(ANSI_COLOR_YELLOW "t=%d: %s has got a seat in zone N\n" ANSI_COLOR_RESET, p->arr_time+wait_time, p->name);
            }

            while(teams[1]->goals < p->num_goals && spectate_time < X){
                spectate_time++;
                sleep(1);
            }
            // SPEC EVENT 5
            if(teams[1]->goals >= p->num_goals){
                zones[p->curr_zone]++;
                printf(ANSI_COLOR_MAGENTA "t=%d: %s is leaving due to bad performance of his team\n" ANSI_COLOR_RESET, p->arr_time+wait_time+spectate_time, p->name);
                sleep(1);
                p->atExit = 1;
                // SPEC EVENT 6
                printf(ANSI_COLOR_BLUE "t=%d: %s is waiting for their friends at the exit\n" ANSI_COLOR_RESET, p->arr_time+wait_time+spectate_time+1, p->name);
            }
            // SPEC EVENT 4
            else if(spectate_time>=X){
                zones[p->curr_zone]++;
                printf(ANSI_COLOR_YELLOW "t=%d: %s watched the match for %d seconds and is leaving\n" ANSI_COLOR_RESET, p->arr_time+wait_time+spectate_time, p->name, X);
                sleep(1);
                p->atExit = 1;
                // SPEC EVENT 6
                printf(ANSI_COLOR_BLUE "t=%d: %s is waiting for their friends at the exit\n" ANSI_COLOR_RESET, p->arr_time+wait_time+spectate_time+1, p->name);
            }

        }
    }
    else if(p->fan=='A'){
        // wait till zones are empty
        while(zones[1]->capacity == 0 && wait_time<p->patience){
            wait_time++;
            sleep(1);
        }
        wait_time++;
        sleep(1);
        if(wait_time > p->patience){
            // SPEC EVENT 3
            printf(ANSI_COLOR_YELLOW "t=%d: %s could not get a seat\n" ANSI_COLOR_RESET, p->arr_time+wait_time, p->name);
            sleep(1);
            // SPEC EVENT 6
            printf(ANSI_COLOR_BLUE "t=%d: %s is waiting for their friends at the exit\n" ANSI_COLOR_RESET, p->arr_time+wait_time+1, p->name);
        }
        else{
            // SPEC EVENT 2
            if(zones[1]->capacity >0){
                zones[1]->capacity--;
                p->curr_zone = 1;
                printf(ANSI_COLOR_YELLOW "t=%d: %s has got a seat in zone A\n" ANSI_COLOR_RESET, p->arr_time+wait_time, p->name);
            }

            while(teams[0]->goals < p->num_goals && spectate_time < X){
                spectate_time++;
                sleep(1);
            }
            // SPEC EVENT 5
            if(teams[0]->goals >= p->num_goals){
                zones[p->curr_zone]++;
                printf(ANSI_COLOR_MAGENTA "t=%d: %s is leaving due to bad performance of his team\n" ANSI_COLOR_RESET, p->arr_time+wait_time+spectate_time, p->name);
                sleep(1);
                p->atExit = 1;
                // SPEC EVENT 6
                printf(ANSI_COLOR_BLUE "t=%d: %s is waiting for their friends at the exit\n" ANSI_COLOR_RESET, p->arr_time+wait_time+spectate_time+1, p->name);
            }
            // SPEC EVENT 4
            else if(spectate_time>=X){
                zones[p->curr_zone]++;
                printf(ANSI_COLOR_CYAN "t=%d: %s watched the match for %d seconds and is leaving\n" ANSI_COLOR_RESET, p->arr_time+wait_time+spectate_time, p->name, X);
                sleep(1);
                p->atExit = 1;
                // SPEC EVENT 6
                printf(ANSI_COLOR_BLUE "t=%d: %s is waiting for their friends at the exit\n" ANSI_COLOR_RESET, p->arr_time+wait_time+spectate_time+1, p->name);
            }
        }
    }
    if(p->fan=='N'){
        // wait till zones are empty
        while(zones[0]->capacity == 0 && zones[1]->capacity == 0 && zones[2]->capacity == 0 && wait_time<p->patience){
            wait_time++;
            sleep(1);
        }
        wait_time++;
        sleep(1);
        if(wait_time > p->patience){
            // SPEC EVENT 3
            printf(ANSI_COLOR_YELLOW "t=%d: %s could not get a seat\n" ANSI_COLOR_RESET, p->arr_time+wait_time, p->name);
            sleep(1);
            p->atExit = 1;
            // SPEC EVENT 6
            printf(ANSI_COLOR_BLUE "t=%d: %s is waiting for their friends at the exit\n" ANSI_COLOR_RESET, p->arr_time+wait_time+1, p->name);
        }
        else{
            // SPEC EVENT 2
            if(zones[0]->capacity >0){
                zones[0]->capacity--;
                p->curr_zone = 0;
                printf(ANSI_COLOR_YELLOW "t=%d: %s has got a seat in zone H\n" ANSI_COLOR_RESET, p->arr_time+wait_time, p->name);
            }
            else if(zones[1]->capacity >0){
                zones[1]->capacity--;
                p->curr_zone = 1;
                printf(ANSI_COLOR_YELLOW "t=%d: %s has got a seat in zone A\n" ANSI_COLOR_RESET, p->arr_time+wait_time, p->name);
            }
            else if(zones[2]->capacity >0){
                zones[2]->capacity--;
                p->curr_zone = 2;
                printf(ANSI_COLOR_YELLOW "t=%d: %s has got a seat in zone N\n" ANSI_COLOR_RESET, p->arr_time+wait_time, p->name);
            }

            while(spectate_time < X){
                spectate_time++;
                sleep(1);
            }
            // SPEC EVENT 4
            if(spectate_time>=X){
                zones[p->curr_zone]++;
                printf(ANSI_COLOR_CYAN "t=%d: %s watched the match for %d seconds and is leaving\n" ANSI_COLOR_RESET, p->arr_time+wait_time+spectate_time, p->name, X);
                sleep(1);
                p->atExit = 1;
                // SPEC EVENT 6
                printf(ANSI_COLOR_BLUE "t=%d: %s is waiting for their friends at the exit\n" ANSI_COLOR_RESET, p->arr_time+wait_time+spectate_time+1, p->name);
            }
        }
    }

    if(p->atExit){
        bool waitForFriends = false;

        for(int i = 0; i < groups[p->groupID]->k; i++){
            if(!groups[p->groupID]->persons[i]->atExit){
                waitForFriends = true;
                break;
            }
        }

        if(!waitForFriends && groups[p->groupID]->left!=1){
            // BONUS GROUP EVENT
            groups[p->groupID]->left = 1;
            printf("t=%d: Group %d is leaving for dinner\n", p->arr_time+wait_time+spectate_time+1, p->groupID);
        }
    }

    return 0;
}

void initGroup(int i) {
    Group * g = (Group *) malloc(sizeof(Group));

    g->uid = i;
    g->left = 0;

    scanf("%d", &g->k);

    tot_persons += g->k;

    g->persons = (Person **) malloc(sizeof(Person *) * g->k);
    for(int j = 0; j < g->k; j++){
        g->persons[j] = (Person *) malloc(sizeof(Person));
        scanf("%s %c %d %d %d", g->persons[j]->name, &g->persons[j]->fan, &g->persons[j]->arr_time, &g->persons[j]->patience, &g->persons[j]->num_goals);
        g->persons[j]->atExit = 0;
        g->persons[j]->groupID = i;
        g->persons[j]->curr_zone = -1;
    }

    groups[i] = g;
}

void createPersonThreads(int i) {
    for(int j = 0; j<groups[i]->k; j++){
        pthread_create(&person_t[per_iter], 0, personRunner, groups[i]->persons[j]);
        per_iter++;
    }
}