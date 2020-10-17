/*! \file DisplayVideo.cpp
 *	\brief Applying diferent blur filters
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
	/// Creating the variables to the original and altered images
	Mat image;
	Mat imageAVG;
	Mat imageGAUS;
	Mat imageMED;
	Mat imageBI;
	
	if(argc != 4){
		cout << "Usage: ./GausAndBlur [image_file] [flag] [size_of_the_kernel]\n" << endl;
		cout << "Flags: \n" << endl;
		cout << "-a : Average blur " << endl;
		cout << "-g : Gaussian blur " << endl;
		cout << "-m : Median blur " << endl;
		cout << "-b : Bilateral blur " << endl;
		return 0;
	}
	/// Loads an image
	image = imread(argv[1], IMREAD_COLOR ); 

	/// Check if image is loaded fine
	if( image.empty()) 
	{
		printf("Error opening image\n");
		return -1;
	}
	string flag = argv[2];
	int i = stoi(argv[3]);
	if( flag.compare("-a") == 0)
	{
		blur(image, imageAVG, Size(i,i));
		imshow("Average BLur", imageAVG);
	}
	else if (flag.compare("-g") == 0)
	{
		GaussianBlur(image, imageGAUS, Size(i,i), 0);
		imshow("Gaussian BLur", imageGAUS);
	}
	else if(flag.compare("-m") == 0)
	{
		medianBlur(image, imageMED,i);
		imshow("Median BLur", imageMED);
	}
	else if(flag.compare("-b") == 0)
	{
		bilateralFilter(image, imageBI,i,i*2,i/2);
		imshow("Bilateral BLur", imageBI);
	}
	else
	{
		cout << "Invalid Flag" << endl;
	}
	
	if (waitKey() == 27) {
        	return -1;
   	}	

	destroyAllWindows();
	return 0;
}
