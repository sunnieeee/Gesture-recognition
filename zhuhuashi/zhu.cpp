#include "zhu.h"
double Ztmoment(Mat src,int p,int q)
{   double ztmoment=0;
	for(int i=0;i<src.rows;i++)
 {uchar *data=src.ptr<uchar>(i);
 for(int j=0;j<src.cols;j++)
 {ztmoment+=(Zpower(i,p)*Zpower(j,q)*data[j]);
 }
	}
return ztmoment;}
double Zjmoment(Mat src,int p,int q)
{double zjmoment=0;
 double imoment=(Ztmoment(src,1,0)/Ztmoment(src,0,0));
 double jmoment=(Ztmoment(src,0,1)/Ztmoment(src,0,0));
 for(int i=0;i<src.rows;i++)
 {uchar *data=src.ptr<uchar>(i);
  for(int j=0;j<src.cols;j++)
  {zjmoment+=(Zpower(i-imoment,p)*Zpower(j-jmoment,q)*data[j]);
  }
 }
 return zjmoment;
}
double Zgjmoment(Mat src,int p,int q)
{double zgjmoment=Zjmoment(src,p,q)/(Zpower((Zjmoment(src,0,0)),(p+q)/2));
return zgjmoment;
 
}
double Zunmoment(Mat src,int a)
{    if(a==1)  return (Zgjmoment(src,2,0)+Zgjmoment(src,0,2));
else if(a==2)  return (Zpower((Zgjmoment(src,2,0)-Zgjmoment(src,0,2)),2)+4*Zpower(Zgjmoment(src,1,1),2));
else if(a==3)  return (Zpower(Zgjmoment(src,2,0)-3*Zgjmoment(src,1,2),2)+3*Zpower((Zgjmoment(src,2,1)-Zgjmoment(src,0,3)),2));
else if(a==4)  return  (Zpower((Zgjmoment(src,3,0)+Zgjmoment(src,1,2)),2)+Zpower((Zgjmoment(src,2,1)+Zgjmoment(src,0,3)),2));
else {cout<<"fuck you";return 0;}
 }
double Zpower(double x,int n)
{   double result=1;
	for(int i=0;i<n;i++)
{  result*=x;
}
	return result;
}  
void Zsample(Mat& src,int a,int b,Mat& dst){
	 

	float small[432][532*3]={0};
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
	


}



int zhupyrDown(Mat& src,Mat& dst,int r){
	int down[(432*3)/2][(576*3)/2]={0};
	for(int i=0;i<src.rows;i++){
		uchar* data=src.ptr<uchar>(i);
		for(int j=0;j<src.cols;j++){
			if(j%3==0){down[static_cast<int>(i/(r))][static_cast<int>(j/(3*r))]+=data[j];}
			if(j%3==1){down[static_cast<int>(i/(r))][static_cast<int>(j/(3*r))+1]+=data[j];}
			if(j%3==2){down[static_cast<int>(i/(r))][static_cast<int>(j/(3*r))+2]+=data[j];}
		}
	}
	Mat dst1(432/2,576/2,CV_8UC3);
	for(int i=0;i<dst1.rows;i++){
		uchar* data1=dst1.ptr<uchar>(i);
		for(int j=0;j<3*dst1.cols;j++){
			data1[j]=static_cast<uchar>(down[i][j]/(r*r));
			
		}
	}
	dst1.copyTo(dst);return 0;

}
void Zcircle(Mat src,Point point)
{
	int thickless = -1;
	int lineType = 8;
	circle(src,point,src.cols/64,Scalar(0,0,255),thickless,lineType);
}