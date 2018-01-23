#include"Yang.h"
using namespace std;
using namespace cv;
float HSanl[255][255]={0},search1=2,Dthreshold=0.96;
int n=0,f=0,inter=50;
double r=150;
int begin0=0,rad=30,pre=3,bili=0;
int size=2;Mat element=getStructuringElement(MORPH_RECT,Size(2*size+1,2*size+1),Point(size,size));
int main(){
	cout<<"push the button 'S'to run the program";
	VideoCapture cap(0);
	Mat srcRGB1,srcRGB,mark(480,640,CV_8UC3,Scalar::all(0)),Dhand(480,640,CV_8UC1,Scalar::all(0));
	Point core(320,240);
	while(1){
	cap>>srcRGB1;
	cvtColor(srcRGB1,srcRGB,CV_RGB2HSV);
	//Yrby(srcRGB);

	//Yshowchannel(srcRGB,1);
	//Yshowchannel(srcRGB,2);

	blur(srcRGB,srcRGB,Size(3,3));
	srcRGB1.copyTo(mark);
	YclearUC1(Dhand);
	circle(mark,core,rad,Scalar::all(255),1);
	
	if(waitKey(1)=='s')
		f++;



	//prestudy/////////////////
	
	if((f!=0)&&(begin0==0))
	{   cout<<"study brgin";
		for(int i=0;i<inter;i++){
			cap>>srcRGB1;
	cvtColor(srcRGB1,srcRGB,CV_RGB2HSV);
	blur(srcRGB,srcRGB,Size(3,3));
	srcRGB1.copyTo(mark);
	circle(mark,core,rad,Scalar::all(255),1);
	imshow("study",mark);
			Yanalysis(core,rad,n,inter,srcRGB,HSanl);

	YshowHist(HSanl);
	n++;}
		begin0=1;cout<<"prestudy is complete";
	}
	
	//then work//////////////////////
	else if(begin0==1){
		Yanalysis(core,rad,n,inter,srcRGB,HSanl);
	YshowHist(HSanl);
	n++;
	}
	if(f!=0){
	if((core.x<=rad*(2*search1-1))&&
		(core.y<=rad*(2*search1-1))&&
		(core.x>=(640-rad*(2*search1-1)))&&
		(core.y>=(480-rad*(2*search1-1))))
	{cout<<"put your hand in the window"<<endl;
	core.x=320;core.y=480;
	f=0;n=0;
	}
	else{core=Ytrack(srcRGB,core,search1,rad,HSanl,pre);}}
	
	Yskin(srcRGB,Dhand,HSanl,Dthreshold,r,core,rad);


	
	dilate(Dhand,Dhand,element);


	//if(bili!=0){r=r*(Ybili(Dhand,core,r,bili));}

	//bili=Ygetbili(Dhand,core,r);



	
	//erode(Dhand,Dhand,element);

	//blur(Dhand,Dhand,Size(31,31));
	
	imshow("Dhand",Dhand);
	imshow("1",mark);



	
	if(waitKey(1)=='q')break;

	}



return 0;
}
