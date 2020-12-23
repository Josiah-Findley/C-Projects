/************************************************
**Josiah Findley ~ COMP233.A ~ Spring 2020****
**
**Multiplying a 250,000x3 with a 3x7 matrix
**
**Code adapted from Aditya Ranjan at 
**https://www.geeksforgeeks.org/c-program-multiply-two-matrices/
*************************************************/
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h> 
#include <stdlib.h>
#include <omp.h>
#include <time.h>

// method to multiply the small with big matrix
double* multiply(const double* smallMatrix, const double* bigMatrix,
	int smCOL, int bmROW, int commonElement);

// method to multiply the small with big matrix in Parallel
double* multiplyParallel(const double* smallMatrix, 
	const double* bigMatrix,int smCOL, int bmROW, int commonElement);

//initialize the matrices
void initialize(double* smallMatrix, double* bigMatrix);

//prints paralized version
void printToFile(double* resultMatrix, FILE* fp);

int main()
{
	//Variable Dictionary
	double startTime, stopTime;//timing variables
	double startTimeP, stopTimeP;//timing variables parallel
	//A 3x7 matrix
	double* smallMatrix = (double*)malloc(3 * 7 * sizeof(double));
	//A 250000x3 matrix
	double* bigMatrix = (double*)malloc(2500000 * 3 * sizeof(double));
	//store matrix that is parallel computed
	double* resultMatrix;
	//File being written to
	FILE* fp;

	//initialize the matrices
	initialize(smallMatrix, bigMatrix);

	startTime = omp_get_wtime(); // start timer
	//Passes in both matrices with the indexes - commonElement of 3
	 multiply(smallMatrix, bigMatrix, 7, 2500000, 3);
	stopTime = omp_get_wtime(); // end timer

	startTimeP = omp_get_wtime(); // start timer
	//Passes in both matrices with the indexes - commonElement of 3
	resultMatrix = multiplyParallel(smallMatrix, 
		bigMatrix, 7, 250000, 3);
	stopTimeP = omp_get_wtime(); // end timer

	/* open the file for writing*/
	fp = fopen("Findley.csv", "w");

	//Header
	fprintf(fp, "Josiah Findley ~COMP233.A ~Spring 2020\n");

	//Prints to a file
	printToFile(resultMatrix, fp);

	//Print the times
	fprintf(fp, "\nElapsed time for serial is %.3e\n",
		stopTime - startTime);
	fprintf(fp, "\nElapsed time for parallel is %.3e\n",
		stopTimeP - startTimeP);

	/* close the file*/
	fclose(fp);

	//Trash collection

	free(smallMatrix);
	free(bigMatrix);
	free(resultMatrix);

	return 0;
}

/*------------------------------------------------------------------
 * Function:    printToFile
 * Purpose:     This function prints desired output

 * Input args:  resultMatrix, fp - fil to print to
 */

void printToFile(double* resultMatrix, FILE* fp)
{
	/* write the final matrix into the file stream*/
	for (int i = 0; i < 250000; i++)
	{
		for (int j = 0; j < 7; j++)
		{
			fprintf(fp, "%.3e, ", *(resultMatrix + i * 7 + j));
		}
		fprintf(fp, "\n");
	}
}/*printToFile*/



/*------------------------------------------------------------------
 * Function:    initialize
 * Purpose:     This function inititializes smallMatrix[][], bigMatrix[][]

 * Input args:  smallMatrix, bigMatrix - matrices being initialized
 */

void initialize(double* smallMatrix, double* bigMatrix)
{

	//Initializing the 3x7 small matrix
	for (int j = 0; j < 4; j++) {
		// row 1
		*(smallMatrix + 0 * 7 + j) = 0.9 + 0.02 * j;
		// row 2
		*(smallMatrix + 1 * 7 + j) = 0.85 + 0.03 * j;
		// row 3
		*(smallMatrix + 2 * 7 + j) = 0.75 + 0.05 * j;

		double r = 0.9 + 0.02 * j;
	}

	for (int i = 0; i < 3; i++) {//last 3 columns
		for (int j = 4; j < 7; j++) {
			*(smallMatrix + i * 7 + j) = j - 4 + 1.0;
		}
	}

	//Initializing the 250000x3 big matrix
	for (int i = 0; i < 250000; i++) {//last 3 columns
		for (int j = 0; j < 3; j++) {
			//Starting with 0.0 in bigMatrix[0][0] and work 
			//across the rows, increasing the values by 0.03125 (1/32).  
			*(bigMatrix + i * 3 + j) = (3 * i + j) * (0.03125);
		}
	}
}/*initialize*/




/*------------------------------------------------------------------
 * Function:    multiply
 * Purpose:     This function multiplies smallMatrix[][] and bigMatrix[][] 
 *				and stores the result in resultMatrix[][] 
 * Input args:  smallMatrix, bigMatrix - matrices being multiplied. 
 *              smCOL, bmROW, commonElement - sizes needed for multip.
 */

double* multiply(const double* smallMatrix, const double* bigMatrix, 
	int smCOL, int bmROW, int commonElement)
{

	//The resulting bmROW x smCOL matrix
	double* resultMatrix = 
		(double*)malloc(bmROW * smCOL * sizeof(double));

		for (int i = 0; i < bmROW; i++) //iterate through Rows of big M.
		{
			for (int j = 0; j < smCOL; j++)//iterate through Col of sm M.
			{
				*(resultMatrix + i * smCOL + j) = 0.0;//initialize result
				for (int k = 0; k < commonElement; k++)//Calc. dot product
					//Adding results together
					*(resultMatrix + i * smCOL + j) +=
					*(bigMatrix + i * commonElement + k) *
					*(smallMatrix + k * smCOL + j);
			}
		}

	

	return resultMatrix;
}/*multiply*/


/*------------------------------------------------------------------
 * Function:    multiplyParallel
 * Purpose:     This function multiplies smallMatrix[][] and bigMatrix[][]
 *				and stores the result in resultMatrix[][] using parallel
 * Input args:  smallMatrix, bigMatrix - matrices being multiplied.
 *              smCOL, bmROW, commonElement - sizes needed for multip.
 */

double* multiplyParallel(const double* smallMatrix, 
	const double* bigMatrix, int smCOL, int bmROW, int commonElement)
{

	//The resulting bmROW x smCOL matrix
	double* resultMatrix = 
		(double*)malloc(bmROW * smCOL * sizeof(double));

		////fork a team of threads
		////SPMD - share work across thread team by chunks
#pragma omp parallel for schedule(static, 1) num_threads(2) collapse(3)

	for (int i = 0; i < bmROW; i++) //iterate through Rows of big M.
	{
		for (int j = 0; j < smCOL; j++)//iterate through Col of sm M.
		{
			*(resultMatrix + i * smCOL + j) = 0.0;//initialize result
			for (int k = 0; k < commonElement; k++)//Calc. dot product
				//Adding results together
				*(resultMatrix + i * smCOL + j) +=
				*(bigMatrix + i * commonElement + k) *
				*(smallMatrix + k * smCOL + j);
		}
	}	
	return resultMatrix;
}/*multiplyParallel*/




