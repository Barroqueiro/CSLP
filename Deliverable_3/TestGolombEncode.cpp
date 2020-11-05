/*! \file Golomb.cpp
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
	int toEncode [7] = {11,32,21,7,3,54,61} ;
	Golomb e = Golomb("out.bin",4,1);
	for (int enc:toEncode){
		e.encode(enc);
	}
	e.close();
	printf("Elements to encode: \n");
	printf("[");
	for(int i = 0 ; i < 7 ; i++){
		if(i == 6){
			printf("%d",toEncode[i]);
		}else{
			printf("%d ,",toEncode[i]);
		}
	}
	printf("]\n");
	return 0;
}








