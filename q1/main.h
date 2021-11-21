#include <pthread.h>

#ifndef __MAIN_H
#define __MAIN_H

typedef struct Student {
    int uid;

    double callibre;
    int pref_one;
    int pref_two;
    int pref_three;
    int arr_time;
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

    pthread_mutex_t mutex;
} Course;

typedef struct Lab {
    int uid;

   	char name[50];
   	int n_tas;
   	int max_times; 

   	int curr_times[50];
   	int availability[50];

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