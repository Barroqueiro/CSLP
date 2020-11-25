/*! \file TestDecode.cpp
 *	\brief Decode a video by passing the file it was encoded to
 *	       
 *	First and only flag -> Name of the file to decode from
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
	LosslessEncoder d(0,0,0,"",0,argv[1]);
	d.decode();
	return 0;
}








