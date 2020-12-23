/************************************************
**Josiah Findley ~ COMP233.A ~ Spring 2020****
**
**Running a Monte Carlo Simulation with circuits.
**We want to find the probability of a specific 
**working and we are running this in serial and 
**parallel to show the power of parallel computing.
*************************************************/

#define _CRT_SECURE_NO_WARNINGS //iso standard is good for us

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

// Problem consants
const int MAX = 1 << 28;


int main(void) {
	//Variable Dictionary
	const int ThreadCount = 2;// num of threads

	// hold the serial and parallel num_working
	int working[25];
	int workingP[25];

	//used for randomly testing on or off
	int numA, numB, numC, numD, numE, numF, numG;

	//how reliable is circuit
	int reliabilityA, reliabilityB, reliabilityC, reliabilityD,
		reliabilityE, reliabilityF, reliabilityG;

	// State of the Device
	int deviceA, deviceB, deviceC, deviceD, deviceE, deviceF, deviceG;
	int num_working;//number of working circuits

	double startTime, stopTime;//used for timing serial
	double startTimeP, stopTimeP;//used for timing parallel
	unsigned int timeOfDay = time(NULL); //std time of day

	//1.0 Initialize
	srand(timeOfDay);//seed the rng normally

#pragma omp parallel num_threads(ThreadCount)
	{
		//seed each thread uniquely 
		srand((unsigned int)time(NULL) + 4 * omp_get_thread_num());
	}

	//init reliabilities
	reliabilityA = 98;
	reliabilityB = 92;
	reliabilityC = 89;
	reliabilityD = 91;
	reliabilityE = 93;
	reliabilityF = 88;
	reliabilityG = 90;


	//2.0 Test circuits Process

	/*********************Serial***************************/

	startTime = omp_get_wtime(); // start timer 

	//Start with a Trial of 16 tests, then 32 tests...2^28 tests
	for (int numTest = 16; numTest <= MAX; numTest *= 2) {
		num_working = 0; //start with 0 circuits working
		//Run numTest number of times
		for (int i = 0; i < numTest; i++) {
			//Start with all device 'false'
			deviceA = 0;
			deviceB = 0;
			deviceC = 0;
			deviceD = 0;
			deviceE = 0;
			deviceF = 0;
			deviceG = 0;

			//Random number betw. 0-99
			numA = rand() % 100;
			numB = rand() % 100;
			numC = rand() % 100;
			numD = rand() % 100;
			numE = rand() % 100;
			numF = rand() % 100;
			numG = rand() % 100;

			//check whether device is working
			if (numA < reliabilityA) {
				deviceA = 1;
			}
			if (numB < reliabilityB) {
				deviceB = 1;
			}
			if (numC < reliabilityC) {
				deviceC = 1;
			}
			if (numD < reliabilityD) {
				deviceD = 1;
			}
			if (numE < reliabilityE) {
				deviceE = 1;
			}
			if (numF < reliabilityF) {
				deviceF = 1;
			}
			if (numG < reliabilityG) {
				deviceG = 1;
			}

			//checks to see if circuit is working
			if ((deviceA && (deviceB || deviceC)) || ((deviceD 
				|| deviceE) && (deviceF || deviceG))) {
				num_working++;
			}


		}
		//updates number of working circuits
		working[(int)(log(numTest) / log(2) - 4)] = num_working;
	}


	stopTime = omp_get_wtime(); // stop timer 




	/*********************Parallel***************************/

	startTimeP = omp_get_wtime(); // start timer parallel

		//Start with a Trial of 16 tests, then 32 tests...2^28 tests
	for (int numTest = 16; numTest <= MAX; numTest *= 2) {
		num_working = 0; //start with 0 circuits working

		//Fork a team of threads - reliability already initialized
#pragma omp parallel num_threads(ThreadCount) \
	shared(timeOfDay) firstprivate(reliabilityA, reliabilityB, \
	reliabilityC, reliabilityD, reliabilityE, reliabilityF, reliabilityG)\
	private(deviceA, deviceB, deviceC, deviceD, deviceE, deviceF, deviceG\
	,numA, numB, numC, numD, numE, numF, numG) 

		{
			int myId = omp_get_thread_num();//Who am I?
			//How many threads given
			int numthreads = omp_get_num_threads();

			//Run numTest number of times
			for (int i = myId; i < numTest; i+=numthreads) {
				//Start with all device 'false'
				deviceA = 0;
				deviceB = 0;
				deviceC = 0;
				deviceD = 0;
				deviceE = 0;
				deviceF = 0;
				deviceG = 0;

				//Random number betw. 0-99
				numA = rand() % 100;
				numB = rand() % 100;
				numC = rand() % 100;
				numD = rand() % 100;
				numE = rand() % 100;
				numF = rand() % 100;
				numG = rand() % 100;

				//check whether device is working
				if (numA < reliabilityA) {
					deviceA = 1;
				}
				if (numB < reliabilityB) {
					deviceB = 1;
				}
				if (numC < reliabilityC) {
					deviceC = 1;
				}
				if (numD < reliabilityD) {
					deviceD = 1;
				}
				if (numE < reliabilityE) {
					deviceE = 1;
				}
				if (numF < reliabilityF) {
					deviceF = 1;
				}
				if (numG < reliabilityG) {
					deviceG = 1;
				}


				//checks to see if circuit is working
				if ((deviceA && (deviceB || deviceC)) || 
					((deviceD || deviceE) && (deviceF || deviceG))) {
#pragma omp atomic //avoids race condition
					num_working++;
				}
			}
		}
		//updates number of working circuit
		workingP[(int)(log(numTest) / log(2) - 4)] = num_working;
	
	}

	stopTimeP = omp_get_wtime(); // stop timer parallel


	// 3.0 Print Results

	//Header
	printf("Josiah Findley ~ COMP233.A ~ Spring 2020 ~");
	printf(" Circuit Reliability\n\n");

	/*********************Serial***************************/
	printf("Monte Carlo Similation in Serial \n");

	printf("%20s %20s %20s\n", "Tests", "# Worked",
		"SuccessRate");

	//print #test and #of working cycles and percentages
	for (int i = 16; i <= MAX; i *= 2) {
		printf("%20d %20d %20.5f %%\n", i,
			working[(int)(log(i) / log(2) - 4)],
			(float)100 * working[(int)(log(i) / log(2) - 4)] / i);
	}

	//Serial time
	printf("\nElapsed time for serial is %.3e sec\n",
		stopTime - startTime);


	/*********************Parallel***************************/
	printf("\n\nMonte Carlo Similation in Parallel (2 threads) \n");

	printf("%20s %20s %20s", "Tests", "# Worked",
		"SuccessRate\n");

	//print #test and #of working cycles and percentages
	for (int i = 16; i <= MAX; i *= 2) {
		printf("%20d %20d %20.5f %%\n", i,
			workingP[(int)(log(i) / log(2) - 4)],
			(float)100 * workingP[((int)(log(i) / log(2) - 4))] / i);
	}
	//Parallel time
	printf("\nElapsed time for parallel is %.3e sec\n",
		stopTimeP - startTimeP);


	//Speed up and effeciency
	printf("\nSpeed up is %.2e\n", (stopTime - startTime)/
		(stopTimeP - startTimeP));
	printf("Effeciency is %.3f %%\n", (stopTime - startTime) /
		(stopTimeP - startTimeP)/ThreadCount*100);

	printf("\n\n<<Normal Termination>>\n\n");

	return 0;
}








