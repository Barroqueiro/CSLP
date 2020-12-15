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
	//Code used to create the results.txt document where we encoded the same video in 420 with m's being (2,3,4), all the predictors, 4 diferente block sizes (2,4,8,16) and 2 diferent search spaces (5,10) and 2 diferent periods (5,7)
	int ms[3] = {2,3,4};
	int mini = 10000;
	string minimo="";
	int block_sizes[3] = {2,4,8};
	int search_spaces[2] = {5,10};
	int periods[2] = {5,7};
	for (int m:ms){
		for(int bs:block_sizes){
			for(int ss:search_spaces){
				for(int p :periods){
					for(int i=0; i< 8; i++){
						LosslessHybrid l(argv[1],bs,ss,2,i,m,p,1,"Encoded420|M="+to_string(m)+"|BS="+to_string(bs)+"|SS="+to_string(ss)+"|PRED="+to_string(i+1)+"|P="+to_string(p)+".bin");
						l.encode();
						ifstream in_file("Encoded420|M="+to_string(m)+"|BS="+to_string(bs)+"|SS="+to_string(ss)+"|PRED="+to_string(i+1)+"|P="+to_string(p)+".bin", ios::binary);
   						in_file.seekg(0, ios::end);
   						int file_size = in_file.tellg();
   						double ent = file_size*8/(176*144*300*1.5);
   						if (ent < mini){
   							mini = ent;
   							minimo = "Encoded420|M="+to_string(m)+"|BS="+to_string(bs)+"|SS="+to_string(ss)+"|PRED="+to_string(i+1)+"|P="+to_string(p)+".bin";
   						}
   						cout<<"M="+to_string(m)+"|BS="+to_string(bs)+"|SS="+to_string(ss)+"|PRED="+to_string(i+1)+"|P="+to_string(p)<<" "<< ent<<" "<<"Bits per pixel\t\t";
   						in_file.close();
   						const char* file = ("Encoded420|M="+to_string(m)+"|BS="+to_string(bs)+"|SS="+to_string(ss)+"|PRED="+to_string(i+1)+"|P="+to_string(p)+".bin").c_str();
   						if (remove(("Encoded420|M="+to_string(m)+"|BS="+to_string(bs)+"|SS="+to_string(ss)+"|PRED="+to_string(i+1)+"|P="+to_string(p)+".bin").c_str()) != 0){
   							cout << "Erro " <<endl;
   						}
					}
					cout << endl;
				}
			}
		}
	}
	cout << "Minimo: " << minimo << " | " << mini << "bpp" << endl;
	return 0;
}








