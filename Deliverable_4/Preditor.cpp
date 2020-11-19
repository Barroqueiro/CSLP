/*! \file Preditor.cpp
 *	\brief Class to encode and decode Golomb code
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
#include <fstream> 
#include <vector>
#include <math.h>
#include "Golomb.cpp"
#include <string>

using namespace cv;
using namespace std;

/*! Golomb code class */
class Preditor {
	private:
		int m/*! Paramter m for the Glomb code */;
		int type/*! Flag to signal if we are reading or writing */;
		int typeVideo;
		int n_frames;
		int linhas;
		int colunas;
		int start;
		Golomb *g;
		WBitStream *wbs;
	public: 
	
	   	//! A constructor, Initiates the type of BitStream considering the flag, and atributes value to the class atributes
	    /*!
	      \param file A string with the name of the file
	      \param M Parameter m for the golomb code
	      \param Flag Flag to signal if we are reading(0) or Writing(1)
	    */
	   	Preditor(int Type, int M,string file,int TypeVideo,int n_f,int flag){
	   		if (flag == 1){// Encoding
		   		m = M;
		   		type = Type;
		   		typeVideo = TypeVideo;
		   		g = new Golomb(file,m,1);
		   		start = 1;
		   		n_frames = n_f;
		   		cout << "Write header" << endl;
		   		wbs = new WBitStream(file);
		   		wbs->writeNBits(typeVideo,8);
		   		wbs->writeNBits(type,8);
		   		wbs->writeNBits(M,8);
		   	}else{ // Decoding
		   		RBitStream rbs(file);
		   		typeVideo = rbs.readNBits(8);
		   		type = rbs.readNBits(8);
		   		m = rbs.readNBits(8);
		   		colunas = stoi(rbs.readString(4));
		   		linhas = stoi(rbs.readString(4));
		   		n_frames = stoi(rbs.readString(4));
		   		cout << "Type video: " << typeVideo << endl;
		   		cout << "Type : " << type<< endl;
		   		cout << "m: " << m << endl;
		   		cout << "colunas: " << colunas << endl;
		   		cout << "linhas: " << linhas<< endl;
		   		cout << "n_frames: " << n_frames<< endl;
		   		rbs.close();
		   		g = new Golomb(file,m,0);
				g->SkipNBytes(15);
				//cout << g->decode() << endl;
		   	}
	   	}
	   	
	   	//! Encode de number passed as a parameter, calculating the quotient and the remainder and ecoding inunary and binary respectively depending on the value of m
	    /*!
	      \param n The number to encode
	    */
	    
	    	int get_frames(){
	    		return n_frames;
	    	}
	    	
	    	int get_type(){
	    		return type;
	    	}
	    	
	    	
	   	void encodeJPEG1(Mat f){
	   		int lin = f.rows;
	   		int col = f.cols;
	   		if (start == 1){
		   		string width  = to_string(col);
		   		string height = to_string(lin);
		   		string frames = to_string(n_frames);
		   		for (int w = 0; w < 4-width.length();w++){
		   			width = "0"+width;
		   		}
		   		for (int h = 0; h < 4-height.length();h++){
		   			height = "0"+height;
		   		}
		   		for (int f = 0; f < 4-frames.length();f++){
		   			frames = "0"+frames;
		   		}
		   		wbs->writeString(width);
		   		wbs->writeString(height);
		   		wbs->writeString(frames);
		   		cout << "Processing video" << endl;
		   		start=0;
			}
			int pixel;
			int lastPixel = 0;
			int ind = 1;
			Mat res(lin,col,0);
			int count=0;
			for(int c = 0; c < lin; c++){
				for(int i = 0; i < col; i++){
					if(i-1 >= 0){
						lastPixel = (int) f.at<uchar>(c,i-1);
					}else{
						lastPixel = 0;
					}
					pixel = (int) f.at<uchar>(c,i);
					g->encode(pixel-lastPixel);
					//if(count < 57){
					//	cout << pixel-lastPixel << endl;
					//}
					res.at<uchar>(c,i) = (unsigned char) (pixel-lastPixel);
					count++;
				}
			}
			//cout << f << endl;
			//imshow("wtf",f);
			//waitKey(0);
	   	}
	   	
	   	void encodeJPEG2(Mat f){
	   		int lin = f.rows;
	   		int col = f.cols;
	   		if (start == 1){
		   		string width  = to_string(col);
		   		string height = to_string(lin);
		   		string frames = to_string(n_frames);
		   		for (int w = 0; w < 4-width.length();w++){
		   			width = "0"+width;
		   		}
		   		for (int h = 0; h < 4-height.length();h++){
		   			height = "0"+height;
		   		}
		   		for (int f = 0; f < 4-frames.length();f++){
		   			frames = "0"+frames;
		   		}
		   		wbs->writeString(width);
		   		wbs->writeString(height);
		   		wbs->writeString(frames);
		   		cout << "Processing video" << endl;
		   		start=0;
			}
			int pixel;
			int lastPixel = 0;
			int ind = 1;
			Mat res(lin,col,0);
			int count=0;
			for(int c = 0; c < lin; c++){
				for(int i = 0; i < col; i++){
					if(c-1 >= 0){
						lastPixel = (int) f.at<uchar>(c-1,i);
					}else{
						lastPixel = 0;
					}
					pixel = (int) f.at<uchar>(c,i);
					g->encode(pixel-lastPixel);
					//if(count < 57){
					//	cout << pixel-lastPixel << endl;
					//}
					res.at<uchar>(c,i) = (unsigned char) (pixel-lastPixel);
					count++;
				}
			}
			//cout << f << endl;
			//imshow("wtf",f);
			//waitKey(0);
	   	}
	   	
	   	void encodeJPEG3(Mat f){
	   		int lin = f.rows;
	   		int col = f.cols;
	   		if (start == 1){
		   		string width  = to_string(col);
		   		string height = to_string(lin);
		   		string frames = to_string(n_frames);
		   		for (int w = 0; w < 4-width.length();w++){
		   			width = "0"+width;
		   		}
		   		for (int h = 0; h < 4-height.length();h++){
		   			height = "0"+height;
		   		}
		   		for (int f = 0; f < 4-frames.length();f++){
		   			frames = "0"+frames;
		   		}
		   		wbs->writeString(width);
		   		wbs->writeString(height);
		   		wbs->writeString(frames);
		   		cout << "Processing video" << endl;
		   		start=0;
			}
			int pixel;
			int lastPixel = 0;
			int ind = 1;
			Mat res(lin,col,0);
			int count=0;
			for(int c = 0; c < lin; c++){
				for(int i = 0; i < col; i++){
					if(c-1 >= 0 && i-1>=0){
						lastPixel = (int) f.at<uchar>(c-1,i-1);
					}else{
						lastPixel = 0;
					}
					pixel = (int) f.at<uchar>(c,i);
					g->encode(pixel-lastPixel);
					//if(count < 57){
					//	cout << pixel-lastPixel << endl;
					//}
					res.at<uchar>(c,i) = (unsigned char) (pixel-lastPixel);
					count++;
				}
			}
			//cout << f << endl;
			//imshow("wtf",f);
			//waitKey(0);
	   	}
	   	
	   	void encodeJPEG4(Mat f){
	   		int lin = f.rows;
	   		int col = f.cols;
	   		if (start == 1){
		   		string width  = to_string(col);
		   		string height = to_string(lin);
		   		string frames = to_string(n_frames);
		   		for (int w = 0; w < 4-width.length();w++){
		   			width = "0"+width;
		   		}
		   		for (int h = 0; h < 4-height.length();h++){
		   			height = "0"+height;
		   		}
		   		for (int f = 0; f < 4-frames.length();f++){
		   			frames = "0"+frames;
		   		}
		   		wbs->writeString(width);
		   		wbs->writeString(height);
		   		wbs->writeString(frames);
		   		cout << "Processing video" << endl;
		   		start=0;
			}
			int pixel;
			int lastPixel = 0;
			int ind = 1;
			Mat res(lin,col,0);
			int count=0;
			int a;
			int b;
			int z;
			for(int c = 0; c < lin; c++){
				for(int i = 0; i < col; i++){
					pixel = f.at<uchar>(c,i);
					if(i-1 >=0){
						a=(int)f.at<uchar>(c,i-1);
					}else{
						a=0;
					}
					if(c-1 >=0){
						b=(int)f.at<uchar>(c-1,i);
					}else{
						b=0;
					}
					if(i-1 >=0 && c-1 >=0){
						z=(int)f.at<uchar>(c-1,i-1);
					}else{
						z=0;
					}
					lastPixel = a+b-z;
					g->encode(pixel-lastPixel);
					//if (count == 1){
					//	cout << pixel-lastPixel << endl;
					//}
					res.at<uchar>(c,i) = (unsigned char) (pixel-lastPixel);
					count++;
				}
			}
			//cout << f << endl;
			//imshow("wtf",f);
			//waitKey(0);
	   	}
	   	
	   	void encodeJPEG5(Mat f){
	   		int lin = f.rows;
	   		int col = f.cols;
	   		if (start == 1){
		   		string width  = to_string(col);
		   		string height = to_string(lin);
		   		string frames = to_string(n_frames);
		   		for (int w = 0; w < 4-width.length();w++){
		   			width = "0"+width;
		   		}
		   		for (int h = 0; h < 4-height.length();h++){
		   			height = "0"+height;
		   		}
		   		for (int f = 0; f < 4-frames.length();f++){
		   			frames = "0"+frames;
		   		}
		   		wbs->writeString(width);
		   		wbs->writeString(height);
		   		wbs->writeString(frames);
		   		cout << "Processing video" << endl;
		   		start=0;
			}
			int pixel;
			int lastPixel = 0;
			int ind = 1;
			Mat res(lin,col,0);
			int count=0;
			int a;
			int b;
			int z;
			for(int c = 0; c < lin; c++){
				for(int i = 0; i < col; i++){
					pixel = f.at<uchar>(c,i);
					if(i-1 >=0){
						a=(int)f.at<uchar>(c,i-1);
					}else{
						a=0;
					}
					if(c-1 >=0){
						b=(int)f.at<uchar>(c-1,i);
					}else{
						b=0;
					}
					if(i-1 >=0 && c-1 >=0){
						z=(int)f.at<uchar>(c-1,i-1);
					}else{
						z=0;
					}
					lastPixel = a+(b-z)/2;
					g->encode(pixel-lastPixel);
					//if (count == 1){
					//	cout << pixel-lastPixel << endl;
					//}
					res.at<uchar>(c,i) = (unsigned char) (pixel-lastPixel);
					count++;
				}
			}
			//cout << f << endl;
			//imshow("wtf",f);
			//waitKey(0);
	   	}
	   	
	   	void encodeJPEG6(Mat f){
	   		int lin = f.rows;
	   		int col = f.cols;
	   		if (start == 1){
		   		string width  = to_string(col);
		   		string height = to_string(lin);
		   		string frames = to_string(n_frames);
		   		for (int w = 0; w < 4-width.length();w++){
		   			width = "0"+width;
		   		}
		   		for (int h = 0; h < 4-height.length();h++){
		   			height = "0"+height;
		   		}
		   		for (int f = 0; f < 4-frames.length();f++){
		   			frames = "0"+frames;
		   		}
		   		wbs->writeString(width);
		   		wbs->writeString(height);
		   		wbs->writeString(frames);
		   		cout << "Processing video" << endl;
		   		start=0;
			}
			int pixel;
			int lastPixel = 0;
			int ind = 1;
			Mat res(lin,col,0);
			int count=0;
			int a;
			int b;
			int z;
			for(int c = 0; c < lin; c++){
				for(int i = 0; i < col; i++){
					pixel = f.at<uchar>(c,i);
					if(i-1 >=0){
						a=(int)f.at<uchar>(c,i-1);
					}else{
						a=0;
					}
					if(c-1 >=0){
						b=(int)f.at<uchar>(c-1,i);
					}else{
						b=0;
					}
					if(i-1 >=0 && c-1 >=0){
						z=(int)f.at<uchar>(c-1,i-1);
					}else{
						z=0;
					}
					lastPixel = b+(a-z)/2;
					g->encode(pixel-lastPixel);
					//if (count == 1){
					//	cout << pixel-lastPixel << endl;
					//}
					res.at<uchar>(c,i) = (unsigned char) (pixel-lastPixel);
					count++;
				}
			}
			//cout << f << endl;
			//imshow("wtf",f);
			//waitKey(0);
	   	}
	   	
	   	void encodeJPEG7(Mat f){
	   		int lin = f.rows;
	   		int col = f.cols;
	   		if (start == 1){
		   		string width  = to_string(col);
		   		string height = to_string(lin);
		   		string frames = to_string(n_frames);
		   		for (int w = 0; w < 4-width.length();w++){
		   			width = "0"+width;
		   		}
		   		for (int h = 0; h < 4-height.length();h++){
		   			height = "0"+height;
		   		}
		   		for (int f = 0; f < 4-frames.length();f++){
		   			frames = "0"+frames;
		   		}
		   		wbs->writeString(width);
		   		wbs->writeString(height);
		   		wbs->writeString(frames);
		   		cout << "Processing video" << endl;
		   		start=0;
			}
			int pixel;
			int lastPixel = 0;
			int ind = 1;
			Mat res(lin,col,0);
			int count=0;
			int a;
			int b;
			int z;
			for(int c = 0; c < lin; c++){
				for(int i = 0; i < col; i++){
					pixel = f.at<uchar>(c,i);
					if(i-1 >=0){
						a=(int)f.at<uchar>(c,i-1);
					}else{
						a=0;
					}
					if(c-1 >=0){
						b=(int)f.at<uchar>(c-1,i);
					}else{
						b=0;
					}
					if(i-1 >=0 && c-1 >=0){
						z=(int)f.at<uchar>(c-1,i-1);
					}else{
						z=0;
					}
					lastPixel = (a+b)/2;
					g->encode(pixel-lastPixel);
					//if (count == 1){
					//	cout << pixel-lastPixel << endl;
					//}
					res.at<uchar>(c,i) = (unsigned char) (pixel-lastPixel);
					count++;
				}
			}
			//cout << f << endl;
			//imshow("wtf",f);
			//waitKey(0);
	   	}
	   	
	   	void encodeJPEG_LS(Mat f){
	   		int col = f.size().width;
	   		int lin = f.size().height;
	   		if (start == 1){
		   		string width  = to_string(lin);
		   		string height = to_string(col);
		   		for (int w = 0; w < 4-width.length();w++){
		   			width = "0"+width;
		   		}
		   		for (int h = 0; h < 4-height.length();h++){
		   			height = "0"+height;
		   		}
			}
			int pixel;
			int lastPixel;
			int a=0;
			int b=0;
			int z=0;
			int maximo;
			int minimo;
			for(int c = 0; c < lin; c++){
				for(int i = 0; i < col; i++){
					pixel = f.at<uchar>(Point(c,i));
					if(i-1 >=0){
						a=f.at<uchar>(Point(c,i-1));
					}else{
						a=0;
					}
					if(c-1 >=0){
						b=f.at<uchar>(Point(c-1,i));
					}else{
						b=0;
					}
					if(i-1 >=0 && c-1 >=0){
						z=f.at<uchar>(Point(c-1,i-1));
					}else{
						z=0;
					}
					maximo = max(a,b);
					minimo = min(a,b);
					if(z>= maximo){
						lastPixel = minimo;					
					}else if(z <= minimo){
						lastPixel = maximo;
					}else{
						lastPixel = a+b-z;
					}
					g->encode(pixel-lastPixel);
				}
			}
	   	}
	   	
	   	Mat decodeJPEG1(){
	   		int lastPixel=0;
	   		Mat result(linhas,colunas,0);
			int count = 0;
			for(int c = 0; c < linhas; c++){
				for(int i = 0; i < colunas; i++){
					if(i-1 >= 0){
						lastPixel = (int) result.at<uchar>(c,i-1);
					}else{
						lastPixel = 0;
					}
					int d = g->decode();
					//if(count < 57){
					//	cout << d << endl;
					//}
					result.at<uchar>(c,i) = (unsigned char) (d+lastPixel);
					count++;				
				}
			}
			//cout << result << endl;
			//imshow("decode",result);
			//waitKey(0);
			return result;
	   	}
	   	
	   	Mat decodeJPEG2(){
	   		int lastPixel=0;
	   		Mat result(linhas,colunas,0);
			int count = 0;
			for(int c = 0; c < linhas; c++){
				for(int i = 0; i < colunas; i++){
					if(c-1 >= 0){
						lastPixel = (int) result.at<uchar>(c-1,i);
					}else{
						lastPixel = 0;
					}
					int d = g->decode();
					//if(count < 57){
					//	cout << d << endl;
					//}
					result.at<uchar>(c,i) = (unsigned char) (d+lastPixel);
					count++;				
				}
			}
			//cout << result << endl;
			//imshow("decode",result);
			//waitKey(0);
			return result;
	   	}
	   	
	   	Mat decodeJPEG3(){
	   		int lastPixel=0;
	   		Mat result(linhas,colunas,0);
			int count = 0;
			for(int c = 0; c < linhas; c++){
				for(int i = 0; i < colunas; i++){
					if(c-1 >= 0 && i-1>=0){
						lastPixel = (int) result.at<uchar>(c-1,i-1);
					}else{
						lastPixel = 0;
					}
					int d = g->decode();
					//if(count < 57){
					//	cout << d << endl;
					//}
					result.at<uchar>(c,i) = (unsigned char) (d+lastPixel);
					count++;				
				}
			}
			//cout << result << endl;
			//imshow("decode",result);
			//waitKey(0);
			return result;
	   	}
	   	
	   	Mat decodeJPEG4(){
	   		int lastPixel=0;
	   		Mat result(linhas,colunas,0);
			int count = 0;
			int a;
			int b;
			int z;
			for(int c = 0; c < linhas; c++){
				for(int i = 0; i < colunas; i++){
					if(i-1 >=0){
						a=(int)result.at<uchar>(c,i-1);
					}else{
						a=0;
					}
					if(c-1 >=0){
						b=(int)result.at<uchar>(c-1,i);
					}else{
						b=0;
					}
					if(i-1 >=0 && c-1 >=0){
						z=(int)result.at<uchar>(c-1,i-1);
					}else{
						z=0;
					}
					lastPixel = a+b-z;
					int d = g->decode();
					//if(count == 1){
					//	cout << d << endl;
					//}
					result.at<uchar>(c,i) = (unsigned char) (d+lastPixel);
					count++;				
				}
			}
			//cout << result << endl;
			//imshow("decode",result);
			//waitKey(0);
			return result;
	   	}
	   	
	   	Mat decodeJPEG5(){
	   		int lastPixel=0;
	   		Mat result(linhas,colunas,0);
			int count = 0;
			int a;
			int b;
			int z;
			for(int c = 0; c < linhas; c++){
				for(int i = 0; i < colunas; i++){
					if(i-1 >=0){
						a=(int)result.at<uchar>(c,i-1);
					}else{
						a=0;
					}
					if(c-1 >=0){
						b=(int)result.at<uchar>(c-1,i);
					}else{
						b=0;
					}
					if(i-1 >=0 && c-1 >=0){
						z=(int)result.at<uchar>(c-1,i-1);
					}else{
						z=0;
					}
					lastPixel = a+(b-z)/2;
					int d = g->decode();
					//if(count == 1){
					//	cout << d << endl;
					//}
					result.at<uchar>(c,i) = (unsigned char) (d+lastPixel);
					count++;				
				}
			}
			//cout << result << endl;
			//imshow("decode",result);
			//waitKey(0);
			return result;
	   	}
	   	
	   	Mat decodeJPEG6(){
	   		int lastPixel=0;
	   		Mat result(linhas,colunas,0);
			int count = 0;
			int a;
			int b;
			int z;
			for(int c = 0; c < linhas; c++){
				for(int i = 0; i < colunas; i++){
					if(i-1 >=0){
						a=(int)result.at<uchar>(c,i-1);
					}else{
						a=0;
					}
					if(c-1 >=0){
						b=(int)result.at<uchar>(c-1,i);
					}else{
						b=0;
					}
					if(i-1 >=0 && c-1 >=0){
						z=(int)result.at<uchar>(c-1,i-1);
					}else{
						z=0;
					}
					lastPixel = b+(a-z)/2;
					int d = g->decode();
					//if(count == 1){
					//	cout << d << endl;
					//}
					result.at<uchar>(c,i) = (unsigned char) (d+lastPixel);
					count++;				
				}
			}
			//cout << result << endl;
			//imshow("decode",result);
			//waitKey(0);
			return result;
	   	}
	   	
	   	Mat decodeJPEG7(){
	   		int lastPixel=0;
	   		Mat result(linhas,colunas,0);
			int count = 0;
			int a;
			int b;
			int z;
			for(int c = 0; c < linhas; c++){
				for(int i = 0; i < colunas; i++){
					if(i-1 >=0){
						a=(int)result.at<uchar>(c,i-1);
					}else{
						a=0;
					}
					if(c-1 >=0){
						b=(int)result.at<uchar>(c-1,i);
					}else{
						b=0;
					}
					if(i-1 >=0 && c-1 >=0){
						z=(int)result.at<uchar>(c-1,i-1);
					}else{
						z=0;
					}
					lastPixel = (a+b)/2;
					int d = g->decode();
					//if(count == 1){
					//	cout << d << endl;
					//}
					result.at<uchar>(c,i) = (unsigned char) (d+lastPixel);
					count++;				
				}
			}
			//cout << result << endl;
			//imshow("decode",result);
			//waitKey(0);
			return result;
	   	}
	   	
	   	
	   	//! simply close the write stream considering we mingh not encode a number of bits multiple of 8
	   	void close(){
	   	}
		
};



















