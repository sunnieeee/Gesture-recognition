#include<opencv2/opencv.hpp>
#include<iostream>

using  namespace std;
using namespace cv;

/****参数准备****/
Mat srcImage, grayImage, dstImage,frame;
int g_nThresh = 100, g_nThreshType=3;                  //初始化由静态确定的最佳阈值及二值化类型
vector<vector<Point>> g_contours;
vector<Vec4i> g_hier;

int main(int argc, char** argv)
{

/****调用摄像头采集图像****/
	VideoCapture capture(0);
/*	TermCriteria termcrit(TermCriteria::COUNT || TermCriteria::EPS, 10, 0.3);              定义迭代器*/

	while (1)
	{
		capture >> frame;
		frame.copyTo(srcImage);
/****图像预处理****/
		cvtColor(srcImage, grayImage, COLOR_BGR2GRAY);                         //转化为灰度图像

		namedWindow("1", WINDOW_AUTOSIZE);
		imshow("1", srcImage);

		threshold(grayImage,grayImage,g_nThresh,255,g_nThreshType);             //二值化
		Canny(grayImage, dstImage, g_nThresh, g_nThresh * 2, 3);                //边缘检测

/****查找并绘制图像轮廓****/
		//查找轮廓
		findContours(dstImage, g_contours, g_hier, RETR_CCOMP, CHAIN_APPROX_SIMPLE,Point(0,0));

		//查找图像矩
		vector<Moments> ma(g_contours.size());
		for (int i = 0; i < g_contours.size(); i++)
		{
			ma[i] = moments(g_contours[i], false);
		}

		//查找图像中心矩
		vector<Point2f> mc(g_contours.size());
		for (int i = 0; i < g_contours.size(); i++)
		{
			mc[i] = Point2f(static_cast<float>(ma[i].m10 / ma[i].m00), static_cast<float>(ma[i].m01 / ma[i].m00));
		}

	/*	vector<vector<Point>> g_contours_poly(g_contours.size());
		vector<Point2f> center(g_contours.size());
		vector<float> radius(g_contours.size());
		
		for (int i = 0; i < g_contours.size(); i++)
		{
			approxPolyDP(g_contours[i], g_contours_poly[i], 3, true);
			minEnclosingCircle(g_contours_poly[i], center[i], radius[i]);
		}
		*/                                                                                        //算法优化，不再使用最小包围圆

		//绘制轮廓及中心点
		Mat drawing = Mat::zeros(dstImage.size(), CV_8UC3);   //int type的选择
		for (int i = 0; i < g_contours.size(); i++)
		{
			Scalar color = Scalar(255);
			drawContours(drawing, g_contours, i, color, 1, 8, vector<Vec4i>(), 0, Point()); //参数不理解
			circle(drawing, mc[i], 5, color, 5 , 8, 0);
		}

		namedWindow("2", WINDOW_AUTOSIZE);
		imshow("2", drawing);

		frame.release();
		drawing.release();
		
		char key = (char)waitKey(100);
		if(key==27)
			break;
	}
		return 0;

	}


	
		

		
