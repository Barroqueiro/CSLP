/*! \file DisplayVideo.cpp
 *	\brief Adding a Water Mark to an image
 *	       
 *	
 *	File DisplayVideoRGB.cpp is a program to create a simple video display
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
	
	Mat image;
	
	// Loads an image
	image = imread(argv[1], IMREAD_COLOR ); 

	// Check if image is loaded fine
	if( image.empty()) 
	{
		printf("Error opening image\n");
		return -1;
	}
	
	/// Creating the text to make the water mark
	string text = "WaterMark";
	
	/// Inserting the text in the image
	putText(image, text, Point(50,50) , FONT_ITALIC, 1, Scalar(256, 256, 256), 2);
	
	/// Displaying the image
	namedWindow("Original", WINDOW_AUTOSIZE);
	imshow("Original", image);
	
	if (waitKey() == 27) {
        	return -1;
   	}	

	destroyAllWindows();
	return 0;
}
