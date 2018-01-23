#include<opencv2/opencv.hpp>
#include<iostream>

using  namespace std;
using namespace cv;


/****参数准备，定义全局变量****/
Mat srcImage,grayImage, dstImage;                                  //定义输入输出及灰度图像            
int g_nThresh = 100, g_nMaxThresh = 255;                           //初始化阈值的最大最小值
vector<vector<Point>> g_contours;                                                                     
vector<Vec4i> g_hier;                                               //vector数组开辟绘制轮廓的空间
void on_ThreshChange(int, void*);                                   //定义一供阈值变化的函数，待回调

int main(int argc, char** argv)                                     //括号中argc等可写，此为标准形式
{

/*****图像预处理****/
	srcImage=imread("H.jpg",1);                                      //读入图像文件
	cvtColor(srcImage, grayImage, COLOR_BGR2GRAY);                   //转化为灰度图

	namedWindow("1", WINDOW_AUTOSIZE);
	imshow("1",srcImage);

	createTrackbar("阈值", "1", &g_nThresh, g_nMaxThresh, on_ThreshChange);      //创建改变阈值的滑动条
	on_ThreshChange(0, 0);                                                       //给定初始值

	waitKey (0);
	return 0;

}


/****回调滑动条函数****/
void on_ThreshChange(int, void*)
{

/****找图像轮廓*****/
	Canny(grayImage, dstImage, g_nThresh, g_nThresh*2 , 3);                   //是否*2？  用threshold没有发现明显区别
	findContours(dstImage,g_contours,g_hier,RETR_CCOMP,CHAIN_APPROX_SIMPLE);  //轮廓查找


/****确定最小包围圆****/
	//参数准备
	vector<vector<Point>> g_contours_poly(g_contours.size());
	vector<Point2f> center(g_contours.size());
	vector<float> radius(g_contours.size());


	//循环查找最小圆
	for (int i = 0; i < g_contours.size(); i++)
	{
		approxPolyDP(g_contours[i], g_contours_poly[i], 3, true);              //true的含义及参数值确定？
		minEnclosingCircle(g_contours_poly[i], center[i], radius[i]);          //主体函数，查找最小包围圆
	}
	

	//画板准备
	Mat drawing = Mat::zeros(dstImage.size(), CV_8UC3);                         //int type的选择?
	for (int i = 0; i < g_contours.size(); i++)
	{
		Scalar color = Scalar(255);
		drawContours(drawing, g_contours_poly, i, color, 1, 8, vector<Vec4i>(), 0, Point()); //参数不理解
		circle(drawing, center[i], 5, color,-1,8, 0);                                    //thickness为负值时即实心圆
	}


	namedWindow("2", WINDOW_AUTOSIZE);
	imshow("2", drawing);

}
