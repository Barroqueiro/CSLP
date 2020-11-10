/*! \file TestGolombEncode.cpp
 *	\brief Just some quick tests for the Golomb class 
 *	       
 *	
 *
 *	
 *  
 *	Press ESC to exit.
 *
 */


#include <iostream>
#include <stdio.h>
#include <fstream> 
#include <vector>
#include <math.h>
#include "Golomb.cpp"

using namespace std;



int main( int argc, char** argv )
{
	int s = 10;
	int toEncode [10] = {11,32,21,7,3,54,61,5,255,160} ;
	Golomb e = Golomb("out.bin",4,1);
	for (int enc:toEncode){
		e.encode(enc);
	}
	e.close();
	printf("Elements to encode: \n");
	printf("[");
	for(int i = 0 ; i < s ; i++){
		if(i == s-1){
			printf("%d",toEncode[i]);
		}else{
			printf("%d ,",toEncode[i]);
		}
	}
	printf("]\n");
	return 0;
}








