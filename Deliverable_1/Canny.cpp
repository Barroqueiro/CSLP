/*! \file Canny.cpp
 *	\brief Canny edge algorithm | flags: kernel size as an integer
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
	
	/// Opening the video
	VideoCapture cap(argv[1]);
	
	if (!cap.isOpened()){
		cout << "Error opening the file" << endl;
	}
	
	/// Displaying frame by frame
	while (1){
		
		cap >> image;
		
		/// Changing color space to gray
		int i = stoi(argv[2]);
		cvtColor(image,imageCan,COLOR_RGB2GRAY,0);
		
		/// Applying the canny function
		Canny(image,imageCan,i,i*2,3,false);

		/// Displaying the result
		imshow("Canny ", imageCan);
			
	   	char c = (char) waitKey(25);
		if( c == 27 ){
			break;
		}
	  }

	destroyAllWindows();
	return 0;
}
