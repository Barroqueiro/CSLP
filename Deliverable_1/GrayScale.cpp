/*! \file GrayScale.cpp
 *	\brief Transforming the image into Gray scale and apply histogram equalization
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
	Mat imageGRAY;
	Mat dst;
	
	/// Loads an image
	image = imread(argv[1], IMREAD_COLOR ); 

	/// Check if image is loaded fine
	if( image.empty()) 
	{
		printf("Error opening image\n");
		return -1;
	}
	
	/// Image scale to gray
	cvtColor(image, imageGRAY, COLOR_RGB2GRAY);

	
	/// Displaying the image
	namedWindow("GRAY", WINDOW_AUTOSIZE);
	imshow("GRAY", imageGRAY);
	
    	equalizeHist( imageGRAY, dst );
    	imshow( "Equalized Image", dst );
	
	if (waitKey() == 27) {
        	return -1;
   	}	

	destroyAllWindows();
	return 0;
}
