#include "zhu.h"
int main()
{   Mat compare=imread("D:\\1.jpg");
    Point point;
    Mat src=imread("D:\\2.jpg");
	compare.copyTo(src);
	
	//VideoCapture capture;   
	//capture.open("D:\\1.mp4");
	int minHessian = 400;
	double min_dist=100;
	SurfFeatureDetector detector(minHessian);
	SurfDescriptorExtractor extractor;
	Mat descriptors_object, descriptors_scene;
	vector<KeyPoint> keypoints_object, keypoints_scene;
	
	
	vector <DMatch> good_matches;
	detector.detect(compare,keypoints_object);
	extractor.compute(compare, keypoints_object,descriptors_object);
	FlannBasedMatcher matcher;
	vector<DMatch> matches;
	vector<Point2f>obj_corners(4);
	vector<Point2f>scene_corners(4);
	obj_corners[0]=cvPoint(0,0);
	obj_corners[1]=cvPoint(compare.cols,0);
	obj_corners[2]=cvPoint(compare.cols,compare.rows);
	obj_corners[3]=cvPoint(0,compare.rows);
	vector<Point2f> obj;
	vector<Point2f> scene;
	cout<<descriptors_object.size()<<endl;
//	while(1)
	//{//capture>>src;
	detector.detect(src,keypoints_scene);
	extractor.compute(src,keypoints_scene,descriptors_scene);
	cout<<descriptors_scene.size();
	matcher.match(descriptors_object, descriptors_scene,matches);
	for(int i=0;i<descriptors_object.rows;i++)
	{double dist = matches[i].distance;
	 if(dist<min_dist) min_dist=dist;
	}
	for(int i=0;i<descriptors_object.rows;i++)
	{
		if(matches[i].distance<3*min_dist)
		{good_matches.push_back (matches[i]);
		}
	}
	
	for(unsigned int i=0;i<good_matches.size();i++)
	{
		obj.push_back(keypoints_object[good_matches[i].queryIdx].pt);
		scene.push_back(keypoints_scene[good_matches[i].trainIdx].pt);

	}
	Mat show(compare.rows,compare.cols,CV_8UC1,Scalar::all(0));
	cout<<obj.size()<<endl<<scene.size();
	Mat H = findHomography(obj,scene,CV_RANSAC);
	perspectiveTransform(obj_corners,scene_corners,H);

	line(src,(scene_corners[0]+Point2f(static_cast<float>(compare.cols),0)),(scene_corners[1]+Point2f(static_cast<float>(compare.cols),0)),Scalar(255,0,123),4);
    line(src,(scene_corners[1]+Point2f(static_cast<float>(compare.cols),0)),(scene_corners[2]+Point2f(static_cast<float>(compare.cols),0)),Scalar(255,0,123),4);
    line(src,(scene_corners[2]+Point2f(static_cast<float>(compare.cols),0)),(scene_corners[3]+Point2f(static_cast<float>(compare.cols),0)),Scalar(255,0,123),4);
	line(src,(scene_corners[3]+Point2f(static_cast<float>(compare.cols),0)),(scene_corners[0]+Point2f(static_cast<float>(compare.cols),0)),Scalar(255,0,123),4);
	for(int i=0;i<scene.size();i++)
	{uchar *data=show.ptr<uchar>(scene[i].x);
	data[int(scene[i].y)]=255;

	}
	for(int i=0;i<3;i++)
	{uchar *data=show.ptr<uchar>(scene_corners[i].x);
	data[int(scene_corners[i].y)]=1;

	
	cout<<src.size();
	point.x=Ztmoment(show,1,0)/Ztmoment(show,0,0);
	point.y=Ztmoment(show,0,1)/Ztmoment(show,0,0);
	Zcircle(src,point);
	imshow("perspetive",src);
	obj.clear();
	scene.clear();
	good_matches.clear();
	matches.clear();
    scene_corners.clear();
	keypoints_scene.clear();
	//}
	return 0;
}