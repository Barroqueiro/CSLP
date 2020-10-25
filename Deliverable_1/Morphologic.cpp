/*! \file Morphologic.cpp
 *	\brief Morphological Operations | Flags : -e (Erosion) ; -d (Dilation) ; -o (Opening) ; -c (Closing) ; -g (Gradient) ; -t (Top Hat) ; -b (Black hat) and add the kernel to each one
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
	Mat imageRes;
	
	if(argc != 4){
		cout << "Usage: ./GausAndBlur [image_file] [flag] [Size_of_kernel] \n" << endl;
		cout << "Flags: \n" << endl;
		cout << "-e : Erosion " << endl;
		cout << "-d : Dilation " << endl;
		cout << "-o : Opening " << endl;
		cout << "-c : Closing " << endl;
		cout << "-g : Gradient " << endl;
		cout << "-t : Top Hat " << endl;
		cout << "-b : Black Hat " << endl;
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
		int i = stoi(argv[3]);
		if( flag.compare("-e") == 0)
		{
			erode(image,imageRes,Mat::ones(Size(i,i),CV_32F));
			imshow("Erosion ", imageRes);
		}
		else if (flag.compare("-d") == 0)
		{
			dilate(image,imageRes,Mat::ones(Size(i,i),CV_32F));
			imshow("Dilation ", imageRes);
		}
		else if(flag.compare("-o") == 0)
		{
			morphologyEx(image,imageRes,MORPH_OPEN,Mat::ones(Size(i,i),CV_32F));
			imshow("Opening ", imageRes);
		}
		else if(flag.compare("-c") == 0)
		{
			morphologyEx(image,imageRes,MORPH_CLOSE,Mat::ones(Size(i,i),CV_32F));
			imshow("Closing ", imageRes);
		}
		else if(flag.compare("-g") == 0)
		{
			morphologyEx(image,imageRes,MORPH_GRADIENT,Mat::ones(Size(i,i),CV_32F));
			imshow("Gradient ", imageRes);
		}
		else if(flag.compare("-t") == 0)
		{
			morphologyEx(image,imageRes,MORPH_TOPHAT,Mat::ones(Size(i,i),CV_32F));
			imshow("Top Hat ", imageRes);
		}
		else if(flag.compare("-b") == 0)
		{
			morphologyEx(image,imageRes,MORPH_BLACKHAT,Mat::ones(Size(i,i),CV_32F));
			imshow("Black Hat ", imageRes);
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
