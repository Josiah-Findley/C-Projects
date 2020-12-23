/************************************************
**Josiah Findley ~ COMP233.A ~ Spring 2020****
**
**Sorting bubble sort, odd even, and comb sort
*************************************************/

#define _CRT_SECURE_NO_WARNINGS //iso standard is good for us

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

//Our problem constants
int const Array_Size = 32767;  //number of data points

void verify(const int arr[], int a);    /* Verifying array */
void SortTimes(double times[], double time); //Sorts the times

int main(void) {
	//Variable Dictionary
	// array of 32767 integers
	int* array = (int*)malloc(Array_Size * sizeof(int));
	const int ROWS = 6, COLS = 5;// hold the times
	double* times[ROWS];//used for times
	for (int i = 0; i < ROWS; i++)
		times[i] = (double*)malloc(COLS * sizeof(double));
	int a, b, c; //used for randomly shuffling array
	int i, tmp, phase; //used for odd even sort
	int check = 0; //used for verification
	double startTime, stopTime;
	int lastSwapped, temp; //used to store temporary values for bubblesort
	int n;//control variable for bubble
	int gap, sorted; //used for comb sort
	float shrink; //used for comb sort

	// All the copy arrays
	int* array1 = (int*)malloc(Array_Size * sizeof(int));
	int* array2 = (int*)malloc(Array_Size * sizeof(int));
	int* array3 = (int*)malloc(Array_Size * sizeof(int));
	int* array4 = (int*)malloc(Array_Size * sizeof(int));
	int* array5 = (int*)malloc(Array_Size * sizeof(int));
	int* array6 = (int*)malloc(Array_Size * sizeof(int));

	//1.0 Initialize
	for (int i = 0; i < Array_Size; i++)
	{
		array[i] = i; //fill the array with value of index
	}


	//seeds the random number generator
	srand(time(0));

	//Shuffle array
	for (int i = 0; i < Array_Size; i++)
	{
		a = rand() % Array_Size; //first random number
		b = rand() % Array_Size; //second random number
		c = array[a]; // save stuff in array[a]
		array[a] = array[b]; //swap stuff of a and stuff of b
		array[b] = c;
	}

	//initialize times array
	for (int i = 0; i < ROWS; i++) {
		for (int j = 0; j < COLS; j++) {
			times[i][j] = 0.0;
		}
	}



	//2.0 Process


	for (int k = 0; k < 5; k++) {

		//copy over all the master data to the copy arrays
		for (int i = 0; i < Array_Size; i++) {
			array1[i] = array[i];
			array2[i] = array[i];
			array3[i] = array[i];
			array4[i] = array[i];
			array5[i] = array[i];
			array6[i] = array[i];
		}


		/*********************Bubble Sort***************************/

		startTime = omp_get_wtime(); // start timer 

		do {
			lastSwapped = 0;
			// passes through the array
			for (i = 1; i <= Array_Size - 1; i++) {
				if (array1[i - 1] > (array1[i])) { // checks the order
					// swaps the larger with the smaller
					temp = array1[i - 1];
					array1[i - 1] = array1[i];
					array1[i] = temp;
					lastSwapped = i;
				}
			}
			n = lastSwapped;//updates n
		} while (n > 1);

		stopTime = omp_get_wtime(); // end timer


		// Sorting the times in ascending order
		SortTimes(times[0], stopTime - startTime);

		//Verify odd even
		if (k == 0) { // only once
			verify(array1, 1);
		}


		/***************************Odd Even Sort******************************/


				/********************* 2 Threads ***************************/

		startTime = omp_get_wtime(); // start timer 

# pragma omp parallel num threads(2) \
default(none) shared(array2, Array_Size) private(i, tmp, phase)//fork team
		for (phase = 0; phase < Array_Size; phase++) {
			if (phase % 2 == 0) //if even
# pragma omp for
				for (i = 1; i < Array_Size; i += 2) {
					if (array2[i - 1] > array2[i]) {// checks the order
						// swaps the larger with the smaller
						tmp = array2[i - 1];
						array2[i - 1] = array2[i];
						array2[i] = tmp;
					}
				}
			else //if odd
# pragma omp for
				for (i = 1; i < Array_Size - 1; i += 2) {
					if (array2[i] > array2[i + 1]) {// checks the order
						// swaps the larger with the smaller
						tmp = array2[i + 1];
						array2[i + 1] = array2[i];
						array2[i] = tmp;
					}
				}
		}

		stopTime = omp_get_wtime(); // end timer


		// Sorting the times in ascending order
		SortTimes(times[1], stopTime - startTime);

		//Verify odd even
		if (k == 0) { // only once
			verify(array2, 2);
		}



		/********************* 4 Threads ***************************/
		startTime = omp_get_wtime(); // start timer 

# pragma omp parallel num threads(4) \
default(none) shared(array3, Array_Size) private(i, tmp, phase)//fork team
		for (phase = 0; phase < Array_Size; phase++) {
			if (phase % 2 == 0) //if even
# pragma omp for
				for (i = 1; i < Array_Size; i += 2) {
					if (array3[i - 1] > array3[i]) {// checks the order
						// swaps the larger with the smaller
						tmp = array3[i - 1];
						array3[i - 1] = array3[i];
						array3[i] = tmp;
					}
				}
			else //if odd
# pragma omp for
				for (i = 1; i < Array_Size - 1; i += 2) {
					if (array3[i] > array3[i + 1]) {// checks the order
						// swaps the larger with the smaller
						tmp = array3[i + 1];
						array3[i + 1] = array3[i];
						array3[i] = tmp;
					}
				}
		}

		stopTime = omp_get_wtime(); // end timer


		// Sorting the times in ascending order
		SortTimes(times[2], stopTime - startTime);

		//Verify odd even
		if (k == 0) { // only once
			verify(array3, 3);
		}


		/*********************Comb Sort Serial***************************/

		startTime = omp_get_wtime(); // start timer

		gap = Array_Size; // Initialize gap size
		shrink = 1.3; // Set the gap shrink factor
		sorted = 0;

		while (sorted == 0) {
			// Update the gap value for a next comb
			gap = floor(gap / shrink);
			if (gap <= 1) {
				gap = 1;
				sorted = 1; //If there are no swaps this pass, we are done
			}

			// A single "comb" over the input list
			for (int i = 0; i < Array_Size - gap; i++) {
				if (array4[i] > array4[i + gap]) {
					// swaps the larger with the smaller
					tmp = array4[i + gap];
					array4[i + gap] = array4[i];
					array4[i] = tmp;
					sorted = 0;
				}

			}


		}

		stopTime = omp_get_wtime(); // end timer

		// Sorting the times in ascending order
		SortTimes(times[3], stopTime - startTime);

		//Verify odd even
		if (k == 0) { // only once
			verify(array4, 4);
		}



		/*********************Comb Sort Parallel*************************/

			/*********************Two Threads***************************/


		startTime = omp_get_wtime(); // start timer

		gap = Array_Size; // Initialize gap size
		shrink = 1.3; // Set the gap shrink factor
		sorted = 0;

		while (sorted == 0) {
			// Update the gap value for a next comb
			gap = floor(gap / shrink);
			if (gap <= 1) {
				gap = 1;
				sorted = 1; //If there are no swaps this pass, we are done
			}

			if (gap > 0.5 * Array_Size) {
				// A single "comb" over the input list
# pragma omp num_threads(2)
				for (int i = 0; i < Array_Size - gap; i++) {
					if (array5[i] > array5[i + gap]) {
						// swaps the larger with the smaller
						tmp = array5[i + gap];
						array5[i + gap] = array5[i];
						array5[i] = tmp;
						sorted = 0;
					}

				}
			}
			else {

# pragma omp num_threads(2)
				{	//fork a team of threads
					//SPMD - share work across thread team by chunks
					int myID = omp_get_thread_num();
					int numThreads = omp_get_num_threads();

					//Go through till gap 
					for (int t = myID; t < gap; t += numThreads) {
						//Go through the given chain
						for (int i = t; i < Array_Size; i += gap) {
							if (i + gap< Array_Size && array5[i] > array5
								[i + gap]) {
								// swaps the larger with the smaller
								tmp = array5[i + gap];
								array5[i + gap] = array5[i];
								array5[i] = tmp;
								sorted = 0;
							}

						}
					}
				}//parallel region



			}


		}

		stopTime = omp_get_wtime(); // end timer


		// sorting the times in ascending order
		SortTimes(times[4], stopTime - startTime);

		//verify odd even
		if (k == 0) { // only once
			verify(array5, 5);
		}


		/*********************Four Threads***************************/


		startTime = omp_get_wtime(); // start timer

		gap = Array_Size; // Initialize gap size
		shrink = 1.3; // Set the gap shrink factor
		sorted = 0;

		while (sorted == 0) {
			// Update the gap value for a next comb
			gap = floor(gap / shrink);
			if (gap <= 1) {
				gap = 1;
				sorted = 1; //If there are no swaps this pass, we are done
			}

			if (gap > 0.5 * Array_Size) {
				// A single "comb" over the input list
# pragma omp num_threads(4)
				for (int i = 0; i < Array_Size - gap; i++) {
					if (array6[i] > array6[i + gap]) {
						// swaps the larger with the smaller
						tmp = array6[i + gap];
						array6[i + gap] = array6[i];
						array6[i] = tmp;
						sorted = 0;
					}

				}
			}
			else {

# pragma omp num_threads(4)
				{	//fork a team of threads
					//SPMD - share work across thread team by chunks
					int myID = omp_get_thread_num();
					int numThreads = omp_get_num_threads();

					//Go through till gap 
					for (int t = myID; t < gap; t += numThreads) {
						//Go through the given chain
						for (int i = t; i < Array_Size; i += gap) {
							if (i + gap< Array_Size && array6[i] >
								array6[i + gap]) {
								// swaps the larger with the smaller
								tmp = array6[i + gap];
								array6[i + gap] = array6[i];
								array6[i] = tmp;
								sorted = 0;
							}

						}
					}
				}//parallel region

			}

		}

		stopTime = omp_get_wtime(); // end timer



		// sorting the times in ascending order
		SortTimes(times[5], stopTime - startTime);

		//verify 
		if (k == 0) { // only once
			verify(array6, 6);
		}

	}



	//3.0 Print

	//Table of times
	printf("%20s %20s %20s %20s %20s\n\n", "Sorting Algorithm", "Best Time",
		"Worst Time", "Avg Time", "Thread Count");

	printf("%20s %20.2e %20.2e %20.2e %20s\n", "Bubble Sort",
		times[0][4], times[0][0], (times[0][0] + times[0][1] +
			times[0][2] + times[0][3] + times[0][4]) / 5, "");

	printf("%20s %20.2e %20.2e %20.2e %20s\n", "Odd Even Sort",
		times[1][4], times[1][0], (times[1][0] + times[1][1] +
			times[1][2] + times[1][3] + times[1][4]) / 5, "2 Threads");

	printf("%20s %20.2e %20.2e %20.2e %20s\n", "Odd Even Sort",
		times[2][4], times[2][0], (times[2][0] + times[2][1] +
			times[2][2] + times[2][3] + times[2][4]) / 5, "4 Threads");

	printf("%20s %20.2e %20.2e %20.2e %20s\n", "Comb Sort Serial",
		times[3][4], times[3][0], (times[3][0] + times[3][1] +
			times[3][2] + times[3][3] + times[3][4]) / 5, "");

	printf("%20s %20.2e %20.2e %20.2e %20s\n", "Comb Sort Parallel",
		times[4][4], times[4][0], (times[4][0] + times[4][1] +
			times[4][2] + times[4][3] + times[4][4]) / 5, "2 Threads");

	printf("%20s %20.2e %20.2e %20.2e %20s\n\n", "Comb Sort Parallel",
		times[5][4], times[5][0], (times[5][0] + times[5][1] +
			times[5][2] + times[5][3] + times[5][4]) / 5, "4 Threads");

	//Speed up 
	printf("Speed up for Odd Sort (2 threads) is %.2e\n", times[0][4]
		/ times[1][4]);
	printf("Speed up for Odd Sort (4 threads) is %.2e\n\n", times[0][4]
		/ times[2][4]);

	printf("Speed up for Comb Sort (2 threads) compared to Comb Sort serial is %.2e\n", times[3][4] / times[4][4]);
	printf("Speed up for Comb Sort (4 threads) compared to Comb Sort serial is %.2e\n\n", times[3][4] / times[5][4]);

	printf("Speed up for Comb Sort (2 threads) compared to Bubble Sort serial is %.2e\n", times[0][4] / times[4][4]);
	printf("Speed up for Comb Sort (4 threads) compared to Bubble Sort serial is %.2e\n\n", times[0][4] / times[5][4]);

	//Effeciency
	printf("Effeciency for Odd Sort (2 threads) is %.2e\n", times[0][4]
		/ times[1][4] * 0.5);
	printf("Effeciency for Odd Sort (4 threads) is %.2e\n\n", times[0][4]
		/ times[2][4] * 0.25);

	printf("Effeciency for Comb Sort (2 threads) compared to Comb Sort serial is %.2e\n", times[3][4] / times[4][4] * 0.5);
	printf("Effeciency for Comb Sort (4 threads) compared to Comb Sort serial is %.2e\n\n", times[3][4] / times[5][4] * 0.25);

	printf("Effeciency for Comb Sort (2 threads) compared to Bubble Sort serial is %.2e\n", times[0][4] / times[4][4] * 0.5);
	printf("Effeciency for Comb Sort (4 threads) compared to Bubble Sort serial is %.2e\n\n", times[0][4] / times[5][4] * 0.25);


	//Trash collection
	for (int i = 0; i < ROWS; i++) 	//must free for each malloc
		free(times[i]);

	free(array);
	free(array1);
	free(array2);
	free(array3);
	free(array4);
	free(array5);
	free(array6);

	return 0;
}/*main*/




/*------------------------------------------------------------------
 * Function:    verify
 * Purpose:     Verify Array wasn't messed with
 * Input args:  arr[], a - which array
 */
void verify(const int arr[], int a) {
	int check = 0;
	//check if The array is being properly sorted
	for (int i = 0; i < Array_Size; i++) {
		if (arr[i] != i) {
			check = 1;
		}
	}

	// Print if the array isn't being properly sorted
	if (check) {
		printf("Array %d isn't being properly sorted!!\n", a);
	}
}  /* verify */



/*------------------------------------------------------------------
 * Function:    SortTimes
 * Purpose:     Sort times for a given sort
 * Input args:  times[], time - given time
 */
void SortTimes(double times[], double time) {
	// Sorting the times in ascending order
	if (times[0] == 0.0) { //checks if col 1 in row is empty
		times[0] = time;
	}
	else {
		int j = 0;
		while (j < 5 && times[j] > time)//finds pos
			j++;
		for (int r = 4; r > j; r--) {//shifts everything else
			times[r] = times[r - 1];
		}
		times[j] = time;//places new time
	}
}  /* SortTimes */