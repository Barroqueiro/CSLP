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
class PreditorDecode {
	private:
		int m/*! Paramter m for the Glomb code */;
		int type/*! Flag to signal if we are reading or writing */;
		int typeVideo;
		int n_frames;
		int linhas;
		int colunas;
		Golomb *g;
	public: 
	
	   	//! A constructor, Initiates the type of BitStream considering the flag, and atributes value to the class atributes
	    /*!
	      \param file A string with the name of the file
	      \param M Parameter m for the golomb code
	      \param Flag Flag to signal if we are reading(0) or Writing(1)
	    */
	   	PreditorDecode(string file){
	   		RBitStream rbs(file);
	   		typeVideo = rbs.readNBits(8);
	   		type = rbs.readNBits(8);
	   		m = rbs.readNBits(8);
	   		linhas = stoi(rbs.readString(4));
	   		colunas = stoi(rbs.readString(4));
	   		n_frames = stoi(rbs.readString(4));
	   		g = new Golomb(file,m,0);
	   		g->skipNBytes(15);
	   	}
	   	
	   	//! Encode de number passed as a parameter, calculating the quotient and the remainder and ecoding inunary and binary respectively depending on the value of m
	    /*!
	      \param n The number to encode
	    */
	   	
	   	
	   	
	   	//! Decode a number from the file passed, first we read 0 bits until we find a bit with the value of 1, the number of 0's is the quotient, secondly we read NBits from the file with N being the log2(m), lastly return the value of the integer decoded, this is not true when m is not a power of 2 since we need to do some more calculations
	   	Mat decodeJPEG1(){
	   		cout << typeVideo << endl;
	   		cout << type << endl;
	   		cout << m << endl;
	   		cout << linhas << endl;
	   		cout << colunas << endl;
	   		cout << frames << endl;
	   		//int lastPixel=0;
	   		Mat result(Size(colunas,linhas),0);
	   		//for(int c = 0; c < linhas; c++){
			//	for(int i = 0; i < colunas; i++){
			//		if (i-1>=0){
			//			lastPixel = result.at<uchar>(Point(c,i-1))
			//		}else{
			//			lastPixel = 0;
			//		}
			//		result.at<uchar>(Point(c,i)) = g->decode()+lastPixel;
			//	}
			//}
	   		return result;
	   	}
	   	
	   	//! simply close the write stream considering we mingh not encode a number of bits multiple of 8
	   	void close(){
	   	}
		
};



















