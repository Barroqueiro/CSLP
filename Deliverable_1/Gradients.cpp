/*! \file Gradients.cpp
 *	\brief Getting the gradients
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
	Mat imageSx;
	Mat imageSy;
	Mat imageSCx;
	Mat imageSCy;
	Mat imageL;
	
	if(argc != 3){
		cout << "Usage: ./GausAndBlur [image_file] [flag] \n" << endl;
		cout << "Flags: \n" << endl;
		cout << "-s : Sobel Derivative " << endl;
		cout << "-sc: Scharr Derivatives " << endl;
		cout << "-l : Laplacian Derivatives " << endl;
		return 0;
	}
	/// Opening the video
	VideoCapture cap(argv[1]);
	
	if (!cap.isOpened()){
		cout << "Error opening the file" << endl;
	}
	
	/// Displaying frame by frame
	while (1){
		
		cap >> image;

		
		string flag = argv[2];
		cvtColor(image,image,COLOR_RGB2GRAY,0);
		if( flag.compare("-s") == 0)
		{
			Sobel(image, imageSx, CV_8U, 1, 0, 3, 1, 0, BORDER_DEFAULT);
			Sobel(image, imageSy, CV_8U, 0, 1, 3, 1, 0, BORDER_DEFAULT);
			imshow("Sobel x ", imageSx);
			imshow("Sobel y ", imageSy);
		}
		else if (flag.compare("-sc") == 0)
		{
			Scharr(image, imageSCx, CV_8U, 1, 0, 1, 0, BORDER_DEFAULT);
			Scharr(image, imageSCy, CV_8U, 0, 1, 1, 0, BORDER_DEFAULT);
			imshow("Scharr x ", imageSCx);
			imshow("Scharr y ", imageSCy);
		}
		else if(flag.compare("-l") == 0)
		{
			Laplacian(image, imageL, CV_8U, 1, 1, 0, BORDER_DEFAULT);
			imshow("Laplacian ", imageL);
		}
		else
		{
			cout << "Invalid Flag" << endl;
		}
		
		char c = (char) waitKey(25);
		if( c == 27 ){
			break;
		}
	   }

	destroyAllWindows();
	return 0;
}
