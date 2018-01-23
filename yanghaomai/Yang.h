
	//Ysmooth(srcRGB ,srcRGBsm);
	//Ysharpen(srcRGBsm,sharp);
	//YcvtRGB2GRAY(sharp,Sgray);
	//YGaussianT(Sgray,SgrayS);

	//Yanalysis();
	//Yanalysis(cv::Point& core,int scale,int& n,int inter,Mat& src,float** HSavg)
	//Yskin(Mat& src,Mat& dst,float** HS,float threshold)
	//circle(srcRGB,core,40,Scalar::all(0),1);
	//Ytrack(Mat& src,Point& core,float scale,int rad,float** HSavg,int pre)
	//YshowHist(float hist[255][255]);
	//YclearUC1(Mat&);


#ifndef _Yang_h


#define _Yang_h

#include"zhu.h"
//define output mat's scale first
void Ysharpen(Mat&,Mat&);

// one channel is allowed
//define output mat's scale first
void YGaussianT(Mat&,Mat&);


double abs(double a);

void YcvtRGB2GRAY(Mat&,Mat&);

//just nine is allowed

uchar Ymedium(uchar*);

void Ysmooth(Mat& input ,Mat& output);

void Yanalysis(cv::Point& core,int scale,int& n,int inter,Mat& src,float HSavg[255][255]);

//dst has only one channel
void Yskin(Mat& src,Mat& dst,float HS[255][255],float threshold,int r,Point& point,int R);

Point Ytrack(Mat& src,Point& core,float scale=1,int rad=40,float HSavg[255][255]=NULL,int pre=1);

void YshowHist(float hist[255][255]);

void YclearUC1(Mat&);

void Yrby(Mat&);

void Yshowchannel(Mat& S,int c);

double Ybili(Mat& S,Point& core,double r,double bili);

int Ygetbili(Mat& S,Point& core,double r);

#endif