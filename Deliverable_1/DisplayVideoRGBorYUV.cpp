/** \file DisplayVideoRGBorYUV.cpp
 *	\brief Video display iwth options for RGB, YUV, and the subsamples of 4:2:2 and 4:2:0 | flags: -rgb ; -yuv ; -yuv420 ; -yuv422
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
	/// Opening the video
	VideoCapture cap(argv[1]);
	
	if (!cap.isOpened()){
		cout << "Error opening the file" << endl;
	}
	
	string flag = argv[2];
	
	/// Displaying frame by frame
	while (1){
		Mat frame;
		
		string flag = argv[2];
		if( flag.compare("-rgb") == 0)
		{
			cap >> frame;
			/// If there is no frame, exit the loop
			if(frame.empty()){
				return 0;
			}
			imshow("Display",frame);
		}
		else if (flag.compare("-yuv") == 0)
		{
			cap >> frame;
			if(frame.empty()){
				return 0;
			}
			cvtColor(frame, frame, COLOR_RGB2YUV);
			imshow("Display",frame);
		}
		else if(flag.compare("-yuv422") == 0)
		{
			cap >> frame;
			if(frame.empty()){
				return 0;
			}
			cvtColor(frame, frame, COLOR_RGB2YUV );
			/// Partir a imagem nos seus planos
			vector<Mat> planes;
			split(frame, planes);
			/// Criar os planos subamostrados
			Mat new_plane_1(Size(planes[1].size().width,planes[1].size().height/2),0);
			Mat new_plane_2(Size(planes[1].size().width,planes[1].size().height/2),0);
			uchar* ptr_new_plane = planes[1].ptr<uchar>();
			for(int c = 0; c < planes[1].size().width; c++){
				for(int i = 0; i < planes[1].size().height; i+=2){
					new_plane_1.at<uchar>(Point(c,i/2)) = planes[1].at<uchar>(Point(c,i));
					new_plane_2.at<uchar>(Point(c,i/2)) = planes[2].at<uchar>(Point(c,i));
				}
			}
			/// Criar os planos para desfazer a subamostração
			Mat result_u(Size(planes[1].size().width,planes[1].size().height),0);
			Mat result_v(Size(planes[1].size().width,planes[1].size().height),0);
			for(int c = 0; c < planes[1].size().width; c++){
				for(int i = 0; i < planes[1].size().height; i++){
					result_u.at<uchar>(Point(c,i)) = new_plane_1.at<uchar>(Point(c,i/2));
					result_v.at<uchar>(Point(c,i)) = new_plane_2.at<uchar>(Point(c,i/2));
				}
			}
			Mat result;
			vector<Mat> channels = {planes[0],result_u,result_v};
			/// JUntar de novo todos os planos
			merge(channels,result);
			cvtColor(result, result, COLOR_YUV2RGB );
			imshow("Display",result);
		}
		else if(flag.compare("-yuv420") == 0)
		{
			cap >> frame;
			if(frame.empty()){
				return 0;
			}
			cvtColor(frame, frame, COLOR_RGB2YUV );
			vector<Mat> planes;
			split(frame, planes);
			Mat new_plane_1(Size(planes[1].size().width/2,planes[1].size().height/2),0);
			Mat new_plane_2(Size(planes[1].size().width/2,planes[1].size().height/2),0);
			for(int c = 0; c < planes[1].size().width; c+=2){
				for(int i = 0; i < planes[1].size().height; i+=2){
					new_plane_1.at<uchar>(Point(c/2,i/2)) = planes[1].at<uchar>(Point(c,i));
					new_plane_2.at<uchar>(Point(c/2,i/2)) = planes[2].at<uchar>(Point(c,i));
				}
			}
			Mat result_u(Size(planes[1].size().width,planes[1].size().height),0);
			Mat result_v(Size(planes[1].size().width,planes[1].size().height),0);
			for(int c = 0; c < planes[1].size().width; c++){
				for(int i = 0; i < planes[1].size().height; i++){
					result_u.at<uchar>(Point(c,i)) = new_plane_1.at<uchar>(Point(c/2,i/2));
					result_v.at<uchar>(Point(c,i)) = new_plane_2.at<uchar>(Point(c/2,i/2));
				}
			}
			Mat result;
			vector<Mat> channels = {planes[0],result_u,result_v};
			merge(channels,result);
			cvtColor(result, result, COLOR_YUV2RGB );
			imshow("Display",result);
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
	
	cap.release();
	
	destroyAllWindows();
	return 0;
}
