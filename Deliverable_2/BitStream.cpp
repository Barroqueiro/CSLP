/*! \file BitStream.cpp
 *	\brief Class to write bits to a binary file
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

using namespace std;

class BitStream {
	private:
		unsigned char cacheWrite;
		int posWrite;
		string FileName;
		
	public: 
		BitStream(string file){
			FileName = file;
			cacheWrite = 0;
			posWrite = 7;
		}
				
		void writeBit(int i){
			if (i == 1){
				unsigned char u = 1 << posWrite;
				cacheWrite |= u;
			}
			posWrite--;
			if (posWrite == -1){
				close();
			}
		}
		
		
		void close(){
			ofstream ofsb(FileName, ios::binary | ios::app);
			ofsb.seekp(0);
			ofsb << cacheWrite;
			posWrite = 7;
			cacheWrite = 0;
		}
};


int main( int argc, char** argv )
{	
	BitStream bs = BitStream("out.bin");
	bs.writeBit(1);
	bs.writeBit(1);
	bs.writeBit(1);
	bs.writeBit(1);
	bs.writeBit(1);
	bs.writeBit(1);
	bs.writeBit(1);
	bs.writeBit(0);
	bs.writeBit(0);
	bs.writeBit(1);
	bs.writeBit(0);
	bs.writeBit(0);
	bs.writeBit(0);
	bs.writeBit(1);
	bs.writeBit(1);
	bs.writeBit(0);
	return 0;
}



















