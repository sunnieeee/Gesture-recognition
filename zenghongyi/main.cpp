#include"opencv2/opencv.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/video/background_segm.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <stdio.h>
using namespace std;
using namespace cv;


static void help()
{
	printf("\n\n\n\t此程序展示了用高斯背景建模进行视频的背景分离方法.\n\n\t主要采用cvUpdateBGStatModel()函数\n"
		
		"\n\t按下空格后开始手势检测。\n\n");
}


//-----------------------------------【main( )函数】--------------------------------------------
//		描述：控制台应用程序的入口函数，我们的程序从这里开始
//-------------------------------------------------------------------------------------------------
int main(int argc, const char** argv)
{
	help();
	Mat element=getStructuringElement(MORPH_RECT,Size(5,5));//形态学滤波元素
	VideoCapture cap(0);//开摄像头
	bool update_bg_model = true;//是否开始手势检测
	float x_past,y_past,x_present,y_present;//判断手重心移动
	namedWindow("image", WINDOW_AUTOSIZE);
	namedWindow("foreground mask", WINDOW_AUTOSIZE);
	namedWindow("foreground image", WINDOW_AUTOSIZE);
	BackgroundSubtractorMOG2 bg_model(100, 16, false);//混合高斯背景初始化
	Point2f mc;
	Mat img, fgmask, fgimg;
	bool n=true;
	for(;;)
	{
		cap >> img;

		if(! img.empty() )
		{

			cvtColor(img, img, COLOR_BGR2YCrCb);
			vector<Mat> ycrcb;
			split(img,ycrcb);//分离YCrCb空间
		    Mat y=ycrcb.at(0);
			Mat cr=ycrcb.at(1);
			Mat cb=ycrcb.at(2);
			//***********用指针遍历Cr和Cb空间，二值化肤色提取**********//
			Mat cr1(img.rows,img.cols,CV_8UC1,Scalar(0));
			Mat cb1(img.rows,img.cols,CV_8UC1,Scalar(0));
            for(int i=0;i<cr.rows;i++)
			{
				uchar* data1=cr.ptr<uchar>(i);
				uchar* data1a1=cr1.ptr<uchar>(i);
				for(int j=0;j<cr.cols;j++)
				{
					if ((data1[j]>=135 && data1[j]<=170))
					
						data1a1[j]=255;
					    else  data1a1[j]=0;
					
				}	
			}
			for(int i=0;i<cb.rows;i++)
			{
				uchar* data2=cb.ptr<uchar>(i);
				uchar* data2a2=cb1.ptr<uchar>(i);
				for(int j=0;j<cb.cols;j++)
				{
					if (data2[j]>=100 && data2[j]<=127 )
					
						data2a2[j]=255;
					else data2a2[j]=0;
				}
			}
			bitwise_and(cr1,cb1,img);
			//*************************************以上得到肤色图像****************//
		if( fgimg.empty() )
			fgimg.create(img.size(), img.type());

		//更新模型
		bg_model(img, fgmask, update_bg_model ? -1 : 0);//对二值化图像提取背景
		fgimg = Scalar::all(0);
		img.copyTo(fgimg, fgmask);
		Mat bgimg;
		//bg_model.getBackgroundImage(bgimg);
		morphologyEx(fgmask,fgmask,MORPH_OPEN,element);//形态学开运算处理背景
		vector<vector<Point> > contours;
		vector<Vec4i>Hierrchy;
		findContours(fgmask,contours,Hierrchy,CV_RETR_EXTERNAL,CV_CHAIN_APPROX_SIMPLE,Point(0,0));//寻找轮廓
		Mat reasult(fgmask.size(),CV_8U,cv::Scalar(255));
		int t=0,i=0;
		for( i=0;i<contours.size();i++)
		{
			if(contourArea(contours[i])>contourArea(contours[t])) t=i;//提取面积最大的轮廓
		}
		if (contourArea(contours[t])>500 && contourArea(contours[t])<500000 )//除去过小面积轮廓和往往是背景的过大面积轮廓
 		drawContours(reasult,contours,t,Scalar(0),5,8,vector<Vec4i>(),0,Point());
		Moments mu;//初始化矩
		if (contourArea(contours[t])>500 && contourArea(contours[t])<500000 && !update_bg_model)
		{
			mu=moments(contours[t],false);//计算最大面积轮廓的矩
	
			mc=Point2f(static_cast<float>(mu.m10/mu.m00),static_cast<float>(mu.m01/mu.m00));//计算重心
			if(n)
			{
				x_past=static_cast<float>(mu.m10/mu.m00);
				y_past=static_cast<float>(mu.m01/mu.m00);
				n=!n;
			}
			else
			{
				x_present=static_cast<float>(mu.m10/mu.m00);
				y_present=static_cast<float>(mu.m01/mu.m00);
				if (x_present-x_past>100 && x_present-x_past<500 ){ printf("右右右右右右\n"); }//移动距离过小过大多是背景影响
				if (x_present-x_past<-100 && x_present-x_past>-500 ){ printf("左左左左左左\n");}
				x_past=static_cast<float>(mu.m10/mu.m00);
				y_past=static_cast<float>(mu.m01/mu.m00);
			}
			circle(reasult,mc,7,Scalar(0),-1,8);
		}
		//****************以上获取运动方向**************//
		vector<Point> fingerTips;
		Point temp;//牛牛牛牛牛牛
		int max=0,count=0,notice=0;
		if(!update_bg_model)
		{
		for(int i=0;i<contours[t].size();i++)//遍历最大轮廓
		{
			temp=contours[t][i];
			int dist=(temp.x-x_present)*(temp.x-x_present)+(temp.y-y_present)*(temp.y-y_present);//计算距离
			if(dist>max )
			{
				max=dist;
				notice=i;
			}
			if(dist!=max)
			{
				count++;
				if(count>contours[t].size()*0.12)//如果距离不等的点达到阈值，设为关注点
				{
					count=0;
					max=0;
					bool flag=false;
					if(y_present<temp.y) continue;//不能低于重心
					//if(dist<50000) continue;
					for(int j=0;j<fingerTips.size();j++)
					{
						if(abs(contours[t][i].x-fingerTips[j].x)<15)//两点间距不能过小
						{
							flag=true;
							break;
						}
					}
					if(flag) continue;
					Point pr,pl;
					pl=contours[t][i-5];
					pr=contours[t][i+5];
					int dot=(pl.x-temp.x)*(pr.y-temp.y)-(pr.x-temp.x)*(pl.y-temp.y);//计算关注点附近曲线的曲率
					if(dot<60 && dot>-60)
					{
						int cross=(pl.x-temp.x)*(pr.y-temp.y)-(pr.x-temp.x)*(pl.y-temp.y);
						if(cross>0)//要求轮廓程凸型
						{
							fingerTips.push_back(contours[t][i]);
							for (int i = 0; i < fingerTips.size(); i++)
							{
							circle(reasult, fingerTips[i], 6, Scalar(0), CV_FILLED);		//画出指尖点
							line(reasult, mc, fingerTips[i], Scalar(0), 2);				//连线
							}
						}
					}
				}
		    }
		}
	}	
		//****************以上获得指尖点***************//
		imshow("image", img);
		imshow("foreground mask", reasult);
		imshow("foreground image", fgimg);
		char k = (char)waitKey(30);
		if( k == 27 ) break;
		if( k == ' ' )
		{
			update_bg_model = !update_bg_model;
			if(update_bg_model)
				printf("\t>手势识别已关闭\n");
			else
				printf("\t>手势识别已打开\n");//开启关闭识别
		}
	}
	}

	return 0;
}
