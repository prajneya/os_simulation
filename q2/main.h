#include <pthread.h>

#ifndef __MAIN_H
#define __MAIN_H

typedef struct Zone{
    int uid;

    char name;
    int capacity;
} Zone;

typedef struct Person {
    int uid;

    char name[50];
    char fan;
    int arr_time;
    int patience;
    int num_goals;

    int atExit;
    int groupID;

} Person;

typedef struct Group {
    int uid;

    int k;
    Person ** persons;

    int left;
} Group;

typedef struct Team {
    int uid;

    int iter;
    int goals;

    char name;
    int goal_time[50];
    double chance[50];
} Team;

Group ** groups;
Team ** teams;
Zone ** zones;

pthread_t * person_t,
          * team_t;

int G,           // number of Groups
    X,           // spectate time
    c_H,         // capacity of Zone H
    c_A,         // capacity of Zone A
    c_N,         // capacity of Zone N
    C,           // Number of chances
    tot_persons, // Total number of persons
    per_iter;    // Iterator for persons thread

#endif // __MAIN_H