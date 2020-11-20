/*! \file LosslessEncoder.cpp
 *	\brief Class to encode and decode videos
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

/*! LosslessEncode code class */
class LosslessEncoder {
	private:
		int type/*! type of the video*/;
		int pred/*! type of the predictor to be encoded*/;
		int m/*! param m of the golomb class*/;
		string File/*! Name of the file we are encoding*/;
		Preditor *p/*! Pointer to the predictor that will encode/decode each frame */;
		
	public: 
	
	   	//! A constructor, Initiates the paramters of the encoder, the values dont realy matter in decoding since tha info comes from the file
	    /*!
	      \param file A string with the name of the video file to encode
	      \param outFile A string with the name of the file to encode to or to decode from
	      \param M Parameter m for the golomb code
	      \param Flag Flag to signal if we are encoding(1) or Decoding(0)
	      \param Type Type of the video
	      \param Pred predictor to encode or decode frames
	    */
	   	LosslessEncoder(int Type, int Pred, int M,string file,int flag,string outFile){
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
	   			p = new Preditor(pred,m,outFile,type,count,flag);
	   		}else{
	   			p = new Preditor(0,0,outFile,0,0,0);
	   		}
	   		cout << "aa" << endl;
	   	}
	   	
	   	//! Encode the video passed as a paramter, switch to decide witch function of the preditor will be used based on with prediting encoding we want
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
						cout << "Encoding Frame: "<< count << endl;
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
						cout << "Encoding Frame: "<< count << endl;
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
						cout << "Encoding Frame: "<< count << endl;
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
						cout << "Encoding Frame: "<< count << endl;
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
						cout << "Encoding Frame: "<< count << endl;
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
						cout << "Encoding Frame: "<< count << endl;
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
						cout << "Encoding Frame: "<< count << endl;
						cap >> frame;
						count++;
					}
	   				break;	   					   					
	   			case 7:
	   				while(!frame.empty()){
						vector<Mat> planes;
						split(frame, planes);
						p->encodeJPEG_LS(planes[0]);
						p->encodeJPEG_LS(planes[1]);
						p->encodeJPEG_LS(planes[2]);
						cout << "Encoding Frame: "<< count << endl;
						cap >> frame;
						count++;
					}	   				
	   				break;
	   				
	   		}
	   	}
	   	//! Decode the video encoded in the file, switch to decide with preditor encoding was used after a initial read of the header
	   	int decode(){
	   		Mat result;
			Mat m1;
	   		Mat m2;
	   		Mat m3;
	   		vector<Mat> channels ;
	   		int fra = p->get_frames();
	   		int mode = p->get_type();
			switch (mode){
	   			case 0:
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
						cout << "Decoding frame: " << b+1 << endl;
					}
	   				break;
	   			case 1:
					for(int b = 0; b<fra;b++){
						m1 = p->decodeJPEG2();
	   					m2 = p->decodeJPEG2();
	   					m3 = p->decodeJPEG2();
	   					channels = {m1,m2,m3};
	   					merge(channels,result);
						imshow("Display",result);
						if(waitKey(30) >=0){
							break;
						}
						cout << "Decoding frame: " << b+1 << endl;
					}
	   				break;
		   		case 2:
					for(int b = 0; b<fra;b++){
						m1 = p->decodeJPEG3();
	   					m2 = p->decodeJPEG3();
	   					m3 = p->decodeJPEG3();
	   					channels = {m1,m2,m3};
	   					merge(channels,result);
						imshow("Display",result);
						if(waitKey(30) >=0){
							break;
						}
						cout << "Decoding frame: " << b+1 << endl;
					}
	   				break;
	  	   		case 3:
					for(int b = 0; b<fra;b++){
						m1 = p->decodeJPEG4();
	   					m2 = p->decodeJPEG4();
	   					m3 = p->decodeJPEG4();
	   					channels = {m1,m2,m3};
	   					merge(channels,result);
						imshow("Display",result);
						if(waitKey(30) >=0){
							break;
						}
						cout << "Decoding frame: " << b+1 << endl;
					}
	   				break;
		   		case 4:
					for(int b = 0; b<fra;b++){
						m1 = p->decodeJPEG5();
	   					m2 = p->decodeJPEG5();
	   					m3 = p->decodeJPEG5();
	   					channels = {m1,m2,m3};
	   					merge(channels,result);
						imshow("Display",result);
						if(waitKey(30) >=0){
							break;
						}
						cout << "Decoding frame: " << b+1 << endl;
					}
	   				break;
	  			case 5:
					for(int b = 0; b<fra;b++){
						m1 = p->decodeJPEG6();
	   					m2 = p->decodeJPEG6();
	   					m3 = p->decodeJPEG6();
	   					channels = {m1,m2,m3};
	   					merge(channels,result);
						imshow("Display",result);
						if(waitKey(30) >=0){
							break;
						}
						cout << "Decoding frame: " << b+1 << endl;
					}
	   				break;
 	   			case 6:
					for(int b = 0; b<fra;b++){
						m1 = p->decodeJPEG7();
	   					m2 = p->decodeJPEG7();
	   					m3 = p->decodeJPEG7();
	   					channels = {m1,m2,m3};
	   					merge(channels,result);
						imshow("Display",result);
						if(waitKey(30) >=0){
							break;
						}
						cout << "Decoding frame: " << b+1 << endl;
					}
	   				break;	   					   					
	   			case 7:
					for(int b = 0; b<fra;b++){
						m1 = p->decodeJPEGLS();
	   					m2 = p->decodeJPEGLS();
	   					m3 = p->decodeJPEGLS();
	   					channels = {m1,m2,m3};
	   					merge(channels,result);
						imshow("Display",result);
						if(waitKey(30) >=0){
							break;
						}
						cout << "Decoding frame: " << b+1 << endl;
					}   				
	   				break;
	   				
	   		}
	   		return 1;
	   	}	
};



















