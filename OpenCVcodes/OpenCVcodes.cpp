
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

clock_t t;
int minX,maxX,minY,maxY;
double minVal1,minVal2,minVal3;
double maxVal1,maxVal2,maxVal3;
Mat ROIr,ROIb,ROIg;
int array_for_min[2] = {0,0};


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
void min_arr( int *arr , int length ){

    int pos = 0;
    int minimum = arr[0];
    for (int i = 1; i < length; i++) {
        if (minimum > arr[i]) {
            minimum = arr[i];
            pos = i;
        }
    }
    array_for_min[0] = minimum;
    array_for_min[1] = pos;
    // return 0;
}

Mat estimateA(Mat I, Mat J, int numPixels){
     Mat A;

    int array[numPixels] ;
    int arrayx[numPixels] ;
    int arrayy[numPixels] ;
    for(int k = 0;k<numPixels;k++){
                 array[k] = 0;
                 arrayx[k] = 0;
                 arrayy[k] = 0;
                }

    for(int i = 0;i<J.cols;i++){
        for(int j=0;j<J.rows;j++){

            min_arr(array,numPixels);

            if (int(J.at<uchar>(i,j)) > array_for_min[0]){
                arrayx[array_for_min[1]] = i;
                arrayy[array_for_min[1]] = j;
                array[array_for_min[1]] = int(J.at<uchar>(i,j));
            }
        }
    }

    //      Find the highest intensity pixel from the original Image using the
    //      list calculated above
   
    int highestIntensity[3] = {0,0,0};
    int x,y,intensity;
    for(int i = 0;i<numPixels;i++) {
         x = arrayx[i];
         y = arrayy[i];
         intensity = I.at<Vec3b>(x,y)[0] + I.at<Vec3b>(x,y)[1] + I.at<Vec3b>(x,y)[2];
        if(intensity > (highestIntensity[0] + highestIntensity[1] + highestIntensity[2])){
            highestIntensity[0] = I.at<Vec3b>(x,y)[0];
            highestIntensity[1] = I.at<Vec3b>(x,y)[1];
            highestIntensity[2] = I.at<Vec3b>(x,y)[2];
        }
    }

    A = Mat::zeros(I.size(),I.type());
    A = cv::Scalar(highestIntensity[0], highestIntensity[1], highestIntensity[2]);  
    return A;
}



 void unsharpMask(cv::Mat& im) 
{
    cv::Mat tmp;
    cv::GaussianBlur(im, tmp, cv::Size(0,0), 3);
    cv::addWeighted(im, 1.5, tmp, -0.5, 0, im);
}

Mat getDehazed(Mat input,Mat A,Mat tx_map){
Mat dehazed;
    dehazed = Mat::zeros(input.size(),input.type());

    for(int i = 0;i<dehazed.rows;i++){
        for(int j=0;j<dehazed.cols;j++){
            for(int c = 0;c<3;c++){

                dehazed.at<Vec3b>(i,j)[c] = 255*(input.at<Vec3b>(i,j)[c] - A.at<Vec3b>(i,j)[c])/max(int(tx_map.at<uchar>(i,j)),26) + A.at<Vec3b>(i,j)[c] ;
                // cout << max(int(tx_out.at<uchar>(i,j)),26) <<endl;
                if(255*(input.at<Vec3b>(i,j)[c] - A.at<Vec3b>(i,j)[c])/max(int(tx_map.at<uchar>(i,j)),26) + A.at<Vec3b>(i,j)[c] <0){
                    dehazed.at<Vec3b>(i,j)[c] = 0;
                }
                if(255*(input.at<Vec3b>(i,j)[c] - A.at<Vec3b>(i,j)[c])/max(int(tx_map.at<uchar>(i,j)),26) + A.at<Vec3b>(i,j)[c] >255){
                    dehazed.at<Vec3b>(i,j)[c] = 255;
                }
            }
        }
    }

    return dehazed;
}


// for images

int main(){

 Mat input;
double arielPerspective = 0.95;

 input = imread("10.png");

resize(input, input, Size(), 0.25, 0.25, INTER_LINEAR);
//input.convertTo(input,CV_32FC3,1/255.0);

   
Mat dark_ch = makeDarkChannel(input,17);
// imshow("dark_ch",dark_ch);
int numBrightestPixels = cvCeil(0.001 * dark_ch.rows * dark_ch.cols); // Use the cieling to overestimate number needed
      
Mat A = estimateA(input,dark_ch,numBrightestPixels);
Mat tx_map;
Mat out;

divide(input,A,out,255);

tx_map = 255 - arielPerspective*makeDarkChannel(out,17);
    
// Mat tx_out,input_gray;
// cvtColor(input,input_gray,CV_BGR2GRAY);
// tx_out = tx_map;//
// tx_out = guidedFilter(input_gray, tx_map, 3, 1e-6);
 
// Now to reconstruct the Dehazed image
    
Mat dehazed = getDehazed(input,A,tx_map);
    
// cout << tx_out;
     
     
resize(dehazed, dehazed, Size(), 4, 4, INTER_LINEAR);
unsharpMask(dehazed);
unsharpMask(dehazed);
// unsharpMask(dehazed);
    
t = clock();
imshow("Final ",dehazed);
imwrite("../out.png",dehazed);
cout << ((float)t)/CLOCKS_PER_SEC << "seconds" << endl;
waitKey(0);
return 0;
}


 // for video
//    int main()
// {
//    double arielPerspective = 0.95;
//    const string source      = "2.mp4";           // the source file name
//    const bool askOutputType =true;  // If false it will use the inputs codec type

//    VideoCapture inputVideo(source);              // Open input
//    if (!inputVideo.isOpened())
//    {
//         cout  << "Could not open the input video: " << source << endl;
//         return -1;
//    }

//    string::size_type pAt = source.find_last_of('.');                  // Find extension point
//    const string NAME = "output2.avi";   // Form the new name with container
//    int ex = static_cast<int>(inputVideo.get(CV_CAP_PROP_FOURCC));     // Get Codec Type- Int form

//    // Transform from int to char via Bitwise operators
//    char EXT[] = {(char)(ex & 0XFF) , (char)((ex & 0XFF00) >> 8),(char)((ex & 0XFF0000) >> 16),(char)((ex & 0XFF000000) >> 24), 0};

//    Size S = Size((int) inputVideo.get(CV_CAP_PROP_FRAME_WIDTH),    // Acquire input size
//                   (int) inputVideo.get(CV_CAP_PROP_FRAME_HEIGHT));


//    VideoWriter outputVideo(NAME,CV_FOURCC('P','I','M','1'),inputVideo.get(CV_CAP_PROP_FPS),S,true);                                        // Open the output
//    // if (askOutputType)
//    //     outputVideo.open(NAME, ex=-1, inputVideo.get(CV_CAP_PROP_FPS), S, true);
//    // else
//    //     outputVideo.open(NAME, ex, inputVideo.get(CV_CAP_PROP_FPS), S, true);

//    // if (!outputVideo.isOpened())
//    // {
//    //     cout  << "Could not open the output video for write: " << source << endl;
//    //     return -1;
//    // }

//    cout << "Input frame resolution: Width=" << S.width << "  Height=" << S.height
//         << " of nr#: " << inputVideo.get(CV_CAP_PROP_FRAME_COUNT) 
//         << "FPS: " << inputVideo.get(CV_CAP_PROP_FPS) <<endl;
//    cout << "Input codec type: " << EXT << endl;

//    int channel = 2; // Select the channel to save
//    int alpha = 0.8;
//    Mat frame;
//    int count = 0;
//    for(;;) //Show the image captured in the window and repeat
//    {   

//         inputVideo >> frame;              // read
//         if (frame.empty()) break;         // check if at end
//         resize(frame, frame, Size(), 0.25, 0.25, INTER_LINEAR);

//        Mat dark_ch = makeDarkChannel(frame,17);
//    // imshow("dark_ch",dark_ch);
//    int numBrightestPixels = cvCeil(0.001 * dark_ch.rows * dark_ch.cols); // Use the cieling to overestimate number needed
//    Mat A_prev;  
//    // if(count != 0){Mat A_prev = A;}  
//    Mat A = estimateA(frame,dark_ch,numBrightestPixels);

    
//      if(count != 0){
//         // Do alpha blending
//         addWeighted(A,alpha,A_prev,1-alpha,0.0,A);
//      }

//    Mat tx_map;
//    Mat out;

//    divide(frame,A,out,255);

//    tx_map = 255 - arielPerspective*makeDarkChannel(out,17);
   
//    // Now to reconstruct the Dehazed image
    
//    Mat dehazed = getDehazed(frame,A,tx_map);
    
//      // cout << tx_out;
//      // SUGGESTION : Have an alpha blending between A, so the sharp changes are averted
//      // on all therre channels

     
//      resize(dehazed, dehazed, Size(), 4, 4, INTER_LINEAR);
//      unsharpMask(dehazed);
//      unsharpMask(dehazed);
//        //outputVideo.write(res); //save or
//      A_prev = A;
//        // outputVideo << dehazed;
//      outputVideo.write(dehazed);
//    }

//    cout << "Finished writing" << endl;
//    return 0;
// }
    
