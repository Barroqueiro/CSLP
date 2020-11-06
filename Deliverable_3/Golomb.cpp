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
		int m/*! Paramter m for the Glomb code */;
		int flag/*! Flag to signal if we are reading or writing */;
		RBitStream* rbs/*! Reading BitStream */;
		WBitStream* wbs/*! Writing BitStream */;;
		
	public: 
	
	   	//! A constructor, Initiates the type of BitStream considering the flag, and atributes value to the class atributes
	    /*!
	      \param file A string with the name of the file
	      \param M Parameter m for the golomb code
	      \param Flag Flag to signal if we are reading(0) or Writing(1)
	    */
	   	Golomb(string file, int M, int Flag){
	   		if(flag == 0){
	   			rbs = new RBitStream(file);
	   		}else{
	   			wbs = new WBitStream(file);
	   		}
	   		m = M;
	   		flag = Flag;
	   	}
	   	
	   	//! Encode de number passed as a parameter, calculating the quotient and the remainder and ecoding inunary and binary respectively
	    /*!
	      \param n The number to encode
	    */
	   	void encode(int n){
	   		int r = n % m;
	   		int q = n / m;
	   		// Unary part
	   		for(int c = 0 ; c < q; c++){
	   			wbs->writeBit(0);
	   		}
	   		wbs->writeBit(1);
	   		// Binary part
	   		int pow = log2(m);
	   		int temp = 0;
	   		unsigned char u = (unsigned char) r;
	   		for (int i = 7 -(7-pow) -1 ; i != -1 ; i--) {
				wbs->writeBit((int)((u & (1 << i)) != 0));
			}
	   	}
	   	//! Decode a number from the file passed, first we read 0 bits until we find a bit with the value of 1, the number of 0's is the quotient, secondly we readingNBits from the filess with N being the log2(m), lastly return the value of the integer decoded
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
	   	
	   	//! simply close the write stream considering we mingh not encode a number of bits multiple of 8
	   	void close(){
	   		if (flag == 1){
	   			wbs->close();
	   		}
	   	}
		
};



















