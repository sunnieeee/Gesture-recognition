#include"zhu.h"
bool Zcircle(double i,double j,double scale)
{
	if((j>(-0.414)*(i-scale))&&
	(j>((-2.414)*i+scale))&&
	(j<(2.414*i+scale))&&
	(j<(0.414*i+1.58579*scale))&&
	(j<((-0.414)*i+2.414*scale))&&
	(j<((-2.414)*i+5.828*scale))&&
	(j>(2.414*i+(-3.828)*scale))&&
	(j>(0.414*(i-scale))))return true;
	else {return false;}

}

//a is row,b is col
void Zsample(Mat& src,int a,int b,Mat& dst){
	 

	float small[160][120]={0};
	 for(int i=0;i<src.rows;i++){
		 uchar* data=src.ptr<uchar>(i);
		 for(int j=0;j<src.cols;j++){
			 small[static_cast<int>(i/(a+1))+1][static_cast<int>(j/(b+1))+1]+=data[j];
}
}
	 Mat showSmall(src.rows/a,src.cols/b,CV_8UC1);
	 for(int i=0;i<showSmall.rows;i++)
	 {uchar* data1=showSmall.ptr<uchar>(i);
	  for(int j=0;j<showSmall.cols;j++)
		  data1[j]=(small[i][j]/(a*b));
	 }
	 showSmall.copyTo(dst);
	 //imshow("fuck",showSmall);
	 //waitKey(1);


}