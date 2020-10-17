/*! \file Canny.cpp
 *	\brief Canny edge algorithm
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
	
	Mat image;
	Mat imageCan;
	
	/// Loads an image
	image = imread(argv[1], IMREAD_COLOR ); 

	/// Check if image is loaded fine
	if( image.empty()) 
	{
		printf("Error opening image\n");
		return -1;
	}
	
	/// Changing color space to gray
	int i = stoi(argv[2]);
	cvtColor(image,imageCan,COLOR_RGB2GRAY,0);
	
	/// Applying the canny function
	Canny(image,imageCan,i,i*2,3,false);

	/// Displaying the result
	imshow("Canny ", imageCan);
	
	if (waitKey() == 27) {
        	return -1;
   	}	

	destroyAllWindows();
	return 0;
}
