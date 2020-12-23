/************************************************
**Josiah Findley ~ COMP233.A ~ Spring 2020****
**
**String 233 Class
**We are attempting to ‘replace’ the C++ String class  with String233.
*************************************************/

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <iostream>
#include <cstring>
#include "String233.h"

/***********************************************
***********************  Constructors **********
************************************************/

/* default constructor */
String233::String233() {
	size = 0; 
	stringPtr = new char[capacity];// creates standard char[]
}

/* c-string constructor */
String233::String233(const char* s) {
	size = strlen(s);
	if (size > CHUNK_SIZE) { //checks if size if greater than the standard
		stringPtr = new char[size+1]; // creates a large enough char[]
	}
	else {
		stringPtr = new char[capacity]; // creates standard char[]
	}
	copy(s, size); //copies over the data of the stringPtr
}
/* copy constructor ** takes in a String233 and makes a Copy*/
String233::String233(const String233& str) {
	size = str.size;
	if (size > CHUNK_SIZE) { //checks if size if greater than the standard
		stringPtr = new char[size + 1]; // creates a large enough char[]
	}
	else {
		stringPtr = new char[capacity]; // creates standard char[]
	}
	copy(str.stringPtr, size); //copies over the data of the stringPtr
		
}

/***********************************************
***********************  Additional Methods ****
************************************************/

//Convert String233 to c-string **  returns new stringPtr
const char* String233::c_str() const
{
	stringPtr[size] = '\0'; //adds the null character
	return stringPtr;

};

/*Overwrite the = operator ** takes in String233 and sets this equal to it
**returns this
*/
String233& String233::operator = (const String233& rhs) {
	if (this != &rhs) { //the fool has NOT struck
		capacity = rhs.capacity;
		size = rhs.size;
		delete[] stringPtr;	//recycle whatever ‘old’ string we have in object
		stringPtr = new char[capacity];		//make a new string (diff capacity?)
		for (int i = 0; i < capacity; i++)
			stringPtr[i] = rhs.stringPtr[i];	//deep copy
	}
	return *this;	//if this == &rhs, we just come here
}
/*deconstructor*/
String233::~String233() {
	delete[] stringPtr;	//garbage collection for dynamic memory
}

/***********************************************
***********************  Methods      **********
************************************************/

/*length of the String233**returns the length*/
int String233::length() {
	return size;
};

/*copy method: creates specified copy of a char[]
** takes in a pointer, how long to copy, and where to start copying
*/
int String233::copy(const char* s, int len, int pos) {
	//sets the size
	size = len;
	//copies the characters over
	for (int i = pos; i < len+pos; i++) {
		stringPtr[i-pos] = s[i];
	}
	return 1;
};

/*append method: creates an append of two strings
** takes in a String233 by reference
** appends String233 to this
** returns this
*/
String233& String233::append(const String233& source) {
	int i = 0;// counter for chars in the loop
	char* stringPtr2; // Temp Pointer for char[]

	if (size + source.size > CHUNK_SIZE) {//checks if total size if greater than the standard
		stringPtr2 = new char[size + source.size + CHUNK_SIZE]; // creates a large enough char[]
	}
	else {
		stringPtr2 = new char[capacity]; // creates a standard char[]
	}
	memcpy(stringPtr2, stringPtr, size); // copies over the data in stringPtr to stringPtr2
	delete[] stringPtr; //deletes the data in stringPtr

	//adds the characters 
	while (i < source.size) {
		stringPtr2[size + i] = source.stringPtr[i];
		i++;
	}
	size += i;//configures the size
	stringPtr = stringPtr2; //Points stringPtr to stuff in stringPtr2
	stringPtr2 = NULL; //sets stringPtr2 to null
	return *this;
};
/* compare method  
** takes in a String233 by reference
** compares String233 to this
** returns the value of comparison
*/
int String233::compare(const String233& str) const {
	// Variable Dictionary
	int retval = 2; // returned value
	int min; // smallest length
	int count = 0; // counter for chars in string233

	// finds the string233 with smallest length
	if (str.size < size)
		min = str.size;
	else if (str.size > size)
		min = size;
	else
		min = size;

	// Iterates through length of min string
	while (count < min && retval == 2) {
		// character in str is less than character in this
		if (str.stringPtr[count] < stringPtr[count])
			retval = 1;
		// character in str is greater than character in this
		else if (str.stringPtr[count] > stringPtr[count])
			retval = -1;
		// last iteration and both strings still equal
		else if (str.size == size && count == min - 1)
			retval = 0;
		// last iteration and length of str is less than this
		else if (str.size < size&& count == min - 1)
			retval = 1;
		// last iteration and length of str is greater than this
		else if (str.size > size && count == min - 1)
			retval = -1;

		count++;

	}
	return retval;
};

