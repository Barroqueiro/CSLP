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
	
	// Opening the video
	VideoCapture cap(argv[1]);
	
	if (!cap.isOpened()){
		cout << "Error opening the file" << endl;
	}
	
	/// Displaying frame by frame
	while (1){

		cap >> image;
	
		/// Image scale to gray
		cvtColor(image, imageGRAY, COLOR_RGB2GRAY);

		
		/// Displaying the image
		namedWindow("GRAY", WINDOW_AUTOSIZE);
		imshow("GRAY", imageGRAY);
		
	    	equalizeHist( imageGRAY, dst );
	    	imshow( "Equalized Image", dst );
		
		char c = (char) waitKey(25);
		if( c == 27 ){
			break;
		}
	   }	

	destroyAllWindows();
	return 0;
}
