#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <cmath>
#include <Windows.h>
#include "change.h"

using namespace std;
using namespace cv;
int main()
{
	//读原图
	Mat image;
	image = imread("pic.jpg", CV_LOAD_IMAGE_COLOR);

	if (!image.data)
	{
		cout << "找不到图片或无法打开图片\n";
		return -1;
	}
	namedWindow("image_source", CV_WINDOW_AUTOSIZE);
	imshow("原图", image);
	
	//平移
	shift(image, 10, 20);

	//缩放
	scale(image, 0.8);

	//旋转
	rotation(image, 45);

	//垂直偏移
	voffset(image, 1);

	//水平偏移
	hoffset(image, 1);
	//连续播放变换后的图像
	string picname = "";
	Mat image2;
	for (int counter = 1; counter < 6; counter++)
	{
		picname = '0'+ counter;
		picname += ".jpg";
		image2 = imread(picname,CV_LOAD_IMAGE_COLOR);
		imshow("变换后的图像", image2);	
		waitKey(3000);
	}

	waitKey(0);
	return 0;
}