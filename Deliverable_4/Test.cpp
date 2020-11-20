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
#include <string>

using namespace std;



int main( int argc, char** argv )
{	
	int m_values [3] = {7,8,9};
	for (int  m:m_values){
		LosslessEncoder l(0,0,m,argv[1],1,"Encoded"+to_string(m)+".bin");
		l.encode();
	}
	//LosslessEncoder d(0,0,0,argv[1],0,"Encoded7.bin");
	//d.decode();
	return 0;
}








