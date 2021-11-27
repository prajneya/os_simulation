# Question 1: Alternate Course Allocation Portal

Implementation of an alternate course allocation portal for students based on multithreading, concurrency and parallelism concepts coded in C.

## Implmentation

For tackling this question, I maintain arrays of students, courses and labs. The program behaves exactly as described in the problem pipeline. Each student, course and lab is a thread.

Labs endlessly check whether they have TAs available for courses or not. A lab's thread exits when there are no worthy TAs available for any course. 
Courses endlessly check for available TAs, and if any TA is found available, based on thei equal opportunities scheme (BONUS), a TA is allocated to the course. As soon as the TA is allocated, the course is allocated a random number of seats and a tutorial is conducted if more than 0 student(s) have their current preference as the said course. Once the tutorial ends (after a sleep of 5 seconds), TA leaves the course and students selects/withdraws the course to either permanently lock their choice or to move on to their next preference. A final check is made to see if there are any labs available for the said course, and if there isn't thread exits.
The preference of Students is filled in the student thread without checking for any condition (no busy waiting),and while the student does not withdraw/selects a course permanently, course is allocated to the student.

Synchronization is achieved by the use of mutex locks. Each student, lab and course has it's own mutex lock.

Students iterate over all courses (after acquiring the lock for the same).

Courses iterate over all the valid labs (after acquiring the lock for the same).

Labs only check for their TAs' availability.

## Assumptions

* Large input is to be avoided, since a large number of threads cause a problem.
* Names of the courses have to be limited to 50 characters
* Names of the labs have have to be limited to 50 characters
* Number of labs designated for a course has to be limited to 50
* Number of TAs in a lab has to be limited to 50

## Usage

```bash
make clean
```

To remove all unnecessary output files

```bash
make
```

To compile all the required files

```bash
./q1
```

To run the output file to send input