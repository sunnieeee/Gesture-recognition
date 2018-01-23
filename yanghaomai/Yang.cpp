#include"Yang.h"

uchar Ymedium(uchar* a){

	uchar min,num=9,ex;
	min=0;
	for(int i=0;i<5;i++){
		for(int j=i;j<9;j++){
			if(a[j]<a[min]){min=j;}
		}
		ex=a[i];
		a[i]=a[min];
		a[min]=ex;
		min=i+1;
	}
	return a[4];

}



void Ysharpen( Mat& input,Mat& output){
	int core[3][3]={{0,-1,0},{-1,4,-1},{0,-1,0}};
	for(int i=1;i<input.rows-1;i++){
		uchar* indata0=input.ptr<uchar>(i-1);
		uchar* indata1=input.ptr<uchar>(i);
		uchar* indata2=input.ptr<uchar>(i+1);
		uchar* outdata=output.ptr<uchar>(i);
		for(int j=3;j<(input.cols-1)*3;j=j+3){
		int sum0=0,sum1=0,sum2=0;
		sum0=core[0][0]*indata0[j-3]+core[0][1]*indata0[j]+core[0][2]*indata0[j+3]
			+core[1][0]*indata1[j-3]+core[1][1]*indata1[j]+core[1][2]*indata1[j+3]
			+core[2][0]*indata2[j-3]+core[2][1]*indata2[j]+core[2][2]*indata2[j+3];
		sum0=(sum0>=0)? sum0:0;

		outdata[j]=((((sum0+indata1[j])>=255)? 255:(sum0+indata1[j]))-indata1[j]);

		sum1=core[0][0]*indata0[j-2]+core[0][1]*indata0[j+1]+core[0][2]*indata0[j+4]
			+core[1][0]*indata1[j-2]+core[1][1]*indata1[j+1]+core[1][2]*indata1[j+4]
			+core[2][0]*indata2[j-2]+core[2][1]*indata2[j+1]+core[2][2]*indata2[j+4];
		sum1=(sum1>=0)? sum1:0;

		outdata[j+1]=((((sum1+indata1[j+1])>=255)? 255:(sum1+indata1[j+1]))-indata1[j+1]);

		sum2=core[0][0]*indata0[j-1]+core[0][1]*indata0[j+2]+core[0][2]*indata0[j+5]
			+core[1][0]*indata1[j-1]+core[1][1]*indata1[j+2]+core[1][2]*indata1[j+5]
			+core[2][0]*indata2[j-1]+core[2][1]*indata2[j+2]+core[2][2]*indata2[j+5];
		sum2=(sum2>=0)? sum2:0;

		outdata[j+2]=((((sum2+indata1[j+2])>=255)? 255:(sum2+indata1[j+2]))-indata1[j+2]);}}};






		void YGaussianT(Mat& input,Mat& output){
			unsigned int sum=0;
			// average
			for(int i=0;i<input.rows;i++){
				uchar* datas=input.ptr<uchar>(i);
				for(int j=0;j<input.cols;j++){sum+=datas[j];}}
			int avg=0;
			avg=static_cast<int>(sum/(input.rows*input.cols));
			unsigned long long var=0;
			for(int i=0;i<input.rows;i++){
				uchar* dataa=input.ptr<uchar>(i);
				for(int j=0;j<input.cols;j++){
					var+=(abs(dataa[j]-avg))*(abs(dataa[j]-avg));
				
				}}
			var=static_cast<unsigned long long>(var/(input.rows*input.cols));
			int sqrtVar=sqrt(static_cast<double>(var));

			int sec2=avg+50*sqrtVar;
			int sec1=avg+1.5*sqrtVar;

			for(int i=0;i<input.rows;i++){
				uchar* data=input.ptr<uchar>(i);
				uchar* data1=output.ptr<uchar>(i);
				for(int j=0;j<input.cols;j++){
					if(data[j]>sec1&&data[j]<sec2){data1[j]=255;}
					else data1[j]=0;
				}}
			

		}


			void YcvtRGB2GRAY(Mat& input,Mat& output){
			for(int i=0;i<input.rows;i++){
				uchar* data=input.ptr<uchar>(i);
				uchar* data1=output.ptr<uchar>(i);
				for(int j=0;j<input.cols;j++){
					data1[j]=static_cast<uchar>( ((float)(data[3*j]+data[3*j+1]+data[3*j+2]))/3 );
				}}

		}



			void Ysmooth(Mat& input ,Mat& output){
				
				for(int i=1;i<input.rows-1;i++){
		uchar* indata0=input.ptr<uchar>(i-1);
		uchar* indata1=input.ptr<uchar>(i);
		uchar* indata2=input.ptr<uchar>(i+1);
		uchar* outdata=output.ptr<uchar>(i);
		for(int j=3;j<(input.cols-1)*3;j=j+3){
			 uchar B[9]={indata0[j-3],indata0[j],indata0[j+3],
					indata1[j-3],indata1[j],indata1[j+3],
					indata2[j-3],indata2[j],indata2[j+3]};
			 uchar G[9]={indata0[j-2],indata0[j+1],indata0[j+4],
					indata1[j-2],indata1[j+1],indata1[j+4],
					indata2[j-2],indata2[j+1],indata2[j+4]};
			 uchar R[9]={indata0[j-1],indata0[j+2],indata0[j+5],
					indata1[j-1],indata1[j+2],indata1[j+5],
					indata2[j-1],indata2[j+2],indata2[j+5]};
			 outdata[j]=Ymedium(B);
			 outdata[j+1]=Ymedium(G);
			 outdata[j+2]=Ymedium(R);
		
		}}

			
			}




			void Yanalysis(cv::Point& core,int scale,int& n,int inter,Mat& src,float HSavg[255][255]){

				//Mat RIO=srcRGB(Rect(100,50,100,50));
				//Ysharpen(RIO,RIO);
				float HS[255][255]={0};//H,then S

				Mat RIO=src(Rect(core.x-scale,core.y-scale,2*scale,2*scale));
				for(int i=0;i<2*scale;i++){
					uchar* data=RIO.ptr<uchar>(i);
					for(int j=0;j<2*scale;j++){              //H=data[j*3];S=data[3*j+1];(double)
						if(Zcircle(j,i,scale))
						{HS[static_cast<int>(data[3*j])][static_cast<int>(data[3*j+1])]+=1;}
					}
				
				}
				int max=HS[0][0];
				for(int i=0;i<255;i++){
					for(int j=0;j<255;j++){
						if(max<HS[i][j])max=HS[i][j];
					}
				}

				for(int i=0;i<255;i++){
					for(int j=0;j<255;j++){
					if(HS[i][j]!=0)HS[i][j]/=max;
					}
				}
				double pwm=((double)n/(n+1)),pwm0=((double)inter/(inter+1));
				if(n<inter){
					for(int i=0;i<255;i++){
						for(int j=0;j<255;j++){
						HSavg[i][j]=pwm*HSavg[i][j]+(1-pwm)*HS[i][j];
						}
					}
				}
				else{for(int i=0;i<255;i++){
						for(int j=0;j<255;j++){
						HSavg[i][j]=pwm0*HSavg[i][j]+(1-pwm0)*HS[i][j];
						}
					}}

				
			}



			void Yskin(Mat& src,Mat& dst,float HS[255][255],float threshold,int r,Point& point,int R){
				uchar* data,*data1;
				for(int i=0;i<src.rows;i++){
					data=src.ptr<uchar>(i);
					data1=dst.ptr<uchar>(i);
					for(int j=0;j<src.cols;j++){
					if(Zcircle(i+r-point.y,j+r-point.x,r)){
						if(HS[data[3*j]][data[3*j+1]]>(1-threshold))data1[j]=255;}

					}}}





			Point Ytrack(Mat& src,Point& core,float scale,int rad,float HSavg[255][255],int pre){

				float HS[255][255]={0},minium[3]={65535,0,0};
				Mat RIOsearch;
				uchar* datasrc;
				
				int r=static_cast<int>(rad*(scale-1)-1);//,R=static_cast<int>(scale*rad);
				Mat RIOcore=src(Rect(core.x-r,core.y-r,2*r,2*r));

				for(int i=0;i<RIOcore.rows;i=i+pre){
					
					for(int j=0;j<RIOcore.cols;j=j+pre){
						RIOsearch=src(Rect(core.x-r+j-rad,core.y-r+i-rad,
							2*rad,2*rad));
						for(int m=0;m<2*rad;m++){
							datasrc=RIOsearch.ptr<uchar>(m);
							for(int n=0;n<2*rad;n++){
								if( Zcircle(n,m,rad))HS[(datasrc[3*n])][(datasrc[3*n+1])]+=1;                                   //zhu shi if

							}
						}
						float var=0;
						int max=HS[0][0];
				for(int a=0;a<255;a++){
					for(int b=0;b<255;b++){
						if(max<HS[a][b])max=HS[a][b];
					}
				}
				for(int a=0;a<255;a++){
					for(int b=0;b<255;b++){
					if(HS[a][b]!=0){HS[a][b]/=max;
					var+=abs(HS[a][b]-HSavg[a][b])/(HSavg[a][b]+0.00001);}

					}
				}
				if(minium[0]>var){minium[0]=var;minium[1]=core.x-r+j;minium[2]=core.y-r+i;}

				var=0;




					}}

				return Point(minium[1],minium[2]);

			}


			void YshowHist(float hist[255][255]){
			
				Mat H(255,255,CV_8UC1);

				for(int i=0;i<255;i++){
					uchar* data=H.ptr<uchar>(i);
					for(int j=0;j<255;j++){
					data[j]=static_cast<uchar>(hist[i][j]*255);
					}
				}
				imshow("hist",H);
				waitKey(1);
			}


			void YclearUC1(Mat& D){
				uchar *data
					;
				for(int i=0;i<D.rows;i++){
				data=D.ptr<uchar>(i);
				for(int j=0;j<D.cols;j++){
					if(data[j]!=0)data[j]=0;
				}

				}
			}
			


			void Yrby(Mat& Y){
				uchar* data;
				for(int i=0;i<Y.rows;i++){
					data=Y.ptr<uchar>(i);
					for(int j=0;j<Y.cols;j++){
						data[3*j]=data[3*j+1];
						data[3*j+1]=data[3*j+2];
					}
				}
			}

			void Yshowchannel(Mat& S,int c){
				Mat one(S.rows,S.cols,CV_8UC1,Scalar::all(0));
				uchar* data,*data1;

				for(int i=0;i<S.rows;i++){
					data=S.ptr<uchar>(i);
					data1=one.ptr<uchar>(i);
					for(int j=0;j<S.cols;j++){
						data1[j]=data[3*j+c-1];
					}
				}

				imshow("channel",one);
				waitKey(1);
			}







			double Ybili(Mat& S,Point& core,double r,double bili){

				double now=0;

				uchar* data;
				for(int i=0;i<S.rows;i++){
					data=S.ptr<uchar>(i);
					for(int j=0;j<S.cols;j++){
						if(Zcircle(i+r-core.y,j+r-core.x,r)){
							now++;

						}

					}
				}
				return (now/bili);
			}


			int Ygetbili(Mat& S,Point& core,double r){
				int bili=0;
				uchar* data;
				for(int i=0;i<S.rows;i++){
					data=S.ptr<uchar>(i);
					for(int j=0;j<S.cols;j++){
						if(Zcircle(i+r-core.y,j+r-core.x,r)){
							bili++;

						}

					}
				}
				return bili;

			}
