/*! \file TestGolombDecodeN.cpp
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
	int s=10;	
	Golomb d = Golomb("out.bin",4,0);
	printf("Decoded Elements: \n");
	for(int i = 0 ; i < s ; i++){
		printf("%d\n",d.decode());
	}
	d.close();
	return 0;
}








