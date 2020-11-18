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
#include "LosslessEncoder.cpp"

using namespace std;



int main( int argc, char** argv )
{	
	LosslessEncoder l(0,0,5,argv[1]);
	l.decode();
	return 0;
}








