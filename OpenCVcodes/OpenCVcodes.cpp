// add the different routines here in form of functions

#include <iostream>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/core.hpp"

#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <time.h>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;
using namespace cv;


Mat makeDarkChannel(Mat I,int patch_size){
	int image_x = I.rows;
  	int image_y = I.cols;
  	int channels = 3;


  	Mat J = Mat::zeros(I.size(),I.type());
  	cvtColor(J,J,CV_BGR2GRAY);
	int padding = cvFloor(patch_size/2);
	Mat padded(I.rows + 2*padding, I.cols + 2*padding, I.type());
	padded.setTo(0);

	I.copyTo(padded(Rect(padding, padding, I.cols, I.rows)));
	int minX,maxX,minY,maxY;
	vector<Mat> channelImages;
  	split(padded,channelImages);
  	double minVal1,minVal2,minVal3;
  	double maxVal1,maxVal2,maxVal3;
    Mat ROIr,ROIb,ROIg;

	for(int i = 0;i<image_x;i++){
		minX = i;
		maxX = i + 2*cvFloor(patch_size/2);
		for(int j=0;j<image_y;j++){

			minY = j;
			maxY = j + 2*cvFloor(patch_size/2);
            			
                        // cout << "BITCH" <<j << image_y<<endl;
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
// removeHaze()
// guided_filter()
// estimateA()

int main(){

	Mat input;
	input = imread("10.png");
    Mat out = makeDarkChannel(input,21);
	imshow("Input",input);
    imshow("Output",out);
    clock_t t;
    t = clock();
    cout << ((float)t)/CLOCKS_PER_SEC << "seconds" << endl;
	waitKey(0);
	return 0;

}

