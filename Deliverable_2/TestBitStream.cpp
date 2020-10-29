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
	ws.writeNBits(3,3);
	ws.writeNBits(4,3);
	ws.writeNBits(2,2);
	ws.writeBit(1);
	ws.writeBit(1);
	ws.writeBit(0);
	ws.writeString("STRING");
	ws.writeNBits(5,5);
	ws.writeNBits(2,3);
	RBitStream rs = RBitStream("out.bin");
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
	printf("%d\n",rs.readNBits(3));
	printf("%d\n",rs.readNBits(3));
	printf("%d\n",rs.readNBits(2));
	printf("%d",rs.readBit());
	printf("%d",rs.readBit());
	printf("%d\n",rs.readBit());
	cout << rs.readString(6) << endl;
	printf("%d\n",rs.readNBits(5));
	printf("%d\n",rs.readNBits(3));
	return 0;
}



















