/*! \file HSV_YUV.cpp
 *	\brief Transforming the image into different color spaces 
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
	/// Creating the variables to the original and altered images
	Mat image;
	Mat imageHSV;
	Mat imageYUV;
	
	/// Opening the video
	VideoCapture cap(argv[1]);
	
	if (!cap.isOpened()){
		cout << "Error opening the file" << endl;
	}
	
	/// Displaying frame by frame
	while (1){
		
		cap >> image;
	
		cvtColor(image, imageHSV, COLOR_RGB2HSV);
		cvtColor(image, imageYUV, COLOR_RGB2YUV);
		
		/// Displaying the 3 images
		namedWindow("HSV", WINDOW_AUTOSIZE);
		imshow("HSV", imageHSV);
		
		namedWindow("YUV", WINDOW_AUTOSIZE);
		imshow("YUV", imageYUV);
		
		char c = (char) waitKey(25);
		if( c == 27 ){
			break;
		}
	   }	

	destroyAllWindows();
	return 0;
}
