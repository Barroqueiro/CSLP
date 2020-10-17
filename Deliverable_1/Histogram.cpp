/*! \file Histogram.cpp
 *	\brief Histogram of an image
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
	/// Create the matrix for the original image
	Mat src;
	
	/// Loads an image
	src = imread(argv[1], IMREAD_COLOR ); 

	/// Check if image is loaded fine
	if( src.empty()) 
	{
		printf("Error opening image\n");
		return -1;
	}
	
	/// Separate the image into its 3 planes
	vector<Mat> bgr_planes;
	split(src, bgr_planes);
	
	/// Configuration of the histogram
	int histSize = 256;
	
	float range[] = {0,256};
	const float* histRange = {range};
	
	bool uniform = true, accumulate = false;
	
	Mat b_hist, g_hist, r_hist;
	
	/// Calculate the histogram of each plane
    	calcHist( &bgr_planes[0], 1, 0, Mat(), b_hist, 1, &histSize, &histRange, uniform, accumulate );
    	calcHist( &bgr_planes[1], 1, 0, Mat(), g_hist, 1, &histSize, &histRange, uniform, accumulate );
    	calcHist( &bgr_planes[2], 1, 0, Mat(), r_hist, 1, &histSize, &histRange, uniform, accumulate );

	/// Creating the image of the histogram to be displayed
	int hist_w = 512, hist_h = 400;
    	int bin_w = cvRound( (double) hist_w/histSize );
    	Mat histImage( hist_h, hist_w, CV_8UC3, Scalar( 0,0,0) );
    	
    	/// Normalize the histogram
    	normalize(b_hist, b_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
    	normalize(g_hist, g_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
    	normalize(r_hist, r_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
    	
    	/// Draw each line of each plane
    	for( int i = 1; i < histSize; i++ )
    	{
        	line( histImage, Point( bin_w*(i-1), hist_h - cvRound(b_hist.at<float>(i-1)) ),
              		Point( bin_w*(i), hist_h - cvRound(b_hist.at<float>(i)) ),
              		Scalar( 255, 0, 0), 2, 8, 0  );
        	line( histImage, Point( bin_w*(i-1), hist_h - cvRound(g_hist.at<float>(i-1)) ),
              		Point( bin_w*(i), hist_h - cvRound(g_hist.at<float>(i)) ),
              		Scalar( 0, 255, 0), 2, 8, 0  );
       	line( histImage, Point( bin_w*(i-1), hist_h - cvRound(r_hist.at<float>(i-1)) ),
              		Point( bin_w*(i), hist_h - cvRound(r_hist.at<float>(i)) ),
              		Scalar( 0, 0, 255), 2, 8, 0  );
    	}
    	
    	/// Display the histogram
    	imshow("Source image: ", src );
    	imshow("Histogram", histImage );
    
    
	if (waitKey() == 27) {
        	return -1;
   	}	

	destroyAllWindows();
	return 0;
}
