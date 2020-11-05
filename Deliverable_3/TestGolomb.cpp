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
	Golomb e = Golomb("out.bin",4,1);
	e.encode(19);
	e.encode(32);
	e.close();
	Golomb d = Golomb("out.bin",4,0);
	printf("%d\n",d.decode());
	printf("%d\n",d.decode());
	d.close();
	return 0;
}



















