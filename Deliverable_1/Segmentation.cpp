/*! \file Segmentation.cpp
 *	\brief Segmentations algorithms
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
	Mat imgLaplacian;
	
	/// Loads an image
	image = imread(argv[1], IMREAD_COLOR ); 

	/// Check if image is loaded fine
	if( image.empty()) 
	{
		printf("Error opening image\n");
		return -1;
	}
	
	/// Transforming the background to black, basicly just going pixel by pixel and turning white to black
	for ( int i = 0; i < image.rows; i++ ) {
		for ( int j = 0; j < image.cols; j++ ) {
		    if ( image.at<Vec3b>(i, j) == Vec3b(255,255,255) )
		    {
		        image.at<Vec3b>(i, j)[0] = 0;
		        image.at<Vec3b>(i, j)[1] = 0;
		        image.at<Vec3b>(i, j)[2] = 0;
		    }
		}
	    }
	
	/// Kernel to sharpen the image
	Mat kernel = (Mat_<float>(3,3) <<
                  1,  1, 1,
                  1, -8, 1,
                  1,  1, 1); 
        /// Do the laplacian filtering  
	filter2D(image, imgLaplacian, CV_32F, kernel);
	Mat sharp;
	image.convertTo(sharp, CV_32F);
	Mat imgResult = sharp - imgLaplacian;

	imgResult.convertTo(imgResult, CV_8UC3);
	imgLaplacian.convertTo(imgLaplacian, CV_8UC3);
	
	
	/// Making a binary image from the original image
	Mat bw;
    	cvtColor(imgResult, bw, COLOR_BGR2GRAY);
    	threshold(bw, bw, 40, 255, THRESH_BINARY | THRESH_OTSU);
    	
    	/// Perform the distance transform algorithm
    	Mat dist;
	distanceTransform(bw, dist, DIST_L2, 3);
	normalize(dist, dist, 0, 1.0, NORM_MINMAX);
	
	/// Making the markers for the foreground objects
	threshold(dist, dist, 0.4, 1.0, THRESH_BINARY);
	
    	Mat kernel1 = Mat::ones(3, 3, CV_8U);
    	dilate(dist, dist, kernel1);
    	imshow("Peaks", dist);
    	
    	/// Making the background markers
    	Mat dist_8u;
    	dist.convertTo(dist_8u, CV_8U);
    	vector<vector<Point> > contours;
    	findContours(dist_8u, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
    	/// Create the marker image for the watershed algorithm
    	Mat markers = Mat::zeros(dist.size(), CV_32S);
    	/// Draw the foreground markers
    	for (size_t i = 0; i < contours.size(); i++)
    	{
        	drawContours(markers, contours, static_cast<int>(i), Scalar(static_cast<int>(i)+1), -1);
    	}
    	
    	/// Draw the background marker
    	circle(markers, Point(5,5), 3, Scalar(255), -1);
    	
    	/// PUsing the watershed algorithm
    	watershed(imgResult, markers);
    	Mat mark;
    	markers.convertTo(mark, CV_8U);
    	bitwise_not(mark, mark);

    	/// Generating some random colors
    	vector<Vec3b> colors;
    	for (size_t i = 0; i < contours.size(); i++)
    	{
        	int b = theRNG().uniform(0, 256);
        	int g = theRNG().uniform(0, 256);
        	int r = theRNG().uniform(0, 256);
        	colors.push_back(Vec3b((uchar)b, (uchar)g, (uchar)r));
   	}
    	/// Create the result image
    	Mat dst = Mat::zeros(markers.size(), CV_8UC3);
    	/// Fill labeled objects with random colors
    	for (int i = 0; i < markers.rows; i++)
    	{
        	for (int j = 0; j < markers.cols; j++)
        	{
            		int index = markers.at<int>(i,j);
            		if (index > 0 && index <= static_cast<int>(contours.size()))
            		{
                		dst.at<Vec3b>(i,j) = colors[index-1];
            		}
        	}
    	}
    	
    	/// Seeing the final result
    	imshow("Final Result", dst);
    	
    	
	if (waitKey() == 27) {
        	return -1;
   	}	

	destroyAllWindows();
	return 0;
}
