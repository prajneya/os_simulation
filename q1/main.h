#include <pthread.h>

#ifndef __MAIN_H
#define __MAIN_H

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_WHITE   "\x1b[37m"
#define ANSI_COLOR_GREY    "\x1b[38m"
#define ANSI_COLOR_BRED    "\x1b[39m"
#define ANSI_COLOR_BGREEN  "\x1b[40m"
#define ANSI_COLOR_BYELLOW "\x1b[41m"
#define ANSI_COLOR_BBLUE   "\x1b[42m"
#define ANSI_COLOR_RESET   "\x1b[0m"

typedef struct Student {
    int uid;

    double callibre;
    int pref_one;
    int pref_two;
    int pref_three;
    int arr_time;

    int finalised;
    int curr_pref;
    int curr_alloc;

    pthread_mutex_t mutex;
} Student;

typedef struct Course {
    int uid;

   	char name[50];
   	double interest;
   	int course_max_slots; 
   	int p;
   	int course_labs[50];

   	int d;
   	int ta_allocated;
    int lab_allocated;
    int tutorial;
    int tut_seats;
    int courseValid;

    pthread_mutex_t mutex;
} Course;

typedef struct Lab {
    int uid;

   	char name[50];
   	int n_tas;
   	int max_times; 

   	int curr_times[50];
   	int availability[50];
    int ta_worthy;

    pthread_mutex_t mutex;
} Lab;

Student ** students;
Course ** courses;
Lab ** iiit_labs;

pthread_t * student_t,
			    * course_t,
			    * lab_t;

int S, // number of students
    L, // number of labs
    C; // number of courses

int randRange(int l, int r);

#endif // __MAIN_H