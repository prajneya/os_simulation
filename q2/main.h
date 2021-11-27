#include <pthread.h>

#ifndef __MAIN_H
#define __MAIN_H

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

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
    int curr_zone;

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
    int goal_time[100];
    double chance[100];
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