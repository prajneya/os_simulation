# Question 2: The Classico Experience

Simulation of an football match taking place where particpants' movement and mood is tracked based on multithreading, concurrency and parallelism concepts coded in C.

## Implementation

For tackling this question, I maintain arrays of groups, teams and zone. The program behaves exactly as described in the problem pipeline. Each person and team is a thread.

No threads endlessly check for a condition, hence no busy waiting is there.

Since there are 2 teams (Home, and Away), two threads for each run which sleep for time intervals, wake up when they have to shoot a goal, shoot(or miss) according to random probabilities assigned to them and then sleep again.

Each person has a thread assigned to them, and tracks their arrival time. It sends them to a Zone according to availability and tracks spectating time. According to the Group array, BONUS IS IMPLEMENTED, where people wait for their friends to join in and go to dinner.

## Assumptions

* Large input is to be avoided, since a large number of threads cause a problem.
* The name of the person has to be limited to 50 characters.
* The total time within which goals can be scored is t=100

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
./q2
```

To run the output file to send input