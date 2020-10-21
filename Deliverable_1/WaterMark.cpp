/*! \file WaterMark.cpp
 *	\brief Adding a Water Mark to an image
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
	
	
	VideoCapture cap(argv[1]);
	
	if (!cap.isOpened()){
		cout << "Error opening the file" << endl;
	}
	
	/// Displaying frame by frame
	while (1){
		Mat image;
		
		cap >> image;
	
		
		/// Creating the text to make the water mark
		string text = "WaterMark";
		
		/// Inserting the text in the image
		putText(image, text, Point(50,50) , FONT_ITALIC, 1, Scalar(256, 256, 256), 2);
		
		/// Displaying the image
		if(image.empty()){
			break;
		}
		
		imshow("Display", image); 
		
		char c = (char) waitKey(25);
		if( c == 27 ){
			break;
		}
	   }

	destroyAllWindows();
	return 0;
}
