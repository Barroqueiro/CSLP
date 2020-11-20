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
	//int m_values [8] = {2,3,4,5,6,7,8,9};
	//for (int  m:m_values){
		//for(int i = 0;i<8;i++){
			LosslessEncoder l(0,7,4,argv[1],1,"Encoded"+to_string(4)+"JPEGLS"+".bin");
			l.encode();
		//}
	//}
	//LosslessEncoder l(0,7,5,argv[1],1,"EncodedJPEGLS.bin");
	//l.encode();
	return 0;
}








