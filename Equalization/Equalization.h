#pragma once
#include <string>
#include <opencv2/core/core.hpp>
#include <iostream>
#define LEAVE 256
using namespace cv;

class Equalization {
public:
	//初始化各个私有成员。
	Equalization();
	//设置要处理的图片
	void setImage(Mat &image_in );
	//分别对图片的RGB值进行直方图统计
	void statistic();
	//对原图进行均衡处理
	Mat queProcess();
private:
	//三个私有变量分别统计channel 0, 1,2的灰度级的像素个数
	int statistic_R[LEAVE];
	int statistic_G[LEAVE];
	int statistic_B[LEAVE];
	//将要处理的图像
	Mat image;
};
