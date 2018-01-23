#ifndef zhu
#define zhu
#include <opencv2/opencv.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/nonfree/nonfree.hpp>
#include <opencv/cv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <iostream>





using namespace cv;
using namespace std;
double Ztmoment(Mat src,int p,int q);
double Zjmoment(Mat src,int p,int q);
double Zgjmoment(Mat src,int p,int q);
double Zunmoment(Mat src,int a);
void Zsample(Mat& src,int a,int b,Mat& dst);
double Zpower(double x,int n);
int zhupyrDown(Mat& src,Mat& dst,int r=8);
void Zcircle(Mat src,Point point);


#endif
