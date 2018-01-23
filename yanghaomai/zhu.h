#ifndef _Zhu_h
#define _Zhu_h

#include<opencv2\opencv.hpp>
#include<cmath>
#include<vector>
#include<iostream>
using namespace std;
using namespace cv;

bool Zcircle(double i,double j,double scale);

void Zsample(Mat& src,int a,int b,Mat& dst);




#endif
