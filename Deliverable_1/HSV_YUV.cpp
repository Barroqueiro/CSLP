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
	
	/// Loads an image
	image = imread(argv[1], IMREAD_COLOR ); 

	/// Check if image is loaded fine
	if( image.empty()) 
	{
		printf("Error opening image\n");
		return -1;
	}
	
	/// Extracting the color spaces
	cvtColor(image, imageHSV, COLOR_RGB2HSV);
	cvtColor(image, imageYUV, COLOR_RGB2YCrCb);
	
	/// Displaying the 3 images
	namedWindow("HSV", WINDOW_AUTOSIZE);
	imshow("HSV", imageHSV);
	
	namedWindow("YUV", WINDOW_AUTOSIZE);
	imshow("YUV", imageYUV);
	
	if (waitKey() == 27) {
        	return -1;
   	}	

	destroyAllWindows();
	return 0;
}
