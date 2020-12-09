/*! \file TestEncode.cpp
 *	\brief Encode a video file by hybrid encoding (intra and inter frame)
 *	       
 *	First Flag -> Name of the video file to encode\n
 *	Second Flag -> Block size\n
 *	Third Flag -> Search space\n
 *	Fourth Flag -> Type of the video\n
 *	Fifth Flag -> Type of the predictor\n
 *	Sixth Flag -> Parameter M of the golomb code\n
 *	Seventh Flag -> Period to encode a intra frame instead of considering the last frame
 *	Eighth Flag -> File to encode to
 *
 *	Press ESC to exit.
 *
 */


#include <iostream>
#include <stdio.h>
#include <fstream> 
#include <vector>
#include <math.h>
#include "LosslessHybrid.cpp"
#include <string>

using namespace std;



int main( int argc, char** argv )
{	
	//Code used to create the results.txt document where we encoded all type of prediction with m's ranging from 2 to 9
	//int m_values [8] = {2,3,4,5,6,7,8,9};
	//for (int  m:m_values){
		//for(int i = 0;i<8;i++){
			//LosslessEncoder l(0,7,4,argv[1],1,"Encoded"+to_string(4)+"JPEGLS"+".bin");
			//l.encode();
		//}
	//}
	LosslessHybrid l(argv[1],stoi(argv[2]),stoi(argv[3]),stoi(argv[4]),stoi(argv[5]),stoi(argv[6]),stoi(argv[7]),1,argv[8]);
	l.encode();
	return 0;
}








