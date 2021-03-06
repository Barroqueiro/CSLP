/*! \file GausAndBlur.cpp
 *	\brief Applying diferent blur filters | Flags: -a (Average blur) ; -g (Gaussian blur) ; -m (Median blur) ; -b (Billateral blur) and with each add the size of the kernel
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
		
		char c = (char) waitKey(25);
		if( c == 27 ){
			break;
		}
	}
	
	destroyAllWindows();
	return 0;
}
