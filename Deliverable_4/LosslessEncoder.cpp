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
#include <ctime>
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
		Preditor *p;
		
	public: 
	
	   	//! A constructor, Initiates the type of BitStream considering the flag, and atributes value to the class atributes
	    /*!
	      \param file A string with the name of the file
	      \param M Parameter m for the golomb code
	      \param Flag Flag to signal if we are reading(0) or Writing(1)
	    */
	   	LosslessEncoder(int Type, int Pred, int M,string file,int flag){
	   		m = M;
	   		type = Type;
	   		pred = Pred;
	   		File = file;
		   	if (flag == 1){
		   		Mat fra;
		   		VideoCapture capa(File);
		   		capa >> fra;
		   		int count = 0;
		   		while(!fra.empty()){
		   			count++;
		   			capa >> fra;
		   		}
	   			p = new Preditor(pred,m,"Encoded.bin",type,count,flag);
	   		}else{
	   			p = new Preditor(0,0,"Encoded.bin",0,0,0);
	   		}
	   		cout << "aa" << endl;
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
	   		switch (pred){
	   			case 0:
	   				while(!frame.empty()){
						vector<Mat> planes;
						split(frame, planes);
						p->encodeJPEG1(planes[0]);
						p->encodeJPEG1(planes[1]);
						p->encodeJPEG1(planes[2]);
						cap >> frame;
						cout << count << endl;
						count++;
					}
	   				break;
	   			case 1:
	   				while(!frame.empty()){
						vector<Mat> planes;
						split(frame, planes);
						p->encodeJPEG2(planes[0]);
						p->encodeJPEG2(planes[1]);
						p->encodeJPEG2(planes[2]);
						cap >> frame;
						count++;
					}
	   				break;
		   		case 2:
	   				while(!frame.empty()){
						vector<Mat> planes;
						split(frame, planes);
						p->encodeJPEG3(planes[0]);
						p->encodeJPEG3(planes[1]);
						p->encodeJPEG3(planes[2]);
						cap >> frame;
						count++;
					}
	   				break;
	  	   		case 3:
	   				while(!frame.empty()){
						vector<Mat> planes;
						split(frame, planes);
						p->encodeJPEG4(planes[0]);
						p->encodeJPEG4(planes[1]);
						p->encodeJPEG4(planes[2]);
						cap >> frame;
						count++;
					}
	   				break;
		   		case 4:
	   				while(!frame.empty()){
						vector<Mat> planes;
						split(frame, planes);
						p->encodeJPEG5(planes[0]);
						p->encodeJPEG5(planes[1]);
						p->encodeJPEG5(planes[2]);
						cap >> frame;
						count++;
					}
	   				break;
	  			case 5:
	   				while(!frame.empty()){
						vector<Mat> planes;
						split(frame, planes);
						p->encodeJPEG6(planes[0]);
						p->encodeJPEG6(planes[1]);
						p->encodeJPEG6(planes[2]);
						cap >> frame;
						count++;
					}
	   				break;
 	   			case 6:
	   				while(!frame.empty()){
						vector<Mat> planes;
						split(frame, planes);
						p->encodeJPEG7(planes[0]);
						p->encodeJPEG7(planes[1]);
						p->encodeJPEG7(planes[2]);
						cap >> frame;
						count++;
					}
	   				break;	   					   					case 7:
	   				while(!frame.empty()){
						vector<Mat> planes;
						split(frame, planes);
						p->encodeJPEG_LS(planes[0]);
						p->encodeJPEG_LS(planes[1]);
						p->encodeJPEG_LS(planes[2]);
						cap >> frame;
						count++;
					}	   				
	   				break;
	   				
	   		}
	   	}
	   	//! Decode a number from the file passed, first we read 0 bits until we find a bit with the value of 1, the number of 0's is the quotient, secondly we read NBits from the file with N being the log2(m), lastly return the value of the integer decoded, this is not true when m is not a power of 2 since we need to do some more calculations
	   	int decode(){
	   		Mat result;
			Mat m1;
	   		Mat m2;
	   		Mat m3;
	   		vector<Mat> channels ;
	   		int fra = p->get_frames();
			/// JUntar de novo todos os planos
			for(int b = 0; b<fra;b++){
				m1 = p->decodeJPEG1();
	   			m2 = p->decodeJPEG1();
	   			m3 = p->decodeJPEG1();
	   			channels = {m1,m2,m3};
	   			merge(channels,result);
				imshow("Display",result);
				if(waitKey(30) >=0){
					break;
				}
			}
			waitKey(0);
	   		return 1;
	   	}
	   	
	   	//! simply close the write stream considering we mingh not encode a number of bits multiple of 8
	   	void close(){
	   	}
		
};



















