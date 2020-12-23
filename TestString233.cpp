/************************************************
** D. Valentine ~  COMP233   ~  C++ Classes    **
** Test Harness for String233 Class            **
*************************************************
** For many tests, we assumed the c_str()      **
**   method is working.  That allowed us to    **
**   use the c-string functions to aid our     **
**   testing.  So try to be sure c_str()       **
**   is up and running to use this harness     **
** Likewise, we used copy() to initialize      **
**   String233 objects for testing.            **
*************************************************
** In a test harness, each test must be as     **
** stand-alone as possible.  That is, init     **
** all your needed values (do NOT rely on them **
** retaining their value from the last test    **
************************************************/

#define _CRT_SECURE_NO_WARNINGS		//be quiet VisStudio

#include "String233.h"
#include <stdio.h>
#include <cstring>

String233 myFunction();	//used to test copyConstr & = operator

/**************************************************/
/***************************** M A I N ************/
/**************************************************/
int main() {
	int errorCnt = 0;		//error count for one set of test
	int totErrors = 0;		//error count for all tests
	bool isError;			//did we find an error in this test?
	int testCnt;			//track #tests we do (we keep adding 'em)
	char* theResult = new char[512];	//testing String233 methods

	printf("D. Valentine ~ COMP233 ~ String233 Class Program\n\n");
	printf("<<<<<< Test Harness for String233 Class >>>>>>\n");
	printf("< Be advised we use methods str_c() & copy() >\n");
	printf("<<<<<<<  to conduct some of our tests. >>>>>>>\n");

	/***********************************************
	***********************  Constructors **********
	************************************************/
	/* Test #1: default constructor */
	printf("%22s", "\nTesting Constructors: ");
	testCnt = 1;
	String233 str1;		//default constructor
	isError = ((str1.length() != 0)); //did NOT make empty string

	if (isError) {
		printf("\n\t1: Default Constructor did not make empty string");
		errorCnt++;
	}

	/* Test #2: c-string constructor */
	String233 str2("ABCDEFGHIJKLMNOPQRSTUVWXYZ");
	testCnt++;
	strcpy(theResult, str2.c_str());	//use the c-string functions to test
	isError = (strcmp(theResult, "ABCDEFGHIJKLMNOPQRSTUVWXYZ") != 0);

	if (isError) {
		printf("\n\t2: c-string Constructor did not make correctly sized string");
		errorCnt++;
	}

	/* Test #3: copy constructor */
	testCnt++;
	str2.copy("abcdefghijklmnopqrstuvwxyz", 26, 0);  //init str2
	String233 str3(str2);
	strcpy(theResult, str3.c_str());
	isError = (0 != strcmp(theResult, "abcdefghijklmnopqrstuvwxyz"));
	if (isError) {
		printf("\n\t3: Copy Constructor did not make correctly sized string");
		errorCnt++;
	}

	//tally results of Constructor testing & tell human
	if (errorCnt == 0)
		printf("\t *** no errors in Constructors **\n");
	else {
		totErrors += errorCnt;
		errorCnt = 0;	//reset counter
	}


	/***********************************************
	***********************  Methods      **********
	************************************************/
	printf("\n%22s", "Testing Methods: ");

	/* Test #4: copy method ~ whole string*/
	testCnt++;
	str3.copy("0123456789", 10);	//uses default 0 for pos
	strcpy(theResult, str3.c_str()); //let us use strcmp
	isError = (0 != strcmp(theResult, "0123456789"));
	if (isError) {
		printf("\n\t4: Copy Method did not make correctly sized string");
		errorCnt++;
	}

	/* Test #5: copy method ~ substring */
	testCnt++;
	str3.copy("0123456789", 4, 2);
	strcpy(theResult, str3.c_str());
	isError = (0 != strcmp(theResult, "2345"));
	if (isError) {
		printf("\n\t5: Copy Method did not make correctly sized string");
		errorCnt++;
	}



//
	/* Test #7: compare method */
	testCnt++;
	str1.copy("Hello ", 6);
	str2.copy("World", 5);
	isError = (str1.compare(str2) >= 0); //should be NEG
	if (isError) {
		printf("\n\t7: Compare method failed comparing '%s' to '%s'",
			str1.c_str(), str2.c_str());
		errorCnt++;
	}

	/* Test #8: compare method */
	testCnt++;
	str1.copy("Hello ", 6);
	str2.copy("World", 5);

	isError = (str2.compare(str1) <= 0); //should be POS
	if (isError) {
		printf("\n\t8: Compare method failed comparing '%s' to '%s'",
			str2.c_str(), str1.c_str());
		errorCnt++;
	}

	/* Test #9: compare method  */
	testCnt++;
	str2.copy("World", 5);

	isError = (str2.compare(str2) != 0); //should be ZERO
	if (isError) {
		printf("\n\t9: Compare method failed comparing '%s' to '%s'",
			str2.c_str(), str2.c_str());
		errorCnt++;
	}

	/* Test #10: compare method  */
	testCnt++;
	str1.copy("cat", 3);
	str2.copy("catacombs", 9); //'cat' < 'catacombs'

	isError = (str1.compare(str2) >= 0); //should be NEG
	if (isError) {
		printf("\n\t10: Compare method failed comparing '%s' to '%s'",
			str1.c_str(), str2.c_str());
		errorCnt++;
	}

	/* Test #11: compare method  */
	testCnt++;
	str1.copy("cat", 3);
	str2.copy("catacombs", 9); //'cat' < 'catacombs'
	isError = (str2.compare(str1) <= 0);
	if (isError) {
		printf("\n\t11: Compare method failed comparing '%s' to '%s'",
			str2.c_str(), str1.c_str());
		errorCnt++;
	}



	/* Test #6: append method */
	testCnt++;
	str1.copy("Hello ", 6);
	str2.copy("World", 5);
	str1.append(str2);
	strcpy(theResult, str1.c_str());
	isError = 0 != strcmp(theResult, "Hello World");
	if (isError) {
		printf("\n\t6: Append Method failed 'Hello ' + 'World'");
		errorCnt++;
	}


	/* Test #12: append a really LONG string */
	testCnt++;
	str1.copy("Hello ", 6);
	String233 strLong("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789");
	str1.append(strLong);
	strcpy(theResult, str1.c_str());
	isError = (0 != strcmp(theResult,
		"Hello abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789"));
	if (isError) {
		printf("\n\t12: Append failed with LONG string");
		errorCnt++;
	}
	

	/* Test #13: function return value - assignment operator */
	testCnt++;
	str1 = myFunction();	//assign a return value from function
	str2.copy("hi mom", 6);
	isError = (str2.compare(str1) != 0);
	if (isError) {
		printf("\n\t13: Function return value failed comparing '%s' to '%s'",
			str1.c_str(), str2.c_str());
		errorCnt++;
	}

	/* Test #14 assignment operator */
	testCnt++;
	str1.copy("Hello World", 11);
	str2 = str1;
	isError = (0 != strcmp(str1.c_str(), str2.c_str()));
	if (isError) {
		printf("\n\t14: Assignment Operator failed '%s' assigned to '%s'",
			str1.c_str(), str2.c_str());
		errorCnt++;
	}

	if (errorCnt == 0)
		printf("\t *** no errors in Methods **\n\n");
	else {
		totErrors += errorCnt;
		errorCnt = 0;
	}

	/************************ Finish Up *********/
	delete[] theResult;
	printf("\n\n\n\t< termination with %d errors in %d tests>\n",
		totErrors, testCnt);
	return 0;
}//Main


/******************* myFunction *****************
********** will force a copy constructor call  **
********** and will let us use the assignment  **
********** operator on the result ***************
*************************************************/
String233 myFunction() {
	String233 aStr("hi mom");
	return aStr;
}

