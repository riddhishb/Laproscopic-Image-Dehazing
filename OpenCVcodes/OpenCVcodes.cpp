// add the different routines here in form of functions

#include <iostream>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/core.hpp"
#include "guidedfilter.h"
#include "guidedfilter.cpp"

#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <time.h>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;
using namespace cv;

int minX,maxX,minY,maxY;
double minVal1,minVal2,minVal3;
double maxVal1,maxVal2,maxVal3;
Mat ROIr,ROIb,ROIg;

Mat makeDarkChannel(Mat I,int patch_size){

  	Mat J = Mat::zeros(I.size(),I.type());
  	cvtColor(J,J,CV_BGR2GRAY);
	int padding = cvFloor(patch_size/2);

    // padding the given image
	Mat padded(I.rows + 2*padding, I.cols + 2*padding, I.type());
	padded.setTo(0);
	I.copyTo(padded(Rect(padding, padding, I.cols, I.rows)));
	
	vector<Mat> channelImages;
  	split(padded,channelImages);
  
	for(int i = 0;i<I.rows;i++){
		minX = i;
		maxX = i + 2*cvFloor(patch_size/2);
		for(int j=0;j<I.cols;j++){

			minY = j;
			maxY = j + 2*cvFloor(patch_size/2);
            			
            ROIb = channelImages.at(0)(Rect(minY,minX,patch_size-1,patch_size-1));
            ROIg = channelImages.at(1)(Rect(minY,minX,patch_size-1,patch_size-1));
            ROIr = channelImages.at(2)(Rect(minY,minX,patch_size-1,patch_size-1));

            minMaxIdx(ROIb,&minVal1,&maxVal1);
			minMaxIdx(ROIg,&minVal2,&maxVal2);
			minMaxIdx(ROIr,&minVal3,&maxVal3);

            J.at<uchar>(i,j) =  fmin(minVal1,fmin(minVal3,minVal2));    
		}

	}

    return J;
}

int main(){

	Mat input;
	input = imread("10.png");
    input = input(Rect((input.cols - input.rows)/2,0,input.rows-1,input.rows-1)); // cropping the sides.

    Mat tx = imread("tx.jpg");
    Mat guide = imread("guide.jpg");
    //resize(input, input, Size(), 0.238, 0.238, INTER_LINEAR);
   // Mat out = makeDarkChannel(input,21);
    Mat tx_out;
    cvtColor(guide,guide,CV_BGR2GRAY);
    tx_out = guidedFilter(guide, tx, 3, 1e-6);
   // resize(out, out, Size(), 4.2017, 4.2017, INTER_LINEAR);	
    clock_t t;
    t = clock();
    imshow("Input",tx);
    imshow("Output",tx_out);
    cout << ((float)t)/CLOCKS_PER_SEC << "seconds" << endl;
	waitKey(0);
	return 0;
}

