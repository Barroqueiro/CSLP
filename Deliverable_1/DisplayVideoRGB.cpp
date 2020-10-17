/** \file DisplayVideoRGB.cpp
 *	\brief Creating a simple video display 
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

using namespace cv;
using namespace std;


int main( int argc, char** argv )
{	
	/// Opening the video
	VideoCapture cap(argv[1]);
	
	if (!cap.isOpened()){
		cout << "Error opening the file" << endl;
	}
	
	/// Displaying frame by frame
	while (1){
		Mat frame;
		
		cap >> frame;
		
		/// If there is no frame, exit the loop
		if(frame.empty()){
			break;
		}
		
		imshow("Display", frame); 
		
		char c = (char) waitKey(25);
		if( c == 27 ){
			break;
		}
	}
	
	cap.release();
	
	destroyAllWindows();
	return 0;
}
