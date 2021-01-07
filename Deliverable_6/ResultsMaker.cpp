/*! \file ResultsMaker.cpp
 *	\brief Scrip to create results and see what are the best encoding params
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
#include "LosslessHybrid.cpp"
#include <string>

using namespace std;



int main( int argc, char** argv )
{	

	for(int c=1; c< 4; c++){
		LosslessHybrid l(argv[1],8,10,2,7,2,7,1,"Encoded420|M=2|BS=8|SS=10|PRED=LS|P=7|Q="+to_string(c)+".bin",c);
		l.encode();
		ifstream in_file("Encoded420|M=2|BS=8|SS=10|PRED=LS|P=7|Q="+to_string(c)+".bin", ios::binary);
		in_file.seekg(0, ios::end);
		int file_size = in_file.tellg();
		double ent = file_size*8/(176*144*300*1.5);
		cout<<"Encoded420|M=2|BS=8|SS=10|PRED=LS|P=7|Q="+to_string(c)<<" "<< ent<<" "<<"Bits per pixel\t\t";
		in_file.close();
		const char* file = ("Encoded420|M=2|BS=8|SS=10|PRED=LS|P=7|Q="+to_string(c)+".bin").c_str();
		if (remove(("Encoded420|M=2|BS=8|SS=10|PRED=LS|P=7|Q="+to_string(c)+".bin").c_str()) != 0){
			cout << "Erro " <<endl;
		}
	}
	return 0;
}








