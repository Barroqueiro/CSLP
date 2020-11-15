/*! \file LosslessEncoder.cpp
 *	\brief Class to encode and decode Golomb code
 *	      
 *	
 *
 *	
 *  
 *	Press ESC to exit.
 *
 */

#include "opencv2/opencv.hpp"
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <stdio.h>
#include <iostream>
#include <stdio.h>
#include <fstream> 
#include <vector>
#include <math.h>
#include "Preditor.cpp"

using namespace std;
using namespace cv;

/*! Golomb code class */
class LosslessEncoder {
	private:
		int type/*! */;
		int pred/*! */;
		int m/*! */;
		string File;
		Preditor *p1;
		Preditor *p2;
		Preditor *p3;
	public: 
	
	   	//! A constructor, Initiates the type of BitStream considering the flag, and atributes value to the class atributes
	    /*!
	      \param file A string with the name of the file
	      \param M Parameter m for the golomb code
	      \param Flag Flag to signal if we are reading(0) or Writing(1)
	    */
	   	LosslessEncoder(int Type, int Pred, int M,string file){
	   		m = M;
	   		type = Type;
	   		pred = Pred;
	   		File = file;
	   		p1 = new Preditor(pred,m,"plane1.bin",type);
			p2 = new Preditor(pred,m,"plane2.bin",type);
			p3 = new Preditor(pred,m,"plane3.bin",type);
	   	}
	   	
	   	//! Encode de number passed as a parameter, calculating the quotient and the remainder and ecoding inunary and binary respectively depending on the value of m
	    /*!
	      \param n The number to encode
	    */
	   	void encode(){
	   		int count = 1;
	   		Mat frame;
	   		Mat frame1;
	   		VideoCapture cap(File);
	   		cap >> frame;
			while(!frame.empty()){
				vector<Mat> planes;
				split(frame, planes);
				p1->encodeJPEG1(planes[0]);
				p1->encodeJPEG1(planes[1]);
				p1->encodeJPEG1(planes[2]);
				cap >> frame;
				cout << count << endl;
				count++;
			}
	   	}
	   	//! Decode a number from the file passed, first we read 0 bits until we find a bit with the value of 1, the number of 0's is the quotient, secondly we read NBits from the file with N being the log2(m), lastly return the value of the integer decoded, this is not true when m is not a power of 2 since we need to do some more calculations
	   	int decode(){
	   		return 1;
	   	}
	   	
	   	//! simply close the write stream considering we mingh not encode a number of bits multiple of 8
	   	void close(){
	   	}
		
};



















