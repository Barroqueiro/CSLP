/*! \file TestBitStream.cpp
 *	\brief Just some quick tests for the BitStream classes
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



int main( int argc, char** argv )
{	WBitStream ws = WBitStream("out.bin");
	/// Writting the bits 00011111 and 11000 bit by bit to test the writeBit() function
	ws.writeBit(0);
	ws.writeBit(0);
	ws.writeBit(0);
	ws.writeBit(1);
	ws.writeBit(1);
	ws.writeBit(1);
	ws.writeBit(1);
	ws.writeBit(1);
	ws.writeBit(1);
	ws.writeBit(1);
	ws.writeBit(0);
	ws.writeBit(0);
	ws.writeBit(0);
	/// Writting the integers 3,4,2 in 3,3 and 2 bits respectivly to test the writeNBits() function
	ws.writeNBits(3,3);
	ws.writeNBits(4,3);
	ws.writeNBits(2,2);
	/// Writting the bits 110 to test that the buffer is working the same with both functions
	ws.writeBit(1);
	ws.writeBit(1);
	ws.writeBit(0);
	/// Writting the string "STRING" to test the function writeString() with some bits in bettewn
	ws.writeNBits(5,5);
	ws.writeString("STRING");
	ws.writeNBits(2,3);
	RBitStream rs = RBitStream("out.bin");
	/// Reading the bits 00011111 and 11000 bit by bit to test the readBit() function
	printf("%d",rs.readBit());
	printf("%d",rs.readBit());
	printf("%d",rs.readBit());
	printf("%d",rs.readBit());
	printf("%d",rs.readBit());
	printf("%d",rs.readBit());
	printf("%d",rs.readBit());
	printf("%d\n",rs.readBit());
	printf("%d",rs.readBit());
	printf("%d",rs.readBit());
	printf("%d",rs.readBit());
	printf("%d",rs.readBit());
	printf("%d\n",rs.readBit());
	/// Reading the integers 3,4,2 in 3,3 and 2 bits respectivly to test the readNBits() function
	printf("%d\n",rs.readNBits(3));
	printf("%d\n",rs.readNBits(3));
	printf("%d\n",rs.readNBits(2));
	/// Reading the bits 110 to test that the buffer is working the same with both functions
	printf("%d",rs.readBit());
	printf("%d",rs.readBit());
	printf("%d\n",rs.readBit());
	/// Reading the string "STRING" to test the function readString() with some bits in bettewn
	printf("%d\n",rs.readNBits(5));
	cout << rs.readString(6) << endl;
	printf("%d\n",rs.readNBits(3));
	return 0;
}



















