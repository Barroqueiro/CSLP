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
	//int m_values [3] = {7,8,9};
	//for (int  m:m_values){
	//	for(int i = 0;i<8;i++){
	//	LosslessEncoder l(0,i,m,argv[1],1,"Encoded"+to_string(m)+"JPEG"+to_string(i)+".bin");
	//	l.encode();
	//	}
	//}
	LosslessEncoder l(0,5,8,argv[1],1,"EncodedT.bin");
	l.encode();
	//LosslessEncoder d(0,0,0,argv[1],0,"Encoded7.bin");
	//d.decode();
	return 0;
}








