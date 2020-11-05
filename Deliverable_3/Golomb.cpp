/*! \file Golomb.cpp
 *	\brief Class to encode and decode Golomb code
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
#include "RBitStream.cpp"
#include "WBitStream.cpp"

using namespace std;

/*! Golomb code class */
class Golomb {
	private:
		int m;
		int flag;
		RBitStream* rbs;
		WBitStream* wbs;
		
	public: 
	   	
	   	Golomb(string file, int M, int Flag){
	   		if(flag == 0){
	   			rbs = new RBitStream(file);
	   		}else{
	   			wbs = new WBitStream(file);
	   		}
	   		m = M;
	   		flag = Flag;
	   	}
	   	
	   	void encode(int n){
	   		int r = n % m;
	   		int q = n / m;
	   		// Unary part
	   		for(int c = 0 ; c < q; c++){
	   			wbs->writeBit(0);
	   			printf("%d",0);
	   		}
	   		wbs->writeBit(1);
	   		printf("%d\n",1);
	   		// Binary part
	   		int pow = log2(m);
	   		int temp = 0;
	   		unsigned char u = (unsigned char) r;
	   		for (int i = 7 -(7-pow) -1 ; i != -1 ; i--) {
				printf("%d",(int)((u & (1 << i)) != 0));
				wbs->writeBit((int)((u & (1 << i)) != 0));
			}
			printf("\n");
	   	}
	   	
	   	int decode(){
	   		int bit = rbs->readBit();
	   		int quo = 0;
	   		while(bit != 1){
	   			bit = rbs->readBit();
	   			quo++;
	   		}
	   		int pow = log2(m);
	   		int r = rbs->readNBits(pow);
	  		return quo*m + r;
	   	}
	   	
	   	void close(){
	   		if (flag == 1){
	   			wbs->close();
	   		}
	   	}
		
};



















