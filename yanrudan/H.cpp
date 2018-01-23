#include "opencv2/video/tracking.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include "opencv2/highgui.hpp"

#include <iostream>
#include <ctype.h>
using namespace cv;
using namespace std;

static void help()
{}
Point2f point;
bool addRemovePt = false;//什么意思？
static void onMouse(int event, int x, int y, int, void*)//static是干什么用的？
{
	if (event == EVENT_LBUTTONDOWN)//左键按下
		point = Point2f((float)x, (float)y);
	addRemovePt = true;
}

int main(int argc, char** argv)//前一个用来统计运行时发送给main函数命令执行的个数，默认值为1，第二个用来存放指向字符串参数的指针数组，每个元素指向一个参数。
{
	VideoCapture capture(1);
    TermCriteria termcrit(TermCriteria::COUNT | TermCriteria::EPS, 20, 0.03);//终止条件为迭代到最大迭代次数或或阈值终止，以上的宏对应的c++的版本分别为TermCriteria::EPS
    Size subPixWinSize(10,10), winSize(31,31);
	const int MAX_COUNT = 1;
bool needToInit = false;//初始化
bool nightMode = false;//黑暗模式

help();
cv::CommandLineParser parser(argc, argv, "{@input|0|}");
string input = parser.get<string>("@input");
if(input.size() == 1 && isdigit(input[0]))//判断元素个数函数，主要用于检查参数是否为十进制数字字符。用于判断字符c是否为数字，当c为数字0~9时，返回非零值，否则返回零
capture.open(input[0] - '0');
else
capture.open(input);
if (!capture.isOpened())//感叹号放前面的意思是如果他是0，则条件为真，如果不为0，这条件为假。
{
	cout << "Could not initialize capturing...\n";//初始化捕捉视频。

	return 0;
}
namedWindow("点追踪", 0);//0是可以改变窗口大小
setMouseCallback("点追踪", onMouse, 0);//鼠标操作回调函数
Mat gray, prevGray, image,frame;//灰，预处理，图像
vector<Point2f> points[2];//vector表示向量，定义数组，储存需要跟踪的点
for(;;)

{
	capture >> frame;//从摄像头读入一帧
	if (frame.empty())//判断是否是空
		break;
	frame.copyTo(image);//复制image矩阵
	cvtColor(image, gray, COLOR_BGR2GRAY);//通道转为灰度图像Gray
	if (nightMode)
		image = Scalar::all(0);
	if (needToInit)//他为0，条件为假，不为0，条件为真
	{
		//自动初始化
		goodFeaturesToTrack(gray, points[1], MAX_COUNT, 0.01, 10, Mat(), 3, 3, 0, 0.04);//MAX_COUNT=500,该函数可以初始化一个机遇点的对象跟踪操作，待会去掉一个3看看
		cornerSubPix(gray, points[1], subPixWinSize, Size(-1, -1), termcrit);//用21乘21的大小的搜索窗口，（-1，-1）表示没有死区
		addRemovePt = false;
	}
	else if (!points[0].empty())
	{
		vector<uchar> status;//可能用于某些点被取消
		vector<float> err;
		if (prevGray.empty())//预处理
			gray.copyTo( prevGray);
		calcOpticalFlowPyrLK(prevGray, gray, points[0], points[1], status, err, winSize, 3, termcrit, 0, 0.001);//计算一个稀疏特征集的光流，使用金字塔中的迭代 Lucas-Kanade 方法
		size_t i, k;
		for(i=k=0;i<points[1].size();i++)
		{
			if (addRemovePt)
			{
				if (norm(point - points[1][i]) <= 5)//NORM是一个函数，功能是一种可以在向量空间里对向量赋予长度和大小，格式是n=norm(A,p)，返回A的最大奇异值，即max(svd(A)) n=norm(A,p)，根据p的不同，返回不同的值。
				{
					addRemovePt = false;
					continue;//当表达式为真，结束本轮循环，跳过其后的循环体语句，继续下一轮循环。
				}
			}
			if (!status[i])
				continue;

			points[1][k++] = points[1][i];
			circle(image, points[1][i], 4, Scalar(0,0,255), 3, 8);//points那个是定义点，半径，下面是颜色，粗线（所以是实心）
			
		}
      
		points[1].resize(k);//函数 Resize 将图像 src 改变尺寸得到与 dst 同样大小。若设定 ROI，函数将按常规支持 ROI
	}
	cout << "图上点的坐标";
	cout << points[1] << "  " << points[0] << endl;//what?
	if (addRemovePt && points[1].size() < (size_t)MAX_COUNT)
	{
		vector<Point2f> tmp;//变量名
		tmp.push_back(point);//push_back  :stl 容器中的函数，用于在后面添加一项
		cornerSubPix(gray, tmp, winSize, Size(-1, -1), termcrit);//亚像素点.(-1,-1)表示没有死区，求角点迭代过程中的终止条件
		points[1].push_back(tmp[0]);
		addRemovePt = false;
	}
	needToInit = false;
	imshow("点追踪", image);
	

	char c = (char)waitKey(10);//如果程序想响应某个按键，可利用if(waitKey(1)==Keyvalue)
	if (c == 27)//按下ESC键，程序退出ascii码对应27
		break;
	switch (c)
	{
	case 'r':
		needToInit = true;
		break;
	case 'c':
		points[0].clear();
		points[1].clear();
		break;//删除所有点
	case 'n':
		nightMode = !nightMode;
		break;
	}

	std::swap(points[1], points[0]);
	cv::swap(prevGray, gray);
}

return 0;
}

