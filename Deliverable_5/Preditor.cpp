/*! \file Preditor.cpp
 *	\brief Class to encode and decode frames from a video based on with predictive tecnique was used
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
		int type/*! type of the predictor */;
		int typeVideo/*! type of the video */;
		int n_frames/*! Number of frames of the video to be decoded*/;
		int linhas/*! Height of the frame we are decoding */;
		int colunas/*! Width of the frame we are decoding */;
		int start/*! Signaling if we are encoding from the start so we can write some more information on the header*/;
		int period/* period by which intra frames are encoded */;
		Golomb *g/*! Pointer to a golomb encoder to write/read the values we get from the preditor */;
		int block_size/*! Size of the blocks to encode*/;
		int search_space/*! Search space for which we cant search for a block to match */;
		vector<Mat> lastFrame/*! Last Frame encoded or decoded, this is needed since we are doing inter frame encoding and decoding*/;
		int od/*! offset in decoding */;
		string fileName;
		WBitStream *wbs/*! Write Bit stream to write the header to the file when we are encoding */;
	public: 
	
	   	//! A constructor, Initiates the atributes, if we are encoding then we match the elements passed to the respective atributes of the class, if we are decoding we set these atributes by reading the header of the file
	    /*!
	      \param Type The type of predictor to be used
	      \param M Parameter m for the golomb code
	      \param file String with the name of the file to encode
	      \param TypeVideo type of the video to encode
	      \param n_f number of frames we are encoding
	      \param flag flag to signal if we are encoding of decoding
	      \param bs block size
	      \param ss search size for the block encoding
	      \param outFile String with the file to encode to/decode from
	    */
	   	Preditor(int Type, int M,string file,int TypeVideo,int n_f,int flag,int bs,int ss,int peri){
	   		fileName=file;
	   		if (flag == 1){// Encoding
	   			block_size = bs;
	   			search_space = ss;
		   		m = M;
		   		type = Type;
		   		typeVideo = TypeVideo;
		   		start = 1;
		   		n_frames = n_f;
		   		period = peri;
		   		//cout << "Write header" << endl;
		   		wbs = new WBitStream(file);
		   		wbs->writeNBits(TypeVideo,8);
		   		wbs->writeNBits(type,8);
		   		wbs->writeNBits(M,8);
		   		wbs->writeNBits(bs,8);
		   		wbs->writeNBits(ss,8);
		   		wbs->writeNBits(period,8);
		   	}else{ // Decoding
		   		RBitStream rbs(file);
		   		od = 0;
		   		typeVideo = rbs.readNBits(8);
		   		type = rbs.readNBits(8);
		   		m = rbs.readNBits(8);
		   		block_size = rbs.readNBits(8);
		   		search_space = rbs.readNBits(8);
		   		period = rbs.readNBits(8);
		   		colunas = stoi(rbs.readString(4));
		   		linhas = stoi(rbs.readString(4));
		   		n_frames = stoi(rbs.readString(4));
		   		cout << "Type video: " << typeVideo << endl;
		   		cout << "Type : " << type<< endl;
		   		cout << "m: " << m << endl;
		   		cout << "colunas: " << colunas << endl;
		   		cout << "linhas: " << linhas<< endl;
		   		cout << "n_frames: " << n_frames<< endl;
		   		cout << "Search space: " << search_space << endl;
		   		cout << "Block size: " << block_size << endl;
		   		cout << "Period: " << period << endl;
		   		rbs.close();
		   		g = new Golomb(file,m,0);
				g->SkipNBytes(18);
				//cout << g ->decode() << endl;
		   	}
	   	}
	   	
	   	//! Getting the frames of the video we are decoding
	    
	    	int get_frames(){
	    		return n_frames;
	    	}
	    	
	    	//! Getting the type of the predictor we are using
	    	
	    	int get_type(){
	    		return type;
	    	}
	    	
	    	//! Getting the type of the video we are using
	    	
	    	int get_VideoType(){
	    		return typeVideo;
	    	}
	    	
	    	//! Setting the last frame encoded or decoded
	    	
	    	void set_last_frame(vector<Mat> planes){
			lastFrame = planes;
	    	}
	    	
	    	//! Getting the period of encoding intra frames
	    	
	    	int get_period(){
	    		return period;
	    	}
	    	
	    	void close(){
	    		g->close();
	    	}
	    	
	    	//! Function to encode by blocks the frame passed as paramter, gets all the blocks of each planes and matches a block from the last frame that best fits the current block, encodes de diference in position and the diference of each pixel
	    	/*!
	    	  \param planes frame to encode (all 3 channels)
	    	*/
	    	
	    	void encode_by_blocks(vector<Mat> planes){
	    		//cout << "Encoding frame by blocks" << endl;
	    		int lin;
	   		int col;
	   		int count = 0;
	   		int count_blocks= 0;
	   		Mat current_block;
	   		Mat possible_block;
	   		Mat min_block;
	   		int lastSum = 100000;
	   		int currentSum = 0;
	   		int d_x=0;
	   		int d_y=0;
	    		for(Mat matrix :planes){
	    			//cout << count << endl;
	    			//cout << matrix << endl;
	    			lin = matrix.rows;
	    			col = matrix.cols;
	    			for(int c = 0; c < lin; c+=block_size){
					for(int i = 0; i < col; i+=block_size){
						current_block = matrix.colRange(i,i+block_size).rowRange(c,c+block_size);
						for(int k = c-search_space; k < c+search_space+block_size; k++){
							for(int j = i-search_space; j < i +search_space+block_size  ; j++){
								if (k+block_size < lin && k >= 0 && j >= 0 && j+block_size < col){
									possible_block = lastFrame[count].colRange(j,j+block_size).rowRange(k,k+block_size);
									for(int a = 0; a < block_size; a++){
										for(int b = 0; b < block_size; b++){
											currentSum = currentSum + abs(((int)current_block.at<uchar>(a,b)-(int)possible_block.at<uchar>(a,b)));
										}
									}
									//cout << k << "," << j <<endl;
									//cout << currentSum << endl;
									if (currentSum < lastSum){
										//cout << "Nova soma" << endl;
										//cout << currentSum << endl;
										lastSum = currentSum;
										d_x = k;
										d_y = j;
										min_block = possible_block;
									}
								}
								currentSum = 0;
							}
						}
						lastSum = 10000;
						g->encode(c-d_x);
						g->encode(i-d_y);
						//cout << "Current block" << endl;
						//cout << c << "," << i << endl;
						//cout << current_block << endl;
						//cout << "Min block" << endl;
						//cout << d_x << "," << d_y << endl;
						//cout << min_block << endl;
						//exit(0);
						for(int n = 0; n < block_size; n++){
							for(int m = 0; m < block_size; m++){
								int z= (int)((int)min_block.at<uchar>(n,m)-(int)current_block.at<uchar>(n,m));
								//cout << ((int)min_block.at<uchar>(n,m))-((int)current_block.at<uchar>(n,m)) <<endl;
								g->encode(z);
							}
						}
						count_blocks++;
					}
				}
				count ++;
	    		}
	    		//cout << "Blocks: " << count_blocks << endl;
	    		//exit(0);
	    	}
	    	
	    	//! Function to decode by blocks the next frame in the binary file, gets each position and block numbers and matches with the last frame for the best match and reconstructs each plane of the frame
	    	
	    	vector<Mat> decode_by_blocks(){
	    	    	vector<Mat> res;
	    		if (typeVideo == 0){
	    			Mat m1(linhas,colunas,0);
	    			Mat m2(linhas,colunas,0);
	    			Mat m3(linhas,colunas,0);
	    			Mat current_block;
	    			int x;
	    			int y;
	    			int count = 0;
	    			for(int c = 0; c < linhas; c+=block_size){
					for(int i = 0; i < colunas; i+=block_size){
						x = -(g->decode()-c);
						y = -(g->decode()-i);
						current_block = lastFrame[count].colRange(y,y+block_size).rowRange(x,x+block_size);
						for(int a = 0; a < block_size; a++){
							for(int b = 0; b < block_size; b++){
							m1.at<uchar>(c+a,i+b) = (unsigned char)-(g->decode()-(int) current_block.at<uchar>(a,b));							
							}
						}				
					}
				}
				count++;
				for(int c = 0; c < linhas; c+=block_size){
					for(int i = 0; i < colunas; i+=block_size){
						x = -(g->decode()-c);
						y = -(g->decode()-i);
						current_block = lastFrame[count].colRange(y,y+block_size).rowRange(x,x+block_size);
						for(int a = 0; a < block_size; a++){
							for(int b = 0; b < block_size; b++){
								m2.at<uchar>(c+a,i+b) = (unsigned char)-(g->decode()-(int) current_block.at<uchar>(a,b));
							}
						}				
					}
				}
				count++;
				for(int c = 0; c < linhas; c+=block_size){
					for(int i = 0; i < colunas; i+=block_size){
						x = -(g->decode()-c);
						y = -(g->decode()-i);
						current_block = lastFrame[count].colRange(y,y+block_size).rowRange(x,x+block_size);
						for(int a = 0; a < block_size; a++){
							for(int b = 0; b < block_size; b++){
								m3.at<uchar>(c+a,i+b) = (unsigned char)-(g->decode()-(int) current_block.at<uchar>(a,b));
							}
						}				
					}
				}
				res = {m1,m2,m3};
	    			
	    		}else if (typeVideo == 1){
	    			Mat m1(linhas,colunas,0);
	    			Mat m2(linhas,colunas,0);
	    			Mat temp2(linhas,colunas/2,0);
	    			Mat m3(linhas,colunas,0);
	    			Mat temp3(linhas,colunas/2,0);
	    			Mat current_block;
	    			int x;
	    			int y;
	    			int count = 0;
	    			int decoded;
	    			int pixel;
	    			for(int c = 0; c < linhas; c+=block_size){
					for(int i = 0; i < colunas; i+=block_size){
						x = -(g->decode()-c);
						y = -(g->decode()-i);
						current_block = lastFrame[count].colRange(y,y+block_size).rowRange(x,x+block_size);
						for(int a = 0; a < block_size; a++){
							for(int b = 0; b < block_size; b++){
								m1.at<uchar>(c+a,i+b) = (unsigned char)-(g->decode()-(int) current_block.at<uchar>(a,b));							
							}
						}				
					}
				}
				count++;
				for(int c = 0; c < linhas; c+=block_size){
					for(int i = 0; i < colunas/2; i+=block_size){
						x = -(g->decode()-c);
						y = -(g->decode()-i);
						current_block = lastFrame[count].colRange(y,y+block_size).rowRange(x,x+block_size);
						for(int a = 0; a < block_size; a++){
							for(int b = 0; b < block_size; b++){
								decoded = g->decode();
								pixel = -(decoded-((int) current_block.at<uchar>(a,b)));
								temp2.at<uchar>(c+a,i+b) = (unsigned char)(pixel);
							}
						}				
					}
				}
				count++;
				for(int c = 0; c < linhas; c+=block_size){
					for(int i = 0; i < colunas/2; i+=block_size){
						x = -(g->decode()-c);
						y = -(g->decode()-i);
						current_block = lastFrame[count].colRange(y,y+block_size).rowRange(x,x+block_size);
						for(int a = 0; a < block_size; a++){
							for(int b = 0; b < block_size; b++){
								decoded = g->decode();
								pixel = -(decoded-(int) current_block.at<uchar>(a,b));
								temp3.at<uchar>(c+a,i+b) = (unsigned char)(pixel);
							}
						}				
					}
				}
				for(int c = 0; c < linhas; c++){
					for(int i = 0; i < colunas; i+=2){
						m2.at<uchar>(c,i) = temp2.at<uchar>(c,i/2);		
						m2.at<uchar>(c,i+1) = temp2.at<uchar>(c,i/2);		
						m3.at<uchar>(c,i) = temp3.at<uchar>(c,i/2);		
						m3.at<uchar>(c,i+1) = temp3.at<uchar>(c,i/2);	
					}
				}
				count++;
				res = {m1,m2,m3};
				set_last_frame({m1,temp2,temp3});
	    			
	    		}else if (typeVideo == 2){
	    			Mat m1(linhas,colunas,0);
	    			Mat m2(linhas,colunas,0);
	    			Mat temp2(linhas/2,colunas/2,0);
	    			Mat m3(linhas,colunas,0);
	    			Mat temp3(linhas/2,colunas/2,0);
	    			Mat current_block;
	    			int x;
	    			int y;
	    			int count = 0;
	    			int decoded;
	    			int pixel;
	    			for(int c = 0; c < linhas; c+=block_size){
					for(int i = 0; i < colunas; i+=block_size){
						x = -(g->decode()-c);
						y = -(g->decode()-i);
						current_block = lastFrame[count].colRange(y,y+block_size).rowRange(x,x+block_size);
						for(int a = 0; a < block_size; a++){
							for(int b = 0; b < block_size; b++){
								m1.at<uchar>(c+a,i+b) = (unsigned char)-(g->decode()-(int) current_block.at<uchar>(a,b));							
							}
						}				
					}
				}
				count++;
				for(int c = 0; c < linhas/2; c+=block_size){
					for(int i = 0; i < colunas/2; i+=block_size){
						x = -(g->decode()-c);
						y = -(g->decode()-i);
						current_block = lastFrame[count].colRange(y,y+block_size).rowRange(x,x+block_size);
						for(int a = 0; a < block_size; a++){
							for(int b = 0; b < block_size; b++){
								decoded = g->decode();
								pixel = -(decoded-((int) current_block.at<uchar>(a,b)));
								temp2.at<uchar>(c+a,i+b) = (unsigned char)(pixel);
							}
						}				
					}
				}
				count++;
				for(int c = 0; c < linhas/2; c+=block_size){
					for(int i = 0; i < colunas/2; i+=block_size){
						x = -(g->decode()-c);
						y = -(g->decode()-i);
						current_block = lastFrame[count].colRange(y,y+block_size).rowRange(x,x+block_size);
						for(int a = 0; a < block_size; a++){
							for(int b = 0; b < block_size; b++){
								decoded = g->decode();
								pixel = -(decoded-(int) current_block.at<uchar>(a,b));
								temp3.at<uchar>(c+a,i+b) = (unsigned char)(pixel);
							}
						}				
					}
				}
				for(int c = 0; c < linhas; c+=2){
					for(int i = 0; i < colunas; i+=2){
						m2.at<uchar>(c,i) = temp2.at<uchar>(c/2,i/2);	
						m2.at<uchar>(c+1,i) = temp2.at<uchar>(c/2,i/2);	
						m2.at<uchar>(c,i+1) = temp2.at<uchar>(c/2,i/2);	
						m2.at<uchar>(c+1,i+1) = temp2.at<uchar>(c/2,i/2);	
						m3.at<uchar>(c,i) = temp3.at<uchar>(c/2,i/2);	
						m3.at<uchar>(c+1,i) = temp3.at<uchar>(c/2,i/2);	
						m3.at<uchar>(c,i+1) = temp3.at<uchar>(c/2,i/2);	
						m3.at<uchar>(c+1,i+1) = temp3.at<uchar>(c/2,i/2);
					}
				}
				count++;
				res = {m1,m2,m3};
				set_last_frame({m1,temp2,temp3});
	    			
	    		}
	    		return res;
	    	}
	    	
	    //! Encode de frame passed as a parameter by predicting encoding from JPEG1
	    /*!
	      \param f Mat type object to encode to the file
	    */
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
		   		wbs->closeNoWrite();
		   		g = new Golomb(fileName,m,1);
		   		//cout << "Processing video" << endl;
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
					res.at<uchar>(c,i) = (unsigned char) (pixel-lastPixel);
					count++;
				}
			}
	   	}
	   	
	   //! Encode de frame passed as a parameter by predicting encoding from JPEG2
	    /*!
	      \param f Mat type object to encode to the file
	    */
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
		   		wbs->closeNoWrite();
		   		g = new Golomb(fileName,m,1);
		   		//cout << "Processing video" << endl;
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
					res.at<uchar>(c,i) = (unsigned char) (pixel-lastPixel);
					count++;
				}
			}
	   	}
	   	
	   //! Encode de frame passed as a parameter by predicting encoding from JPEG3
	    /*!
	      \param f Mat type object to encode to the file
	    */
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
		   		wbs->closeNoWrite();
		   		g = new Golomb(fileName,m,1);
		   		//cout << "Processing video" << endl;
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
					res.at<uchar>(c,i) = (unsigned char) (pixel-lastPixel);
					count++;
				}
			}
	   	}
	   	
	   //! Encode de frame passed as a parameter by predicting encoding from JPEG4
	    /*!
	      \param f Mat type object to encode to the file
	    */
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
		   		wbs->closeNoWrite();
		   		g = new Golomb(fileName,m,1);
		   		//cout << "Processing video" << endl;
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
					res.at<uchar>(c,i) = (unsigned char) (pixel-lastPixel);
					count++;
				}
			}
	   	}
	   	
	   	
	   //! Encode de frame passed as a parameter by predicting encoding from JPEG5
	    /*!
	      \param f Mat type object to encode to the file
	    */
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
		   		wbs->closeNoWrite();
		   		g = new Golomb(fileName,m,1);
		   		//cout << "Processing video" << endl;
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
					res.at<uchar>(c,i) = (unsigned char) (pixel-lastPixel);
					count++;
				}
			}
	   	}
	   	
	   	
	   //! Encode de frame passed as a parameter by predicting encoding from JPEG6
	    /*!
	      \param f Mat type object to encode to the file
	    */
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
		   		wbs->closeNoWrite();
		   		g = new Golomb(fileName,m,1);
		   		//cout << "Processing video" << endl;
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
					res.at<uchar>(c,i) = (unsigned char) (pixel-lastPixel);
					count++;
				}
			}
	   	}
	   	
	   //! Encode de frame passed as a parameter by predicting encoding from JPEG7
	    /*!
	      \param f Mat type object to encode to the file
	    */
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
		   		wbs->closeNoWrite();
		   		g = new Golomb(fileName,m,1);
		   		//cout << "Processing video" << endl;
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
					res.at<uchar>(c,i) = (unsigned char) (pixel-lastPixel);
					count++;
				}
			}
	   	}
	   	
	   //! Encode de frame passed as a parameter by predicting encoding from JPEGLS
	    /*!
	      \param f Mat type object to encode to the file
	    */
	   	void encodeJPEG_LS(Mat f){
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
		   		wbs->closeNoWrite();
		   		g = new Golomb(fileName,m,1);
		   		//cout << "Processing video" << endl;
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
			int maximo;
			int minimo;
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
					res.at<uchar>(c,i) = (unsigned char) (pixel-lastPixel);
					count++;
				}
			}
	   	}
	   	
	   	//! Decode the next frame on the file using the predictor from JPEG1
	   	Mat decodeJPEG1(){
	   		Mat result;
	   		if (typeVideo == 0){
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
						result.at<uchar>(c,i) = (unsigned char) (d+lastPixel);
						count++;				
					}
				}
				return result;
			} else if (typeVideo == 1) {
				if (od % 3 == 0){
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
							result.at<uchar>(c,i) = (unsigned char) (d+lastPixel);
							count++;				
						}
					}
					od++;
					return result;
				}else if (od % 3 == 1 || od % 3 == 2) {
					int lastPixel=0;
		   			Mat result(linhas,colunas,0);
					int count = 0;
					for(int c = 0; c < linhas; c++){
						for(int i = 0; i < colunas; i+=2){
							if(i-1 >= 0){
								lastPixel = (int) result.at<uchar>(c,i-1);
							}else{
								lastPixel = 0;
							}
							int d = g->decode();
							result.at<uchar>(c,i) = (unsigned char) (d+lastPixel);
							result.at<uchar>(c,i+1) = (unsigned char) (d+lastPixel);
							count++;				
						}
					}
					od++;
					return result;
				}
			} else if (typeVideo == 2) {
				if (od % 3 == 0){
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
							result.at<uchar>(c,i) = (unsigned char) (d+lastPixel);
							count++;				
						}
					}
					od++;
					return result;
				}else if (od % 3 == 1 || od % 3 == 2) {
					int lastPixel=0;
		   			Mat result(linhas,colunas,0);
					int count = 0;
					for(int c = 0; c < linhas; c+=2){
						for(int i = 0; i < colunas; i+=2){
							if(i-1 >= 0){
								lastPixel = (int) result.at<uchar>(c,i-1);
							}else{
								lastPixel = 0;
							}
							int d = g->decode();
							result.at<uchar>(c,i) = (unsigned char) (d+lastPixel);
							result.at<uchar>(c,i+1) = (unsigned char) (d+lastPixel);
							result.at<uchar>(c+1,i) = (unsigned char) (d+lastPixel);
							result.at<uchar>(c+1,i+1) = (unsigned char) (d+lastPixel);
							count++;				
						}
					}
					od++;
					return result;
				}
			}
			return result;
	   	}
	   	
	   	//! Decode the next frame on the file using the predictor from JPEG2
	   	Mat decodeJPEG2(){
	   		Mat result;
	   		if (typeVideo == 0){
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
						result.at<uchar>(c,i) = (unsigned char) (d+lastPixel);
						count++;				
					}
				}
				return result;
			} else if (typeVideo == 1) {
				if (od % 3 == 0){
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
							result.at<uchar>(c,i) = (unsigned char) (d+lastPixel);
							count++;				
						}
					}
					od++;
					return result;
				}else if (od % 3 == 1 || od % 3 == 2) {
					int lastPixel=0;
		   			Mat result(linhas,colunas,0);
					int count = 0;
					for(int c = 0; c < linhas; c++){
						for(int i = 0; i < colunas; i+=2){
							if(c-1 >= 0){
								lastPixel = (int) result.at<uchar>(c-1,i);
							}else{
								lastPixel = 0;
							}
							int d = g->decode();
							result.at<uchar>(c,i) = (unsigned char) (d+lastPixel);
							result.at<uchar>(c,i+1) = (unsigned char) (d+lastPixel);
							count++;				
						}
					}
					od++;
					return result;
				}
			} else if (typeVideo == 2) {
				if (od % 3 == 0){
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
							result.at<uchar>(c,i) = (unsigned char) (d+lastPixel);
							count++;				
						}
					}
					od++;
					return result;
				}else if (od % 3 == 1 || od % 3 == 2) {
					int lastPixel=0;
		   			Mat result(linhas,colunas,0);
					int count = 0;
					for(int c = 0; c < linhas; c+=2){
						for(int i = 0; i < colunas; i+=2){
							if(c-1 >= 0){
								lastPixel = (int) result.at<uchar>(c-1,i);
							}else{
								lastPixel = 0;
							}
							int d = g->decode();
							result.at<uchar>(c,i) = (unsigned char) (d+lastPixel);
							result.at<uchar>(c,i+1) = (unsigned char) (d+lastPixel);
							result.at<uchar>(c+1,i) = (unsigned char) (d+lastPixel);
							result.at<uchar>(c+1,i+1) = (unsigned char) (d+lastPixel);
							count++;				
						}
					}
					od++;
					return result;
				}
			}
			return result;
	   	}
	   	
	   	//! Decode the next frame on the file using the predictor from JPEG3
	   	Mat decodeJPEG3(){
	   		Mat result;
	   		if (typeVideo == 0){
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
						result.at<uchar>(c,i) = (unsigned char) (d+lastPixel);
						count++;				
					}
				}
				return result;
			} else if (typeVideo == 1) {
				if (od % 3 == 0){
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
							result.at<uchar>(c,i) = (unsigned char) (d+lastPixel);
							count++;				
						}
					}
					od++;
					return result;
				}else if (od % 3 == 1 || od % 3 == 2) {
					int lastPixel=0;
		   			Mat result(linhas,colunas,0);
					int count = 0;
					for(int c = 0; c < linhas; c++){
						for(int i = 0; i < colunas; i+=2){
							if(c-1 >= 0 && i-1>=0){
								lastPixel = (int) result.at<uchar>(c-1,i-1);
							}else{
								lastPixel = 0;
							}
							int d = g->decode();
							result.at<uchar>(c,i) = (unsigned char) (d+lastPixel);
							result.at<uchar>(c,i+1) = (unsigned char) (d+lastPixel);
							count++;				
						}
					}
					od++;
					return result;
				}
			} else if (typeVideo == 2) {
				if (od % 3 == 0){
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
							result.at<uchar>(c,i) = (unsigned char) (d+lastPixel);
							count++;				
						}
					}
					od++;
					return result;
				}else if (od % 3 == 1 || od % 3 == 2) {
					int lastPixel=0;
		   			Mat result(linhas,colunas,0);
					int count = 0;
					for(int c = 0; c < linhas; c+=2){
						for(int i = 0; i < colunas; i+=2){
							if(c-1 >= 0 && i-1>=0){
								lastPixel = (int) result.at<uchar>(c-1,i-1);
							}else{
								lastPixel = 0;
							}
							int d = g->decode();
							result.at<uchar>(c,i) = (unsigned char) (d+lastPixel);
							result.at<uchar>(c,i+1) = (unsigned char) (d+lastPixel);
							result.at<uchar>(c+1,i) = (unsigned char) (d+lastPixel);
							result.at<uchar>(c+1,i+1) = (unsigned char) (d+lastPixel);
							count++;				
						}
					}
					od++;
					return result;
				}
			}
			return result;
	   	}
	   	
	   	//! Decode the next frame on the file using the predictor from JPEG4
	   	Mat decodeJPEG4(){
	   		Mat result;
	   		int a;
			int b;
			int z;
	   		if (typeVideo == 0){
		   		int lastPixel=0;
		   		Mat result(linhas,colunas,0);
				int count = 0;
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
						result.at<uchar>(c,i) = (unsigned char) (d+lastPixel);
						count++;				
					}
				}
				return result;
			} else if (typeVideo == 1) {
				if (od % 3 == 0){
					int lastPixel=0;
		   			Mat result(linhas,colunas,0);
					int count = 0;
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
							result.at<uchar>(c,i) = (unsigned char) (d+lastPixel);
							count++;				
						}
					}
					od++;
					return result;
				}else if (od % 3 == 1 || od % 3 == 2) {
					int lastPixel=0;
		   			Mat result(linhas,colunas,0);
					int count = 0;
					for(int c = 0; c < linhas; c++){
						for(int i = 0; i < colunas; i+=2){
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
							result.at<uchar>(c,i) = (unsigned char) (d+lastPixel);
							result.at<uchar>(c,i+1) = (unsigned char) (d+lastPixel);
							count++;				
						}
					}
					od++;
					return result;
				}
			} else if (typeVideo == 2) {
				if (od % 3 == 0){
					int lastPixel=0;
		   			Mat result(linhas,colunas,0);
					int count = 0;
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
							result.at<uchar>(c,i) = (unsigned char) (d+lastPixel);
							count++;				
						}
					}
					od++;
					return result;
				}else if (od % 3 == 1 || od % 3 == 2) {
					int lastPixel=0;
		   			Mat result(linhas,colunas,0);
					int count = 0;
					for(int c = 0; c < linhas; c+=2){
						for(int i = 0; i < colunas; i+=2){
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
							result.at<uchar>(c,i) = (unsigned char) (d+lastPixel);
							result.at<uchar>(c,i+1) = (unsigned char) (d+lastPixel);
							result.at<uchar>(c+1,i) = (unsigned char) (d+lastPixel);
							result.at<uchar>(c+1,i+1) = (unsigned char) (d+lastPixel);
							count++;				
						}
					}
					od++;
					return result;
				}
			}
			return result;
	   	}
	   	
	   	//! Decode the next frame on the file using the predictor from JPEG5
	   	Mat decodeJPEG5(){
	   		Mat result;
	   		int a;
			int b;
			int z;
	   		if (typeVideo == 0){
		   		int lastPixel=0;
		   		Mat result(linhas,colunas,0);
				int count = 0;
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
						result.at<uchar>(c,i) = (unsigned char) (d+lastPixel);
						count++;				
					}
				}
				return result;
			} else if (typeVideo == 1) {
				if (od % 3 == 0){
					int lastPixel=0;
		   			Mat result(linhas,colunas,0);
					int count = 0;
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
							result.at<uchar>(c,i) = (unsigned char) (d+lastPixel);
							count++;				
						}
					}
					od++;
					return result;
				}else if (od % 3 == 1 || od % 3 == 2) {
					int lastPixel=0;
		   			Mat result(linhas,colunas,0);
					int count = 0;
					for(int c = 0; c < linhas; c++){
						for(int i = 0; i < colunas; i+=2){
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
							result.at<uchar>(c,i) = (unsigned char) (d+lastPixel);
							result.at<uchar>(c,i+1) = (unsigned char) (d+lastPixel);
							count++;				
						}
					}
					od++;
					return result;
				}
			} else if (typeVideo == 2) {
				if (od % 3 == 0){
					int lastPixel=0;
		   			Mat result(linhas,colunas,0);
					int count = 0;
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
							result.at<uchar>(c,i) = (unsigned char) (d+lastPixel);
							count++;				
						}
					}
					od++;
					return result;
				}else if (od % 3 == 1 || od % 3 == 2) {
					int lastPixel=0;
		   			Mat result(linhas,colunas,0);
					int count = 0;
					for(int c = 0; c < linhas; c+=2){
						for(int i = 0; i < colunas; i+=2){
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
							result.at<uchar>(c,i) = (unsigned char) (d+lastPixel);
							result.at<uchar>(c,i+1) = (unsigned char) (d+lastPixel);
							result.at<uchar>(c+1,i) = (unsigned char) (d+lastPixel);
							result.at<uchar>(c+1,i+1) = (unsigned char) (d+lastPixel);
							count++;				
						}
					}
					od++;
					return result;
				}
			}
			return result;
	   	}
	   	
	   	//! Decode the next frame on the file using the predictor from JPEG6
	   	Mat decodeJPEG6(){
	   		Mat result;
	   		int a;
			int b;
			int z;
	   		if (typeVideo == 0){
		   		int lastPixel=0;
		   		Mat result(linhas,colunas,0);
				int count = 0;
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
						result.at<uchar>(c,i) = (unsigned char) (d+lastPixel);
						count++;				
					}
				}
				return result;
			} else if (typeVideo == 1) {
				if (od % 3 == 0){
					int lastPixel=0;
		   			Mat result(linhas,colunas,0);
					int count = 0;
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
							result.at<uchar>(c,i) = (unsigned char) (d+lastPixel);
							count++;				
						}
					}
					od++;
					return result;
				}else if (od % 3 == 1 || od % 3 == 2) {
					int lastPixel=0;
		   			Mat result(linhas,colunas,0);
					int count = 0;
					for(int c = 0; c < linhas; c++){
						for(int i = 0; i < colunas; i+=2){
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
							result.at<uchar>(c,i) = (unsigned char) (d+lastPixel);
							result.at<uchar>(c,i+1) = (unsigned char) (d+lastPixel);
							count++;				
						}
					}
					od++;
					return result;
				}
			} else if (typeVideo == 2) {
				if (od % 3 == 0){
					int lastPixel=0;
		   			Mat result(linhas,colunas,0);
					int count = 0;
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
							result.at<uchar>(c,i) = (unsigned char) (d+lastPixel);
							count++;				
						}
					}
					od++;
					return result;
				}else if (od % 3 == 1 || od % 3 == 2) {
					int lastPixel=0;
		   			Mat result(linhas,colunas,0);
					int count = 0;
					for(int c = 0; c < linhas; c+=2){
						for(int i = 0; i < colunas; i+=2){
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
							result.at<uchar>(c,i) = (unsigned char) (d+lastPixel);
							result.at<uchar>(c,i+1) = (unsigned char) (d+lastPixel);
							result.at<uchar>(c+1,i) = (unsigned char) (d+lastPixel);
							result.at<uchar>(c+1,i+1) = (unsigned char) (d+lastPixel);
							count++;				
						}
					}
					od++;
					return result;
				}
			}
			return result;
	   	}
	   	
	   	//! Decode the next frame on the file using the predictor from JPEG7
	   	Mat decodeJPEG7(){
	   		Mat result;
	   		int a;
			int b;
			int z;
	   		if (typeVideo == 0){
		   		int lastPixel=0;
		   		Mat result(linhas,colunas,0);
				int count = 0;
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
						result.at<uchar>(c,i) = (unsigned char) (d+lastPixel);
						count++;				
					}
				}
				return result;
			} else if (typeVideo == 1) {
				if (od % 3 == 0){
					int lastPixel=0;
		   			Mat result(linhas,colunas,0);
					int count = 0;
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
							result.at<uchar>(c,i) = (unsigned char) (d+lastPixel);
							count++;				
						}
					}
					od++;
					return result;
				}else if (od % 3 == 1 || od % 3 == 2) {
					int lastPixel=0;
		   			Mat result(linhas,colunas,0);
					int count = 0;
					for(int c = 0; c < linhas; c++){
						for(int i = 0; i < colunas; i+=2){
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
							result.at<uchar>(c,i) = (unsigned char) (d+lastPixel);
							result.at<uchar>(c,i+1) = (unsigned char) (d+lastPixel);
							count++;				
						}
					}
					od++;
					return result;
				}
			} else if (typeVideo == 2) {
				if (od % 3 == 0){
					int lastPixel=0;
		   			Mat result(linhas,colunas,0);
					int count = 0;
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
							result.at<uchar>(c,i) = (unsigned char) (d+lastPixel);
							count++;				
						}
					}
					od++;
					return result;
				}else if (od % 3 == 1 || od % 3 == 2) {
					int lastPixel=0;
		   			Mat result(linhas,colunas,0);
					int count = 0;
					for(int c = 0; c < linhas; c+=2){
						for(int i = 0; i < colunas; i+=2){
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
							result.at<uchar>(c,i) = (unsigned char) (d+lastPixel);
							result.at<uchar>(c,i+1) = (unsigned char) (d+lastPixel);
							result.at<uchar>(c+1,i) = (unsigned char) (d+lastPixel);
							result.at<uchar>(c+1,i+1) = (unsigned char) (d+lastPixel);
							count++;				
						}
					}
					od++;
					return result;
				}
			}
			return result;
	   	}
	   	
	   	//! Decode the next frame on the file using the predictor from JPEGLS
	   	Mat decodeJPEGLS(){
	   		Mat result;
	   		int lastPixel = 0;
	   		int a;
			int b;
			int z;
			int maximo;
			int minimo;
	   		if (typeVideo == 0){
		   		int lastPixel=0;
		   		Mat result(linhas,colunas,0);
				int count = 0;
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
						maximo = max(a,b);
						minimo = min(a,b);
						if(z>= maximo){
							lastPixel = minimo;					
						}else if(z <= minimo){
							lastPixel = maximo;
						}else{
							lastPixel = a+b-z;
						}
						int d = g->decode();
						result.at<uchar>(c,i) = (unsigned char) (d+lastPixel);
						count++;				
					}
				}
				return result;
			} else if (typeVideo == 1) {
				if (od % 3 == 0){
					int lastPixel=0;
		   			Mat result(linhas,colunas,0);
					int count = 0;
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
							maximo = max(a,b);
							minimo = min(a,b);
							if(z>= maximo){
								lastPixel = minimo;					
							}else if(z <= minimo){
								lastPixel = maximo;
							}else{
								lastPixel = a+b-z;
							}
							int d = g->decode();
							result.at<uchar>(c,i) = (unsigned char) (d+lastPixel);
							count++;				
						}
					}
					od++;
					return result;
				}else if (od % 3 == 1 || od % 3 == 2) {
					int lastPixel=0;
		   			Mat result(linhas,colunas,0);
					int count = 0;
					for(int c = 0; c < linhas; c++){
						for(int i = 0; i < colunas; i+=2){
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
							maximo = max(a,b);
							minimo = min(a,b);
							if(z>= maximo){
								lastPixel = minimo;					
							}else if(z <= minimo){
								lastPixel = maximo;
							}else{
								lastPixel = a+b-z;
							}
							int d = g->decode();
							result.at<uchar>(c,i) = (unsigned char) (d+lastPixel);
							result.at<uchar>(c,i+1) = (unsigned char) (d+lastPixel);
							count++;				
						}
					}
					od++;
					return result;
				}
			} else if (typeVideo == 2) {
				if (od % 3 == 0){
					int lastPixel=0;
		   			Mat result(linhas,colunas,0);
					int count = 0;
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
							maximo = max(a,b);
							minimo = min(a,b);
							if(z>= maximo){
								lastPixel = minimo;					
							}else if(z <= minimo){
								lastPixel = maximo;
							}else{
								lastPixel = a+b-z;
							}
							int d = g->decode();
							result.at<uchar>(c,i) = (unsigned char) (d+lastPixel);
							count++;				
						}
					}
					od++;
					return result;
				}else if (od % 3 == 1 || od % 3 == 2) {
					int lastPixel=0;
		   			Mat result(linhas,colunas,0);
					int count = 0;
					for(int c = 0; c < linhas; c+=2){
						for(int i = 0; i < colunas; i+=2){
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
							maximo = max(a,b);
							minimo = min(a,b);
							if(z>= maximo){
								lastPixel = minimo;					
							}else if(z <= minimo){
								lastPixel = maximo;
							}else{
								lastPixel = a+b-z;
							}
							int d = g->decode();
							result.at<uchar>(c,i) = (unsigned char) (d+lastPixel);
							result.at<uchar>(c,i+1) = (unsigned char) (d+lastPixel);
							result.at<uchar>(c+1,i) = (unsigned char) (d+lastPixel);
							result.at<uchar>(c+1,i+1) = (unsigned char) (d+lastPixel);
							count++;				
						}
					}
					od++;
					return result;
				}
			}
			return result;
	   	}
};



















