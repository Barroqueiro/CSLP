/*! \file Preditor.cpp
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
#include <fstream> 
#include <vector>
#include <math.h>
#include "Golomb.cpp"
#include <string>

using namespace cv;
using namespace std;

/*! Golomb code class */
class Preditor {
	private:
		int m/*! Paramter m for the Glomb code */;
		int type/*! Flag to signal if we are reading or writing */;
		int typeVideo;
		int start;
		Golomb *g;
		WBitStream *wbs;
	public: 
	
	   	//! A constructor, Initiates the type of BitStream considering the flag, and atributes value to the class atributes
	    /*!
	      \param file A string with the name of the file
	      \param M Parameter m for the golomb code
	      \param Flag Flag to signal if we are reading(0) or Writing(1)
	    */
	   	Preditor(int Type, int M,string file,int TypeVideo){
	   		m = M;
	   		type = Type;
	   		typeVideo = TypeVideo;
	   		g = new Golomb(file,m,1);
	   		start = 1;
	   		cout << "Write header" << endl;
	   		wbs = new WBitStream(file);
	   		wbs->writeNBits(typeVideo,8);
	   		wbs->writeNBits(type,8);
	   		wbs->writeNBits(M,8);
	   	}
	   	
	   	//! Encode de number passed as a parameter, calculating the quotient and the remainder and ecoding inunary and binary respectively depending on the value of m
	    /*!
	      \param n The number to encode
	    */
	   	void encodeJPEG1(Mat f){
	   		int col = f.size().width;
	   		int lin = f.size().height;
	   		if (start == 1){
		   		string width  = to_string(lin);
		   		string height = to_string(col);
		   		for (int w = 0; w < 4-width.length();w++){
		   			width = "0"+width;
		   		}
		   		for (int h = 0; h < 4-height.length();h++){
		   			height = "0"+height;
		   		}
		   		cout << "Processing video" << endl;
		   		start=0;
			}
			int pixel;
			int lastPixel = 0;
			int ind = 1;
			for(int c = 0; c < lin; c++){
				lastPixel = 0;
				for(int i = 0; i < col; i++){
					pixel = f.at<uchar>(Point(c,i));
					g->encode(pixel-lastPixel);
					lastPixel = pixel;					
				}
			}
	   	}
	   	
	   	void encodeJPEG2(Mat f){
	   		int col = f.size().width;
	   		int lin = f.size().height;
	   		if (start == 1){
		   		string width  = to_string(lin);
		   		string height = to_string(col);
		   		for (int w = 0; w < 4-width.length();w++){
		   			width = "0"+width;
		   		}
		   		for (int h = 0; h < 4-height.length();h++){
		   			height = "0"+height;
		   		}
			}
			int pixel;
			int lastPixel;
			for(int c = 0; c < lin; c++){
				for(int i = 0; i < col; i++){
					pixel = f.at<uchar>(Point(c,i));
					if(i-1 >=0){
						lastPixel=f.at<uchar>(Point(c,i));
					}else{
						lastPixel=0;
					}
					g->encode(pixel-lastPixel);
				}
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



















