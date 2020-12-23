/************************************************
**Josiah Findley ~ COMP233.A ~ Spring 2020****
**
**Running Parrallel Pi using all the patterns we 
**have learned in OMP to create a beautiful piece
**of code that effectively shows the power of OMP
**
** PiSerial, PiSPMD, PiLoop, PiTask, and Picomp
** all come from Tim Mattsons OMP slides at 
** https://www.openmp.org/mp-documents/Intro_To_OpenMP_Mattson.pdf
*************************************************/

#define _CRT_SECURE_NO_WARNINGS //iso standard is good for us
#define MIN_BLK 10000000 // Min size block for tasks
#define M_PI 3.14159265358979323846 //Our value for pi

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

// Problem consants
const long NUM_INTERVALS = 300000000; //300 million sub-intervals
const int MAX_THREADS = 2;// num of threads
const int NUM_TEST = 4;// num of test being run

//Our Helper Methods
void PiSerial(long num_steps, float* timeNerror);
void PiSPMD(long num_steps, float* timeNerror);
void PiLoop(long num_steps, float* timeNerror);
void PiTask(long num_steps, float* timeNerror);
double pi_comp(int Nstart, int Nfinish, double step); //used in PiTask
void BestOfFive(void (*fPtr)(long, float*), float* bestTime,
    float* bestError, int testNum);


int main(void) {
	//Variable Dictionary
    float bestTimes[NUM_TEST]; //Holds the best times for tests
    float error[NUM_TEST];//Holds the best error for tests

	//1.0 Run the Test
 BestOfFive(PiSerial, bestTimes, error, 0); //First test
 BestOfFive(PiSPMD, bestTimes, error, 1); //Second test
 BestOfFive(PiLoop, bestTimes, error, 2); //Third test
 BestOfFive(PiTask, bestTimes, error, 3); //Fourth test

    // 2.0 Print Results

 //Header
 printf("Josiah Findley ~ COMP233.A ~ Spring 2020 ~\n");
 printf("Parallel Patterns with Pi\n\n");

 //Title
 printf("%20s %20s %20s %20s %20s\n", "Test", "RunTime",
     "SpeedUp", "Effeciency", "Error");

 //Results
 printf("%20s %20.2e %20s %20s %20.2e\n", "PiSerial", bestTimes[0],
     "", "", error[0]);
 printf("%20s %20.2e %20.2e %20.2f %20.2e\n", "PiSPMD", bestTimes[1],
     bestTimes[0] / bestTimes[1], bestTimes[0] / 
     bestTimes[1]/MAX_THREADS, error[1]);

 printf("%20s %20.2e %20.2e %20.2f %20.2e\n", "PiLoop", bestTimes[2],
     bestTimes[0] / bestTimes[2], bestTimes[0] / 
     bestTimes[2] / MAX_THREADS, error[2]);

 printf("%20s %20.2e %20.2e %20.2f %20.2e\n", "PiTask", bestTimes[3],
     bestTimes[0] / bestTimes[3], bestTimes[0] /
     bestTimes[3] / MAX_THREADS, error[3]);

 //Normal Termination
 printf("\n\n<<Normal Termination>>\n\n");

}


/*------------------------------------------------------------------
 * Function:    PiSerial
 * Purpose:     This function runs the pi program in serial
 * Input args: num_steps is the num intervals and timeNerror keeps
 * track of the run_time and error
 */

void PiSerial(long num_steps, float* timeNerror)
{
    //Variables
    int i; double x, pi, sum = 0.0; //Used to calc. pi
    double start_time, run_time; //Used to track the timings
    double step = 1.0 / (double)num_steps; //calc. step size for intervals

    start_time = omp_get_wtime(); //start time

    for (i = 0; i < num_steps; i++) { //runs through each step
        //calculates sum for the step
        x = (i + 0.5) * step;
        sum = sum + 4.0 / (1.0 + x * x);
    }

    // adds sum*stepsize to pi
    pi = step * sum;

    run_time = omp_get_wtime() - start_time; //total time

    //overwrites the run_time and pi approx
    timeNerror[0] = run_time; 
    timeNerror[1] = pi;

}/*PiSerial*/


/*------------------------------------------------------------------
 * Function:    SPMD
 * Purpose:     This function runs the pi program in parallel using
 * SPMD.
 * Input args: num_steps is the num intervals and timeNerror keeps
 * track of the run_time and error
 */

void PiSPMD(long num_steps, float* timeNerror)
{
    //Variables
    double start_time, run_time;//Used to track the timings
    //Used to calc. pi
    double pi = 0.0; double step = 1.0 / (double)num_steps; 
    omp_set_num_threads(MAX_THREADS); //Set num threads

    start_time = omp_get_wtime(); //starts the timer

#pragma omp parallel // Splits up the work
    {
        //private var for thread control and calc. pi
        int i, id, nthrds; double x, sum;
        //Initialization
        id = omp_get_thread_num();
        nthrds = omp_get_num_threads();
        //runs through each step
        for (i = id, sum = 0.0; i < num_steps; i = i + nthrds) {
            //calculates sum for the step
            x = (i + 0.5) * step;
            sum += 4.0 / (1.0 + x * x);
        }
#pragma omp critical //One thread at a time
        // adds sum*stepsize to pi
        pi += sum * step;
    }

    run_time = omp_get_wtime() - start_time;//total time

    //overwrites the run_time and pi approx
    timeNerror[0] = run_time;
    timeNerror[1] = pi;


}/**/

/*------------------------------------------------------------------
 * Function:    PiLoop
 * Purpose:     This function runs the pi program using the for 
 * function 
 * Input args: num_steps is the num intervals and timeNerror keeps
 * track of the run_time and error
 */

void PiLoop(long num_steps, float* timeNerror)
{
    //Variables
    int i; double pi, sum = 0.0;//Used to calc. pi
    double start_time, run_time;//Used to track the timings
    //initialize step
    double step;
    step = 1.0 / (double)num_steps;

    start_time = omp_get_wtime(); //start timer

    omp_set_num_threads(MAX_THREADS); //Set num threads

    // Splits up the work
#pragma omp parallel
    {
        //private var for calc. pi
        double x;
#pragma omp for reduction(+:sum)
        //runs through each step
        for (i = 0; i < num_steps; i++) {
            //calculates sum for the step
            x = (i + 0.5) * step;
            sum = sum + 4.0 / (1.0 + x * x);
        }
    }
    // adds sum*stepsize to pi
    pi = step * sum;

    run_time = omp_get_wtime() - start_time;//total time

    //overwrites the run_time and pi approx
    timeNerror[0] = run_time;
    timeNerror[1] = pi;
}/**/

/*------------------------------------------------------------------
 * Function:    PiTask
 * Purpose:     This function runs the Pi estimation in parallel 
 * using tasks.
 * Input args: num_steps is the num intervals and timeNerror keeps
 * track of the run_timeand error
 */

void PiTask(long num_steps, float* timeNerror)
{
    //Variable Dictionary
    double step, pi, sum; //Used to calc. pi
    double start_time, run_time; //Holds the times
    //initialize step
    step = 1.0 / (double)num_steps;

    start_time = omp_get_wtime(); //start time

    //Set the number of threads
    omp_set_num_threads(MAX_THREADS);

    // Splits up the work in a divide and conquer pattern
#pragma omp parallel
    {
#pragma omp single
        //calculates sum 
        sum = pi_comp(0, num_steps, step); 
    }
    // adds sum*stepsize to pi
    pi = step * sum;


    run_time = omp_get_wtime() - start_time;//total time

    //overwrites the run_time and pi approx
    timeNerror[0] = run_time;
    timeNerror[1] = pi;

}/*PiTask*/


/*------------------------------------------------------------------
 * Function:    pi_comp
 * Purpose:     This function split the problem into tasks and uses 
 * the divide and conquer technique.
 * Input args:  The size of the intervals and where they start 
 * and finish.
 */

double pi_comp(int Nstart, int Nfinish, double step)
{
    //Used to calc. pi
    int i, iblk;
    double x, sum = 0.0, sum1, sum2;
    //If task is small enough just do it
    if (Nfinish - Nstart < MIN_BLK) {
        for (i = Nstart; i < Nfinish; i++) {
            //calculates sum for the step
            x = (i + 0.5) * step;
            sum = sum + 4.0 / (1.0 + x * x);
        }
    }
    //Otherwise break it up recursively until small 
    //enough and add the sums together
    else {
        iblk = Nfinish - Nstart;
#pragma omp task shared(sum1)
        sum1 = pi_comp(Nstart, Nfinish - iblk / 2, step);
#pragma omp task shared(sum2)
        sum2 = pi_comp(Nfinish - iblk / 2, Nfinish, step);
#pragma omp taskwait
        sum = sum1 + sum2;
    }return sum;//return the sum
}/*pi_comp*/

/*------------------------------------------------------------------
 * Function:    BestOfFive
 * Purpose:     This function find the best time and error for a given
 * test after 5 trials
 * Input args: Takes a function, the bestTime and bestError to keep tabs
 * on the best times and Error and then also the test num to know where 
 * store the given test data.
 */

void BestOfFive(void (*fPtr)(long, float*), float* bestTime, 
    float* bestError, int testNum)
{
    //Variables
    float fastestTime = 1000.0; // fastest of the 5 times 
    float timeNerror[2];//Used to hold Time and Error
    float approx; //holds the error


    for (int k = 0; k < 5; k++) {//Run through 5 times

        (*fPtr)(NUM_INTERVALS, timeNerror); //runs specific test

        //finds fastest of 5 times
        if (timeNerror[0] < fastestTime)
        {
            //Update the best time and errors
            fastestTime = timeNerror[0];
            approx = timeNerror[1];
        }

    }

    //Update the best time and errors to main
    bestTime[testNum] = fastestTime;
    //Makes sure Error is positive
    if(M_PI>approx) bestError[testNum] = M_PI - approx;
    else bestError[testNum] = approx - M_PI;

}/*BestOfFive*/